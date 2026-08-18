# Session State — Project_C

**Updated:** 2026-08-18

## Project
- **Name:** Project_C
- **Division:** Game Dev (Unreal)
- **Stage:** A — Greenfield (proyecto Unreal vacío, sin diseño ni implementación)
- **User role:** Solo dev (diseño + código + arte)

## Stack detected
- **Engine:** Unreal Engine 5.8 (`Project_C.uproject`)
- **Project type:** Blueprint-only (sin `Source/`, sin módulo C++)
- **Plugins enabled:** ModelingToolsEditorMode (Editor)
- **Content:** vacío (0 assets)
- **Config:** DefaultEditor/Engine/Game/Input.ini (defaults de UE)
- **VCS:** ninguno (no es repo git)
- **Arcane:** v2.1.0 — perfiles core + unreal-dev + unity-design + statusline (80 skills, 13 rules, agents: quality, game)

## Existing artifacts
- Ninguno. Sin GDD, sin art bible, sin specs, sin código.

## Session intent
Doble vía en paralelo:
1. **Ideación desde 0** — el usuario tiene una idea semilla pero quiere idearla estructuradamente.
2. **Setup técnico profesional** — dejar el proyecto Unreal con base sólida antes de producir.

## Recommended flow
1. `/brainstorm` — concepto: género, core loop, pilares, fantasía de jugador, referencias → game-concept.md
2. `/ue-project-context` — documento de contexto técnico: módulos, plataformas target, convenciones, estándares
3. `/map-systems` — descomponer el concepto en sistemas, dependencias y orden de diseño

## Technical setup backlog
- [ ] `git init` + `.gitignore` de Unreal + Git LFS para assets binarios
- [ ] Módulo C++ (`Source/`) vía `/ue-module-build-system` — decidir C++ vs Blueprint-only
- [ ] Estructura de carpetas de `Content/` + naming conventions (`/ue-naming-conventions`)
- [ ] MCP del editor Unreal (`/install-mcp`)
- [ ] Test/automation harness (`/test-setup`, `/ue-testing-debugging`)
- [ ] Plataformas target + performance budgets

## Next action
Base técnica completada (git+LFS, ignore rules, README, docs/unreal/project-context.md).
Pendiente: mudanza a C:\Users\sebas\UnrealEngine\Project_C, luego `/brainstorm`.
