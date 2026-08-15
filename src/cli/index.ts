#!/usr/bin/env node
import { Command } from "commander";
import { generateMap, type ConnectionSpec } from "../generator/index.js";
import { saveMap, loadMap, validateAndPreview, savePreview } from "../mapIO.js";
import { validateMap } from "../validator/index.js";
import { formatReport } from "../validator/report.js";
import { listAvailableTypes } from "../data/loader.js";
import { loadWorldRegistry, saveWorldRegistry, registerMap } from "../world/worldGraph.js";
import type { Direction, ProgressionTier } from "../schema/types.js";
import { buildDemoWorld } from "../world/demoWorld.js";

const program = new Command();
program.name("pokemap").description("Générateur de cartes Pokémon (test-gba)").version("1.0.0");

function collect(value: string, previous: string[]): string[] {
  return [...previous, value];
}

program
  .command("generate-map")
  .description("Génère une map (et ses éventuels intérieurs), la valide et produit une preview.")
  .requiredOption("--type <type>", "Type de biome (town, village, route, forest, cave, mountain, beach, special)")
  .requiredOption("--name <name>", "Nom affiché de la map")
  .option("--id <id>", "Identifiant technique (slug), déduit du nom si absent")
  .option("--seed <seed>", "Graine numérique (reproductibilité)", (v) => parseInt(v, 10))
  .option("--region <region>", "Nom de la région", "Région Démo")
  .option("--progression <tier>", "early | mid | late", "early")
  .option("--width <n>", "Largeur en tuiles (sinon tirée depuis le template)", (v) => parseInt(v, 10))
  .option("--height <n>", "Hauteur en tuiles (sinon tirée depuis le template)", (v) => parseInt(v, 10))
  .option("--directions <list>", "Directions de sortie séparées par des virgules (ex: north,south)")
  .option("--link <direction=mapId>", "Câble une connexion vers une map déjà générée (répétable)", collect, [])
  .action(async (opts) => {
    const connections: ConnectionSpec[] = [];
    if (opts.link.length > 0) {
      for (const spec of opts.link as string[]) {
        const [direction, mapId] = spec.split("=");
        connections.push({ direction: direction as Direction, mapId });
      }
    } else if (opts.directions) {
      for (const d of (opts.directions as string).split(",")) connections.push({ direction: d.trim() as Direction });
    }

    const { map, interiorMaps } = generateMap({
      type: opts.type,
      name: opts.name,
      id: opts.id,
      seed: opts.seed,
      region: opts.region,
      progression: opts.progression as ProgressionTier,
      width: opts.width,
      height: opts.height,
      connections: connections.length > 0 ? connections : undefined,
    });

    saveMap(map);
    for (const im of interiorMaps) saveMap(im);

    const registry = loadWorldRegistry();
    registerMap(registry, map);
    for (const im of interiorMaps) registerMap(registry, im);
    saveWorldRegistry(registry);

    const { reportText } = await validateAndPreview(map);
    console.log(reportText);
    for (const im of interiorMaps) {
      const { reportText: interiorReport } = await validateAndPreview(im);
      console.log("\n" + interiorReport);
    }
    console.log(`\nMap sauvegardée dans maps/${map.metadata.id}/`);
  });

program
  .command("validate-map <mapId>")
  .description("Rejoue le validateur sur une map déjà générée.")
  .action((mapId: string) => {
    const map = loadMap(mapId);
    const report = validateMap(map);
    console.log(formatReport(report));
    process.exitCode = report.passed ? 0 : 1;
  });

program
  .command("preview-map <mapId>")
  .description("Régénère uniquement le preview.png d'une map déjà générée.")
  .action(async (mapId: string) => {
    const map = loadMap(mapId);
    const out = await savePreview(map);
    console.log(`Preview régénérée: ${out}`);
  });

program
  .command("list-templates")
  .description("Liste les types de map disponibles dans templates/.")
  .action(() => {
    for (const t of listAvailableTypes()) console.log(t);
  });

program
  .command("generate-world")
  .description("Génère le petit monde de démonstration connecté (ville → route → forêt) avec Centre Pokémon.")
  .option("--seed <seed>", "Graine racine du monde", (v) => parseInt(v, 10), 20260815)
  .action(async (opts) => {
    await buildDemoWorld(opts.seed);
  });

program.parseAsync(process.argv);
