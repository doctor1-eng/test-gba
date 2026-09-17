import type { SVGProps } from 'react';

type IconProps = SVGProps<SVGSVGElement>;

const wrap = (children: React.ReactNode) => (props: IconProps) => (
  <svg viewBox="0 0 64 64" fill="currentColor" xmlns="http://www.w3.org/2000/svg" {...props}>
    {children}
  </svg>
);

// --- Héros ---
const CroiseIcon = wrap(<path d="M28 4h8v20h20v8H36v28h-8V32H8v-8h20V4z" />);

const HeretiqueIcon = wrap(
  <>
    <path d="M18 20C10 8 4 10 6 20c2-4 8-4 12 2z" />
    <path d="M46 20c8-12 14-10 12 0-2-4-8-4-12 2z" />
    <circle cx="32" cy="28" r="14" />
    <path d="M20 34h24l-4 16h-6l-2-8-2 8h-6z" />
    <circle cx="26" cy="26" r="3" fill="#0b0807" />
    <circle cx="38" cy="26" r="3" fill="#0b0807" />
  </>,
);

const PesteIcon = wrap(
  <>
    <path d="M14 26C14 12 26 6 36 8c14 2 20 12 22 22 2 6-4 4-8 4 0 6-6 10-12 10h-4c0 4-4 6-8 4-4-2-4-6-2-8-8-2-12-8-10-14z" />
    <circle cx="30" cy="24" r="4" fill="#0b0807" />
  </>,
);

const BourreauIcon = wrap(
  <>
    <rect x="30" y="10" width="4" height="46" rx="2" />
    <path d="M34 10c12-2 20 6 18 16-8-2-16-4-18-6z" />
  </>,
);

// --- Ennemis : Le Hameau ---
const FossoyeurIcon = wrap(
  <>
    <rect x="29" y="6" width="4" height="34" rx="2" />
    <path d="M20 40h24l-6 14c-4 4-8 4-12 0z" />
    <rect x="10" y="56" width="44" height="4" />
  </>,
);

const CorbeauxIcon = wrap(
  <path d="M32 14C20 10 8 18 6 28c8-4 14-2 18 2-6 2-12 8-12 16 8-6 14-8 18-6-2 6 0 12 6 14-2-6 0-10 4-12 6 4 14 2 18-4-8 0-12-4-12-10 4-4 10-4 14 0-4-12-16-18-28-14z" />,
);

const VillageoisCorrompuIcon = wrap(
  <>
    <circle cx="32" cy="28" r="18" />
    <path d="M32 10l-4 10 6 4-8 8 8 2-6 10" stroke="#0b0807" strokeWidth="2" fill="none" />
    <rect x="24" y="44" width="16" height="14" rx="4" />
  </>,
);

const PretreRenegatIcon = wrap(
  <>
    <circle cx="32" cy="24" r="16" fill="none" stroke="currentColor" strokeWidth="3" />
    <path d="M28 46h8v14h-8z" />
    <path d="M20 50h24v6H20z" />
  </>,
);

const FaucheuseIcon = wrap(
  <>
    <circle cx="26" cy="26" r="14" />
    <path d="M14 30h24l-4 14h-6l-2-6-2 6h-6z" />
    <circle cx="20" cy="24" r="3" fill="#0b0807" />
    <circle cx="30" cy="24" r="3" fill="#0b0807" />
    <path d="M40 10c16 0 20 16 8 26 4-10-2-20-12-20z" />
    <rect x="38" y="16" width="3" height="40" rx="1" />
  </>,
);

// --- Ennemis : Les Catacombes ---
const SqueletteGuerrierIcon = wrap(
  <>
    <circle cx="32" cy="24" r="14" />
    <path d="M20 30h24l-4 14h-6l-2-6-2 6h-6z" />
    <circle cx="26" cy="22" r="3" fill="#0b0807" />
    <circle cx="38" cy="22" r="3" fill="#0b0807" />
    <path d="M14 44l36 12M50 44l-36 12" stroke="currentColor" strokeWidth="4" fill="none" />
  </>,
);

