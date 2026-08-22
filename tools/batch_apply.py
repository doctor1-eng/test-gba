#!/usr/bin/env python3
"""Apply a batch of translations (JSON: {id: [french, context]} or
{id: french}) onto translation/text_database.tsv, setting status to
TRANSLATED. Only touches rows currently UNTRANSLATED (won't clobber
manual review work)."""
import csv
import json
import sys

DB_PATH = "translation/text_database.tsv"


def main():
    batch_path = sys.argv[1]
    batch = json.load(open(batch_path, encoding="utf-8"))

    rows = list(csv.DictReader(open(DB_PATH, encoding="utf-8"), delimiter="\t"))
    by_id = {r["id"]: r for r in rows}

    n = 0
    skipped = []
    for tid, val in batch.items():
        if isinstance(val, list):
            french, context = val[0], val[1]
        else:
            french, context = val, None
        r = by_id.get(tid)
        if r is None:
            skipped.append((tid, "unknown id"))
            continue
        if r["status"] != "UNTRANSLATED":
            skipped.append((tid, f"status={r['status']}, not overwritten"))
            continue
        r["french"] = french
        if context:
            r["context"] = context
        r["status"] = "TRANSLATED"
        n += 1

    with open(DB_PATH, "w", newline="", encoding="utf-8") as f:
        w = csv.DictWriter(f, fieldnames=rows[0].keys(), delimiter="\t")
        w.writeheader()
        w.writerows(rows)

    print(f"applied {n} translations")
    if skipped:
        print(f"skipped {len(skipped)}:")
        for tid, reason in skipped[:20]:
            print(f"  {tid}: {reason}")


if __name__ == "__main__":
    main()
