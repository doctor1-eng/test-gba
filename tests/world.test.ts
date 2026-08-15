import { describe, it, expect } from "vitest";
import { checkConnectionsConsistency, opposite } from "../src/world/worldGraph.js";
import { generateMap } from "../src/generator/index.js";

describe("checkConnectionsConsistency — section 11", () => {
  it("ne signale aucune erreur pour deux maps correctement reliées", () => {
    const { map: a } = generateMap({
      type: "town",
      name: "A",
      id: "world_a",
      seed: 1,
      connections: [{ direction: "south", mapId: "world_b", offset: 0 }],
    });
    const { map: b } = generateMap({
      type: "route",
      name: "B",
      id: "world_b",
      seed: 2,
      connections: [
        { direction: "north", mapId: "world_a", offset: 0 },
        { direction: "south", mapId: "world_c_missing", offset: 0 },
      ],
    });
    const result = checkConnectionsConsistency([a, b]);
    // world_c_missing n'existe pas dans le lot fourni : doit être signalé.
    expect(result.errors.some((e) => e.includes("world_c_missing"))).toBe(true);
    // En revanche a <-> b doit être jugée cohérente (pas d'erreur les concernant entre elles).
    expect(result.errors.some((e) => e.startsWith("world_a → world_b") || e.includes("world_a ↔ world_b"))).toBe(false);
  });

  it("signale une connexion non réciproque", () => {
    const { map: a } = generateMap({
      type: "town",
      name: "A",
      id: "world_x",
      seed: 1,
      connections: [{ direction: "south", mapId: "world_y", offset: 0 }],
    });
    const { map: b } = generateMap({ type: "route", name: "B", id: "world_y", seed: 2 }); // pas de connexion retour
    const result = checkConnectionsConsistency([a, b]);
    expect(result.errors.some((e) => e.includes("non réciproque"))).toBe(true);
  });

  it("opposite() est involutif", () => {
    expect(opposite(opposite("north"))).toBe("north");
    expect(opposite("north")).toBe("south");
    expect(opposite("east")).toBe("west");
  });
});
