# Layout del nivel (Job 1)

Propuesta propia. Del Episodio 1 original tomamos la escala (11 espacios, 7 salas, mezcla de
salones largos y salas chicas) y nada más: la disposición, las conexiones y la ambientación son
nuestras. Ambientación: la variante biopunk (`docs/rulebook/glossary-biopunk.md`), una planta
química que baja hasta el sumidero donde crece el Growth.

La lectura es vertical: se entra por arriba y se baja. Arriba está la **Planta**, donde arranca
la partida; abajo el **Sumidero**, donde están el objetivo y el jefe (GDD: superficie y subterráneo).

## Plano

`U` = la distancia actual entre casillas (~866 u). Cada celda del plano mide 1 U.

```
        c0        c1        c2        c3        c4        c5        c6
r0  [Lockers ][ Decon  ][======= Loading Dock (START) ======][Control ][ Office ]   PLANTA
r1                 :                                                   [  Lift  ]
r2  [ Batch Chamber    ][Cook lab][Cold st.][Vat room]                 [  Lift  ]   SUMIDERO
r3  [   (OBJETIVO)     ]          [========== Pipe Gallery ==========][  Lift  ]
                   :
             chute: Decon -> Cold store
```

La fila r1 es roca: separa las dos secciones. La única conexión física entre ellas es el Lift;
el chute es un pasaje tipo túnel (arista especial del GDD).

## Espacios

| # | Sala | Espacio | Tamaño | Conecta con | Rol | Props (Meshy) |
|---|---|---|---|---|---|---|
| 1 | Dock | Loading Dock | 3×1 | Decon, Control | **Inicio**. Entrada desde afuera por el borde de arriba | pallets, barriles químicos, montacargas |
| 2 | Staff | Lockers | 1×1 | Decon | Callejón sin salida: búsqueda de Salvage | lockers, banco, trajes hazmat |
| 3 | Staff | Decon | 1×1 | Lockers, Dock, Cold store (chute) | Atajo al sumidero | duchas, rejilla, escotilla del chute |
| 4 | Oficinas | Control | 1×1 | Dock, Office | Paso | consolas, monitores, silla |
| 5 | Oficinas | Office | 1×1 | Control, Lift | Paso al ascensor | escritorio, archivero, caja fuerte |
| 6 | Lift | Freight Lift | 1×3 | Office, Pipe Gallery | Bajada principal. **No se bloquea** | plataforma de jaula, rieles, cadenas |
| 7 | Gallery | Pipe Gallery | 3×1 | Lift, Cold store, Vat room | **Drain verde** | caños gruesos, válvulas, pasarela de rejilla |
| 8 | Lab | Vat room | 1×1 | Gallery, Cold store | **Drain amarillo** | cubas de fermentación con masa orgánica |
| 9 | Lab | Cold store | 1×1 | Gallery, Cook lab, Vat room, Decon (chute) | Nudo del sumidero | racks, bolsas colgadas, salida del chute |
| 10 | Lab | Cook lab | 1×1 | Cold store, Batch Chamber | Única entrada al objetivo | mesadas, vidriería, mecheros |
| 11 | Batch | Batch Chamber | 2×2 | Cook lab | **Objetivo, arena del jefe, Drain rojo** | cuba central, crecimientos de quitina |

Puertas: 11 normales + 1 chute = 12 aristas.

## Distancias desde el inicio

| | Decon | Control | Lockers | Office | Cold | Lift | Gallery | Cook | Vat | Batch |
|---|---|---|---|---|---|---|---|---|---|---|
| Con chute | 1 | 1 | 2 | 2 | 2 | 3 | 3 | 3 | 3 | **4** |
| Chute bloqueado | 1 | 1 | 2 | 2 | 5 | 3 | 4 | 6 | 5 | **7** |

El chute es la decisión del mapa: llegás al objetivo en 4 en vez de 7, pero es bloqueable. Si la
misión lo cierra, toca dar la vuelta por el Lift.

## Chequeo contra los invariantes F8 del GDD

1. Simetría y 2. sin bucles: todas las aristas son de ida y vuelta.
3. Grado entre 1 y 6: el máximo es Cold store con 4.
4. Salas de 1 a 3 espacios: Dock 1, Staff 2, Oficinas 2, Lift 1, Gallery 1, Lab 3, Batch 1.
5. 3 Drains en espacios distintos: Gallery, Vat room y Batch Chamber, todos a distancia ≥ 3 del inicio
   (el mínimo de 3 es la propuesta del GDD, no está en el manual).
6. Conectividad: todo espacio es alcanzable.
7. Objetivo siempre alcanzable: **Cook lab ↔ Batch Chamber y el Lift no pueden ser bloqueables.**
   Con esas dos aristas libres, cerrar el chute nunca deja el objetivo afuera.
