# Unreal Project Context: Project_C

> Lives at `docs/unreal/project-context.md`. Every `ue-*` skill reads this before acting, so
> keeping it accurate is what stops the advice from being generic. Generate or refresh it with
> `/ue-project-context`.

## Document Status
- **Version**: 1.0
- **Last Updated**: 2026-08-18
- **Maintainer**: Sebastián Luser (solo dev)
- **Drafted from**: codebase scan (`.uproject`, `Config/*.ini`, launcher manifest)

> **Read this first**: the game concept does not exist yet. Everything below describes the
> *technical* project as it stands. Anything that depends on what the game actually is
> carries a **TBD** and will be settled after `/brainstorm`. Do not infer intent from the
> template defaults recorded here — they were inherited, not chosen.

## 1. Engine & Project Overview

| Field | Value |
|-------|-------|
| **Project name** | Project_C |
| **One-line description** | **TBD** — concept not defined yet |
| **Engine version** | 5.8.1 (`5.8.1-56057345+++UE5+Release-5.8-Windows`) |
| **Build kind** | Launcher build — `C:\Program Files\Epic Games\UE_5.8` |
| **Project type** | Game |
| **Genre or domain** | **TBD** — post-`/brainstorm` |
| **Target platforms** | **TBD**. Currently configured for Windows / DX12 / SM6 by template default |
| **Created from** | Blank Blueprint template (`TP_BlankBP`, per `ActiveGameNameRedirects`) |
| **Project location** | `C:\Users\sebas\UnrealEngine\Project_C` — no spaces, mirrors the Unity convention at `C:\Users\sebas\Unity\` |

Also installed alongside the engine: QuixelBridge 5.8, Fab plugin 5.8.

## 2. Module Structure

**No C++ modules.** The project is Blueprint-only and has no `Source/` directory.

| Module | Type | Primary? | Public deps | Private deps |
|--------|------|----------|-------------|--------------|
| — | — | — | — | — |

- **Decision pending**: whether to add a C++ game module at all. Depends on the concept —
  revisit with `/ue-module-build-system` after `/brainstorm`. Adding one later is routine;
  the cost is a rebuild, not a migration.

## 3. Plugin Dependencies

| Plugin | Source | Critical to gameplay? | Notes |
|--------|--------|-----------------------|-------|
| ModelingToolsEditorMode | engine | no | Editor-only, enabled by the template |

**Not yet enabled**, and deliberately so — each depends on the concept: GameplayAbilities (GAS),
EnhancedInput, CommonUI, Niagara, PCG, MetaSounds, OnlineSubsystem, MassEntity, StateTree.

> `Config/DefaultGame.ini` carries a `[/Script/CommonUI.CommonUISettings]` block, but the
> **CommonUI plugin itself is not enabled** in `.uproject`. It is inert template residue, not
> a signal that the project uses CommonUI.

**Available for tooling**: the engine ships `ModelContextProtocol` and `ToolsetRegistry`
(both Experimental in 5.8). Not enabled yet — see `/install-mcp`.

## 4. Coding & Asset Conventions

### Asset naming

- **Pattern**: `Prefix_BaseAssetName_Variant_Suffix`
- **Static Mesh prefix**: **`SM_`** — Epic's own tooling (including its MCP server) assumes it,
  which outweighs the Gamemakin guide's `S_`
- **Structs / enums authored in Blueprint**: **`F_` / `E_`** — keeps them visually distinct from
  C++ types at a glance
- **Texture packing order**: **TBD** — set it when the first packed texture exists
- **Prefix collisions resolved**: none encountered yet
- **Top-level content folder**: `Content/Project_C/` — nothing loose in `Content/`
- **`Developers/` in use?**: yes — `Content/Developers/Sebastian/` as a sandbox
- **Charset**: `[A-Za-z0-9_]` only, in every identifier, folder and path. No spaces, no Unicode

**Target content layout.** Folders are created *when the first asset lands in them*, from the
editor — never pre-created empty. The convention forbids empty folders: they clutter the Content
Browser and usually mean an unfixed redirector.

```
Content/
  Project_C/
    Core/              base GameMode, GameState, PlayerController, Character
    Characters/        one folder per character, with its own Meshes/Materials/Animations
    Placeables/        designer-tunable actors
    Maps/              every level, without exception
    MaterialLibrary/   master materials, functions, shared utility textures
      Debug/           strippable before shipping
    Input/             IA_, IMC_
    Data/              DA_, DT_
    UI/                WBP_
  Developers/
    Sebastian/         sandbox; nothing here may be depended on
