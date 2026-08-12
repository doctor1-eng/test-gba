#!/usr/bin/env python3
"""Corrèle les artefacts d'analyse statique (CSV) en régions de 16 KiB.

Entrée : les CSV du pack "Pokemon_Unbound_MAX_Analysis" (fournis par l'utilisateur,
jamais la ROM elle-même — aucun fichier binaire n'est lu ou requis par ce script).
Sortie : docs/pokemon-unbound-analysis/data/region-profile.csv, un profil par région
de 16 KiB (entropie, densité de pointeurs, densité de signatures de compression,
densité de chaînes, densité de blocs répétés, densité de cibles BL ARM/Thumb) plus
une classification heuristique avec score de confiance.

Relançable : ne modifie aucune entrée, écrit toujours le même format de sortie.
"""
import csv
import sys
from pathlib import Path
from collections import defaultdict

REGION_SIZE = 0x4000  # 16 KiB, aligné sur la granularité d'entropy-16k.csv

def region_of(offset_hex):
    return int(offset_hex, 16) // REGION_SIZE

def load_entropy(path):
    regions = {}
    with open(path, newline="") as f:
        for row in csv.DictReader(f):
            r = region_of(row["offset"])
            regions[r] = {
                "entropy": float(row["entropy"]),
                "zero_ratio": float(row["zero_ratio"]),
                "ff_ratio": float(row["ff_ratio"]),
            }
    return regions

def count_by_region(path, offset_field, extra=None):
    counts = defaultdict(int)
    extras = defaultdict(lambda: defaultdict(int))
    with open(path, newline="") as f:
        for row in csv.DictReader(f):
            r = region_of(row[offset_field])
            counts[r] += 1
            if extra:
                extras[r][row[extra]] += 1
    return counts, extras

def sum_by_region(path, offset_field, value_field):
    sums = defaultdict(int)
    with open(path, newline="") as f:
        for row in csv.DictReader(f):
            r = region_of(row[offset_field])
            sums[r] += int(row[value_field])
    return sums

def main():
    raw = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(".")
    out_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else Path(".")
    out_dir.mkdir(parents=True, exist_ok=True)

    entropy = load_entropy(raw / "entropy-16k.csv")
    comp_counts, comp_types = count_by_region(raw / "compression-signatures.csv", "offset", extra="type")
    ptr_counts, _ = count_by_region(raw / "pointer-candidates.csv", "target_offset")
    ptr_refsum = sum_by_region(raw / "pointer-candidates.csv", "target_offset", "reference_count")
    str_counts, _ = count_by_region(raw / "string-index.csv", "offset")
    str_lensum = sum_by_region(raw / "string-index.csv", "offset", "length")
    rep_counts, _ = count_by_region(raw / "repeated-16byte-blocks.csv", "offset")
    arm_counts, _ = count_by_region(raw / "arm-bl-targets.csv", "target_offset")
    thumb_counts, _ = count_by_region(raw / "thumb-bl-targets.csv", "target_offset")

    n_regions = max(entropy.keys()) + 1 if entropy else 2048

    out_path = out_dir / "region-profile.csv"
    with open(out_path, "w", newline="") as f:
        w = csv.writer(f)
        w.writerow([
            "region", "offset_start", "offset_end", "entropy", "zero_ratio", "ff_ratio",
            "comp_sig_count", "comp_lz77", "comp_huffman", "comp_rle",
            "ptr_target_count", "ptr_ref_sum",
            "string_count", "string_bytes",
            "repeated_block_count",
            "arm_bl_target_count", "thumb_bl_target_count",
            "classification", "confidence",
        ])
        for r in range(n_regions):
            e = entropy.get(r, {"entropy": 0.0, "zero_ratio": 0.0, "ff_ratio": 0.0})
            ct = comp_types.get(r, {})
            cls, conf = classify(
                e["entropy"], e["zero_ratio"], e["ff_ratio"],
                comp_counts.get(r, 0), ptr_counts.get(r, 0), ptr_refsum.get(r, 0),
                str_counts.get(r, 0), rep_counts.get(r, 0),
                arm_counts.get(r, 0), thumb_counts.get(r, 0),
            )
            w.writerow([
                r, hex(r * REGION_SIZE), hex((r + 1) * REGION_SIZE - 1),
                f"{e['entropy']:.4f}", f"{e['zero_ratio']:.4f}", f"{e['ff_ratio']:.4f}",
                comp_counts.get(r, 0), ct.get("LZ77", 0), ct.get("Huffman", 0), ct.get("RLE", 0),
                ptr_counts.get(r, 0), ptr_refsum.get(r, 0),
                str_counts.get(r, 0), str_lensum.get(r, 0),
                rep_counts.get(r, 0),
                arm_counts.get(r, 0), thumb_counts.get(r, 0),
                cls, conf,
            ])
    print(f"Écrit {out_path} ({n_regions} régions)")

