#!/usr/bin/env python3
"""
Map Agent — Heart & Soul (fork pokeemerald-expansion)
======================================================

Génère et valide les fichiers de données d'une map Pokémon GBA
(pokeemerald-expansion / pokefirered-style decomp) à partir d'une
spécification YAML lisible par un humain.

Ce script automatise la partie "plomberie de données" du pipeline :
    - data/maps/<Nom>/map.json
    - data/maps/<Nom>/scripts.inc  (squelette de scripts NPC/objets)
    - un snippet à fusionner dans data/layouts/layouts.json
    - un rapport de validation (warps, bornes, doublons, cohérence)

Ce que ce script NE fait PAS (limite volontaire, pas un oubli) :
    - Il ne peint AUCUN tile. Le placement visuel des tiles
      (blockdata / border.bin / map.bin) reste le travail de Porymap :
      c'est une tâche de composition visuelle, pas de données
      structurées, et aucun outil ne peut la déléguer proprement
      sans un moteur de rendu de tileset.
    - Il ne modifie pas automatiquement map_groups.json ni maps.s :
      ces fichiers sont des registres globaux du projet, les modifier
      sans revue humaine risquerait de casser la compilation d'autres
      maps.

Usage
-----
    python3 map_agent.py generate spec.yaml --out ./output
    python3 map_agent.py validate ./output/CinnabarLab

Format de la spec : voir example_spec.yaml et SPEC_FORMAT.md
"""

import argparse
import json
import re
import sys
from pathlib import Path

try:
    import yaml
except ImportError:
    sys.exit(
        "Le module PyYAML est requis : pip install pyyaml --break-system-packages"
    )


