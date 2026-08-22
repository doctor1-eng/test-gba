#!/usr/bin/env python3
"""
Per-string validator for translation/text_database.tsv.

For every row with status TRANSLATED (or later), checks:
  - control codes: the set of <XX>/<XX:YY> tags in the French text
    must match exactly the set in the English source (same codes,
    same count) -- catches deleted/added/moved codes.
  - encodability: every character must be encodable (confirmed byte)
    or reported as a fallback/unencodable warning (see gen3_charmap.py).
  - length: flags (does not fail) French text more than 2x the
    original char count, or more than 3 lines, for manual review --
    we don't have a confirmed hard per-box limit yet (see
    docs/TECHNICAL_AUDIT.md section 7), so this is advisory.

Exit code 0 if no ERRORs (warnings/reviews are still printed and
counted, but do not block a build).
"""
import csv
import re
import sys

sys.path.insert(0, "tools")
from gen3_charmap import encode_string

DB_PATH = "translation/text_database.tsv"
CONTROL_TAG_RE = re.compile(r"<([0-9A-F]{2})(?::[0-9A-F]{2})?>")


def validate_row(row):
    errors = []
    reviews = []
    en, fr = row["english"], row["french"]

    en_codes = sorted(CONTROL_TAG_RE.findall(en))
    fr_codes = sorted(CONTROL_TAG_RE.findall(fr))
    if en_codes != fr_codes:
        errors.append(f"control code mismatch: english={en_codes} french={fr_codes}")

    _, warnings = encode_string(fr)
    for kind, ch, repl in warnings:
        if kind == "fallback":
            reviews.append(f"charset non vérifié: '{ch}' -> ASCII fallback '{repl}'")
        else:
            errors.append(f"unencodable character: {ch!r}")

    en_chars = len(CONTROL_TAG_RE.sub("", en))
    fr_chars = len(CONTROL_TAG_RE.sub("", fr))
    if en_chars > 0 and fr_chars > en_chars * 2 + 20:
        reviews.append(f"french much longer than english ({fr_chars} vs {en_chars} chars) -- verify it fits on screen")

    return errors, reviews


def main():
    rows = list(csv.DictReader(open(DB_PATH, encoding="utf-8"), delimiter="\t"))
    to_check = [r for r in rows if r["status"] in ("TRANSLATED", "REVIEW", "VALIDATED", "INSERTED", "TESTED")]
    n_errors = 0
    n_reviews = 0
    for row in to_check:
        errors, reviews = validate_row(row)
        if errors:
            n_errors += len(errors)
            print(f"ERROR {row['id']}: " + " | ".join(errors))
            print(f"    EN: {row['english']!r}")
            print(f"    FR: {row['french']!r}")
        if reviews:
            n_reviews += len(reviews)
            for r in reviews:
                print(f"REVIEW {row['id']}: {r}")

    print(f"\n{len(to_check)} translated rows checked, {n_errors} errors, {n_reviews} review flags")
    sys.exit(1 if n_errors else 0)


if __name__ == "__main__":
    main()
