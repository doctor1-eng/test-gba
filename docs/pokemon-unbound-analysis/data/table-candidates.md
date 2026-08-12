# Candidats de tables à espacement régulier

Généré par `tools/unbound_analysis/detect_tables.py`. Deux méthodes distinctes, toutes deux des HYPOTHÈSES à valider par un désassemblage réel :

1. **Séquences de cibles de pointeurs à stride constant** — des adresses individuellement référencées par des pointeurs ailleurs dans la ROM, espacées d'un pas constant compatible avec une structure de taille fixe.
2. **Clusters denses de blocs de 16 octets répétés** — zones où au moins 20 blocs de 16 octets déjà vus ailleurs se reproduisent dans une fenêtre de 16 KiB.

## Séquences à stride constant (top 40 par longueur)

| offset début | pas (octets) | longueur | taille totale |
|---|---|---|---|
| 0xebbaec | 36 | 298 | 0x29e8 |
| 0x34f188 | 28 | 294 | 0x2028 |
| 0x34f1a4 | 56 | 213 | 0x2e98 |
| 0x238e8c | 4 | 149 | 0x254 |
| 0x34f188 | 56 | 147 | 0x2028 |
| 0xebaa0c | 32 | 136 | 0x1100 |
| 0x3513fc | 28 | 111 | 0xc24 |
| 0xb1a8f8 | 32 | 87 | 0xae0 |
| 0x3a4618 | 36 | 80 | 0xb40 |
| 0x238e8c | 8 | 75 | 0x258 |
| 0x238e90 | 8 | 74 | 0x250 |
| 0xebaa0c | 64 | 68 | 0x1100 |
| 0xebaa2c | 64 | 68 | 0x1100 |
| 0xebe63c | 36 | 66 | 0x948 |
| 0x1033748 | 128 | 65 | 0x2080 |
| 0x104dc9c | 128 | 64 | 0x2000 |
| 0x87e9f8 | 12 | 56 | 0x2a0 |
| 0x351418 | 56 | 55 | 0xc08 |
| 0x238e8c | 12 | 50 | 0x258 |
| 0x238e90 | 12 | 50 | 0x258 |
| 0x238e94 | 12 | 49 | 0x24c |
| 0x8c1430 | 128 | 49 | 0x1880 |
| 0x3a6a50 | 12 | 46 | 0x228 |
| 0x1fb8740 | 16 | 46 | 0x2e0 |
| 0x3a3bb0 | 36 | 45 | 0x654 |
| 0xb1a8f8 | 64 | 44 | 0xb00 |
| 0xb1a918 | 64 | 43 | 0xac0 |
| 0x1e93053 | 36 | 40 | 0x5a0 |
| 0x1e9305c | 36 | 40 | 0x5a0 |
| 0x1e93065 | 36 | 40 | 0x5a0 |
| 0x1053ca4 | 128 | 39 | 0x1380 |
| 0x238e8c | 16 | 38 | 0x260 |
| 0x399788 | 128 | 38 | 0x1300 |
| 0x238e90 | 16 | 37 | 0x250 |
| 0x238e94 | 16 | 37 | 0x250 |
| 0x238e98 | 16 | 37 | 0x250 |
| 0x3a6898 | 12 | 35 | 0x1a4 |
| 0xebaa0c | 128 | 34 | 0x1100 |
| 0xebaa2c | 128 | 34 | 0x1100 |
| 0xebaa4c | 128 | 34 | 0x1100 |

## Clusters denses de blocs répétés (top 30)

| offset début | offset fin | nb blocs répétés dans la fenêtre |
|---|---|---|
| 0x1b36a0 | 0x1b76a0 | 1025 |
| 0x1feb30 | 0x202b30 | 1025 |
| 0x202b40 | 0x206b40 | 1025 |
| 0x222bf0 | 0x226bf0 | 1025 |
| 0x6cec40 | 0x6d2c40 | 1025 |
| 0x6d2c50 | 0x6d6c50 | 1025 |
| 0x6d6c60 | 0x6dac60 | 1025 |
| 0x8d6c10 | 0x8dac10 | 1025 |
| 0x8dec30 | 0x8e2c30 | 1025 |
| 0x8e2c40 | 0x8e6c40 | 1025 |
| 0x8e6c50 | 0x8eac50 | 1025 |
| 0x8eac60 | 0x8eec60 | 1025 |
| 0x8eec70 | 0x8f2c70 | 1025 |
| 0x8f2c80 | 0x8f6c80 | 1025 |
| 0x8f6c90 | 0x8fac90 | 1025 |
| 0x8faca0 | 0x8feca0 | 1025 |
| 0xb2dca0 | 0xb31ca0 | 1025 |
| 0xb31cb0 | 0xb35cb0 | 1025 |
| 0xb35cc0 | 0xb39cc0 | 1025 |
| 0xb39cd0 | 0xb3dcd0 | 1025 |
| 0xb3dce0 | 0xb41ce0 | 1025 |
| 0xb41cf0 | 0xb45cf0 | 1025 |
| 0xb45d00 | 0xb49d00 | 1025 |
| 0xb49d10 | 0xb4dd10 | 1025 |
| 0x8dac20 | 0x8dec20 | 1024 |
| 0x8d2c00 | 0x8d6c00 | 1023 |
| 0x21ebe0 | 0x222be0 | 1014 |
| 0x17ac10 | 0x17ec10 | 1013 |
| 0x172bf0 | 0x176bf0 | 1010 |
| 0x6cac30 | 0x6cec30 | 1008 |
