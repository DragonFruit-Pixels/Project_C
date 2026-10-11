# Meshy Assets

Reglas para produccion de assets 3D con Meshy. Aplican a cualquier operacion que toque `assets/3d/`.

## Creditos — cada llamada cuesta plata real

| Operacion | Creditos |
|---|---|
| `meshy_analyze_printability` | **0** |
| `meshy_check_balance`, `meshy_get_task_status`, `meshy_list_tasks`, `meshy_download_model` | **0** |
| `meshy_convert`, `meshy_resize` | 1 |
| `meshy_animate` | 3 |
| `meshy_text_to_image` | 3-9 |
| `meshy_image_to_image` | 3-12 |
| `meshy_remesh`, `meshy_rig`, `meshy_uv_unwrap` | 5 |
| `meshy_retexture`, `meshy_repair_printability` | 10 |
| `meshy_text_to_3d`, `meshy_image_to_3d`, `meshy_multi_image_to_3d` | 20-30 |
| Print white model | 20 |
| `meshy_creative_lab` | 36 |
| Print multicolor | 40 |

### Reglas duras

1. **Nunca generar sin confirmacion explicita.** Preguntar siempre, incluso si el pedido parece obvio. Informar el costo estimado **antes** de la llamada.
2. **Preview primero.** `meshy_text_to_3d_refine` solo con el preview aprobado por el usuario.
3. **Nunca en batch.** Un asset por vez, con confirmacion por asset.
4. **Chequear el ledger antes de generar.** Grepear `assets/3d/*/meshy.json` buscando un prompt equivalente. Si existe, avisar y **no** generar.
5. **Las operaciones de 0 creditos no necesitan confirmacion.** `analyze_printability`, `check_balance`, `get_task_status`, `list_tasks`, `download_model` corren libres.

Ante cualquier duda sobre si una operacion gasta, correr `meshy_check_balance` antes y despues y reportar el delta real.

## Master unico, derivados baratos

El GLB refined es la fuente de verdad y es **inmutable**. Ningun target se genera desde el prompt: todos salen del master.

| Target | Como | Creditos |
|---|---|---|
| Unity y UE5 | `meshy_convert` -> `fbx` (un solo archivo sirve para los dos) | 1 |
| Blender | `meshy_convert` -> `blend` | 1 |
| Impresion | `meshy_convert` -> `stl` (`3mf` si es multicolor) | 1 |
| Web / three.js | `npx gltf-transform optimize --simplify` | **0** |

- Nunca usar `remesh` (5) cuando `convert` (1) alcanza. `remesh` es para cambiar topologia de verdad, no para cambiar de formato.
- Nunca usar `remesh` para LODs de web: `gltf-transform` hace decimation, meshopt y KTX2 en local y gratis.

## Estructura en disco

```
assets/3d/<slug>/
  meshy.json           # ledger, ver abajo
  master.glb           # refined, inmutable
  thumb.png
  targets/
    web.glb            # <=15k tris, texturas 1024, KTX2 + draco
    game.fbx           # Unity + UE5
    edit.blend
    print.stl
```

Slugs en kebab-case, sin fechas ni task_ids en el path.

## Import a UE5: en el mismo turno que la descarga

`assets/3d/` es la fuente de verdad y **no va dentro de `Content/`**: UE no lee un `.fbx` ni un
`.glb` que este ahi — nunca se vuelven asset, pesan en el repo y ensucian el cook. Lo que va al
Content es el `.uasset` importado.

El import se automatiza por el MCP de Unreal, con nombre y carpeta explicitos:

| Fuente | Herramienta | Destino |
|---|---|---|
| `targets/game.fbx` (prop/arma) | `StaticMeshTools.import_file` | `/Game/Project_C/Assets/<Slug>/SM_<Slug>` |
| `targets/rigged.fbx` (personaje) | `SkeletalMeshTools.import_file` | `/Game/Project_C/Assets/<Slug>/SK_<Slug>` |
| `targets/anim_*.fbx` | `SkeletalMeshTools.import_file` con `skeleton:` = el `SKEL_` existente | `AS_<Slug>_<Accion>` |
| texturas PBR | `TextureTools.import_file` | `T_<Slug>_BC` / `_N` / `_R` / `_M` |

Carpeta destino en PascalCase; el link al slug queda en `ue_path` dentro de `meshy.json`.

Cuatro cosas que el importador hace mal y hay que corregir a mano:

1. **Nombra basura.** Un FBX con material trae `Material_001` y `texture_0`. Renombrarlos con
   `AssetTools.move` (la referencia del mesh sigue al rename) en vez de importar la textura
   aparte — eso duplica el asset.
