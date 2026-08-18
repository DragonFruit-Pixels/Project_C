# Project_C

An Unreal Engine 5.8 game project in early development by [DragonFruit Pixels](https://github.com/DragonFruit-Pixels).

The concept is still being defined — there is no gameplay yet. What exists today is the
technical foundation: source control, asset conventions, and project documentation.

## Requirements

| | |
|---|---|
| **Engine** | Unreal Engine **5.8** (launcher build) |
| **Platform** | Windows, DirectX 12 / Shader Model 6 |
| **Source control** | Git with [Git LFS](https://git-lfs.com/) — **required** |

## Getting started

Install Git LFS before cloning, or the binary assets arrive as text pointer files
instead of real content:

```bash
git lfs install
git clone https://github.com/DragonFruit-Pixels/Project_C.git
```

Already cloned without LFS? Recover with:

```bash
git lfs install && git lfs pull
```

Then open `Project_C.uproject`. The first launch compiles shaders and builds the
derived data cache — expect several minutes before the editor is usable.

## Rendering

The project runs Unreal's modern rendering stack: **Substrate** materials, **Lumen**
global illumination and reflections, **Virtual Shadow Maps**, and hardware ray tracing.
Static lighting is disabled. This targets desktop hardware at maximum quality.

## Conventions

Asset naming and content layout follow [`docs/unreal/project-context.md`](docs/unreal/project-context.md).
Read it before adding assets — it is also the document every `ue-*` tooling skill
consults, so keeping it accurate keeps the tooling useful.

## Repository layout

```
Config/     Project .ini configuration
Content/    Game assets (Git LFS)
docs/       Project documentation
production/ Session and production state
.claude/    Claude Code Arcane workflow — skills, agents, rules
```

## Status

Pre-production. No release, no builds, no packaged target yet.
