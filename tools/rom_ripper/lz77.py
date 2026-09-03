"""Décompresseur LZ77 GBA (format BIOS LZ77UnCompReadNormalWrite8bit), utilisé pour compresser
la quasi-totalité des graphismes de tileset dans les jeux Pokémon GBA. Implémentation directe de
l'algorithme standard (documenté publiquement, pas une supposition) :

En-tête (4 octets) : octet0 = 0x10 (marqueur LZ77), octets 1-3 = taille décompressée (LE, 24 bits).
Puis une suite de blocs : 1 octet de "flags" suivi de 8 unités (MSB en premier) :
  - bit à 0 : octet littéral, copié tel quel.
  - bit à 1 : référence arrière sur 2 octets (longueur 3-18, distance 1-4096).
"""


def try_decompress(data: bytes, offset: int, max_size: int = 300_000):
    """Tente une décompression LZ77 à `offset`. Retourne les octets décompressés si l'en-tête et
    tout le flux sont valides, sinon None. Rejette rapidement les faux positifs (la plupart des
    octets 0x10 rencontrés en scannant la ROM ne sont pas de vrais en-têtes LZ77)."""
    if offset + 4 > len(data):
        return None
    if data[offset] != 0x10:
        return None
    decompressed_size = data[offset + 1] | (data[offset + 2] << 8) | (data[offset + 3] << 16)
    if decompressed_size == 0 or decompressed_size > max_size:
        return None

    out = bytearray()
    pos = offset + 4
    n = len(data)

    while len(out) < decompressed_size:
        if pos >= n:
            return None
        flags = data[pos]
        pos += 1
        for bit in range(8):
            if len(out) >= decompressed_size:
                break
            if flags & (0x80 >> bit):
                if pos + 2 > n:
                    return None
                b1 = data[pos]
                b2 = data[pos + 1]
                pos += 2
                length = (b1 >> 4) + 3
                disp = (((b1 & 0xF) << 8) | b2) + 1
                if disp > len(out):
                    return None
                for _ in range(length):
                    out.append(out[len(out) - disp])
                    if len(out) >= decompressed_size:
                        break
            else:
                if pos >= n:
                    return None
                out.append(data[pos])
                pos += 1

    return bytes(out)