8. Tokens de túnel: el chute es un solo par.

## Kit modular y salas en UE

`U` = **800 cm, cuadrado**. Las casillas de prueba actuales miden 866 × 696 y no rotan bien, así
que el kit no las sigue.

Piezas en `Content/Project_C/Map/Kit/`, generadas por `assets/3d/kit/kit.py`
(`blender -b --python kit.py -- <carpeta de salida>`):

| Pieza | Medida | Pivote |
|---|---|---|
| `SM_Kit_Floor` | 800 × 800 × 10 | centro, cara de arriba en z = 0 |
| `SM_Kit_Wall` | 800 × 20 × 150 | centro del borde de la celda; crece hacia -Y local (adentro) |
| `SM_Kit_WallDoor` | igual, con hueco de 200 en el medio | igual |
| `SM_Kit_DoorFrame` | 230 × 20 × 220 | igual que la pared |
| `SM_Kit_Corner` | 30 × 30 × 170 | esquina de la celda, crece hacia +X / -Y |
| `SM_Kit_Hatch` | 160 × 160 × 6 | centro, apoyada en el piso |

Cada sala tiene sus 4 paredes metidas hacia adentro, así que dos salas vecinas forman una pared
de 40 cm y los huecos de puerta coinciden. Por eso las salas se pueden mover sueltas.

Las 7 salas ya están armadas y fusionadas en un mesh cada una, en `Content/Project_C/Map/Rooms/`:
`SM_Room_Staff`, `SM_Room_Dock`, `SM_Room_Offices`, `SM_Room_Lift`, `SM_Room_Gallery`,
`SM_Room_Lab` y `SM_Room_Batch`. El pivote es el centro de la primera celda de cada sala
(la de arriba a la izquierda en el plano). Los props y los `BP_Space` van aparte, encima.

Orientación en UE: derecha del plano = +Y, arriba del plano = +X.

## Props

17 props en `Content/Project_C/Map/Props/` (`SM_Prop_*`), FBX fuente en `assets/3d/props/`.
Meshy text-to-3D, `meshy-t2` smart topology, 1000 a 4000 tris, **sin textura** (5 créditos c/u,
85 en total). Pivote en la base, centrado.

| Prop | Sala | Alto |
|---|---|---|
| Pallet, Forklift, Barrel | Dock (Barrel también Gallery y Cold store) | 1.8 / 2.2 / 0.9 m |
| Lockers, Bench, HazmatSuit | Lockers | 1.8 / 1.2 / 1.7 m |
| Console, Chair | Control (Chair también Office) | 1.3 / 0.9 m |
| Desk, FilingCabinet, Safe | Office | 0.75 / 1.2 / 1.1 m |
| Valves | Pipe Gallery | 1.2 m |
| FermentVat | Vat room | 1.8 m |
| ColdRack | Cold store | 1.8 m |
| LabBench | Cook lab | 0.9 m |
| CoreVat | Batch Chamber (pieza del jefe) | 3.6 m |
| ChitinGrowth | Batch, Vat room, Gallery | 1.4 m |

Reescalados en Blender después de Meshy: Safe ×0.5, CoreVat ×3, FermentVat ×1.5, Valves ×1.7.
ChitinGrowth vino de Meshy con 1e-7 m de alto (falla del auto-size) y se reescaló desde el GLB.

### Relleno, detalles y luz

- **16 props de relleno** en `Map/Fill/` (`SM_Fill_*`): 10 industriales (cajas, bidones, balde,
  herramientas, carretel, bolsas, escombros, garrafas, matafuego, cajón) y 6 del Growth (huevos,
  charco de biomasa, frasco con espécimen, hongos, tentáculos, costra). Tamaños fijados por
  `assets/3d/fill/normalize.py`.
- **7 detalles del kit** en `Map/Kit/` (`SM_Det_*`), generados por `assets/3d/kit/details.py`:
  caños de pared (con y sin hueco de puerta), rejilla, tablero, lámpara de pared, lámpara de pie
  y rejilla de piso. Mismo pivote que la pared, así que van con el transform de la pared.
- **`assets/3d/fill/scatter.py`** reparte todo por celda: caños en algunas paredes, una lámpara
  con su PointLight, un tablero o rejilla, y de 3 a 7 objetos de relleno contra las paredes. Deja
  libres el centro de la casilla, las puertas y los props principales. La proporción de Growth
  sube al bajar: 0 % en Staff y Oficinas, 10 % en el Dock, 45 % en la Gallery, 60 % en el Lab y
  85 % en la Batch. Luz cálida en la Planta, celeste en la Gallery y verde en el Lab y la Batch.
  Corre de nuevo sin duplicar: primero borra los actores `Fill_`, `Det_` y `Light_`.

