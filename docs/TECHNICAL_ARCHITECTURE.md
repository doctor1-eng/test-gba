# TECHNICAL_ARCHITECTURE.md

## Moteur retenu
`rh-hideout/pokeemerald-expansion` (fork enrichi de `pret/pokeemerald`, maintenu activement par la
communauté ROM Hacking Hideout), compilé en mode `MODERN=1` (toolchain `gcc-arm-none-eabi`).

**Changement de base depuis la session 1** : on utilisait initialement `pret/pokeemerald` vanilla. Après
analyse des 4 ROMs de référence fournies par Thomas (Unbound, Sword/Shield Ultimate Plus, Heart and Soul,
Squirrels), constat que les hacks de référence "qualité pro" (notamment Unbound, unanimement salué) reposent
sur des moteurs enrichis (CFRU pour Unbound, non compatible decomp — équivalent decomp = pokeemerald-expansion).
Migration effectuée et validée par un 2e build complet (ROM 32 Mo, 79 % d'occupation).

**Licence** : usage libre, attribution "RHH (Rom Hacking Hideout)" requise dans les crédits du jeu final.
Pas de restriction commerciale bloquante (contrairement à CFRU, qui interdit toute monétisation) — sujet
sans impact pour un projet non-commercial mais à garder en tête si le projet évoluait.

## Mécaniques directement disponibles (config activable/désactivable dans `include/config/`)
Cela répond à l'objectif "impression d'un vrai jeu, pas juste Émeraude reskinné" du cahier des charges :
- Type Fée, split Physique/Spécial/Statut (mécaniques post-Gen3, désactivables si on veut rester fidèle Gen1-2)
- Objets, capacités et attaques jusqu'à Écarlate/Violet (à utiliser avec parcimonie — rester crédible dans le cadre "saison 1 / Kanto")
- TM réutilisables, EXP Share moderne configurable, HM sans obligation de les enseigner
- Pokédex façon HeartGold/SoulSilver (interface, tri, recherche)
- Structures dresseurs personnalisables (nature, EVs/IVs, objet tenu, etc. par dresseur — utile pour Team Rocket, champions, Régis)
- Compatible Porymap (versions récentes) pour l'édition de maps si Thomas l'utilise en local

## Toolchain validée dans cet environnement
```
apt-get install build-essential binutils-arm-none-eabi libpng-dev gcc-arm-none-eabi
cd engine && make MODERN=1 -j$(nproc)
```
Sortie : `engine/pokeemerald_modern.gba`. Build testé et fonctionnel (2 compilations réussies à ce jour).

## Encodage des caractères français
Bonne nouvelle confirmée par audit : `engine/charmap.txt` contient déjà nativement
à, â, ç, è, é, ê, î, ï, ô, œ, ù, û ainsi que les majuscules À, Ç, È, É.
→ **La limitation de police rencontrée sur le projet FireRed Rocket Edition (patch binaire) ne s'applique pas ici.**
Seule limite identifiée à ce jour : les guillemets français « » sont absents (seuls “ ” existent) → utiliser “ ” dans tous les dialogues.

## Structure du projet
```
kanto_saison1_project/
  docs/                  ← documentation vivante (ce dossier)
  engine/                ← fork de pret/pokeemerald (code source du jeu)
    data/text/           ← dialogues (fichiers .inc, éditables directement)
    data/scripts/        ← logique d'événements
    data/maps/<NomMap>/  ← scripts, connexions, événements par map
    src/data/region_map/ ← noms de villes affichés à l'écran (JSON)
    graphics/, sound/    ← assets (sprites, tiles, musique)
  tools/                 ← scripts Python maison (rendu de preview de maps, etc.)
```

## Répartition des rôles
- **Claude** : scénario, dialogues FR, données (dresseurs/Pokémon/objets/flags), scripts d'événements,
  structure des maps (fichiers), compilation, QA automatisée, documentation.
- **Thomas** : validation visuelle des maps (via Porymap en local si souhaité) et playtest réel sous émulateur —
  je n'ai ni interface graphique ni émulateur dans ce sandbox.

## Ce qui n'est PAS récupéré du projet TrashMan
Le hack TrashMan reste un patch binaire scellé sur une ROM figée. On repart d'Émeraude vanilla en tant que
moteur source. TrashMan peut servir de référence de contenu/ton si besoin, jamais de base de code.

## Limites connues
- Pas de rendu visuel en direct → prévoir un script de preview PNG (composition des tuiles) avant validation finale d'une map.
- Pas d'émulateur → validation = compilation propre + tests automatisés du framework pokeemerald (`make check` — à explorer en Phase 1).
- Réseau sortant limité à GitHub / PyPI / npm / dépôts Ubuntu — pas d'accès aux forums/outils communautaires hors GitHub.
