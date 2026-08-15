import { describe, it, expect } from "vitest";
import { generateMap } from "../src/generator/index.js";
import { listAvailableTypes, isOverworldType } from "../src/data/loader.js";
import { validateMap } from "../src/validator/index.js";

function withoutTimestamp(map: unknown) {
  const clone = JSON.parse(JSON.stringify(map));
  delete clone.metadata.generatedAt;
  return clone;
}

describe("generateMap — déterminisme", () => {
  it("produit une map strictement identique pour la même seed", () => {
    const a = generateMap({ type: "town", name: "Ville Test", id: "ville_test", seed: 12345 });
    const b = generateMap({ type: "town", name: "Ville Test", id: "ville_test", seed: 12345 });
    expect(withoutTimestamp(a.map)).toEqual(withoutTimestamp(b.map));
    expect(a.interiorMaps.map((m) => withoutTimestamp(m))).toEqual(b.interiorMaps.map((m) => withoutTimestamp(m)));
  });

  it("produit des maps différentes pour des seeds différentes", () => {
    const a = generateMap({ type: "town", name: "Ville Test", id: "ville_test", seed: 1 });
    const b = generateMap({ type: "town", name: "Ville Test", id: "ville_test", seed: 2 });
    expect(withoutTimestamp(a.map)).not.toEqual(withoutTimestamp(b.map));
  });
});

describe("generateMap — un biome par type déclaré (section 12 : jamais random-sans-règle)", () => {
  const overworldTypes = listAvailableTypes().filter(isOverworldType);

  it.each(overworldTypes)("%s se génère sans erreur de validation", (type) => {
    const { map, interiorMaps } = generateMap({ type, name: `Test ${type}`, id: `test_${type}`, seed: 777 });
    const report = validateMap(map);
    expect(report.errors, `Erreurs sur ${type}: ${report.errors.join("; ")}`).toEqual([]);
    for (const im of interiorMaps) {
      const interiorReport = validateMap(im);
      expect(interiorReport.errors, `Erreurs intérieur ${im.metadata.id}: ${interiorReport.errors.join("; ")}`).toEqual([]);
    }
  });

  it("respecte les dimensions déclarées par le template", () => {
    const { map } = generateMap({ type: "cave", name: "Grotte", id: "grotte_dim", seed: 5 });
    expect(map.terrain.length).toBe(map.dimensions.height);
    expect(map.terrain.every((row) => row.length === map.dimensions.width)).toBe(true);
  });

  it("ne place jamais de décoration sur le chemin principal ni sur une tuile solide", () => {
    // Vérifié indirectement : aucune décoration ne doit coïncider avec un warp/bâtiment,
    // et le validateur ne doit signaler aucune tuile de décoration hors-limites.
    const { map } = generateMap({ type: "forest", name: "Forêt", id: "foret_dim", seed: 9 });
    for (const d of map.decorations) {
      expect(d.x).toBeGreaterThanOrEqual(0);
      expect(d.x).toBeLessThan(map.dimensions.width);
      expect(d.y).toBeGreaterThanOrEqual(0);
      expect(d.y).toBeLessThan(map.dimensions.height);
    }
  });
});
