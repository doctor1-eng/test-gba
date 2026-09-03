# Systèmes de gameplay — vue d'ensemble

## Résumé honnête

Sur les ~20 systèmes listés dans la demande d'analyse (exploration, déplacements, collisions, rencontres
sauvages, combats, IA, progression, expérience, niveaux, évolution, objets, boutiques, centres Pokémon,
dresseurs, badges, quêtes, événements scénarisés, sauvegarde, menus, inventaire, Pokédex, capacités,
talents, statistiques, systèmes spécifiques Unbound), **aucun n'est identifiable individuellement** dans
un pack de métadonnées statistiques sans contenu (pas de code désassemblé, pas de texte, pas de
graphismes). Ce document ne liste donc pas 20 sections vides — il explique pourquoi, une seule fois, et
renvoie vers ce qui est réellement exploitable ailleurs dans cette analyse.

## Ce qui EST exploitable indirectement

| Ce qu'on cherchait | Où regarder dans cette analyse | Niveau |
|---|---|---|
| Localisation approximative de code vs données | `rom-map.md`, `code-analysis.md` | HYPOTHÈSE |
| Tables de données structurées candidates (espèces/attaques) | `pokemon-data.md` | HYPOTHÈSE |
| Zones de graphismes/audio compressés candidates | `compression.md` | HYPOTHÈSE, signal partiellement bruité |
| Conventions architecturales générales (moteur FireRed) | `maps.md`, `scripts.md`, `battle-system.md` (sections "référence publique") | PROBABLE, mais par analogie externe, pas par déduction du pack |

## Pourquoi ne pas "combler" avec des suppositions détaillées

La demande initiale insiste explicitement sur la règle "ne jamais transformer une hypothèse en vérité".
Produire ici une description détaillée de "comment fonctionne probablement l'expérience/les niveaux/
l'évolution dans Unbound" reviendrait à rédiger, dans le vide, une description générique de n'importe
quel jeu Pokémon GBA — pas une découverte issue de l'analyse. Ce serait présenté comme une analyse alors
que ce serait en réalité juste... ce qu'on sait déjà publiquement sur tous les jeux Pokémon GBA, sans
valeur ajoutée par le pack fourni. Ça aurait gonflé le document sans gonfler la connaissance réelle.

## Ce que cette analyse a réellement produit de solide

1. Confirmation croisée (source publique + pack) qu'Unbound est bâti sur FireRed.
2. Une cartographie régionale par densité de signal, avec deux signaux explicitement identifiés comme
   probablement peu fiables (ARM BL, signatures de compression) — une découverte méthodologique en soi.
3. Une explication solide (pas une supposition) de pourquoi `string-index.csv` ne représente
   probablement pas le texte de dialogue réel du jeu (encodage charmap vs scan ASCII générique).
4. Des candidats de tables de données par coïncidence de taille de struct, clairement qualifiés HYPOTHÈSE.

C'est moins spectaculaire qu'un inventaire de 20 systèmes détaillés, mais c'est ce que les
données permettent honnêtement de soutenir. Voir `FINAL-REPORT.md` pour la synthèse et
`project-recommendations.md` pour ce qui en découle pour notre projet.