# ----------------------------------------------------------------------
# Tilesets réellement compilés pour un build HNS
# ----------------------------------------------------------------------
# Extrait le 2026-09-08 de src/data/tilesets/headers.h (branche
# claude/pokemon-heart-soul-audit-qv0f4n de doctor1-eng/test-gba),
# bloc "#elif IS_HNS" (lignes ~1543-2713).
#
# POURQUOI CETTE LISTE EXISTE : headers.h sépare les tilesets en 3
# branches #if/#elif MUTUELLEMENT EXCLUSIVES (générique / IS_FRLG /
# IS_HNS). Un tileset défini dans le bloc FRLG n'existe PAS dans le
# binaire d'un build HNS, même si le map.json/layout le référence
# sans erreur de syntaxe. C'est exactement ce qui a fait planter le
# jeu (reboot immédiat à l'entrée) sur les 3 premiers bâtiments de
# Cinnabar avant correction — voir docs/heart_and_soul/technical_map.md.
# Aucun audit JSON ne peut détecter ce piège : il faut connaître la
# liste réelle des tilesets compilés pour la variante ciblée.
#
# À RAFRAÎCHIR si headers.h évolue (nouveaux tilesets ajoutés au jeu) :
#   grep -n "IS_HNS\|IS_FRLG\|#if\|#elif\|#endif" src/data/tilesets/headers.h
#   puis extraire les symboles gTileset_* entre la ligne "#elif IS_HNS"
#   et le "#endif" qui suit.
HNS_COMPILED_TILESETS = frozenset({
    "gTileset_AlolaIsland", "gTileset_AlolaIslandSecondary", "gTileset_ArceusRoom_Hns",
    "gTileset_AzaleaTown_Gym_Hns", "gTileset_AzaleaTown_Hns", "gTileset_Barn_Hns",
    "gTileset_BellchimeTrail_Hns", "gTileset_BikeShop_Hns", "gTileset_BlackthornGym_Hns",
    "gTileset_Blackthorn_Hns", "gTileset_BurnedTower_Hns", "gTileset_Cafe_Hns",
    "gTileset_Cave_Default_Hns", "gTileset_Cave_DragonsDen_Hns", "gTileset_Cave_Gray_Hns",
    "gTileset_Cave_Green_Hns", "gTileset_Cave_Ice_Hns", "gTileset_Cave_MtMoon_Hns",
    "gTileset_Cave_Sandy_Hns", "gTileset_CeladonApartments_Hns", "gTileset_CeladonCity_Hns",
    "gTileset_CeruleanCity_Gym_Hns", "gTileset_CeruleanCity_Hns", "gTileset_CherrygroveCity_Hns",
    "gTileset_CianwoodCity_Gym_Hns", "gTileset_CianwoodCity_Hns", "gTileset_CyclingRoad_Hns",
    "gTileset_DepartmentStore_Hns", "gTileset_DragonsDen_Shrine_Hns", "gTileset_EcruteakCity_Gym_Hns",
    "gTileset_EcruteakTheater_Hns", "gTileset_Ecruteak_City_Hns", "gTileset_FuchsiaCity_Gym_Hns",
    "gTileset_Fuchsia_Hns", "gTileset_GameCorner_Hns", "gTileset_Gate_Standard_Hns",
    "gTileset_General_Hns", "gTileset_GoldenrodCity_TrainStation_Hns",
    "gTileset_GoldenrodUndergroundRocket_Hns", "gTileset_GoldenrodUndergroundTunnel_Hns",
    "gTileset_Goldenrod_Hns", "gTileset_Goldenrod_Underground_Storage_Hns", "gTileset_HallOfFame_Hns",
    "gTileset_House_2_Hns", "gTileset_House_Lab_Hns", "gTileset_IlexForest_Hns",
    "gTileset_IndigoPlateau_Hns", "gTileset_JohtoBikeShop_Hns", "gTileset_JohtoMart_Hns",
    "gTileset_Johto_Building_Hns", "gTileset_Johto_General_Hns", "gTileset_Johto_NorthEast_Hns",
    "gTileset_Johto_NorthWest_Hns", "gTileset_Johto_South_Hns", "gTileset_KantoMart_Hns",
    "gTileset_Kanto_Building_Hns", "gTileset_Kanto_General_Hns", "gTileset_Kanto_PokemonCenter_Hns",
    "gTileset_KurtsHouse_Hns", "gTileset_Lavaridge_Hns", "gTileset_LavenderTown_Hns",
    "gTileset_Lighthouse_Hns", "gTileset_MahoganyTown_Hns", "gTileset_MtEmber_Hns",
    "gTileset_MtSilverSnow_Hns", "gTileset_MtSilver_Ancient_Hns", "gTileset_MtSilver_Exp_Hns",
    "gTileset_MtSilver_NewSinjoh_Hns", "gTileset_Museum_Hns", "gTileset_NationalPark_Hns",
    "gTileset_NewBarkTown_Hns", "gTileset_OlivineCity_Hns", "gTileset_PalletTown_Hns",
    "gTileset_PewterCity_Hns", "gTileset_PlayersHouse_Hns", "gTileset_PokemonCenter_White_Hns",
    "gTileset_PokemonDayCare_Hns", "gTileset_PokemonLeague_Hns", "gTileset_PortIndoor_Hns",
    "gTileset_PowerPlant_GeneratorRoom_Hns", "gTileset_Route32_Hns", "gTileset_Route38_Farmland_Hns",
    "gTileset_Route40_Hns", "gTileset_RuinsOfAlphWriting_Hns", "gTileset_RuinsOfAlph_B1F_Hns",
    "gTileset_RuinsOfAlph_Outside_Hns", "gTileset_SafariZoneJohto_Hns", "gTileset_SafariZone_Entrance_Hns",
    "gTileset_SaffronCity_FightingDojoVIP_Hns", "gTileset_SaffronCity_Gym_Hns", "gTileset_SaffronCity_Hns",
    "gTileset_SeaCottage_Hns", "gTileset_ShopRooftop_Hns", "gTileset_SilphCo_Hns",
    "gTileset_SootopolisGym_Hns", "gTileset_SoulHouse_Hns", "gTileset_TrainerHill_Courtyard_Hns",
    "gTileset_TrainerSchool_Hns", "gTileset_VermilionCity_Gym_Hns", "gTileset_Vermilion_Hns",
    "gTileset_VioletCity_Hns", "gTileset_ViridianCity_Gym_Hns", "gTileset_ViridianCity_Hns",
    "gTileset_ViridianForest_Hns", "gTileset_WhirlIslands_Hns", "gTileset_ssaqua_Hns",
})


# ----------------------------------------------------------------------
# Chargement et helpers
# ----------------------------------------------------------------------

def load_spec(path: str) -> dict:
    with open(path, "r", encoding="utf-8") as f:
        data = yaml.safe_load(f)
    if "map" not in data:
        raise ValueError("La spec doit contenir une clé racine 'map'.")
    return data["map"]


