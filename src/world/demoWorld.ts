import fs from "node:fs";
import path from "node:path";
import { generateMap } from "../generator/index.js";
import { saveMap, mapDir, savePreview } from "../mapIO.js";
import { validateMap } from "../validator/index.js";
import { formatReport } from "../validator/report.js";
import { loadWorldRegistry, saveWorldRegistry, registerMap, checkConnectionsConsistency } from "./worldGraph.js";
import { deriveSeed } from "../generator/rng.js";
import type { GameMap } from "../schema/types.js";

/**
 * Construit le petit monde de démonstration demandé section 21 : une ville
 * de départ, une route, une forêt — connexions bidirectionnelles vérifiées,
 * Centre Pokémon généré en intérieur de la ville, chaque map validée et
 * prévisualisée. Sert de preuve de bout en bout du pipeline (section 19).
 */
export async function buildDemoWorld(rootSeed: number): Promise<void> {
  const TOWN_ID = "bourg_depart";
  const ROUTE_ID = "route_1";
  const FOREST_ID = "foret_emeraude";
  const region = "Région Céladopolis";

  console.log("=== GÉNÉRATION DU MONDE DE DÉMONSTRATION ===\n");

  const { map: town, interiorMaps: townInteriors } = generateMap({
    type: "town",
    name: "Bourg Départ",
    id: TOWN_ID,
    seed: deriveSeed(rootSeed, TOWN_ID),
    region,
    progression: "early",
    connections: [{ direction: "south", mapId: ROUTE_ID, offset: 0 }],
  });

  const { map: route, interiorMaps: routeInteriors } = generateMap({
    type: "route",
    name: "Route 1",
    id: ROUTE_ID,
    seed: deriveSeed(rootSeed, ROUTE_ID),
    region,
    progression: "early",
    connections: [
      { direction: "north", mapId: TOWN_ID, offset: 0 },
      { direction: "south", mapId: FOREST_ID, offset: 0 },
    ],
  });

  const { map: forest, interiorMaps: forestInteriors } = generateMap({
    type: "forest",
    name: "Forêt Émeraude",
    id: FOREST_ID,
    seed: deriveSeed(rootSeed, FOREST_ID),
    region,
    progression: "early",
    connections: [{ direction: "north", mapId: ROUTE_ID, offset: 0 }],
  });

  const allMaps: GameMap[] = [town, route, forest, ...townInteriors, ...routeInteriors, ...forestInteriors];

  for (const m of allMaps) saveMap(m);

  const registry = loadWorldRegistry();
  for (const m of allMaps) registerMap(registry, m);
  saveWorldRegistry(registry);

  const consistency = checkConnectionsConsistency([town, route, forest]);
  console.log("--- Cohérence des connexions inter-maps (section 11) ---");
  if (consistency.errors.length === 0) {
    console.log("✓ Toutes les connexions sont bidirectionnelles et cohérentes.\n");
  } else {
    for (const e of consistency.errors) console.log(`✗ ${e}`);
    console.log("");
  }

  let allPassed = consistency.errors.length === 0;
  for (const m of allMaps) {
    const report = validateMap(m, allMaps);
    const reportText = formatReport(report);
    console.log(reportText);
    console.log("");
    fs.writeFileSync(path.join(mapDir(m.metadata.id), "validation_report.txt"), reportText, "utf-8");
    await savePreview(m);
    if (!report.passed) allPassed = false;
  }

  console.log(
    `=== MONDE GÉNÉRÉ : ${allMaps.length} maps (${[town, route, forest].length} extérieures + ${
      allMaps.length - 3
    } intérieurs) — ${allPassed ? "VALIDATION GLOBALE OK" : "VALIDATION GLOBALE EN ÉCHEC, voir ERRORS ci-dessus"} ===`,
  );
}