### Texturas y materiales

- **Props y relleno:** `meshy_retexture` (meshy-6, PBR, sin luz horneada, 10 créditos c/u) con un
  prompt por asset sobre una base común: pintado estilizado, paleta apagada, bloques de color
  legibles, sin texto. `assets/3d/texnorm.py` reescala cada GLB texturizado a la medida del FBX
  anterior, pisa ese FBX (ahora con UVs) y saca los mapas a 1K en `assets/3d/textures/`
  (`T_<asset>_BC`, `_N`, `_RM` con roughness en G y metallic en B, y `_E` solo para Console,
  LabBench, CoreVat, Fungus y SpecimenJar). Meshy pone emisión espuria en casi todo; por eso la lista.
- **`assets/3d/tex_import.py`** importa en UE: borra el mesh viejo, importa el nuevo con el mismo
  nombre, reapunta los actores `Prop_` y `Fill_` del nivel, importa las texturas a `Map/Textures/` y
  crea `MI_<asset>` hijo de `M_Prop` en `Map/Materials/`. Necesita el cliente MCP del scratchpad
  (`at.py`) en `PYTHONPATH`.
- **ChitinGrowth** se regeneró (su mesh original estaba colapsado y Meshy no lo pudo texturizar).
- **Salas:** el merge las dejó con un solo slot, así que `M_Room` separa por máscara: normal hacia
  arriba y altura < 20 cm es piso, normal hacia arriba más alto es tope de pared (`TopColor`), el
  resto es pared. Texturas tileables de Meshy text-to-image (`assets/3d/kit/textures/`), piso a 4 m
  y pared a 2 m. `MI_Room_Plant` para Staff, Dock, Oficinas y Lift; `MI_Room_Sump` (venas, limo y
  quitina) para Gallery, Lab y Batch.
- **Detalles del kit:** `M_KitSolid` (color, roughness, metallic, emisivo) con `MI_Kit_Trim`,
  `MI_Kit_Pipe`, `MI_Kit_Glow` y `MI_Kit_Hatch`. Las paredes y pisos sueltos del kit usan `MI_Room_Plant`.

### Fondo (diorama) y cámara

El mapa vive en `Levels/Laboratorio` (`Levels/Level` es el prototipo viejo). El fondo mezcla dos
conceptos: maqueta recortada del suelo y oscuridad con niebla.

- **Bases de roca:** `SM_Dio_Base_Plant` (Planta + Lift) y `SM_Dio_Base_Sump`, generadas por
  `assets/3d/kit/diorama.py` con el contorno exacto de las celdas, 4,5 m de roca con borde de abajo
  irregular. Tope a z = -10 cm, debajo del piso.
- **`assets/3d/kit/diorama_place.py`** arma todo en el nivel abierto (borra antes los actores `Dio_`):
  bases, paneles de corte, cuatro piezas 3D de fondo, el cuenco pintado, luces y niebla.
- **Roca:** `MI_Dio_Rock` (hijo de `M_Prop`) con `T_Dio_Rock_*`, generadas por
  `assets/3d/kit/rock_tex.py` (ruido FFT tileable, estratos y grietas). Las bases llevan UV de caja
  a 4 m. Luces bajas sin sombra pegadas a los bordes iluminan el corte.
- **Roca (textura):** `T_Dio_Rock_*` salen de una imagen de Nano Banana (`textures/rock_cliff_src.png`)
  recortada y hecha tileable por `rock_tex.py`.
- **Cortes (Meshy):** recortes del concepto pasados por Nano Banana (imagen limpia del asset) y
  después por image-to-3D. Cada borde exterior lleva un panel de 8,4 m: `SM_Dio_CutPlant` (caños y
  raíces) en toda la Planta, y en el Sumidero alternado con `SM_Dio_CutSump` (raíces), espejados uno
  sí y uno no. Sus texturas van a 2K (`HIRES` en `texnorm.py`): a 1K se veían borrosas en 8,4 m.
  Una luz cálida baja por borde los ilumina. Tamaños en `texnorm.py` (`NEW`).
- **Fondo pintado (360°):** `SM_Dio_Backdrop` es un cuenco de 200 m de radio (`backdrop.py`) que
  rodea el mapa, con `M_Backdrop` (unlit) y `T_Backdrop`: cuatro pinturas de Nano Banana del abismo
  (`textures/backdrop_src_*.png`) unidas sin costura por `backdrop_tex.py`. Girado 45° para que los
  caños colgantes con resplandor ámbar queden detrás de la Planta.