def pascal_case(name: str) -> str:
    return re.sub(r"[^0-9a-zA-Z]", "", name)


def default_id(name: str) -> str:
    # CinnabarLab -> MAP_CINNABAR_LAB
    snake = re.sub(r"(?<!^)(?=[A-Z])", "_", name).upper()
    return f"MAP_{snake}"


def default_layout_id(name: str) -> str:
    snake = re.sub(r"(?<!^)(?=[A-Z])", "_", name).upper()
    return f"LAYOUT_{snake}"


# ----------------------------------------------------------------------
# Génération : map.json
# ----------------------------------------------------------------------

def build_object_events(spec: dict) -> list:
    events = []
    for npc in spec.get("npcs", []):
        events.append({
            "graphics_id": npc["graphics"],
            "x": npc["x"],
            "y": npc["y"],
            "elevation": npc.get("elevation", 0),
            "movement_type": npc.get("movement_type", "MOVEMENT_TYPE_FACE_DOWN"),
            "movement_range_x": npc.get("movement_range_x", 0),
            "movement_range_y": npc.get("movement_range_y", 0),
            "trainer_type": npc.get("trainer_type", "TRAINER_TYPE_NONE"),
            "trainer_sight_or_berry_tree_id": npc.get("trainer_sight", "0"),
            "script": npc.get("script_label", f"{spec['name']}_EventScript_Npc{npc.get('local_id', '')}"),
            "flag": npc.get("flag", "0"),
        })
    for item in spec.get("items", []):
        events.append({
            "graphics_id": "OBJ_EVENT_GFX_ITEM_BALL",
            "x": item["x"],
            "y": item["y"],
            "elevation": item.get("elevation", 0),
            "movement_type": "MOVEMENT_TYPE_LOOK_AROUND",
            "movement_range_x": 1,
            "movement_range_y": 1,
            "trainer_type": "TRAINER_TYPE_NONE",
            "trainer_sight_or_berry_tree_id": item["item"],
            "script": "Common_EventScript_FindItem",
            "flag": item["flag"],
        })
    return events


def build_warp_events(spec: dict) -> list:
    warps = []
    for w in spec.get("warps", []):
        warps.append({
            "x": w["x"],
            "y": w["y"],
            "elevation": w.get("elevation", 0),
            "dest_map": w["dest_map"],
            "dest_warp_id": str(w.get("dest_warp_id", 0)),
        })
    return warps


def build_map_json(spec: dict) -> dict:
    name = spec["name"]
    return {
        "id": spec.get("id", default_id(name)),
        "name": name,
        "layout": spec.get("layout_id", default_layout_id(name)),
        "music": spec.get("music", "MUS_DUMMY"),
        "region": spec.get("region", "REGION_KANTO"),
        "region_map_section": spec.get("region_map_section", "MAPSEC_NONE"),
        "requires_flash": spec.get("requires_flash", False),
        "weather": spec.get("weather", "WEATHER_NONE"),
        "map_type": spec.get("map_type", "MAP_TYPE_INDOOR"),
        "allow_cycling": spec.get("allow_cycling", False),
        "allow_escaping": spec.get("allow_escaping", False),
        "allow_running": spec.get("allow_running", False),
        "show_map_name": spec.get("show_map_name", True),
        "battle_scene": spec.get("battle_scene", "MAP_BATTLE_SCENE_NORMAL"),
        "connections": spec.get("connections") or None,
        "object_events": build_object_events(spec),
        "warp_events": build_warp_events(spec),
        "coord_events": [],
        "bg_events": [],
        "game_version": spec.get("game_version", "hns"),
    }


def build_layout_entry(spec: dict) -> dict:
    name = spec["name"]
    layout = spec["layout"]
    game_version = spec.get("game_version", "hns")
    entry = {
        "id": spec.get("layout_id", default_layout_id(name)),
        "name": f"{name}_Layout",
        "width": layout["width"],
        "height": layout["height"],
        "primary_tileset": layout["primary_tileset"],
        "secondary_tileset": layout["secondary_tileset"],
        "border_filepath": f"data/layouts/{name}/border.bin",
        "blockdata_filepath": f"data/layouts/{name}/map.bin",
        "game_version": game_version,
        "layout_version": layout.get("layout_version", game_version),
    }
    if "border_width" in layout:
        entry["border_width"] = layout["border_width"]
    if "border_height" in layout:
        entry["border_height"] = layout["border_height"]
    return entry


