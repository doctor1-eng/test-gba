#!/usr/bin/env python3
"""
Generation III (English) text encoding table, as used by Pokemon Ruby/
Sapphire/Emerald/FireRed/LeafGreen. This is the standard, publicly
documented charmap used throughout the ROM-hacking community (e.g. the
pret/pokefirered disassembly project's charmap.txt, Advance Text, PGE,
Thingy32, XSE). Pokemon Odyssey is a FireRed (BPRE) hack (confirmed via
ROM header game_code=BPRE, title=POKEMON FIRE), so this table is the
correct starting hypothesis -- it must still be verified against the
actual ROM bytes before being trusted (see tools/verify_charmap.py).

Control codes (subset relevant to dialogue text):
  0x00        space
  0xFC xx     special control code, 1 param byte (e.g. color codes)
  0xFC xx yy  some 0xFC subcodes take 2 param bytes (variable-width)
  0xFD xx     placeholder/variable insert, 1 param byte
  0xFA        prompt: wait for button press, keep text box, no scroll
  0xFB        scroll: wait for button press, then clear/scroll text box
  0xFE        new paragraph (clear box, resume printing)
  0xF9 xx     text speed / other, 1 param
  0xF8 xx     unknown/rare
  0x00-0xFF   see CHARMAP for printable glyphs
  0xFF        string terminator (end of text)

NOTE: an earlier draft of this module guessed 0x01 was a "line break"
control code. That was never empirically confirmed, and it collided
with 0x01='À' once the French accent range was confirmed by a real
emulator test (see TESTING.md) -- the accent mapping wins, since it's
the one actually verified against rendered output. 0x01 is NOT in
CONTROL_CODES below.
"""

CHARMAP = {
    0x00: ' ',
    0xA1: '0', 0xA2: '1', 0xA3: '2', 0xA4: '3', 0xA5: '4',
    0xA6: '5', 0xA7: '6', 0xA8: '7', 0xA9: '8', 0xAA: '9',
    0xAB: '!', 0xAC: '?', 0xAD: '.', 0xAE: '-',
    0xAF: '·',  # ·
    0xB0: '…',  # …
    0xB1: '“',  # “
    0xB2: '”',  # ”
    0xB3: '‘',  # ‘
    0xB4: '’',  # ’
    0xB5: '♂',  # ♂
    0xB6: '♀',  # ♀
    0xB7: '$',
    0xB8: ',',
    0xB9: '×',  # ×
    0xBA: '/',
    0xBB: 'A', 0xBC: 'B', 0xBD: 'C', 0xBE: 'D', 0xBF: 'E',
    0xC0: 'F', 0xC1: 'G', 0xC2: 'H', 0xC3: 'I', 0xC4: 'J',
    0xC5: 'K', 0xC6: 'L', 0xC7: 'M', 0xC8: 'N', 0xC9: 'O',
    0xCA: 'P', 0xCB: 'Q', 0xCC: 'R', 0xCD: 'S', 0xCE: 'T',
    0xCF: 'U', 0xD0: 'V', 0xD1: 'W', 0xD2: 'X', 0xD3: 'Y', 0xD4: 'Z',
    0xD5: 'a', 0xD6: 'b', 0xD7: 'c', 0xD8: 'd', 0xD9: 'e',
    0xDA: 'f', 0xDB: 'g', 0xDC: 'h', 0xDD: 'i', 0xDE: 'j',
    0xDF: 'k', 0xE0: 'l', 0xE1: 'm', 0xE2: 'n', 0xE3: 'o',
    0xE4: 'p', 0xE5: 'q', 0xE6: 'r', 0xE7: 's', 0xE8: 't',
    0xE9: 'u', 0xEA: 'v', 0xEB: 'w', 0xEC: 'x', 0xED: 'y', 0xEE: 'z',
    0xEF: '▲',  # ▲ (up arrow glyph in vanilla table, rarely used)
    0xF0: ':',
    0xF1: 'Ä', 0xF2: 'Ö', 0xF3: 'Ü',
    0xF4: 'ä', 0xF5: 'ö', 0xF6: 'ü',
    0xF7: 'é',  # é  (used in e.g. "Pokémon")

    # CONFIRMED 2026-08-22 by real emulator test (build/Pokemon_Odyssey_CHARTEST.gba,
    # see TESTING.md) -- promoted from HYPOTHESIS_LOWRANGE in
    # tools/build_charset_test.py after visual confirmation.
    0x01: 'À', 0x03: 'Â', 0x04: 'Ç', 0x05: 'È', 0x06: 'É', 0x07: 'Ê',
    0x08: 'Ë', 0x0B: 'Î', 0x0C: 'Ï', 0x0F: 'Ô', 0x10: 'Œ', 0x11: 'Ù',
    0x13: 'Û',
    0x16: 'à', 0x18: 'â', 0x19: 'ç', 0x1A: 'è', 0x1B: 'é', 0x1C: 'ê',
    0x1D: 'ë', 0x20: 'î', 0x21: 'ï', 0x24: 'ô', 0x25: 'œ', 0x26: 'ù',
    0x28: 'û',
}

