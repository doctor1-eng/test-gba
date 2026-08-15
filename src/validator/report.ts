import type { ValidationReport } from "./index.js";

/** Formate le rapport exactement dans le style demandé (section 13). */
export function formatReport(report: ValidationReport): string {
  const lines: string[] = [];
  lines.push(`MAP VALIDATION — ${report.mapName} (${report.mapId})`);
  for (const [key, ok] of Object.entries(report.checklist)) {
    lines.push(`${ok ? "✓" : "✗"} ${key}`);
  }
  if (report.warnings.length > 0) {
    lines.push("WARNINGS");
    for (const w of report.warnings) lines.push(`- ${w}`);
  }
  if (report.errors.length > 0) {
    lines.push("ERRORS");
    for (const e of report.errors) lines.push(`- ${e}`);
  }
  lines.push(report.passed ? "RÉSULTAT: OK" : "RÉSULTAT: ÉCHEC");
  return lines.join("\n");
}