const VampireMineurIcon = wrap(
  <>
    <path d="M32 20C28 10 14 8 6 16c8 0 14 4 18 10-8 0-16 4-20 12 10-2 18-4 24-8-2 6 0 12 4 16 4-4 6-10 4-16 6 4 14 6 24 8-4-8-12-12-20-12 4-6 10-10 18-10-8-8-22-6-26 4z" />
    <path d="M28 30l4 6 4-6" fill="none" stroke="#0b0807" strokeWidth="2" />
  </>,
);

const SpectreHurleurIcon = wrap(
  <>
    <path d="M32 6c14 0 22 14 20 28 6 6 4 16-2 22-4-6-6-12-6-12-4 6-8 8-12 8s-8-2-12-8c0 0-2 6-6 12-6-6-8-16-2-22-2-14 6-28 20-28z" />
    <ellipse cx="32" cy="30" rx="6" ry="8" fill="#0b0807" />
  </>,
);

const GoulePutrideIcon = wrap(
  <>
    <circle cx="32" cy="26" r="15" />
    <path d="M18 32q14 16 28 0l-3 16q-11 6-22 0z" />
    <circle cx="26" cy="24" r="3" fill="#0b0807" />
    <circle cx="38" cy="24" r="3" fill="#0b0807" />
    <circle cx="20" cy="46" r="2" />
    <circle cx="30" cy="50" r="2" />
    <circle cx="42" cy="46" r="2" />
  </>,
);

const CharnierVivantIcon = wrap(
  <>
    <ellipse cx="32" cy="34" rx="24" ry="18" />
    <path d="M14 30h4M20 26h4M26 24h4M34 24h4M40 26h4M46 30h4" stroke="#0b0807" strokeWidth="3" />
    <circle cx="32" cy="30" r="7" fill="#0b0807" />
    <circle cx="32" cy="30" r="3" />
  </>,
);

// --- Types de cartes ---
const AttackIcon = wrap(
  <>
    <path d="M32 6l4 4-4 30-4-30z" />
    <rect x="28" y="38" width="8" height="4" />
    <rect x="30" y="42" width="4" height="16" rx="1" />
  </>,
);

const HealIcon = wrap(
  <>
    <path d="M26 8h12v10l10 26c2 6-2 12-9 12H25c-7 0-11-6-9-12l10-26z" />
    <rect x="29" y="4" width="6" height="8" />
    <path d="M26 40h12M32 34v12" stroke="#0b0807" strokeWidth="3" />
  </>,
);

const GuardIcon = wrap(<path d="M32 6l22 8v16c0 14-10 24-22 28-12-4-22-14-22-28V14z" />);

const SkillIcon = wrap(<path d="M32 4l6 22 22 6-22 6-6 22-6-22-22-6 22-6z" />);

const CurseIcon = wrap(
  <>
    <path d="M4 32c10-16 46-16 56 0-10 16-46 16-56 0z" />
    <circle cx="32" cy="32" r="8" fill="#0b0807" />
    <circle cx="32" cy="32" r="3" />
  </>,
);

export const UNIT_ICONS: Record<string, ReturnType<typeof wrap>> = {
  croise: CroiseIcon,
  heretique: HeretiqueIcon,
  peste: PesteIcon,
  bourreau: BourreauIcon,
  fossoyeur: FossoyeurIcon,
  corbeaux_nuee: CorbeauxIcon,
  villageois_corrompu: VillageoisCorrompuIcon,
  pretre_renegat: PretreRenegatIcon,
  la_faucheuse: FaucheuseIcon,
  squelette_guerrier: SqueletteGuerrierIcon,
  vampire_mineur: VampireMineurIcon,
  spectre_hurleur: SpectreHurleurIcon,
  goule_putride: GoulePutrideIcon,
  charnier_vivant: CharnierVivantIcon,
};

export const CARD_TYPE_ICONS: Record<string, ReturnType<typeof wrap>> = {
  attack: AttackIcon,
  heal: HealIcon,
  guard: GuardIcon,
  skill: SkillIcon,
  curse: CurseIcon,
};