# ----------------------------------------------------------------------
# Génération : scripts.inc (squelette assembleur pokeemerald)
# ----------------------------------------------------------------------

def build_scripts_inc(spec: dict) -> str:
    name = spec["name"]
    lines = [f"{name}_MapScripts::", "\t.byte 0", ""]

    for npc in spec.get("npcs", []):
        label = npc.get("script_label", f"{name}_EventScript_Npc{npc.get('local_id', '')}")
        dialogue = npc.get("dialogue", "TODO: écrire le dialogue.")
        text_label = f"{name}_Text_Npc{npc.get('local_id', '')}"
        lines += [
            f"{label}::",
            "\tlock",
            "\tfaceplayer",
            f"\tmsgbox {text_label}, MSGBOX_DEFAULT",
            "\trelease",
            "\tend",
            "",
        ]

    lines.append(f"@ --- Textes NPC ({name}) ---")
    lines.append(".text")
    for npc in spec.get("npcs", []):
        text_label = f"{name}_Text_Npc{npc.get('local_id', '')}"
        dialogue = npc.get("dialogue", "TODO: écrire le dialogue.")
        lines += [f"{text_label}::", f'\t.string "{dialogue}$"', ""]

    return "\n".join(lines)


# ----------------------------------------------------------------------
# Validation
# ----------------------------------------------------------------------

