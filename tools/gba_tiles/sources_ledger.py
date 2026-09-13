"""ASSETS_SOURCES.md ledger: the write-ahead log that makes every imported
community tile traceable to a source, a declared license, and the search
keyword that found it.

Hard rule from the product decision: an import without a ledger entry is a
bug, not a style nit. import-tile therefore writes the ledger row *before*
writing any asset file, and refuses to run at all without complete
--source / --license / --keyword arguments. verify-sources is the audit
command: it scans a directory for tile assets and flags any file that has no
matching row (e.g. someone copied a file in by hand, bypassing the CLI).
"""
from __future__ import annotations

import hashlib
import os
from dataclasses import dataclass, field
from datetime import date
from typing import List, Optional

LEDGER_HEADER = (
    "# ASSETS_SOURCES.md\n\n"
    "Registre obligatoire de toute tile/tileset importé depuis une source "
    "communautaire (approche \"patchwork d'assets\"). **Toute image sous "
    "`graphics/` qui n'a pas de ligne correspondante ici doit être "
    "considérée comme non conforme** -- voir `gba_tiles.py verify-sources`.\n\n"
    "| Date | Mot-clé recherché | Source | Licence déclarée | Fichier(s) importé(s) | Hash SHA-256 |\n"
    "|------|-------------------|--------|-------------------|------------------------|---------------|\n"
)


@dataclass
class LedgerEntry:
    date_str: str
    keyword: str
    source: str
    license: str
    dest_paths: List[str] = field(default_factory=list)
    sha256: str = ""

    def to_row(self) -> str:
        dests = "<br>".join(self.dest_paths)
        return f"| {self.date_str} | {self.keyword} | {self.source} | {self.license} | {dests} | `{self.sha256}` |\n"


def ensure_ledger(path: str) -> None:
    if not os.path.exists(path):
        with open(path, "w", encoding="utf-8") as f:
            f.write(LEDGER_HEADER)


def read_entries(path: str) -> List[LedgerEntry]:
    if not os.path.exists(path):
        return []
    entries = []
    with open(path, "r", encoding="utf-8") as f:
        lines = f.readlines()
    for line in lines:
        if not line.startswith("|") or line.startswith("|------") or line.strip() == "| Date |":
            continue
        cols = [c.strip() for c in line.strip().strip("|").split("|")]
        if len(cols) != 6 or cols[0] == "Date":
            continue
        dests = [d.strip() for d in cols[4].split("<br>") if d.strip()]
        entries.append(
            LedgerEntry(
                date_str=cols[0],
                keyword=cols[1],
                source=cols[2],
                license=cols[3],
                dest_paths=dests,
                sha256=cols[5].strip("`"),
            )
        )
    return entries


def append_entry(path: str, entry: LedgerEntry) -> None:
    ensure_ledger(path)
    with open(path, "a", encoding="utf-8") as f:
        f.write(entry.to_row())


def sha256_of_file(path: str) -> str:
    h = hashlib.sha256()
    with open(path, "rb") as f:
        for chunk in iter(lambda: f.read(65536), b""):
            h.update(chunk)
    return h.hexdigest()


def require_import_args(source: Optional[str], license_: Optional[str], keyword: Optional[str]) -> None:
    missing = [
        name
        for name, val in (("--source", source), ("--license", license_), ("--keyword", keyword))
        if not val
    ]
    if missing:
        raise SystemExit(
            "Import bloqué : "
            + ", ".join(missing)
            + " requis. Aucun fichier n'a été écrit (règle : pas d'entrée dans "
            "ASSETS_SOURCES.md => pas d'import)."
        )


def today_str() -> str:
    return date.today().isoformat()


def find_untracked_assets(graphics_dir: str, ledger_path: str, extensions=(".png",)) -> List[str]:
    """List every asset file under graphics_dir whose relative path does not
    appear as a dest_path in any ledger entry. Used by verify-sources."""
    tracked = set()
    for entry in read_entries(ledger_path):
        tracked.update(os.path.normpath(p) for p in entry.dest_paths)

    untracked = []
    if not os.path.isdir(graphics_dir):
        return untracked
    for root, _dirs, files in os.walk(graphics_dir):
        for fn in files:
            if not fn.lower().endswith(extensions):
                continue
            full = os.path.join(root, fn)
            rel = os.path.normpath(os.path.relpath(full, start=os.getcwd()))
            rel_to_graphics = os.path.normpath(full)
            if rel not in tracked and rel_to_graphics not in tracked:
                untracked.append(full)
    return untracked
