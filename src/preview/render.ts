import { createCanvas } from "@napi-rs/canvas";
import fs from "node:fs";
import path from "node:path";
import type { GameMap } from "../schema/types.js";
import { loadNpcArchetypes, loadTileset, tilesetLegend } from "../data/loader.js";
import { decorationTileId } from "../generator/decorations.js";

const PREVIEW_TILE_PX = 12;
const HEADER_PX = 28;

const OBJECT_COLOR: Record<string, string> = {
  item_visible: "#f2c14e",
  item_hidden: "#c97fd9",
  berry_tree: "#e07fae",
  sign: "#8a8a8a",
  machine: "#4a90d9",
  npc_object: "#ffffff",
  cuttable_tree: "#2e5b31",
  pushable_rock: "#7c7568",
  pc: "#4a90d9",
  counter: "#8a5a3c",
};

/**
 * Rend une preview PNG de la map (section 14) : structure, lisibilité,
 * circulation, densité. Placeholder graphique en aplats de couleur —
 * cohérent avec le tileset sémantique (voir assets/MISSING_ASSETS.md).
 */
export async function renderMapPreview(map: GameMap, outPath: string): Promise<void> {
  const tileset = loadTileset(map.metadata.tileset);
  const legend = tilesetLegend(tileset);
  const archetypes = loadNpcArchetypes();

  const width = map.dimensions.width * PREVIEW_TILE_PX;
  const height = map.dimensions.height * PREVIEW_TILE_PX + HEADER_PX;
  const canvas = createCanvas(width, height);
  const ctx = canvas.getContext("2d");

  ctx.fillStyle = "#1b1b1b";
  ctx.fillRect(0, 0, width, height);
  ctx.fillStyle = "#ffffff";
  ctx.font = "14px sans-serif";
  ctx.fillText(`${map.metadata.name} — ${map.metadata.kind} (${map.dimensions.width}×${map.dimensions.height})`, 6, 19);

  for (let y = 0; y < map.dimensions.height; y++) {
    for (let x = 0; x < map.dimensions.width; x++) {
      const tileId = map.terrain[y][x];
      const tile = legend.get(tileId);
      ctx.fillStyle = tile?.color ?? "#ff00ff";
      ctx.fillRect(x * PREVIEW_TILE_PX, HEADER_PX + y * PREVIEW_TILE_PX, PREVIEW_TILE_PX, PREVIEW_TILE_PX);
    }
  }

  // Décorations : petit point coloré centré sur la tuile.
  for (const d of map.decorations) {
    const tile = legend.get(decorationTileId(d.type));
    ctx.fillStyle = tile?.color ?? "#cccccc";
    const cx = d.x * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    const cy = HEADER_PX + d.y * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    ctx.beginPath();
    ctx.arc(cx, cy, PREVIEW_TILE_PX * 0.2, 0, Math.PI * 2);
    ctx.fill();
  }

  // Bâtiments : contour + libellé.
  ctx.strokeStyle = "#000000";
  ctx.lineWidth = 1.5;
  for (const b of map.buildings) {
    ctx.strokeRect(b.x * PREVIEW_TILE_PX, HEADER_PX + b.y * PREVIEW_TILE_PX, b.width * PREVIEW_TILE_PX, b.height * PREVIEW_TILE_PX);
    ctx.fillStyle = "#ffffff";
    ctx.font = "9px sans-serif";
    ctx.fillText(b.type, b.x * PREVIEW_TILE_PX + 2, HEADER_PX + b.y * PREVIEW_TILE_PX + 10);
  }

  // Objets interactifs : losange coloré.
  for (const o of map.objects) {
    const color = OBJECT_COLOR[o.type] ?? "#ffffff";
    const cx = o.x * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    const cy = HEADER_PX + o.y * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.moveTo(cx, cy - 5);
    ctx.lineTo(cx + 5, cy);
    ctx.lineTo(cx, cy + 5);
    ctx.lineTo(cx - 5, cy);
    ctx.closePath();
    ctx.fill();
    if (o.hidden) {
      ctx.strokeStyle = "#ffffff";
      ctx.lineWidth = 1;
      ctx.stroke();
    }
  }

  // PNJ : cercle coloré par archétype + lettre de rôle.
  for (const n of map.npcs) {
    const color = archetypes[n.archetype]?.spriteColor ?? "#ffffff";
    const cx = n.x * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    const cy = HEADER_PX + n.y * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    ctx.fillStyle = color;
    ctx.beginPath();
    ctx.arc(cx, cy, PREVIEW_TILE_PX * 0.4, 0, Math.PI * 2);
    ctx.fill();
    ctx.strokeStyle = "#000000";
    ctx.lineWidth = 1;
    ctx.stroke();
  }

  // Warps : marqueur triangulaire jaune.
  for (const w of map.warps) {
    const cx = w.x * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    const cy = HEADER_PX + w.y * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    ctx.fillStyle = "#f2e94e";
    ctx.beginPath();
    ctx.moveTo(cx, cy - 6);
    ctx.lineTo(cx + 6, cy + 6);
    ctx.lineTo(cx - 6, cy + 6);
    ctx.closePath();
    ctx.fill();
  }

  // Landmarks : étoile + libellé.
  for (const l of map.landmarks) {
    const cx = l.x * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    const cy = HEADER_PX + l.y * PREVIEW_TILE_PX + PREVIEW_TILE_PX / 2;
    drawStar(ctx, cx, cy, 7, "#ffffff");
    ctx.fillStyle = "#ffffff";
    ctx.font = "9px sans-serif";
    ctx.fillText(l.label, cx + 8, cy + 3);
  }

  // Connexions : flèche vers le bord.
  ctx.fillStyle = "#7fffd4";
  ctx.font = "10px sans-serif";
  for (const c of map.connections) {
    ctx.fillText(`→ ${c.mapId}`, 4, height - 6);
  }

  fs.mkdirSync(path.dirname(outPath), { recursive: true });
  const buffer = await canvas.encode("png");
  fs.writeFileSync(outPath, buffer);
}

// eslint-disable-next-line @typescript-eslint/no-explicit-any
function drawStar(ctx: any, cx: number, cy: number, r: number, color: string) {
  ctx.fillStyle = color;
  ctx.beginPath();
  for (let i = 0; i < 10; i++) {
    const radius = i % 2 === 0 ? r : r / 2.5;
    const angle = (Math.PI / 5) * i - Math.PI / 2;
    const x = cx + radius * Math.cos(angle);
    const y = cy + radius * Math.sin(angle);
    if (i === 0) ctx.moveTo(x, y);
    else ctx.lineTo(x, y);
  }
  ctx.closePath();
  ctx.fill();
}
