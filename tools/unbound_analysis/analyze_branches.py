#!/usr/bin/env python3
"""Classe les cibles de branchement (BL) ARM/Thumb candidates par nombre de références.

Entrée : arm-bl-targets.csv, thumb-bl-targets.csv (artefacts d'analyse statique fournis).
Sortie : docs/pokemon-unbound-analysis/data/top-branch-targets.md — les cibles les plus
référencées de chaque jeu d'instructions, avec mise en garde explicite sur la fiabilité du
signal ARM (voir code-analysis.md pour la justification empirique).

Relançable, ne modifie aucune entrée.
"""
import csv
import sys
from pathlib import Path

def top_targets(path, n=30):
    rows = []
    with open(path, newline="") as f:
        for row in csv.DictReader(f):
            rows.append((int(row["reference_count"]), row["target_offset"], row["target_gba"]))
    rows.sort(reverse=True)
    return rows[:n]

def main():
    raw = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(".")
    out_dir = Path(sys.argv[2]) if len(sys.argv) > 2 else Path(".")
    out_dir.mkdir(parents=True, exist_ok=True)

    arm_top = top_targets(raw / "arm-bl-targets.csv")
    thumb_top = top_targets(raw / "thumb-bl-targets.csv")

    out_path = out_dir / "top-branch-targets.md"
    with open(out_path, "w") as f:
        f.write("# Cibles de branchement les plus référencées (candidates)\n\n")
        f.write("Généré par `tools/unbound_analysis/analyze_branches.py`. Ne provient pas d'un "
                "désassemblage réel — uniquement du comptage de motifs d'octets candidats fourni "
                "dans le pack d'analyse statique.\n\n")
        f.write("## Mise en garde sur le signal ARM\n\n")
        f.write("Toutes les régions de la ROM (2048/2048, cf. `region-profile.csv`) contiennent au "
                "moins 69 cibles BL ARM candidates, y compris les régions à très haute entropie où du "
                "vrai code ARM ne devrait structurellement pas se trouver (GBA : le code ARM natif ne "
                "représente qu'une fraction infime d'un jeu, l'essentiel étant en Thumb). **HYPOTHÈSE "
                "avec confiance élevée : ce signal est dominé par des faux positifs** (mots alignés "
                "dont les bits correspondent par coïncidence à l'encodage d'une instruction BL ARM). "
                "Le classement ci-dessous est fourni tel quel mais ne doit pas être interprété comme "
                "une liste de fonctions réelles sans validation par désassemblage.\n\n")
        f.write("## Top cibles Thumb BL (signal plus contrasté, cf. code-analysis.md)\n\n")
        f.write("| rang | offset ROM | adresse GBA | références |\n|---|---|---|---|\n")
        for i, (count, off, gba) in enumerate(thumb_top, 1):
            f.write(f"| {i} | {off} | {gba} | {count} |\n")
        f.write("\n## Top cibles ARM BL (signal probablement bruité, voir mise en garde ci-dessus)\n\n")
        f.write("| rang | offset ROM | adresse GBA | références |\n|---|---|---|---|\n")
        for i, (count, off, gba) in enumerate(arm_top, 1):
            f.write(f"| {i} | {off} | {gba} | {count} |\n")
    print(f"Écrit {out_path}")

if __name__ == "__main__":
    main()
