"""search-tiles: rank community tile packs against a free-text keyword.

Design constraints from the product decision ("patchwork d'assets
communautaires" en priorite, generation seulement en secours):

- Never import anything by itself. This module only lists and ranks.
- Be honest about what it can and cannot verify: license text shown here is
  a *starting point* pulled from data/community_index.json (curated once via
  web research, see that file's _meta.last_reviewed) -- the tool always
  tells the user to reconfirm on the source page, and import-tile forces a
  human-typed --license value into the ledger regardless of what's cached
  here.
- Style/palette compatibility against the project's existing tilesets is
  computed for real when a preview image is available locally; otherwise it
  is reported as unavailable rather than guessed.
"""
from __future__ import annotations

import difflib
import json
import os
from dataclasses import dataclass
from typing import Dict, List, Optional, Tuple

from .png_codec import read_png

_INDEX_PATH = os.path.join(os.path.dirname(__file__), "data", "community_index.json")

# Petit lexique FR -> tags EN de l'index, pour que "route forestière" /
# "intérieur pokécenter" matchent les tags anglais du catalogue communautaire.
_FR_SYNONYMS: Dict[str, List[str]] = {
    "route": ["route", "overworld", "path"],
    "foret": ["forest", "trees"],
    "forestiere": ["forest"],
    "interieur": ["interior", "indoor"],
    "pokecenter": ["pokecenter", "interior", "indoor", "building interior"],
    "centre": ["pokecenter", "interior"],
    "pokemon": ["pokemon-style", "overworld"],
    "grotte": ["cave", "dungeon", "underground"],
    "caverne": ["cave", "underground"],
    "donjon": ["dungeon", "cave"],
    "ville": ["town", "village"],
    "village": ["town", "village"],
    "maison": ["house", "building"],
    "batiment": ["building", "house"],
    "eau": ["water"],
    "herbe": ["grass", "overworld"],
    "exterieur": ["exterior", "overworld"],
}


def _normalize(word: str) -> str:
    repl = str.maketrans("éèêëàâäîïôöùûüç", "eeeeaaaiioouuuc")
    return word.lower().translate(repl)


def _expand_query(keyword: str) -> List[str]:
    words = [_normalize(w) for w in keyword.replace(",", " ").split()]
    expanded = set(words)
    for w in words:
        expanded.update(_FR_SYNONYMS.get(w, []))
    return sorted(expanded)


def load_index() -> List[dict]:
    with open(_INDEX_PATH, "r", encoding="utf-8") as f:
        data = json.load(f)
    return data["entries"]


def _keyword_score(query_terms: List[str], entry: dict) -> float:
    haystack = " ".join(
        [entry["title"]] + entry.get("tags", []) + [entry.get("notes", "")]
    )
    haystack_norm = _normalize(haystack)
    hits = 0
    for term in query_terms:
        if term in haystack_norm:
            hits += 1
        else:
            # fuzzy fallback for near-misses (plurals, accents already stripped)
            close = difflib.get_close_matches(term, haystack_norm.split(), n=1, cutoff=0.8)
            if close:
                hits += 0.5
    return hits / max(len(query_terms), 1)


@dataclass
class ImageStyleStats:
    num_colors: int
    avg_saturation: float  # 0..1
    avg_value: float  # 0..1 (brightness)


def analyze_image_style(png_path: str) -> ImageStyleStats:
    img = read_png(png_path)
    rgb_rows = img.to_rgb_rows()
    colors = set()
    sat_sum = val_sum = 0.0
    n = 0
    for row in rgb_rows:
        for (r, g, b) in row:
            colors.add((r, g, b))
            mx, mn = max(r, g, b) / 255.0, min(r, g, b) / 255.0
            val_sum += mx
            sat_sum += 0.0 if mx == 0 else (mx - mn) / mx
            n += 1
    return ImageStyleStats(
        num_colors=len(colors),
        avg_saturation=sat_sum / max(n, 1),
        avg_value=val_sum / max(n, 1),
    )


def style_distance(a: ImageStyleStats, b: ImageStyleStats) -> float:
    """Lower is more compatible. Combines palette-size gap (normalized to
    the 16-color GBA budget) and hue/brightness feel."""
    color_gap = abs(a.num_colors - b.num_colors) / 16.0
    sat_gap = abs(a.avg_saturation - b.avg_saturation)
    val_gap = abs(a.avg_value - b.avg_value)
    return color_gap + sat_gap + val_gap


@dataclass
class RankedResult:
    entry: dict
    keyword_score: float
    style_score: Optional[float]  # None if no preview available to compare


def search(
    keyword: str,
    project_reference_png: Optional[str] = None,
    preview_cache_dir: Optional[str] = None,
) -> List[RankedResult]:
    terms = _expand_query(keyword)
    entries = load_index()

    project_stats = None
    if project_reference_png and os.path.exists(project_reference_png):
        project_stats = analyze_image_style(project_reference_png)

    results = []
    for entry in entries:
        kscore = _keyword_score(terms, entry)
        if kscore <= 0:
            continue
        style_score = None
        if project_stats and preview_cache_dir:
            preview_path = os.path.join(preview_cache_dir, f"{entry['id']}.png")
            if os.path.exists(preview_path):
                try:
                    cand_stats = analyze_image_style(preview_path)
                    style_score = style_distance(project_stats, cand_stats)
                except Exception:
                    style_score = None
        results.append(RankedResult(entry=entry, keyword_score=kscore, style_score=style_score))

    def sort_key(r: RankedResult) -> Tuple[float, float]:
        # Best keyword match first; among ties, lowest (best) style distance,
        # unknown style pushed after known ones.
        style_component = r.style_score if r.style_score is not None else 999.0
        return (-r.keyword_score, style_component)

    results.sort(key=sort_key)
    return results


def format_results(results: List[RankedResult], project_reference_png: Optional[str]) -> str:
    if not results:
        return (
            "Aucun resultat dans le catalogue local pour ce mot-cle.\n"
            "-> Prochaine etape recommandee : generer une tile de secours avec "
            "`generate-fallback`, ou elargir la recherche a la main sur "
            "opengameart.org / itch.io puis l'ajouter au catalogue local."
        )
    lines = []
    for i, r in enumerate(results, 1):
        e = r.entry
        lines.append(f"{i}. {e['title']}  [{e['id']}]")
        lines.append(f"   Source     : {e['url']}")
        lines.append(
            f"   Licence    : {e['declared_license']} (confiance: {e['license_confidence']})"
        )
        lines.append(f"   Tags       : {', '.join(e.get('tags', []))}")
        lines.append(f"   Pertinence mot-cle : {r.keyword_score:.2f}")
        if r.style_score is not None:
            lines.append(f"   Compatibilite style vs projet : {r.style_score:.3f} (plus bas = plus proche)")
        elif project_reference_png:
            lines.append(
                "   Compatibilite style : non calculable (pas d'apercu en cache -- "
                f"placer un PNG dans le dossier de cache sous {e['id']}.png)"
            )
        else:
            lines.append("   Compatibilite style : non calculee (aucun --project-tileset fourni)")
        if e.get("notes"):
            lines.append(f"   Notes      : {e['notes']}")
        lines.append("")
    lines.append(
        "Rappel : aucune de ces tiles n'est importee automatiquement. Verifie la "
        "licence sur la page source, puis utilise `import-tile` avec --source/--license/--keyword "
        "pour convertir et journaliser l'import dans ASSETS_SOURCES.md."
    )
    return "\n".join(lines)
