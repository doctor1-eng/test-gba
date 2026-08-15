import fs from "node:fs";
import path from "node:path";
import { MAPS_DIR } from "./paths.js";
import type { GameMap } from "./schema/types.js";
import { renderMapPreview } from "./preview/render.js";
import { validateMap } from "./validator/index.js";
import { formatReport } from "./validator/report.js";

export function mapDir(mapId: string): string {
  return path.join(MAPS_DIR, mapId);
}

/** Sauvegarde map.json + metadata.json (section 14 : maps/<name>/map.json, metadata.json). */
export function saveMap(map: GameMap): void {
  const dir = mapDir(map.metadata.id);
  fs.mkdirSync(dir, { recursive: true });
  fs.writeFileSync(path.join(dir, "map.json"), JSON.stringify(map, null, 2), "utf-8");
  fs.writeFileSync(path.join(dir, "metadata.json"), JSON.stringify(map.metadata, null, 2), "utf-8");
}

export function loadMap(mapId: string): GameMap {
  const filePath = path.join(mapDir(mapId), "map.json");
  return JSON.parse(fs.readFileSync(filePath, "utf-8"));
}

export function listSavedMapIds(): string[] {
  if (!fs.existsSync(MAPS_DIR)) return [];
  return fs.readdirSync(MAPS_DIR, { withFileTypes: true }).filter((d) => d.isDirectory()).map((d) => d.name);
}

export async function savePreview(map: GameMap): Promise<string> {
  const outPath = path.join(mapDir(map.metadata.id), "preview.png");
  await renderMapPreview(map, outPath);
  return outPath;
}

/** Étapes 11-12 du pipeline (section 16) : valider puis prévisualiser, et écrire le rapport à côté de la map. */
export async function validateAndPreview(map: GameMap, worldMaps?: GameMap[]): Promise<{ reportText: string; passed: boolean }> {
  const report = validateMap(map, worldMaps);
  const reportText = formatReport(report);
  const dir = mapDir(map.metadata.id);
  fs.mkdirSync(dir, { recursive: true });
  fs.writeFileSync(path.join(dir, "validation_report.txt"), reportText, "utf-8");
  await savePreview(map);
  return { reportText, passed: report.passed };
}