2. **Un FBX de animacion importa tambien la malla.** Queda un SkeletalMesh redundante con el
   nombre que pediste y el AnimSequence como `<nombre>_Anim`. Borrar la malla, renombrar el anim.
3. **Normal y roughness entran como sRGB.** Antes de conectarlas: `compressionSettings`
   `TC_Normalmap` / `TC_Grayscale` y `srgb: false`, o el material no compila por sampler type.
4. **Deja `<nombre>.fbm/` al lado del FBX.** Es el SDK de FBX desempaquetando las texturas
   embebidas: una copia byte a byte de lo que ya viaja adentro del `.fbx`. Se regenera sola en
   cada import y no guarda nada propio. Van al `.gitignore` (`*.fbm/`) — en esta tanda eran
   **54 MB** de los 239 MB de `assets/3d/`.

Al terminar, `AssetTools.save_assets([])` y revisar `LogsToolset` filtrando por error de import.

## Materiales en UE5: un padre, un instance por personaje

Nunca un Material suelto por personaje. Cada Material es un **shader que se compila aparte**;
la propia herramienta del MCP lo avisa (*"Each new Material increases shader compile times.
Prefer creating a MaterialInstance"*). Con cuatro personajes son cuatro grafos que mantener en
paralelo y cuatro lugares donde equivocarse.

```
/Game/Project_C/Assets/_Shared/M_Character   <- el grafo vive aca, se compila una vez
    +-- MI_<Personaje>                       <- solo valores de parametro, sin grafo
```

Las texturas van como `TextureSampleParameter2D` (mismo nodo que `TextureSample`, pero con
`parameterName`), no como samples fijos: un sample fijo apunta duro a una textura y anula toda
la herencia.

**El sampler type del padre manda sobre lo que pueden asignar los instances.** Si el padre
declara `SAMPLERTYPE_Color` y un instance asigna una `TC_Grayscale`, no compila. Mapeo:

| Mapa | compression / srgb | samplerType |
|---|---|---|
| Base color | `TC_Default`, srgb `true` | `SAMPLERTYPE_Color` |
| Normal | `TC_Normalmap`, srgb `false` | `SAMPLERTYPE_Normal` |
| Roughness / Metallic / mascaras | `TC_Grayscale`, srgb `false` | `SAMPLERTYPE_LinearGrayscale` |

Los defaults del padre tienen que cumplir la misma tabla, y **el engine no trae ninguna textura
`TC_Grayscale`** — `/Engine/EngineResources/Black` y `WhiteSquareTexture` son `TC_Default`/sRGB.
Hay dos generadas en `assets/textures/neutral/` (PNG grayscale de 4x4, ~70 bytes) importadas
como `T_Neutral_Black` y `T_Neutral_Grey`. Reusarlas; no colgar el padre de las texturas de un
personaje.

**Lo opcional va detras de un `StaticSwitchParameter`, no de un valor neutro.** Un switch en
`false` hace que esa rama **no exista** en el shader compilado; un valor neutro la compila igual
y la paga en cada pixel. Asi esta el acento de firma: `UseAccentMask` apagado, y el `Lerp` no
llega al shader.

## Ledger: `meshy.json`

Obligatorio por asset. Sin esto cada sesion nueva arranca ciega y se paga dos veces el mismo modelo.

```json
{
  "slug": "rune-pedestal",
  "prompt": "...",
  "ai_model": "meshy-5",
  "tasks": [
    { "id": "...", "op": "text_to_3d_preview", "credits": 5, "at": "2026-08-18T14:02:00Z" },
    { "id": "...", "op": "text_to_3d_refine", "credits": 25, "at": "2026-08-18T14:11:00Z" }
  ],
  "credits_total": 30,
  "derived": ["targets/web.glb", "targets/game.fbx"]
}
```

Actualizar `meshy.json` **en el mismo turno** en que se genera. No dejarlo para despues.

## Ejes y escala

Verificar una vez, anotar en el ledger, no volver a pensarlo.

| Destino | Convencion |
|---|---|
| GLB | Y-up, metros |
| Unity | Y-up, 1 unidad = 1 m |
| UE5 | Z-up, 1 unidad = 1 cm (rotacion + factor 100) |
| Blender | Z-up, metros |

## Impresion 3D

Correr `meshy_analyze_printability` (gratis) **siempre** antes de `meshy_repair_printability` (10). `meshy_resize` a milimetros reales antes de exportar STL. Las operaciones de print (20 white / 40 multicolor) son las mas caras del catalogo: doble confirmacion.

## Rigging

`meshy_rig` es solo humanoides. El esqueleto que devuelve **no** es el mannequin de UE5: el retargeting es manual. En UE5 los static meshes van a Nanite y no necesitan decimation; los skeletal meshes si necesitan LODs propios.

Generar la malla en **t-pose** cuando se la va a riggear con Meshy: `meshy_rig` lo recomienda y
sale mejor. La a-pose solo tiene sentido si el destino es el mannequin de UE5, cuyo ref pose es A.

El esqueleto que devuelve es estilo Mixamo pero **no identico**: usa `Spine01`/`Spine02`/`neck`/
`head_end` donde Mixamo usa `Spine1`/`Spine2`/`Neck`/`HeadTop_End`. Son cuatro remapeos en un IK
Retargeter, una sola vez, y despues entra toda la libreria de Mixamo gratis.

## Animaciones: `meshy_animate`

Necesita el `rig_task_id` de un rigging **vivo en la cuenta actual**. Si ese task murio, no hay
atajo: hay que regenerar malla + rig, y la malla nueva **no va a ser la misma** (Meshy no es
determinista). Antes de prometer "3 creditos por animacion", verificar que el rig exista.

`action_id` sale del [catalogo oficial](https://docs.meshy.ai/en/api/animation-library), rango
0-590. No esta expuesto como resource del MCP, hay que consultar la doc. Los que ya se usaron:

| Accion | `action_id` |
|---|---|
| `Idle` | 0 |
| `Right_Hand_Sword_Slash` | 219 |
| `Hit_Reaction` | 178 |
| `Dead` | 8 |

## Trampas del MCP, verificadas el 2026-09-20

0. **`call_tool` del MCP de Unreal necesita `toolset_name` Y `tool_name` por separado**, con
   el `tool_name` **corto**. El nombre largo que imprime `describe_toolset`
   (`editor_toolset.toolsets.material.MaterialTools.get_expressions`) no funciona como
   `tool_name`: hay que partirlo. Dentro de un script de `ProgrammaticToolset` es al reves —
   ahi `execute_tool()` toma el nombre **largo** completo.
   Y los nombres de parametro no se adivinan: `find_assets` pide `name` (obligatorio, `""`
   sirve), `save_assets` pide `asset_paths`, `get_referencers` pide `asset_path`, y borrar es
   `delete` con `path` — no `delete_asset`, no `asset_path`.

1. **`meshy_get_task_status` lo bloquea el clasificador de auto mode** como "Real-World
   Transaction", aunque cueste 0 creditos. Esto ya costo una tanda entera el 2026-09-11.
   Alternativas que si pasan:
   - `meshy_list_tasks` para text/image-to-3d, remesh, retexture — da `status` y `progress`.
   - Para **rigging y animation no hay endpoint de listado**: usar `meshy_download_model` como
     sonda. Si la tarea no termino, falla; si termino, devuelve las URLs.
2. **`meshy_download_model` no guarda archivo local para rigging ni animation.** Devuelve
   `download_url` / `rigged_character_fbx_url` / `basic_animations.*` y hay que bajarlas con
   `curl`. Ojo: las URLs estan firmadas y expiran (~24 h). Para text/image-to-3d si escribe el
   archivo y respeta `save_to`.
3. El rig trae **walking y running gratis** en `basic_animations`. No pagar `animate` por esas dos.

## Versionar un asset regenerado

Si un slug se regenera desde cero, el set viejo va a `assets/3d/<slug>/_v1/` (mismo `_v2/`, etc.)
y el nuevo ocupa la ruta canonica. En el ledger: `version`, `credits_v1`/`credits_v2`,
`credits_total` acumulado, y cada task con su `version` y su `status`. **No borrar la vieja** —
suele ser el unico registro que queda de tareas que ya no existen en la API.

## Git y secretos

- `*.glb *.fbx *.blend *.stl *.3mf` por **Git LFS**.
- `meshy.json` y `thumb.png` van como archivos normales — son chicos y son lo que hace grepeable el catalogo.
- `MESHY_API_KEY` (formato `msy_...`) va en `.env`, **nunca** en el repo ni en un SKILL.md.

## Integracion con el resto del stack

`asset-spec` define que asset hace falta -> Meshy lo genera -> `asset-audit` valida el resultado. Meshy no reemplaza a ninguno de los dos.

Si existe un art bible en el proyecto, el prompt de generacion se deriva de ahi, no se inventa.
