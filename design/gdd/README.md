# 📖 GDD — Project_C

Documento de diseño del juego, partido por tema. **Esta carpeta es la fuente de verdad.**

Videojuego en Unreal Engine 5.8, Blueprint-only. Single player, por turnos, con
presentación 3D. Reimplementa las mecánicas del juego de mesa *Cthulhu: Death May Die*
(Season 3) cambiando la temática.

> **La temática todavía no está decidida.** Todo el vocabulario de estos documentos es
> neutro a propósito: son IDs, no texto de jugador. Hay dos variantes temáticas completas
> y verificadas en `docs/rulebook/`, y el core no debe depender de ninguna.

## Índice

### 🎯 00 — Visión

| Archivo | Qué contiene |
|---|---|
| 🎯 [`00-vision/concepto.md`](00-vision/concepto.md) | Qué es el juego y cuál es su decisión central |
| 📐 [`00-vision/alcance.md`](00-vision/alcance.md) | 1 misión + 1 adversario, y qué implica ese recorte |

### 🧱 01 — Fundamentos

| Archivo | Qué contiene |
|---|---|
| 🔤 [`01-fundamentos/vocabulario.md`](01-fundamentos/vocabulario.md) | Los IDs neutros y por qué existen |
| 🗺️ [`01-fundamentos/mapa-y-espacios.md`](01-fundamentos/mapa-y-espacios.md) | El mapa como grafo, salas, puertas, distancias |

### 🧍 02 — El personaje

| Archivo | Qué contiene |
|---|---|
| 📊 [`02-personaje/barras-y-recursos.md`](02-personaje/barras-y-recursos.md) | Wounds, Reserve y Ratchet |
| ⚙️ [`02-personaje/trinquete.md`](02-personaje/trinquete.md) | **La mecánica central del juego** |
| 🎓 [`02-personaje/skills.md`](02-personaje/skills.md) | 6 skills × 4 niveles |

### 🎲 03 — Resolución

| Archivo | Qué contiene |
|---|---|
| 🎲 [`03-resolucion/dados.md`](03-resolucion/dados.md) | Las 4 caras, rerolls y orden de resolución |
| 🔄 [`03-resolucion/turno.md`](03-resolucion/turno.md) | La secuencia de turno completa |
| 🧩 [`03-resolucion/vocabulario-de-efectos.md`](03-resolucion/vocabulario-de-efectos.md) | Las piezas componibles de las que están hechas las cartas |

### ⚔️ 04 — La oposición

| Archivo | Qué contiene |
|---|---|
| ⚔️ [`04-oposicion/enemigos.md`](04-oposicion/enemigos.md) | Taxonomía, comportamiento y spawn |
| ⏳ [`04-oposicion/reloj-y-avance.md`](04-oposicion/reloj-y-avance.md) | El Doom Track y por qué es impredecible |
| 👑 [`04-oposicion/objetivo-y-adversario.md`](04-oposicion/objetivo-y-adversario.md) | Las dos mitades de la partida |

### 🏁 05 — La partida

| Archivo | Qué contiene |
|---|---|
| 🏁 [`05-partida/victoria-y-derrota.md`](05-partida/victoria-y-derrota.md) | Condiciones de fin y las dos fases de riesgo |
| 📦 [`05-partida/contenido-de-una-mision.md`](05-partida/contenido-de-una-mision.md) | Inventario de contenido a producir |

### 🎛️ 07 — Balance

| Archivo | Qué contiene |
|---|---|
| 🎛️ [`07-balance/perillas-y-constantes.md`](07-balance/perillas-y-constantes.md) | **Dueño único de todos los números**: constantes, perillas con rango seguro, la distribución provisional de dados, y lo que el manual no contiene |

### 🧭 06 — Decisiones

| Archivo | Qué contiene |
|---|---|
| 📌 [`06-decisiones/registro.md`](06-decisiones/registro.md) | Decisiones tomadas, con su razón y su fecha |
| ❓ [`06-decisiones/abiertas.md`](06-decisiones/abiertas.md) | Lo que falta decidir y qué está en juego |

## Cómo leer esto

Si venís de cero, el orden es el del índice. Si venís a implementar algo puntual, andá
derecho al archivo del sistema: cada uno se sostiene solo y linkea a los que necesita.

Las **reglas exactas y los números** salen de la transcripción del manual original en
`docs/rulebook/rulebook.md`. Este GDD las reformula y las explica; no las reemplaza. Ante
una diferencia entre los dos, **gana el manual** y hay que corregir el GDD.

## Documentos relacionados

| Ruta | Qué es |
|---|---|
| `docs/rulebook/rulebook.md` | Transcripción forense 1:1 del manual original, 28 páginas |
| `docs/rulebook/glossary-biopunk.md` | Variante temática A, 66 términos |
| `docs/rulebook/glossary-dnd.md` | Variante temática B, 67 términos |
| `design/course-alignment.md` | Matriz de cobertura del temario de la materia y plan de dos fases |
| [`../architecture/README.md`](../architecture/README.md) | **Cómo se implementa en UE5**: framework, servicios, comunicación y mapa de clases |

## Mantenimiento

**Existe una copia legible para el equipo en ClickUp** (doc `GDD — Project_C`,
`2ky4j7vt-8099`) y tiene que estar **1:1** con esta carpeta: mismas secciones, mismas
reglas, mismos números. La de ClickUp puede estar mejor redactada; no puede decir algo
distinto. Si se cambia una, se cambia la otra.

Reglas para editar:

- **Un tema, un archivo.** Si un archivo empieza a cubrir dos cosas, se parte.
- **Los números viven en un solo lugar**, y ese lugar es
  [`07-balance/perillas-y-constantes.md`](07-balance/perillas-y-constantes.md). Ningún otro
  archivo declara un default ni un rango seguro: linkean a ese. Esta regla se agregó porque el
  problema ya pasó — la misma perilla estaba definida en tres archivos con rangos distintos.
- **Nada de vocabulario temático.** Ni Cthulhu, ni Elder One, ni Mythos, ni nombres de
  ninguna de las dos variantes. Si necesitás nombrar algo nuevo, agregalo al vocabulario.
- **Las decisiones se registran.** Toda decisión de diseño va a `06-decisiones/registro.md`
  con su razón. El "por qué" es lo que se pierde primero.