def validate_spec(spec: dict) -> list:
    """Valide la spec AVANT génération. Retourne une liste de (niveau, message)."""
    issues = []
    layout = spec.get("layout", {})
    w, h = layout.get("width"), layout.get("height")

    if not w or not h:
        issues.append(("ERREUR", "layout.width et layout.height sont requis."))
        return issues

    # --- Garde-fou n°1 : tileset réellement compilé pour la variante ciblée ---
    # C'est le bug qui a fait planter le jeu sur Gym/Manoir/Labo de Cinnabar
    # (voir docs/heart_and_soul/technical_map.md). headers.h compile les
    # tilesets dans des branches #if/#elif mutuellement exclusives par
    # game_version : un tileset FRLG n'existe simplement pas dans un
    # binaire HNS, même si le JSON est syntaxiquement parfait.
    game_version = spec.get("game_version", "hns")
    if game_version == "hns":
        for field in ("primary_tileset", "secondary_tileset"):
            tileset = layout.get(field)
            if tileset and tileset not in HNS_COMPILED_TILESETS:
                issues.append((
                    "ERREUR",
                    f"layout.{field} = '{tileset}' n'est PAS dans la liste des tilesets "
                    f"compilés pour un build HNS (voir src/data/tilesets/headers.h, bloc "
                    f"#elif IS_HNS). Utiliser ce tileset fera COMPILER le projet sans erreur "
                    f"mais PLANTERA le jeu à l'entrée sur cette map (reboot immédiat) — "
                    f"c'est exactement le bug déjà rencontré sur Gym/Manoir/Labo de Cinnabar. "
                    f"Choisir un tileset dans HNS_COMPILED_TILESETS (ex: gTileset_Johto_Building_Hns "
                    f"+ gTileset_House_Lab_Hns pour un intérieur de type labo/maison)."
                ))
    else:
        issues.append((
            "AVERTISSEMENT",
            f"game_version='{game_version}' : la vérification de compatibilité tileset "
            f"n'est faite que pour 'hns'. Vérifiez manuellement contre headers.h."
        ))

    def in_bounds(x, y):
        return 0 <= x < w and 0 <= y < h

    seen_coords = {}

    def check_point(x, y, label):
        if not in_bounds(x, y):
            issues.append(("ERREUR", f"{label} en ({x},{y}) hors des bornes de la map ({w}x{h})."))
        key = (x, y)
        if key in seen_coords:
            issues.append(("AVERTISSEMENT", f"{label} et {seen_coords[key]} partagent la même case ({x},{y})."))
        seen_coords[key] = label

    npc_ids = set()
    for npc in spec.get("npcs", []):
        lid = npc.get("local_id")
        if lid is None:
            issues.append(("ERREUR", "Un NPC n'a pas de local_id."))
        elif lid in npc_ids:
            issues.append(("ERREUR", f"local_id {lid} dupliqué entre plusieurs NPC."))
        else:
            npc_ids.add(lid)
        check_point(npc.get("x", -1), npc.get("y", -1), f"NPC #{lid}")
        if "graphics" not in npc:
            issues.append(("ERREUR", f"NPC #{lid} : champ 'graphics' manquant."))

    for i, item in enumerate(spec.get("items", [])):
        check_point(item.get("x", -1), item.get("y", -1), f"Item #{i} ({item.get('item', '?')})")
        if "flag" not in item:
            issues.append(("ERREUR", f"Item #{i} : un FLAG_ITEM_... unique est requis (sinon il réapparaît infiniment)."))

    dest_maps_seen = set()
    for i, warp in enumerate(spec.get("warps", [])):
        check_point(warp.get("x", -1), warp.get("y", -1), f"Warp #{i}")
        dest = warp.get("dest_map", "")
        if not dest.startswith("MAP_"):
            issues.append(("ERREUR", f"Warp #{i} : dest_map '{dest}' ne suit pas la convention MAP_XXX."))
        if "FRLG" in dest.upper():
            issues.append((
                "AVERTISSEMENT",
                f"Warp #{i} vers '{dest}' cible une map _Frlg. Rappel du piège documenté "
                f"(technical_map.md) : une géométrie _Frlg n'est PAS automatiquement "
                f"réutilisable pour un build HNS — vérifier d'abord que son tileset "
                f"apparaît dans HNS_COMPILED_TILESETS avant de considérer ce warp comme sûr."
            ))
        dest_maps_seen.add(dest)

    for conn in spec.get("connections", []) or []:
        if isinstance(conn, dict) and "FRLG" in conn.get("map", "").upper():
            issues.append((
                "AVERTISSEMENT",
                f"connections vers '{conn.get('map')}' cible une map _Frlg — même mise en garde "
                f"que pour les warps (voir ci-dessus)."
            ))

    reqs = spec.get("requirements", {})
    if "entrances" in reqs and len(spec.get("warps", [])) != reqs["entrances"]:
        issues.append(("AVERTISSEMENT",
                        f"requirements.entrances={reqs['entrances']} mais {len(spec.get('warps', []))} warp(s) définis."))
    if "npcs" in reqs and len(spec.get("npcs", [])) != reqs["npcs"]:
        issues.append(("AVERTISSEMENT",
                        f"requirements.npcs={reqs['npcs']} mais {len(spec.get('npcs', []))} NPC définis."))
    if "items" in reqs and len(spec.get("items", [])) != reqs["items"]:
        issues.append(("AVERTISSEMENT",
                        f"requirements.items={reqs['items']} mais {len(spec.get('items', []))} item(s) définis."))

    if not spec.get("warps"):
        issues.append(("AVERTISSEMENT", "Aucun warp défini : la map sera inaccessible en jeu."))

    return issues


def format_report(spec_name: str, issues: list) -> str:
    if not issues:
        return f"# Rapport de validation — {spec_name}\n\n✅ Aucun problème détecté.\n"
    lines = [f"# Rapport de validation — {spec_name}", ""]
    errors = [i for i in issues if i[0] == "ERREUR"]
    warns = [i for i in issues if i[0] == "AVERTISSEMENT"]
    lines.append(f"**{len(errors)} erreur(s), {len(warns)} avertissement(s)**")
    lines.append("")
    for level, msg in issues:
        marker = "🔴" if level == "ERREUR" else "🟡"
        lines.append(f"- {marker} **{level}** — {msg}")
    return "\n".join(lines) + "\n"


# ----------------------------------------------------------------------
# Commandes CLI
# ----------------------------------------------------------------------

