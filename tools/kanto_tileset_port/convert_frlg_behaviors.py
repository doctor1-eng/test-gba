#!/usr/bin/env python3
"""Reencode les valeurs de Behavior FRLG (MB_FRLG_*) d'un metatile_attributes.bin FRLG
vers la numerotation MB_* unifiee utilisee par ce moteur (voir include/constants/
metatile_behaviors.h et metatile_behaviors_frlg.h), SANS changer le format binaire
(reste du FRLG 32 bits/metatile : Behavior[0:9) | TerrainType[9:14) | EncounterType[24:27)
| LayerType[29:31)). Necessaire car isFrlg=TRUE est conserve (voir mapjson.cpp,
"metatile_format": "frlg") pour garder la largeur d'attributs et la frontiere
primaire/secondaire (640/640/7) fideles a la source Kanto - seule la VALEUR du
Behavior differe entre les deux numerotations, pas le format.

Reprend la table de correspondance officielle de
engine/migration_scripts/frlg_metatile_behavior_converter.py (fournie par
pokeemerald-expansion) sans la modifier.

Usage : python3 convert_frlg_behaviors.py <in.bin> <out.bin>
"""
import struct
import sys
sys.path.insert(0, "/home/user/test-gba/engine/migration_scripts")
from frlg_metatile_behavior_converter import FRLG_BEHAVIORS, EMERALD_BEHAVIORS, FRLG_TO_EMERALD, BEHAVIOR_MASK

def convert(in_path, out_path):
    data = open(in_path, "rb").read()
    n = len(data) // 4
    values = struct.unpack(f"<{n}I", data)
    out = []
    unknown = 0
    for value in values:
        frlg_behavior = value & BEHAVIOR_MASK
        if frlg_behavior not in FRLG_BEHAVIORS:
            # Valeur absente a la fois de la table de migration officielle ET de
            # metatile_behaviors_frlg.h : un trou de numerotation FRLG (jamais nomme),
            # verifie visuellement sur ce tileset (panneaux MART/P.C, jetees/ponts en bois
            # sur l'eau) - MB_NORMAL (aucun mecanisme special) est le choix sur, pas un
            # comportement arbitraire coincidant avec la valeur brute inchangee.
            unknown += 1
            emerald_behavior = EMERALD_BEHAVIORS['MB_NORMAL']
        else:
            emerald_behavior = EMERALD_BEHAVIORS[FRLG_TO_EMERALD[FRLG_BEHAVIORS[frlg_behavior]]]
        new_value = emerald_behavior | (value & ~BEHAVIOR_MASK)
        out.append(new_value)
    open(out_path, "wb").write(struct.pack(f"<{len(out)}I", *out))
    print(f"[ok] {in_path} -> {out_path} : {n} metatiles, {unknown} valeurs de behavior inconnues -> MB_NORMAL")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print(__doc__)
        sys.exit(1)
    convert(sys.argv[1], sys.argv[2])
