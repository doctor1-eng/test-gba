# Analyse du code (ARM/Thumb)

Aucun désassemblage n'a été effectué. Tout ce document s'appuie sur des candidats statistiques
(`arm-bl-targets.csv`, `thumb-bl-targets.csv`) : des adresses qui, à un ou plusieurs endroits de la ROM,
sont précédées d'un motif d'octets compatible avec l'encodage d'une instruction `BL` (branch-and-link)
ARM ou Thumb pointant vers elles. Ce n'est **pas** un contrôle de flot réel.

## Le signal ARM est probablement dominé par le bruit — CONFIRMÉ (statistiquement, sur les données fournies)

Trois observations convergentes, sur les données du pack lui-même (pas une supposition externe) :

1. **Couverture universelle** : `region-profile.csv` montre `arm_bl_target_count >= 69` sur les 2048/2048
   régions, y compris dans des régions à entropie proche du maximum théorique (7.9) où du code ARM ne
   devrait structurellement pas apparaître (GBA : le code ARM natif est une fraction infime d'un jeu
   Pokémon, l'essentiel de la logique étant compilé en Thumb pour la densité).
2. **Absence de fonctions "hub"** : la cible ARM la plus référencée n'a que 13 références
   (`0x013A1818`/`0x00703A34`/`0x00000320`, ex æquo). Un vrai jeu compilé a presque toujours quelques
   fonctions utilitaires (copie mémoire, décompression, gestion d'interruption) appelées des centaines de
   fois — cette absence de "pic" est un signe classique de faux positifs statistiquement dispersés plutôt
   que de vraies références de contrôle de flot.
3. **Contraste avec Thumb** : la cible Thumb la plus référencée (`0x0003FBE8`) a 1345 références, deux
   ordres de grandeur au-dessus du maximum ARM. Cohérent avec une fonction utilitaire très largement
   appelée (HYPOTHÈSE sur son identité — voir plus bas), signe d'un signal Thumb avec une vraie structure
   sous-jacente, même bruitée.

**Conséquence méthodologique** : le signal ARM BL a été explicitement exclu de la classification des
régions dans `correlate_regions.py` (voir `rom-map.md`). Il reste rapporté dans `data/top-branch-targets.md`
pour transparence, mais ne doit pas être utilisé pour localiser du code ARM sans validation externe
(désassembleur réel sur la ROM, hors du périmètre de cette analyse).

## Cibles Thumb les plus référencées

| Rang | Offset | Références | Hypothèse |
|---|---|---|---|
| 1 | `0x0003FBE8` | 1345 | HYPOTHÈSE, confiance faible : fonction utilitaire très générique (candidats plausibles dans un jeu GBA : copie/remplissage mémoire, wrapper d'appel BIOS, décompression). Aucun moyen de confirmer l'identité sans désassemblage. |
| 2 | `0x00000A38` | 1235 | Offset très bas (dans la région 0, la zone d'en-tête/début de code) — HYPOTHÈSE : probablement liée au code de démarrage/interruption, cohérent avec sa position. |
| 3 | `0x0004037C` | 727 | INCONNU |
| 4 | `0x000722CC` | 648 | INCONNU |
| 5 | `0x00A28850` | 605 | Offset élevé (~10.6 Mo) — HYPOTHÈSE faible : pourrait être une fonction de logique de jeu plutôt que d'infrastructure système (les fonctions systèmes sont statistiquement plus proches du début de la ROM dans les projets pret-style, mais ce n'est qu'une convention observée ailleurs, pas une preuve pour Unbound spécifiquement). |

Liste complète (top 30 par jeu d'instructions) : `data/top-branch-targets.md`.

## Ce qui resterait à faire pour aller plus loin

- Désassembler réellement les octets autour des cibles les plus référencées (nécessite les octets bruts
  de la ROM — hors périmètre de cette analyse, qui ne dispose que de métadonnées).
- Comparer la signature d'appel (quels registres, quelle taille de fonction) avec les fonctions connues
  et documentées publiquement de `pret/pokefirered` (mêmes conventions d'architecture, base FireRed
  partagée).

**Niveau de confiance global de cette section : PROBABLE pour "le signal ARM est peu fiable", HYPOTHÈSE
pour toute identification de fonction spécifique.**
