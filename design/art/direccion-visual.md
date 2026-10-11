# 🎨 Dirección visual — biopunk

> ## ⚠️ Provisional, y no es una decisión registrada
>
> Sale de la sesión de exploración con Meshy del **2026-09-10**, no de un proceso de diseño.
> No está en [`registro.md`](../gdd/06-decisiones/registro.md) y no cierra nada. Si se
> aprueba, alguien tiene que darla de alta como decisión y linkearla desde el índice del GDD
> — este documento no lo hace.
>
> Depende de [D-14](../gdd/06-decisiones/registro.md#d-14--la-temática-es-biopunk-provisionalmente)
> (biopunk, provisional) y de la
> [propuesta A-08](../gdd/02-personaje/propuesta-a08-personajes-y-skills.md), que **sigue
> abierta**. Si cambia el roster, cambia este documento.

## Por qué existe

Sin esto, cada tirada de generación re-improvisa el estilo. En la primera tanda pasó
literal: Bulwark salió pintado y Ram salió fotorrealista con el mismo prompt de mundo,
porque el prompt describía la ambientación y no el render. Este documento es lo que evita
pagar dos veces por descubrir lo mismo.

## 1. Estilo de render — cerrado

Va **al principio** del prompt, en los cuatro, sin editar:

```
Painted concept art illustration, visible brushwork, clean dark outline, flat even lighting,
matte surfaces, no photorealism, no glossy highlights, no cinematic lighting.
```

Elegido sobre realista por preferencia del dueño del proyecto. La cláusula está replicada en
el campo `style_clause` de cada `assets/3d/<slug>/meshy.json`.

Agregar siempre `plain flat grey backdrop, no scenery`: sin eso el generador arma escenario
propio, y la basura de fondo contamina la entrada de `image-to-3d`.

## 2. Mundo — de dónde sale, no se inventa

De [`docs/rulebook/glossary-biopunk.md`](../../docs/rulebook/glossary-biopunk.md): ciudad
vertical enterrada bajo su propia industria química, el aire de abajo mata despacio, un
cartel refina un mutágeno que da fuerza y se cobra el cuerpo.

**Restricciones duras, heredadas del glosario:** nada de Zaun / Shimmer / Piltover / Hextech
/ Umbrella. Nada que lea a magia u ocultismo. Es química e industria pesada.

**Paleta:** acero oxidado, negro alquitrán, residuo químico amarillo-verde en las costuras.
Todo mate, desaturado, sucio.

## 3. Regla de silueta — una por personaje, sin repetir

`high-concept.md` declara XCOM como lenguaje de cámara. Eso quiere decir que los personajes
se ven **chicos y desde arriba**: a esa distancia lee la silueta y el color, no el detalle.
Cuatro figuras encapuchadas con equipo scavengeado son cuatro manchas iguales.

| Personaje | Silueta | Lo que la hace única |
|---|---|---|
| `Bulwark` | Bloque cuadrado, ancho y bajo | Placas rígidas, contorno duro |
| `Hunter` | Vertical y flaco | **Sin capucha**, cara descubierta; el rifle cruza en diagonal |
| `Wraith` | Drapeada, sin contorno duro | **El único encapuchado**, telas colgando |
| `Ram` | Torso y brazos desnudos | Piel expuesta, la masa del mutágeno rompe el hombro |

## 4. Regla de máscara — es mecánica, no decoración

**`Bulwark` es el único con respirador sellado de cara completa.**

No es gusto: `Toughness` es la única skill del roster que frena la ganancia de `Ratchet`
([A-08 §3.3](../gdd/02-personaje/propuesta-a08-personajes-y-skills.md)), y Bulwark es el
único que la lleva. Si otro personaje se ve igual de sellado, la mecánica deja de leerse en
el arte.

En la tanda del 2026-09-10 esto se rompió: Hunter volvió con antigás full + goggles y se ve
**más** protegido que Bulwark. Hunter va con media máscara y goggles, nada más.

`Ram` es el otro extremo y es igual de deliberado: sin nada en la cara, respirando el veneno.
Bulwark y Ram son la misma pregunta —¿qué relación tenés con tu propia contaminación?—
contestada al revés.

## 5. Regla de rig — un solo esqueleto: el mannequin de UE5

> **Decisión revertida el 2026-09-20. Esta sección ya no es la que manda.**
>
> Wraith se regeneró y se riggeó con Meshy a propósito, con autorización explícita del dueño
> del proyecto. En el Content hay: `SK_Wraith` sobre `SKEL_Wraith` (24 huesos estilo Mixamo —
> `Hips`, `Spine01`, `RightHand`), `PHYS_Wraith`, `M_Wraith`, y **seis** AnimSequences —
> idle, walk, run, attack, hit, death. Detalle completo en `assets/3d/wraith/meshy.json`.
>
> **Por qué se revirtió.** El pipeline de abajo exige Blender, que está fuera de alcance, y
> el mannequin no está en el proyecto (`find_assets` por "Manny"/"Mannequin" no devuelve nada
> usable). Sostener esta regla daba cero personajes animados por tiempo indefinido.
>
> **Un cambio de esta pasada contradice el punto 1 de abajo:** la malla se generó en
> **T-pose**, no en A-pose. El argumento de la A-pose era calzar con el ref pose de Manny; sin
> Manny en el camino, ese argumento no aplica, y `meshy_rig` riggea mejor sobre T.
>
> **Lo que sigue siendo cierto de esta sección:** un esqueleto por personaje escala mal. Si el
> roster llega a cuatro, son 4 Skeletons y 4 AnimBP. La salida barata cuando haga falta es un
> **IK Retargeter**, no rehacer el skinning — y con eso también entra todo Mixamo gratis. Ojo
> con los nombres: Mixamo usa `Spine1`/`Spine2`/`Neck`/`HeadTop_End` y este rig usa
> `Spine01`/`Spine02`/`neck`/`head_end`. Son cuatro remapeos a mano, una sola vez.
>
> La malla A-pose de la primera tanda quedó archivada en `assets/3d/wraith/_v1/`.


Restricción declarada el 2026-09-10: los cuatro se animan con el set de animaciones de Unreal
y con **un** Animation Blueprint.

Un AnimBP se ata a un solo asset Skeleton. Si cada personaje llega con el esqueleto que genera
Meshy, salen 4 Skeletons, 4 AnimBP y un retarget por cabeza. Por eso la decisión es que los
cuatro compartan el **esqueleto del mannequin de UE5**.

**`meshy_rig` no se usa, y no es por precio.** `meshy-assets.md` ya lo dice: *"el esqueleto que
devuelve no es el mannequin de UE5: el retargeting es manual"*. Un esqueleto que después hay
que tirar no vale los 5 créditos.

### El pipeline

1. **Meshy** entrega la malla **sin riggear** — `image-to-3d` y `convert` a FBX. Nada más.
2. **Blender** — importar el skeleton del mannequin, alinear la malla, *Armature Deform with
   Automatic Weights*, corregir pesos a mano.
3. **UE5** — importar el FBX eligiendo el Skeleton **existente** del mannequin, nunca creando
   uno nuevo.

Los cuatro quedan sobre `SK_Mannequin`: un AnimBP, y todas las animaciones de Unreal, Lyra y
marketplace andan nativas sin retargetear.

### Qué le exige eso al concept

Se corrige en el concept, que cuesta 3 créditos, y no en la malla, que cuesta 30.

1. **A-pose, brazos separados del torso.** El ref pose de Manny **es A-pose**, así que los
   brazos a 45° son el match correcto: pedir T-pose acá lo empeora. Lo que no puede pasar es
   que los brazos toquen el cuerpo — ahí el generador funde torso y brazo en una masa y el
   skinning no tiene de dónde agarrarse.
2. **Proporciones humanas.** Los *automatic weights* mapean contra los huesos del mannequin;
   una proporción rara los rompe. Ancho sí (Bulwark), deforme no.
3. **Nada colgando cerca de las piernas.** Faldones y telas sueltas se estiran entre las
   piernas al caminar.
4. **Nada de props sobre el cuerpo.** Rifle a la espalda, soga, ganchos: `image-to-3d` los
   funde a la malla y después se deforman con el hueso que les tocó. Las armas van como mesh
   aparte, socketeadas.

### Las armas son assets aparte

Confirmado el 2026-09-10: las armas son objetos y modelos 3D propios, no parte del personaje.
Cada una con su `assets/3d/<slug>/` y su ledger, y se montan por socket en UE.

Los prompts de personaje van **sin arma**: el generador no puede fundir lo que no se le pidió.
La única excepción es el brazo mutado de `Ram` — eso es cuerpo, y va en la malla del personaje.

**Consecuencia sobre la tanda del 2026-09-10:** Hunter (rifle cruzado, soga, ganchos) y Wraith
(faldón largo, tiras colgando) no son riggeables como están. Bulwark sí. Ram necesita fondo
plano por otra razón.

**Escala y ejes** (de `meshy-assets.md`): GLB es Y-up en metros, UE5 es Z-up con 1 unidad = 1 cm.
Rotación + factor 100.

## 6. Color de firma — el sistema está cerrado, los colores no

Son dos cosas distintas y antes estaban mezcladas en una sola línea.

**El sistema, decidido el 2026-09-21.** El acento es un **parámetro de material**, no pintura
horneada en la textura. Se implementó así:

```
M_Character                  ← un solo Material, un solo shader compilado
  │  parámetros: BaseColor · Normal · Roughness · Metallic · AccentMask
  │              SignatureTint (color) · UseAccentMask (static switch)
  └── MI_Wraith              ← Material Instance: sólo valores, sin grafo
```

Un personaje nuevo es **un Material Instance y cinco campos** — no compila shaders nuevos. Y
cambiar cómo funciona el acento se hace una vez en el padre y lo heredan todos.

La razón de fondo no es de ingeniería sino de §7: ahí queda abierto *si la contaminación se ve
progresar*. Con el color como parámetro eso ya es posible — un Material Instance Dynamic mueve
el valor en runtime según el `Ratchet` del personaje. Con el acento horneado en la textura
sería imposible sin meshes intercambiables. **Esa puerta queda abierta a propósito.**

`UseAccentMask` arranca en `false`, así que hoy el Lerp ni siquiera existe en el shader
compilado y los personajes se ven exactamente igual que antes. Encenderlo no cuesta créditos.

**Lo que sigue sin decidir:** qué color lleva cada uno. Hoy los cuatro comparten paleta y a
distancia XCOM no se distinguen, que es el problema que §3 plantea y esto todavía no resuelve.
Restricción heredada de §2: el amarillo-verde de residuo químico ya es color del mundo, así que
si un personaje se lo queda, los otros tres van a otro lado del círculo.

**Lo que falta técnicamente:** la máscara. Sin ella `SignatureTint` tiñe el personaje entero
(cara y piel incluidas), que sirve para "Wraith es el verdoso" pero no para "Wraith tiene las
costuras verdes". Se resuelve cuando estén los cuatro y se pueda juzgar a distancia real.

## 7. Lo que este documento NO decide

- **Casting.** Edad, género y etnia de los cuatro. Hoy los define el generador por default
  (Ram salió hombre blanco de treinta y pico porque nadie dijo otra cosa). Los nombres del
  glosario —Wren, Marisol, Odile, Bram, Cas, Nadia, Tess— en su mayoría no apuntan ahí.
- **Nombres finales.** `Bulwark`, `Hunter`, `Wraith` y `Ram` son codenames de diseño.
- **Si la contaminación se ve progresar.** `Ratchet` sube durante la partida, pero los
  concepts muestran el estado final horneado. Si tiene que verse subir, son estados de
  material o meshes intercambiables — decisión de producción, no de arte.

## 8. Dónde están los assets

`assets/3d/<slug>/` con `concept.png` y `meshy.json`. El ledger registra cada tirada con su
costo y, cuando se descartó una, el motivo — para que nadie vuelva a pagarla.
Reglas de producción y costos: [`.claude/rules/meshy-assets.md`](../../.claude/rules/meshy-assets.md).
