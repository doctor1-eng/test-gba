import { useGameStore } from '../../store/gameStore';
import { META_UPGRADES } from '../../data/metaUpgrades';
import { Button } from '../ui/Button';
import './screens.css';

export function UpgradesScreen() {
  const meta = useGameStore((s) => s.meta);
  const purchaseUpgrade = useGameStore((s) => s.purchaseUpgrade);
  const goTo = useGameStore((s) => s.goTo);

  return (
    <div className="screen">
      <h1 className="gothic-title screen-title">Améliorations</h1>
      <p className="screen-subtitle">🜏 {meta.ancientRelics} reliques anciennes</p>

      <div className="choice-list">
        {META_UPGRADES.map((upgrade) => {
          const owned = meta.unlockedUpgradeIds.includes(upgrade.id) || (upgrade.heroId && meta.unlockedHeroIds.includes(upgrade.heroId));
          return (
            <div key={upgrade.id} className="upgrade-card">
              <div className="upgrade-card-head">
                <span className="upgrade-name">{upgrade.name}</span>
                <span className="gold-badge">🜏 {upgrade.cost}</span>
              </div>
              <p className="upgrade-desc">{upgrade.description}</p>
              <Button
                small
                variant={owned ? 'secondary' : 'primary'}
                disabled={!!owned || meta.ancientRelics < upgrade.cost}
                onClick={() => purchaseUpgrade(upgrade.id)}
              >
                {owned ? 'Acquis' : 'Débloquer'}
              </Button>
            </div>
          );
        })}
      </div>

      <Button block onClick={() => goTo('mainMenu')}>Retour</Button>
    </div>
  );
}
