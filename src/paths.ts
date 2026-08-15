import { fileURLToPath } from "node:url";
import path from "node:path";

/** Racine du dépôt, calculée depuis ce fichier (src/paths.ts → racine). */
export const ROOT_DIR = path.resolve(path.dirname(fileURLToPath(import.meta.url)), "..");

export const TEMPLATES_DIR = path.join(ROOT_DIR, "templates");
export const ASSETS_DIR = path.join(ROOT_DIR, "assets");
export const DATA_DIR = path.join(ROOT_DIR, "src", "data");
export const MAPS_DIR = path.join(ROOT_DIR, "maps");
export const WORLD_DIR = path.join(ROOT_DIR, "world");
