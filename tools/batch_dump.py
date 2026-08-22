#!/usr/bin/env python3
"""Dump the next N untranslated rows (id + english, control-code-stripped
markers kept) for manual/LLM translation. Used to drive translation in
manageable batches per mission rule #13 (LOT 13: batches, not everything
at once)."""
import csv
import sys

DB_PATH = "translation/text_database.tsv"


def main():
    n = int(sys.argv[1]) if len(sys.argv) > 1 else 150
    offset = int(sys.argv[2]) if len(sys.argv) > 2 else 0
    rows = list(csv.DictReader(open(DB_PATH, encoding="utf-8"), delimiter="\t"))
    todo = [r for r in rows if r["status"] == "UNTRANSLATED"]
    batch = todo[offset:offset + n]
    for r in batch:
        print(f"{r['id']}\t{r['english']}")
    print(f"# batch {offset}:{offset+n} of {len(todo)} untranslated total", file=sys.stderr)


if __name__ == "__main__":
    main()
