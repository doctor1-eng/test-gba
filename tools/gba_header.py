#!/usr/bin/env python3
"""Dump and verify a GBA ROM header. Read-only, never modifies the ROM."""
import sys
import hashlib


def read_header(path):
    with open(path, "rb") as f:
        data = f.read()
    header = {
        "file_size": len(data),
        "entry_point": data[0:4].hex(),
        "nintendo_logo_present": data[0x04:0xA0] != b"\x00" * 156,
        "title": data[0xA0:0xAC].rstrip(b"\x00").decode("ascii", "replace"),
        "game_code": data[0xAC:0xB0].decode("ascii", "replace"),
        "maker_code": data[0xB0:0xB2].decode("ascii", "replace"),
        "fixed_96": hex(data[0xB2]),
        "main_unit_code": hex(data[0xB3]),
        "device_type": hex(data[0xB4]),
        "software_version": data[0xBC],
        "complement_check": hex(data[0xBD]),
    }
    # verify complement check: sum of bytes 0xA0..0xBC inclusive, then -(sum+0x19) & 0xFF == 0
    chk_range = data[0xA0:0xBD]
    computed = (-(sum(chk_range) + 0x19)) & 0xFF
    header["complement_check_valid"] = computed == data[0xBD]
    header["sha256"] = hashlib.sha256(data).hexdigest()
    return header, data


if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else "roms/original/odyssey_en_v4.1.1.gba"
    header, data = read_header(path)
    for k, v in header.items():
        print(f"{k}: {v}")
