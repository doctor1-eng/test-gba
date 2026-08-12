# Cibles de branchement les plus référencées (candidates)

Généré par `tools/unbound_analysis/analyze_branches.py`. Ne provient pas d'un désassemblage réel — uniquement du comptage de motifs d'octets candidats fourni dans le pack d'analyse statique.

## Mise en garde sur le signal ARM

Toutes les régions de la ROM (2048/2048, cf. `region-profile.csv`) contiennent au moins 69 cibles BL ARM candidates, y compris les régions à très haute entropie où du vrai code ARM ne devrait structurellement pas se trouver (GBA : le code ARM natif ne représente qu'une fraction infime d'un jeu, l'essentiel étant en Thumb). **HYPOTHÈSE avec confiance élevée : ce signal est dominé par des faux positifs** (mots alignés dont les bits correspondent par coïncidence à l'encodage d'une instruction BL ARM). Le classement ci-dessous est fourni tel quel mais ne doit pas être interprété comme une liste de fonctions réelles sans validation par désassemblage.

## Top cibles Thumb BL (signal plus contrasté, cf. code-analysis.md)

| rang | offset ROM | adresse GBA | références |
|---|---|---|---|
| 1 | 0x0003FBE8 | 0x0803FBE8 | 1345 |
| 2 | 0x00000A38 | 0x08000A38 | 1235 |
| 3 | 0x0004037C | 0x0804037C | 727 |
| 4 | 0x000722CC | 0x080722CC | 648 |
| 5 | 0x00A28850 | 0x08A28850 | 605 |
| 6 | 0x01ECEBC8 | 0x09ECEBC8 | 556 |
| 7 | 0x009B3982 | 0x089B3982 | 519 |
| 8 | 0x000751C4 | 0x080751C4 | 464 |
| 9 | 0x00074480 | 0x08074480 | 461 |
| 10 | 0x00A1CBE4 | 0x08A1CBE4 | 433 |
| 11 | 0x00077508 | 0x08077508 | 431 |
| 12 | 0x0007741C | 0x0807741C | 372 |
| 13 | 0x00003F20 | 0x08003F20 | 361 |
| 14 | 0x00002554 | 0x08002554 | 354 |
| 15 | 0x0000838C | 0x0800838C | 332 |
| 16 | 0x001E5E78 | 0x081E5E78 | 329 |
| 17 | 0x00003FA0 | 0x08003FA0 | 318 |
| 18 | 0x00002BC4 | 0x08002BC4 | 318 |
| 19 | 0x00000544 | 0x08000544 | 318 |
| 20 | 0x001E4018 | 0x081E4018 | 316 |
| 21 | 0x000020BC | 0x080020BC | 312 |
| 22 | 0x00008D84 | 0x08008D84 | 310 |
| 23 | 0x00006F8C | 0x08006F8C | 308 |
| 24 | 0x000703EC | 0x080703EC | 294 |
| 25 | 0x01ED6B84 | 0x09ED6B84 | 290 |
| 26 | 0x00070588 | 0x08070588 | 290 |
| 27 | 0x00A1CBF0 | 0x08A1CBF0 | 282 |
| 28 | 0x00044EC8 | 0x08044EC8 | 279 |
| 29 | 0x00A0470C | 0x08A0470C | 274 |
| 30 | 0x01EC5B14 | 0x09EC5B14 | 258 |

## Top cibles ARM BL (signal probablement bruité, voir mise en garde ci-dessus)

| rang | offset ROM | adresse GBA | références |
|---|---|---|---|
| 1 | 0x013A1818 | 0x093A1818 | 13 |
| 2 | 0x00703A34 | 0x08703A34 | 13 |
| 3 | 0x00000320 | 0x08000320 | 13 |
| 4 | 0x013A4B9C | 0x093A4B9C | 8 |
| 5 | 0x013A45C4 | 0x093A45C4 | 8 |
| 6 | 0x0080E6BC | 0x0880E6BC | 8 |
| 7 | 0x0080E0E4 | 0x0880E0E4 | 8 |
| 8 | 0x001DC71C | 0x081DC71C | 8 |
| 9 | 0x0139FF1C | 0x0939FF1C | 7 |
| 10 | 0x0139FDA4 | 0x0939FDA4 | 7 |
| 11 | 0x0104ADC0 | 0x0904ADC0 | 7 |
| 12 | 0x001E0348 | 0x081E0348 | 7 |
| 13 | 0x001E01D0 | 0x081E01D0 | 6 |
| 14 | 0x001DFF3C | 0x081DFF3C | 6 |
| 15 | 0x0139FB10 | 0x0939FB10 | 5 |
| 16 | 0x0139F9E4 | 0x0939F9E4 | 5 |
| 17 | 0x0080E0F4 | 0x0880E0F4 | 5 |
| 18 | 0x006FFD8C | 0x086FFD8C | 5 |
| 19 | 0x001DFE10 | 0x081DFE10 | 5 |
| 20 | 0x013A45D4 | 0x093A45D4 | 4 |
| 21 | 0x013A009C | 0x093A009C | 4 |
| 22 | 0x0139FF8C | 0x0939FF8C | 4 |
| 23 | 0x0139FF24 | 0x0939FF24 | 4 |
| 24 | 0x0139FEF0 | 0x0939FEF0 | 4 |
| 25 | 0x010BC358 | 0x090BC358 | 4 |
| 26 | 0x00E91B5C | 0x08E91B5C | 4 |
| 27 | 0x009CFF54 | 0x089CFF54 | 4 |
| 28 | 0x006FFE0C | 0x086FFE0C | 4 |
| 29 | 0x001E04C8 | 0x081E04C8 | 4 |
| 30 | 0x001E03B8 | 0x081E03B8 | 4 |
