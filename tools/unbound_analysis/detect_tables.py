#!/usr/bin/env python3
"""Détecte des séquences régulièrement espacées parmi les cibles de pointeurs candidates
et parmi les blocs de 16 octets répétés, comme indices de tables de structures fixes.

Limite connue et assumée : pointer-candidates.csv ne donne QUE les adresses cibles et un
compteur de références agrégé — pas la position des pointeurs eux-mêmes. On ne peut donc
pas ici localiser un "en-tête de table de pointeurs" au sens strict, seulement repérer des
cibles individuellement référencées et régulièrement espacées (compatible avec un tableau
de structures de taille fixe, chacune référencée une fois depuis ailleurs).

Sortie : docs/pokemon-unbound-analysis/data/table-candidates.md
Relançable, ne modifie aucune entrée.
"""
import csv
import sys
from pathlib import Path
from collections import defaultdict

COMMON_STRIDES = [4, 8, 12, 16, 20, 24, 28, 32, 36, 40, 44, 48, 56, 64, 100, 128]
MIN_RUN = 5

def load_targets(path):
    targets = []
    with open(path, newline="") as f:
        for row in csv.DictReader(f):
            targets.append(int(row["target_offset"], 16))
    return sorted(set(targets))

def find_runs(sorted_offsets, stride, min_run=MIN_RUN):
    runs = []
    offset_set = set(sorted_offsets)
    used = set()
    for start in sorted_offsets:
        if start in used:
            continue
        run = [start]
        cur = start
        while (cur + stride) in offset_set:
            cur += stride
            run.append(cur)
        if len(run) >= min_run:
            runs.append((start, stride, len(run)))
            used.update(run)
    return runs

def load_repeated_blocks(path):
    offsets = []
    with open(path, newline="") as f:
        for row in csv.DictReader(f):
            offsets.append(int(row["offset"], 16))
    return sorted(set(offsets))

def find_dense_clusters(sorted_offsets, window=0x4000, min_count=20):
    """Regroupe les offsets en clusters denses (regard glissant simple)."""
    clusters = []
    i = 0
    n = len(sorted_offsets)
    while i < n:
        j = i
        while j < n and sorted_offsets[j] - sorted_offsets[i] <= window:
            j += 1
        count = j - i
        if count >= min_count:
            clusters.append((sorted_offsets[i], sorted_offsets[j - 1], count))
        i = j if j > i else i + 1
    return clusters

def main():
    raw = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(".")
    out_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else Path(".")
    out_dir.mkdir(parents=True, exist_ok=True)

    targets = load_targets(raw / "pointer-candidates.csv")
    all_runs = []
    for stride in COMMON_STRIDES:
        all_runs.extend(find_runs(targets, stride))
    all_runs.sort(key=lambda r: -r[2])

    rep_offsets = load_repeated_blocks(raw / "repeated-16byte-blocks.csv")
    clusters = find_dense_clusters(rep_offsets)
    clusters.sort(key=lambda c: -c[2])

    out_path = out_dir / "table-candidates.md"
    with open(out_path, "w") as f:
        f.write("# Candidats de tables à espacement régulier\n\n")
        f.write("Généré par `tools/unbound_analysis/detect_tables.py`. Deux méthodes distinctes, "
                "toutes deux des HYPOTHÈSES à valider par un désassemblage réel :\n\n")
        f.write("1. **Séquences de cibles de pointeurs à stride constant** — des adresses "
                "individuellement référencées par des pointeurs ailleurs dans la ROM, espacées "
                "d'un pas constant compatible avec une structure de taille fixe.\n")
        f.write("2. **Clusters denses de blocs de 16 octets répétés** — zones où au moins 20 blocs "
                "de 16 octets déjà vus ailleurs se reproduisent dans une fenêtre de 16 KiB.\n\n")

        f.write(f"## Séquences à stride constant (top {min(40, len(all_runs))} par longueur)\n\n")
        f.write("| offset début | pas (octets) | longueur | taille totale |\n|---|---|---|---|\n")
        for start, stride, length in all_runs[:40]:
            f.write(f"| {hex(start)} | {stride} | {length} | {hex(stride * length)} |\n")

        f.write(f"\n## Clusters denses de blocs répétés (top {min(30, len(clusters))})\n\n")
        f.write("| offset début | offset fin | nb blocs répétés dans la fenêtre |\n|---|---|---|\n")
        for start, end, count in clusters[:30]:
            f.write(f"| {hex(start)} | {hex(end)} | {count} |\n")

    print(f"Écrit {out_path} ({len(all_runs)} séquences, {len(clusters)} clusters)")

if __name__ == "__main__":
    main()
