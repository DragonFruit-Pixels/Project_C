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

Textura: pendiente. Plan: `meshy_retexture` con un mismo prompt de estilo para los que queden
(10 créditos c/u) o materiales del kit en UE para los simples.

## Abierto

- Las 3 Drains quedaron en el Sumidero: la Planta es tranquila al principio y la presión crece al
  bajar. Si se quiere presión desde el turno 1, una pasa a la Planta (Office, a distancia 2,
  rompe el mínimo propuesto de 3).
- Batch Chamber tiene una sola puerta. Es un embudo deliberado para la pelea final; si en
  playtest se siente trampa, se abre una segunda puerta hacia la Gallery.
