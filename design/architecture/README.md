# 🏗️ Arquitectura

Cómo se construye Project_C en Unreal Engine 5.8, con **arquitectura híbrida C++ + Blueprint**
([D-13](../gdd/06-decisiones/registro.md)). **Esta carpeta responde
"con qué se hace", el [GDD](../gdd/README.md) responde "qué se hace".** Si un documento de acá
contradice al GDD en una regla de juego, gana el GDD; si el GDD propone algo que rompe una de
las reglas de acá, se discute, no se implementa.

## Índice

| Archivo | Qué contiene |
|---|---|
| 🧭 [`01-por-donde-se-empieza.md`](01-por-donde-se-empieza.md) | El orden de fundación de un proyecto UE5, y en qué paso estamos |
| 🧰 [`02-managers-y-subsystems.md`](02-managers-y-subsystems.md) | La respuesta de Unreal a "managers/servicios", y qué se puede hacer sin C++ |
| 🔌 [`03-comunicacion-y-referencias.md`](03-comunicacion-y-referencias.md) | Interfaces, dispatchers y por qué un `Cast` cuesta memoria |
| 🗂️ [`04-mapa-de-clases.md`](04-mapa-de-clases.md) | El esqueleto concreto: qué Blueprint es dueño de qué |
| 📚 [`05-temario-como-orden-de-construccion.md`](05-temario-como-orden-de-construccion.md) | El temario leído como plan de bring-up, clase por clase |
| ⚙️ [`06-limite-cpp-blueprint.md`](06-limite-cpp-blueprint.md) | **Qué va en C++ y qué en Blueprint**, y qué habilita cada uno |

## Las tres reglas que gobiernan todo lo demás

Están arriba porque **cada decisión de los otros documentos sale de una de las tres**. Si hay
que tirar todo y empezar de nuevo, empezar por acá.

### 1. Un dato, un dueño

Cada pieza de estado tiene **exactamente un Blueprint que la escribe**. Todos los demás la
leen. No hay estado compartido con dos escritores.

Es la regla más barata de respetar al principio y la más cara de recuperar después: cuando el
`Doom Track` avanza desde tres lugares distintos, el bug no se debuggea, se arqueologiza.
Quién es dueño de qué está en [`04-mapa-de-clases.md`](04-mapa-de-clases.md).

### 2. Nadie conoce a nadie por su clase concreta

Hablar por **Blueprint Interface** o por **Event Dispatcher**, no por `Cast To BP_Loquesea`.

No es purismo: en Unreal una referencia dura arrastra el asset entero —y su cadena de
dependencias— a memoria, y en un repo de assets binarios además arrastra el archivo a los
conflictos de merge. El detalle está en
[`03-comunicacion-y-referencias.md`](03-comunicacion-y-referencias.md).

### 3. Las reglas leen los números, no los tienen

Ninguna regla escribe un número en un nodo del grafo. Los lee de un Data Asset de
configuración, aunque en la versión 1 ese Data Asset tenga los valores puestos a mano.

**Esto es una consecuencia directa del temario.** La capa de datos —Data Assets, Data Tables,
Gameplay Tags— es clase 12, o sea *después* del 1er parcial. Si el parcial 1 se construye con
los números tipeados adentro de los grafos, la clase 12 no es una migración: es reescribir
todo lo que ya funcionaba. Con la costura puesta desde el día 1, es cambiar de dónde sale el
valor. Ver [`05-temario-como-orden-de-construccion.md`](05-temario-como-orden-de-construccion.md).

Con C++ esta regla se abarata: el `USTRUCT` de configuración se declara una vez y el Data Asset
se autorea en el editor. La costura deja de ser disciplina y pasa a ser un tipo.

Los números en sí siguen viviendo en un solo lugar:
[`../gdd/07-balance/perillas-y-constantes.md`](../gdd/07-balance/perillas-y-constantes.md).

## Documentos relacionados

| Ruta | Qué es |
|---|---|
| [`../gdd/README.md`](../gdd/README.md) | El diseño del juego. Fuente de verdad de las reglas |
| [`../course-alignment.md`](../course-alignment.md) | La matriz de cobertura del temario. **Es el contrato de la materia**; acá está el orden de construcción |
| [`../../docs/unreal/project-context.md`](../../docs/unreal/project-context.md) | Estado técnico del proyecto: versión, plugins, convenciones de naming, layout de `Content/` |
| [`../../docs/rulebook/rulebook.md`](../../docs/rulebook/rulebook.md) | Transcripción del manual original |