def classify(entropy, zero_ratio, ff_ratio, comp_n, ptr_n, ptr_refsum, str_n, rep_n, arm_n, thumb_n):
    """Heuristique de classification par région. Retourne (label, confiance 0-100).
    Toute classification ici est une HYPOTHÈSE dérivée de statistiques, jamais une certitude.

    Seuils calibrés sur la distribution empirique des 2048 régions (percentiles), pas sur des
    valeurs absolues arbitraires — cf. docs/pokemon-unbound-analysis/rom-map.md pour le détail.

    Note : le signal `arm_n` (cibles BL ARM candidates) n'est PAS utilisé ici. Constat empirique :
    arm_n >= 69 sur 100% des régions, y compris les régions à très haute entropie où du vrai code ne
    devrait pas se trouver — signal très probablement saturé de faux positifs (tout mot aligné dont les
    bits correspondent par coïncidence à l'encodage d'une instruction BL ARM). Voir code-analysis.md.
    """
    # Seuils dérivés des percentiles empiriques (voir stats() dans docs/.../rom-map.md)
    STR_HIGH = 342      # p90 string_count
    COMP_HIGH = 347     # p90 comp_sig_count
    PTR_REFSUM_HIGH = 234   # p90 ptr_ref_sum
    PTR_N_MED = 64      # p75 ptr_target_count
    REP_HIGH = 463      # p90 repeated_block_count
    THUMB_HIGH = 74     # p90 thumb_bl_target_count
    COMP_LOW = 28       # p25 comp_sig_count

    # Padding : quasi tout un octet répété
    if zero_ratio > 0.9 or ff_ratio > 0.9:
        return "padding_probable", 85

    # Code Thumb : densité de cibles BL Thumb distinctes dans le top décile, peu de signatures de
    # compression, entropie dans la plage typique du code compilé (ni plate ni quasi-aléatoire)
    if thumb_n >= THUMB_HIGH and comp_n <= COMP_LOW and 4.5 <= entropy <= 7.3:
        conf = min(90, 45 + (thumb_n - THUMB_HIGH) // 4)
        return "code_thumb_probable", conf

    # Données compressées : entropie très haute + signatures de compression dans le top décile,
    # peu de chaînes (le texte n'est presque jamais compressé dans ces moteurs)
    if comp_n >= COMP_HIGH and entropy >= 7.3 and str_n < 40:
        conf = min(90, 40 + (comp_n - COMP_HIGH) // 100)
        return "donnees_compressees_probable", conf

    # Table de texte : densité de chaînes dans le top décile (pas juste "des chaînes", la médiane
    # est déjà à 116/région — il faut un net regroupement pour que ce soit un signal)
    if str_n >= STR_HIGH:
        conf = min(90, 50 + (str_n - STR_HIGH) // 20)
        return "table_texte_probable", conf

    # Table de pointeurs : cumul de références élevé (top décile) ET nombre de cibles distinctes
    # au-dessus de la médiane haute — beaucoup de pointeurs vers peu de cibles = table dense
    if ptr_n >= PTR_N_MED and ptr_refsum >= PTR_REFSUM_HIGH:
        conf = min(85, 45 + (ptr_refsum - PTR_REFSUM_HIGH) // 50)
        return "table_pointeurs_probable", conf

    # Structures répétées (dresseurs, espèces, objets...) : blocs de 16 octets très répétés,
    # au-delà du top décile, mais pas du padding pur (déjà exclu ci-dessus)
    if rep_n >= REP_HIGH:
        conf = min(70, 40 + (rep_n - REP_HIGH) // 30)
        return "structures_repetees_possible", conf

    if comp_n >= 100:
        return "zone_compressee_possible", 30

    if str_n >= 200:
        return "zone_texte_possible", 25

    return "zone_indeterminee", 10

if __name__ == "__main__":
    main()