def cmd_generate(args):
    spec = load_spec(args.spec)
    name = spec["name"]

    issues = validate_spec(spec)
    errors = [i for i in issues if i[0] == "ERREUR"]

    out_dir = Path(args.out) / name
    out_dir.mkdir(parents=True, exist_ok=True)

    report = format_report(name, issues)
    (out_dir / "VALIDATION.md").write_text(report, encoding="utf-8")

    if errors:
        print(report)
        print(f"❌ Génération bloquée : {len(errors)} erreur(s) à corriger dans la spec avant de continuer.")
        sys.exit(1)

    map_json = build_map_json(spec)
    (out_dir / "map.json").write_text(json.dumps(map_json, indent=2, ensure_ascii=False) + "\n", encoding="utf-8")

    scripts_inc = build_scripts_inc(spec)
    (out_dir / "scripts.inc").write_text(scripts_inc, encoding="utf-8")

    layout_entry = build_layout_entry(spec)
    (out_dir / "layout_snippet.json").write_text(
        json.dumps(layout_entry, indent=2, ensure_ascii=False) + "\n", encoding="utf-8"
    )

    next_steps = f"""# Prochaines étapes manuelles — {name}

Ce que le Map Agent a généré :
- `map.json` — prêt à copier dans `data/maps/{name}/map.json`
- `scripts.inc` — squelette de scripts, à compléter et copier dans `data/maps/{name}/scripts.inc`
- `layout_snippet.json` — entrée à fusionner dans `data/layouts/layouts.json` (tableau `layouts`)

Ce qui reste à faire à la main (non automatisable sans Porymap) :
1. Créer le dossier `data/layouts/{name}/` et peindre les tiles dans Porymap
   pour générer `border.bin` et `map.bin`.
2. Ajouter `"{name}"` à la liste des maps dans `data/maps/map_groups.json`
   (dans le bon groupe) et l'entrée correspondante dans `data/maps/maps.s`.
3. Vérifier dans Porymap que la map s'ouvre sans erreur et que les warps
   listés ci-dessus pointent vers des cases valides sur la map de destination.
4. Ajouter les warps retour sur la/les map(s) de destination si nécessaire
   (ce script ne génère qu'un sens du warp).
5. Compiler (`make`) et tester dans mgba.

{report}
"""
    (out_dir / "NEXT_STEPS.md").write_text(next_steps, encoding="utf-8")

    print(f"✅ Map '{name}' générée dans {out_dir}/")
    if any(i[0] == "AVERTISSEMENT" for i in issues):
        print(f"⚠️  {len([i for i in issues if i[0]=='AVERTISSEMENT'])} avertissement(s) — voir VALIDATION.md")


def cmd_validate(args):
    map_dir = Path(args.map_dir)
    map_json_path = map_dir / "map.json"
    if not map_json_path.exists():
        sys.exit(f"Introuvable : {map_json_path}")

    map_json = json.loads(map_json_path.read_text(encoding="utf-8"))
    issues = []

    required_top = ["id", "name", "layout", "warp_events", "object_events"]
    for key in required_top:
        if key not in map_json:
            issues.append(("ERREUR", f"Clé manquante dans map.json : '{key}'"))

    if not map_json.get("warp_events"):
        issues.append(("AVERTISSEMENT", "Aucun warp_event : la map sera inaccessible en jeu."))

    for w in map_json.get("warp_events", []):
        dest = w.get("dest_map", "")
        if not dest.startswith("MAP_"):
            issues.append(("ERREUR", f"warp_event vers '{dest}' ne suit pas la convention MAP_XXX."))

    seen = {}
    for ev in map_json.get("object_events", []):
        key = (ev.get("x"), ev.get("y"))
        if key in seen:
            issues.append(("AVERTISSEMENT", f"Deux object_events partagent la case {key}."))
        seen[key] = ev.get("graphics_id")

    report = format_report(map_json.get("name", map_dir.name), issues)
    print(report)
    if any(i[0] == "ERREUR" for i in issues):
        sys.exit(1)


def main():
    parser = argparse.ArgumentParser(description="Map Agent — Heart & Soul (pokeemerald-expansion)")
    sub = parser.add_subparsers(dest="command", required=True)

    p_gen = sub.add_parser("generate", help="Génère les fichiers d'une map à partir d'une spec YAML")
    p_gen.add_argument("spec", help="Chemin vers le fichier spec.yaml")
    p_gen.add_argument("--out", default="./output", help="Dossier de sortie (défaut: ./output)")
    p_gen.set_defaults(func=cmd_generate)

    p_val = sub.add_parser("validate", help="Valide un map.json déjà généré ou existant")
    p_val.add_argument("map_dir", help="Dossier contenant map.json")
    p_val.set_defaults(func=cmd_validate)

    args = parser.parse_args()
    args.func(args)


if __name__ == "__main__":
    main()