CONTROL_CODES = {0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xF9, 0xF8}
TERMINATOR = 0xFF

REVERSE_CHARMAP = {v: k for k, v in CHARMAP.items()}

# ---------------------------------------------------------------------------
# French accent handling
#
# All accented characters needed for French (à â ç è ê ë î ï ô ù û œ
# and uppercase forms) are now EMPIRICALLY CONFIRMED against this ROM
# (see docs/TECHNICAL_AUDIT.md section 3 and TESTING.md -- confirmed
# 2026-08-22 via build/Pokemon_Odyssey_CHARTEST.gba on a real
# emulator). They're included directly in CHARMAP above.
#
# ASCII_FALLBACK below is now a pure safety net for characters that
# are NOT part of French (accented letters from other languages,
# stray Unicode) so a build never hard-fails on an odd character --
# it degrades gracefully and still gets reported as a review flag.
# ---------------------------------------------------------------------------

ASCII_FALLBACK = {
    'ä': 'a',
}


def encode_char(ch):
    """Return (byte_or_None, warning_or_None) for a single output character."""
    if ch in REVERSE_CHARMAP:
        return REVERSE_CHARMAP[ch], None
    if ch in ASCII_FALLBACK:
        fallback = ASCII_FALLBACK[ch]
        # multi-char fallback (e.g. 'œ' -> 'oe') handled by caller
        return None, ("fallback", ch, fallback)
    return None, ("unencodable", ch, None)


def encode_string(text, control_tag_re=None):
    """
    Encode a translated (French) string into Gen3 bytes, preserving
    <XX> / <XX:YY> control tags verbatim (as originally captured by
    gen3_charmap.decode_bytes), appending the terminator (0xFF).
    Returns (bytes, warnings) where warnings is a list of
    ("fallback"|"unencodable", char, replacement_or_None).
    """
    import re
    if control_tag_re is None:
        control_tag_re = re.compile(r"<([0-9A-F]{2})(?::([0-9A-F]{2}))?>")

    out = bytearray()
    warnings = []
    i = 0
    n = len(text)
    while i < n:
        m = control_tag_re.match(text, i)
        if m:
            out.append(int(m.group(1), 16))
            if m.group(2) is not None:
                out.append(int(m.group(2), 16))
            i = m.end()
            continue
        ch = text[i]
        byte, warn = encode_char(ch)
        if byte is not None:
            out.append(byte)
        elif warn is not None and warn[0] == "fallback":
            for fch in warn[2]:
                fbyte, _ = encode_char(fch)
                out.append(fbyte if fbyte is not None else REVERSE_CHARMAP['?'])
            warnings.append(warn)
        else:
            warnings.append(warn if warn else ("unencodable", ch, None))
            out.append(REVERSE_CHARMAP['?'])
        i += 1
    out.append(TERMINATOR)
    return bytes(out), warnings


def decode_bytes(data: bytes, offset: int, max_len: int = 512):
    """Decode a Gen3-encoded string starting at offset. Returns (text, length_in_bytes, ok)."""
    out = []
    i = offset
    end = min(offset + max_len, len(data))
    while i < end:
        b = data[i]
        if b == TERMINATOR:
            return ''.join(out), i - offset + 1, True
        if b in (0xFC, 0xFD):
            # these take 1+ parameter bytes; treat conservatively as 2 bytes total
            out.append(f'<{b:02X}:{data[i+1]:02X}>')
            i += 2
            continue
        if b in CONTROL_CODES:
            out.append(f'<{b:02X}>')
            i += 1
            continue
        ch = CHARMAP.get(b)
        if ch is None:
            return ''.join(out), i - offset, False
        out.append(ch)
        i += 1
    return ''.join(out), i - offset, False
