#!/usr/bin/env python3
"""
Builds a tiny, single-string test ROM so a human with a real GBA
emulator can visually confirm (or refute) the unverified French accent
mapping in ~30 seconds, per docs/TECHNICAL_AUDIT.md section 3 / mission
rule #9 ("teste réellement l'affichage avant toute grosse réinsertion").

Patches ONLY the "Would you like to save the game?" prompt (reachable
from literally anywhere via Start -> SAVE, no story progress needed)
with a string containing every French accented character.

Output: build/Pokemon_Odyssey_CHARTEST.gba
"""
import sys
sys.path.insert(0, "tools")
from gen3_charmap import TERMINATOR, CONTROL_CODES

ROM_PATH = "roms/original/odyssey_en_v4.1.1.gba"
OUT_PATH = "build/Pokemon_Odyssey_CHARTEST.gba"

# The "Would you like to save the game?" pointer/target, from
# translation/text_database.tsv row ODYSSEY-TXT-001423. Reachable from
# anywhere in the game via Start -> SAVE, no story progress needed.
TARGET_OFFSET = 0x01C55C9
REFERENCES = [0x006F7D4]
FREE_SPACE_ADDR = 0x1413A9C  # start of the largest confirmed 0xFF run

# UNVERIFIED HYPOTHESIS -- community-documented Gen3 international
# low-range charmap (shared table used for FR/DE/ES/IT text in this
# generation of games). NOT confirmed against this specific ROM. This
# is exactly what this test build exists to confirm or refute -- see
# docs/TECHNICAL_AUDIT.md section 3 and TESTING.md. Used ONLY here,
# never in the production encoder (tools/gen3_charmap.encode_string),
# which stays on the safe ASCII-fallback path until this is verified.
HYPOTHESIS_LOWRANGE = {
    'À': 0x01, 'Â': 0x03, 'Ç': 0x04, 'È': 0x05, 'É': 0x06, 'Ê': 0x07,
    'Ë': 0x08, 'Î': 0x0B, 'Ï': 0x0C, 'Ô': 0x0F, 'Œ': 0x10, 'Ù': 0x11,
    'Û': 0x13,
    'à': 0x16, 'â': 0x18, 'ç': 0x19, 'è': 0x1A, 'é': 0x1B, 'ê': 0x1C,
    'ë': 0x1D, 'î': 0x20, 'ï': 0x21, 'ô': 0x24, 'œ': 0x25, 'ù': 0x26,
    'û': 0x28,
}


def encode_hypothesis(text):
    """Encode using the confirmed table where possible, falling back to
    the unverified hypothesis table for accents -- deliberately, this
    is the point of the experiment."""
    import re
    sys.path.insert(0, "tools")
    from gen3_charmap import REVERSE_CHARMAP
    tag_re = re.compile(r"<([0-9A-F]{2})>")
    out = bytearray()
    i = 0
    while i < len(text):
        m = tag_re.match(text, i)
        if m:
            out.append(int(m.group(1), 16))
            i = m.end()
            continue
        ch = text[i]
        if ch in REVERSE_CHARMAP:
            out.append(REVERSE_CHARMAP[ch])
        elif ch in HYPOTHESIS_LOWRANGE:
            out.append(HYPOTHESIS_LOWRANGE[ch])
        else:
            raise ValueError(f"no mapping (confirmed or hypothesis) for {ch!r}")
        i += 1
    out.append(TERMINATOR)
    return bytes(out)


def build():
    with open(ROM_PATH, "rb") as f:
        rom = bytearray(f.read())

    test_string = (
        "TEST CHARSET:<FE>"
        "a e i o u c : à â ç è ê ë î ï ô ù û<FE>"
        "A E I O U C : À Â Ç È Ê Ë Î Ï Ô Ù Û Œ œ"
    )
    encoded = encode_hypothesis(test_string)

    orig_size = 34  # 'Would you like to save the game?' + terminator
    if len(encoded) > orig_size:
        addr = FREE_SPACE_ADDR
        rom[addr:addr + len(encoded)] = encoded
        new_ptr = (0x08000000 + addr).to_bytes(4, "little")
        for ref in REFERENCES:
            rom[ref:ref + 4] = new_ptr
        print(f"relocated to 0x{addr:07X}, {len(encoded)} bytes, pointer(s) updated: {REFERENCES}")
    else:
        rom[TARGET_OFFSET:TARGET_OFFSET + len(encoded)] = encoded
        print("written in place")

    import os
    os.makedirs("build", exist_ok=True)
    with open(OUT_PATH, "wb") as f:
        f.write(rom)
    print(f"wrote {OUT_PATH}")
    print(f"test string: {test_string!r}")
    print("Reachable via: Start menu -> SAVE, from anywhere in the game, no progress needed.")
    print("If the accented rows show correct French letters -> HYPOTHESIS_LOWRANGE is confirmed, promote it into gen3_charmap.py's confirmed CHARMAP.")
    print("If garbage/blank tiles -> hypothesis is wrong, needs real font/table RE (ARM disassembly) instead.")


if __name__ == "__main__":
    build()