- **Anillo 3D (360°):** 28 piezas repartidas alrededor del mapa, alternando cerca (18 a 30 m del
  borde) y lejos (35 a 60 m), con el tope entre 15 y 2 m por debajo del piso para que no tapen el
  mapa desde ningún ángulo de la cámara: `SM_Dio_RuinFacade`, `SM_Dio_Islet` (roca flotante con
  máquinas), `SM_Dio_RootSpire`, `SM_Dio_Tanks`, `SM_Dio_RuinPillars` y `SM_Dio_PipeGantry`.
- **Bancos de niebla:** pocas tarjetas grandes (`/Engine/BasicShapes/Plane` con `M_FogCard`:
  aditivo, unlit, `DepthFade`, textura de humo de Nano Banana suavizada y con caída radial, también
  de `backdrop_tex.py`): tres ámbar detrás de Planta y Lift y cinco verdes en el hueco entre
  secciones. Aditivas para que brillen y nunca ensucien; sin sombra.
  Se descartaron los `LocalFogVolume`: `r.LocalFogVolume.MaxDensityIntoVolumetricFog` (0,01) los
  deja casi invisibles con la niebla volumétrica activa y la captura del editor no los dibuja.
- **Luz:** luna direccional fría (`Dio_Moon`, 0,4) y luz de cielo con hemisferio inferior teal
  (`Dio_Sky`, captura estática: la de tiempo real pide SkyAtmosphere y muestra un aviso) para que las piezas tengan forma; ninguna de las dos ilumina la niebla. Las luces de
  sala (`assets/3d/kit/lighting.py`) mezclan su color con blanco: 90 % del color en Planta, 60 % en
  el Sumidero. La `ExponentialHeightFog` es fina (0,006): más densa tapaba el fondo con un velo
  teal.
- **Color:** `assets/3d/kit/post.py` pone en el `PostProcessVolume` una LUT (`T_GradeLUT`) que
  calcula `grade_lut.py` igualando la media y el desvío de color (Lab) de una captura con los del
  concepto, más viñeta 0,55 y bloom 0,8.
- **Validación:** captura del nivel al lado del concepto (`concept_mix`) con el mismo encuadre
  (brazo 105 m, pitch -52), y desde 4 ángulos de órbita para que el entorno cierre a 360°.
- **Cámara (`BP_CameraPawn`):** FOV 60° e inclinación -45°: con FOV 35° la escena se veía plana,
  sin profundidad. Brazo 2600 por defecto, 1000 a 6500 de zoom (a 6500 entra el mapa entero), paso
  300, sin colisión del brazo (la roca y los pilares lo acortaban). Vale también para Prototype.
  El paneo (WASD) usa la rotación de mundo del brazo, así no depende de cómo aparece el pawn.
  Laboratorio tiene un `PlayerStart` mirando a +X (el encuadre del concepto).

## Tablero y unidades

`assets/3d/kit/units_place.py` arma el tablero de Laboratorio, en la carpeta `Board` del outliner:

- **Casillas:** 11 `BP_Space` (`Space_<espacio>`), una por espacio, con las 12 conexiones del plano
  en `Neighbours`. Cada una está donde no choca con los props (las cubas de Vat room y Batch
  Chamber ocupan el centro) y rotada 90°: así el tile de 816 × 646 entra en la celda y la fila de
  ocupantes corre a lo ancho de la sala.
- **Tile:** usa `M_SpaceOverlay`, translúcido y sin sombra. En reposo no se ve (el piso es el del
  diorama) y con `ApplyHighlight` se tiñe con los mismos parámetros que `M_Space`. El click no
  depende de que se vea: el trace es por el canal de objetos `Space`.
- **Unidades:** Shiv, Brace, Spike y Maul en el Loading Dock, que es el inicio. 3 Cooks en el
  Sumidero: Cold store, Pipe Gallery y Batch Chamber. Así la Planta está tranquila al empezar y
  el peligro está abajo. Cada unidad toma su fila de `DT_Units` del default de su clase y engancha
  su casilla al empezar la partida (`FindStartSpace`, a menos de 300).
- En las salas de varias celdas (Dock, Lift, Gallery, Batch) el tile cubre una sola celda: solo
  esa se puede clickear.

## Abierto

- Las 3 Drains quedaron en el Sumidero: la Planta es tranquila al principio y la presión crece al
  bajar. Si se quiere presión desde el turno 1, una pasa a la Planta (Office, a distancia 2,
  rompe el mínimo propuesto de 3).
- Batch Chamber tiene una sola puerta. Es un embudo deliberado para la pelea final; si en
  playtest se siente trampa, se abre una segunda puerta hacia la Gallery.
