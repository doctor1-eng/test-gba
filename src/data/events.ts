import type { NarrativeEvent } from '../engine/types';

export const EVENTS: NarrativeEvent[] = [
  {
    id: 'autel_profane',
    title: 'L\'Autel Profané',
    text: "Un autel de pierre noire, maculé d'une substance qui n'est plus tout à fait du sang. Des offrandes s'y entassent encore, comme si quelqu'un venait tout juste de les déposer.",
    choices: [
      {
        id: 'prier',
        label: 'Prier devant l\'autel',
        outcomes: [
          { weight: 4, resultText: 'Une chaleur étrange vous traverse. Vous vous sentez plus résolu.', stressDeltaAll: -10 },
          { weight: 2, resultText: 'L\'autel répond, mais pas comme vous l\'espériez.', stressDeltaAll: 12, hpDeltaRandom: -4 },
        ],
      },
      {
        id: 'fouiller',
        label: 'Fouiller les offrandes',
        outcomes: [
          { weight: 3, resultText: 'Quelques pièces, une babiole sans valeur maudite.', goldDelta: 18 },
          { weight: 1, resultText: 'Une relique, enfouie sous les cendres.', relicId: 'bourse_maudite' },
        ],
      },
      {
        id: 'detruire',
        label: 'Détruire l\'autel',
        outcomes: [
          { weight: 3, resultText: 'La pierre se brise en silence. Un poids invisible s\'efface.', stressDeltaAll: -6 },
          { weight: 1, resultText: 'Quelque chose hurle, quelque part sous vos pieds.', quirkId: 'random-negative' },
        ],
      },
    ],
  },
  {
    id: 'mendiant_aveugle',
    title: "Le Mendiant Aveugle",
    text: "Assis contre un mur effondré, un vieillard aux yeux blancs tend la main. « J'ai faim, » murmure-t-il, « depuis plus longtemps que vous ne pouvez l'imaginer. »",
    choices: [
      {
        id: 'nourrir',
        label: 'Lui donner à manger',
        outcomes: [
          { weight: 3, resultText: 'Il sourit, pour la première fois depuis des décennies.', goldDelta: -10, quirkId: 'random-positive' },
          { weight: 1, resultText: 'Un simple merci. Rien de plus.', goldDelta: -10 },
        ],
      },
      {
        id: 'ignorer',
        label: 'Poursuivre votre chemin',
        outcomes: [{ weight: 1, resultText: 'Vous ne vous retournez pas. Vous entendez presque son silence.', stressDeltaAll: 3 }],
      },
      {
        id: 'voler',
        label: 'Lui prendre sa bourse',
        outcomes: [
          { weight: 3, resultText: 'Il ne dit rien. Il vous regarde partir, ou du moins le semble-t-il.', goldDelta: 15, stressDeltaAll: 8 },
          { weight: 1, resultText: 'Sa main se referme sur votre poignet, glacée. Vous ne l\'oublierez pas.', goldDelta: 15, quirkId: 'random-negative' },
        ],
      },
    ],
  },
  {
    id: 'fosse_commune',
    title: 'La Fosse Commune',
    text: "Des dizaines de corps, entassés sans cérémonie. Certains portent encore des bijoux. D'autres portent encore, peut-être, un souffle de vie.",
    choices: [
      {
        id: 'fouiller_fosse',
        label: 'Chercher des reliques parmi les corps',
        outcomes: [
          { weight: 2, resultText: 'Vos mains se referment sur quelque chose d\'ancien.', relicId: 'reliquaire_os' },
          { weight: 2, resultText: 'Une odeur infecte vous prend à la gorge.', hpDeltaAll: -5, stressDeltaAll: 6 },
        ],
      },
      {
        id: 'laisser_fosse',
        label: 'Laisser les morts en paix',
        outcomes: [{ weight: 1, resultText: 'Vous vous signez, par réflexe plus que par foi.', stressDeltaAll: -4 }],
      },
    ],
  },
  {
    id: 'miroir_brise',
    title: 'Le Miroir Brisé',
    text: "Un miroir sur pied, fendu en deux, trône au milieu d'une pièce vide. Votre reflet, dedans, ne bouge pas tout à fait comme vous.",
    choices: [
      {
        id: 'regarder',
        label: 'Regarder dans le miroir',
        outcomes: [
          { weight: 2, resultText: 'Vous y voyez un instant clair, presque un souvenir. Cela vous apaise.', stressDeltaRandom: -15 },
          { weight: 2, resultText: 'Ce que vous y voyez n\'était pas censé exister.', stressDeltaRandom: 20 },
        ],
      },
      {
        id: 'briser',
        label: 'Briser le miroir',
        outcomes: [
          { weight: 3, resultText: 'Le verre cède dans un cri de silence. Rien ne semble en sortir.', goldDelta: 10 },
          { weight: 1, resultText: 'Sept ans de malheur, dit-on. Vous en doutiez encore, avant ce soir.', hpDeltaRandom: -8 },
        ],
      },
    ],
  },
  {
    id: 'chant_lointain',
    title: 'Le Chant Lointain',
    text: "Une voix, quelque part dans les ténèbres, chante un air que vous ne connaissez pas et que, pourtant, vous semblez avoir toujours su.",
    choices: [
      {
        id: 'suivre',
        label: 'Suivre le chant',
        outcomes: [
          { weight: 2, resultText: 'Vous trouvez une bourse abandonnée près d\'un cadavre depuis longtemps froid.', goldDelta: 25 },
          { weight: 2, resultText: 'Le chant se tait d\'un coup. Ce qui l\'accompagnait, non.', hpDeltaAll: -6, stressDeltaAll: 10 },
        ],
      },
      {
        id: 'ignorer_chant',
        label: 'Ne pas y prêter attention',
        outcomes: [{ weight: 1, resultText: 'Le chant persiste un moment dans votre esprit, puis s\'efface.', stressDeltaAll: 2 }],
      },
    ],
  },
  {
    id: 'prisonnier_enchaine',
    title: 'Le Prisonnier Enchaîné',
    text: "Un homme, enchaîné à un mur, lève vers vous un regard qui n'a plus rien d'humain — ou peut-être tout ce qu'il en reste.",
    choices: [
      {
        id: 'liberer',
        label: 'Le libérer',
        outcomes: [
          { weight: 2, resultText: 'Il murmure un remerciement et disparaît dans l\'ombre. Vous trouvez, plus tard, qu\'il vous a laissé quelque chose.', relicId: 'amulette_devote' },
          { weight: 2, resultText: 'Ce n\'était pas un homme. Vous le comprenez un instant trop tard.', hpDeltaRandom: -12, stressDeltaAll: 8 },
        ],
      },
      {
        id: 'laisser',
        label: 'Le laisser enchaîné',
        outcomes: [{ weight: 1, resultText: 'Ses cris vous poursuivent un long moment dans les couloirs.', stressDeltaAll: 5 }],
      },
    ],
  },
];