```

Full prefix tables live in `/ue-naming-conventions`. Recorded here is only what this project
decided or does differently.

### Code conventions

- **Naming prefixes**: Epic standard `F`/`U`/`A`/`E`/`I`
- **Header guards**: `#pragma once`
- **Log categories**: none yet — no C++ module
- **C++ vs Blueprint boundary**: **TBD**. Currently 100% Blueprint by default, not by decision
- **Formatting / linting**: none configured
- **Line endings**: `.gitattributes` sets `* text=auto`. Note the machine's global
  `core.autocrlf` is `input` (a Linux/Mac value); `.gitattributes` governs over it

## 5. Subsystems in Use

Nothing is in use — there is no gameplay code or content yet.

| Subsystem | In use? | How it is used here |
|-----------|---------|---------------------|
| Gameplay Ability System | no | **TBD** — plugin not enabled |
| Enhanced Input | no | **TBD** — plugin not enabled |
| CommonUI | no | **TBD** — settings block present but plugin disabled |
| Replication / multiplayer | no | **TBD** — single biggest architectural fork; decide early |
| World Partition / level streaming | no | Editor settings exist by default; no maps yet |
| Niagara | no | **TBD** |
| Mass Entity / StateTree | no | **TBD** |
| Save system | no | **TBD** |

**Gameplay framework classes**: none. No custom GameMode, GameState, PlayerController or Pawn.

## 6. Build Configuration

- **Target types built**: none yet — never packaged
- **Configurations used**: Development Editor only
- **Game Default Map**: `/Engine/Maps/Templates/OpenWorld` — ⚠️ **this points at an *engine*
  map, not a project map.** It is the template default and the classic cause of a black or
  broken packaged build. Fix it in Project Settings once the first project map exists
- **Packaging**: not configured
- **CI**: none
- **Platform SDKs installed**: Windows only
- **Performance targets**: **TBD** — cannot be set before the concept and target platform are

### Rendering configuration (inherited from the template, not chosen)

| Setting | Value |
|---------|-------|
| Substrate | **enabled** (`r.Substrate=True`, GBuffer format 0) |
| Global illumination | Lumen (`r.DynamicGlobalIlluminationMethod=1`) |
| Reflections | Lumen (`r.ReflectionMethod=1`) |
| Shadows | Virtual Shadow Maps (`r.Shadow.Virtual.Enable=1`) |
| Ray tracing | enabled, including ray tracing proxies |
| Static lighting | **disabled** (`r.AllowStaticLighting=False`) |
| Mesh distance fields | generated |
| RHI | DX12, SM6 (SM5 for D3D11); Vulkan SM6 on Linux, Metal SM6 on Mac |
| Hardware target | Desktop, Maximum quality |
| Audio | 48 kHz, 1024-frame callback, 4 source workers |

> **Substrate deserves attention.** It is enabled and it is a deep rendering commitment —
> backing out later means reauthoring materials. It arrived with the template rather than from a
> decision. Confirm or reverse it early, while `Content/` is still empty and the cost is zero.
>
> Ray tracing plus Lumen plus VSM at "Maximum" is a heavy baseline. Fine for a high-end desktop
> target; revisit if the concept points anywhere else.

## 7. Team Context

- **Team size and roles**: 1 — solo dev covering design, code and art
- **Source control**: **Git + Git LFS**. Remote `DragonFruit-Pixels/Project_C` (**public**)
  - LFS covers `.uasset`/`.umap` plus art, audio and font binaries — see `.gitattributes`
  - ⚠️ **Public repo**: anything committed is permanent and world-readable. The
    AndroidFileServer `SecurityToken` that the template auto-generated in `DefaultEngine.ini`
    was removed before the first commit. Keep service keys out of `Config/*.ini`
  - ⚠️ **LFS quota**: GitHub's free tier gives 1 GB of LFS storage and 1 GB/month of bandwidth,
    organizations included. Revisit the hosting choice at ~700 MB
- **Review expectations**: n/a (solo)
- **Definition of done**: **TBD**
- **Known pain points**: none recorded yet

## Open Questions

Every one of these is blocked on the concept. They are listed so the `ue-*` skills know to ask
rather than assume.

1. **What is the game?** Genre, core loop, pillars — everything else follows. → `/brainstorm`
2. **Single-player or multiplayer?** The one decision that is genuinely expensive to reverse.
   Replication shapes actor design from the first class onward
3. **C++ module, or Blueprint-only?** Depends on system complexity and performance needs
4. **Target platforms?** Determines whether the inherited Maximum/ray-tracing baseline survives
5. **Substrate: keep or disable?** Free to decide now, costly later
6. **GAS?** Powerful and heavy. Worth it for deep ability systems, overkill otherwise
7. **Performance budget?** Target FPS and frame budget, once the platform floor is known

> **Rule**: an unanswered item here is an item the `ue-*` skills will have to ask about. Closing
> them is what makes the rest of the workflow fast.
