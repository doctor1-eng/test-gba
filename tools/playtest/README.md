# tools/playtest — tester la ROM sans émulateur à l'écran

Émulateur GBA headless (libmgba) piloté par script. Il permet de jouer, de prendre des
captures, de lire et d'écrire la mémoire, et d'enregistrer des animations, le tout depuis un
terminal et sans testeur humain. C'est avec cet outil qu'ont été validés « Le Village Caché »
et « Les Arènes Libres ».

## Installation (une fois par conteneur)

```bash
apt-get install -y libmgba-dev          # (apt-get update d'abord si "Unable to fetch")
pip install pillow
gcc -O2 -o tools/playtest/harness tools/playtest/harness.c -lmgba
```

## Utilisation

```bash
python3 tools/playtest/pt.py pokehns.gba scenario.txt --out /tmp/sortie
```

Un scénario contient une commande par ligne :

| Commande | Effet |
|---|---|
| `battery FICHIER.sav` | Branche une sauvegarde « cartouche » et redémarre. Contrairement aux savestates, elle **survit aux recompilations**. |
| `frames N` / `press TOUCHES N [W]` / `tap TOUCHES [n]` | Fait avancer le temps et envoie des entrées (`A`, `B`, `UP`, `A+B`…). |
| `walk DIR N` / `talk [n]` | Marche de N cases / fait défiler n pages de dialogue. |
| `goto MAP_X x y [FLAG_… VAR_…=n]` | Téléporte le joueur et pose des flags ou des variables (via le hook décrit plus bas), puis attend que le joueur ait la main. |
| `setflag` / `clearflag` / `checkflag` / `setvar` / `checkvar` | Lit ou écrit directement la sauvegarde en RAM. |
| `battle N` | Pilote automatique de combat : l'adversaire est ramené à 1 PV, l'équipe du joueur à 999 PV et la 1re attaque garde ses PP. Il sert à valider le **script** autour d'un combat, pas l'équilibrage. |
| `fastbattles` | Style de combat « Défini » et animations coupées. |
| `untilflag FLAG_X` … `end` | Répète le bloc jusqu'à ce que le flag soit posé. Accepte aussi `TRAINER:TRAINER_X`, le flag « dresseur vaincu ». |
| `shot NOM` | Capture PNG (×2). |
| `record PREFIXE N` / `stoprecord` | Enregistre une image toutes les N frames. `make_gif.py` en fait un GIF. |
| `save` / `load FICHIER` | Savestates. Rapides, mais liés à un build précis de la ROM. |

Les constantes (`FLAG_…`, `MAP_…`, `SPECIES_…`) sont résolues par le préprocesseur C sur les
vrais en-têtes du dépôt. Les adresses (`gSaveBlock1Ptr`, `gBattleMons`…) viennent de l'ELF et
les offsets de structures du compilateur croisé : rien n'est codé en dur.

## Le hook `gHnsPlaytest` (src/field_control_avatar.c)

C'est une structure en EWRAM que seul un outil externe peut remplir, via un nombre magique
`"HNPT"`. L'EWRAM est remise à zéro au démarrage et rien dans le jeu n'écrit cette
structure : le hook est donc inerte en jeu normal. Quand il est armé, il pose les
flags/variables demandés, déclenche un warp, puis se désarme. Il est compilé dans la ROM
livrée, pour que le binaire testé soit exactement celui qui est distribué.

## Autres outils

- `mapinfo.py MAP [--doors] [--grid]` : warps, objets, grille de collision, portes.
- `reach.py MAP x y cibles…` : accessibilité par remplissage (les arbres Coupe sont
  considérés comme franchissables).
- `render_layout.py LAYOUT out.png` : rendu d'un layout via `gba_tiles`.
- `make_gif.py DOSSIER out.gif` : GIF compact à partir d'un enregistrement.
