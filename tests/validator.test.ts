import { describe, it, expect } from "vitest";
import { generateMap } from "../src/generator/index.js";
import { validateMap } from "../src/validator/index.js";
import type { GameMap } from "../src/schema/types.js";

describe("validateMap — détecte les cartes réellement cassées", () => {
  it("signale un warp hors-limites", () => {
    const { map } = generateMap({ type: "route", name: "Route", id: "route_broken_warp", seed: 3 });
    const broken: GameMap = JSON.parse(JSON.stringify(map));
    broken.warps.push({
      id: "w_bad",
      x: 9999,
      y: 9999,
      direction: "south",
      destinationMap: "nowhere",
      destinationWarpId: "w_x",
    });
    const report = validateMap(broken);
    expect(report.passed).toBe(false);
    expect(report.checklist.Warps).toBe(false);
    expect(report.errors.some((e) => e.includes("w_bad"))).toBe(true);
  });

  it("signale un objet posé sur une tuile solide", () => {
    const { map } = generateMap({ type: "town", name: "Ville", id: "ville_broken_obj", seed: 4 });
    const broken: GameMap = JSON.parse(JSON.stringify(map));
    // Cherche une tuile solide connue (le cadre d'arbres en bordure de map) et y pose un objet.
    broken.objects.push({ id: "obj_bad", type: "item_visible", x: 0, y: 0 });
    const report = validateMap(broken);
    expect(report.passed).toBe(false);
    expect(report.errors.some((e) => e.includes("obj_bad"))).toBe(true);
  });

  it("signale une table de rencontres incohérente (levelMin > levelMax)", () => {
    const { map } = generateMap({ type: "route", name: "Route", id: "route_broken_lvl", seed: 6 });
    const broken: GameMap = JSON.parse(JSON.stringify(map));
    broken.encounters.grass = [{ pokemon: "Test", levelMin: 50, levelMax: 5, rarity: "common", weight: 1 }];
    const report = validateMap(broken);
    expect(report.passed).toBe(false);
    expect(report.checklist.Encounters).toBe(false);
  });

  it("valide une map correctement générée sans erreur (les warnings n'empêchent pas le passage)", () => {
    const { map } = generateMap({ type: "beach", name: "Plage", id: "plage_ok", seed: 8 });
    const report = validateMap(map);
    expect(report.errors).toEqual([]);
    expect(report.passed).toBe(true);
  });
});
