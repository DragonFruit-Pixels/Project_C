# Para pegar en el doc del grupo

Destino: **"Desarrollo avanzado - High concept"** (Google Doc, lo mantiene `gasparorma13@`).

Ese doc tiene cuatro secciones con título y sin contenido —*Movimiento entre habitaciones*,
*Combate*, *Cartas de evento*, *Cartas de descubrimiento*— y le falta entera la fase de fin de
turno. Todo eso ya está resuelto en este GDD.

**Está escrito en el vocabulario de ellos** (fantasy: habitaciones, aventureros, locura), no
con los IDs del core, para que entre sin traducir. El mapeo entre ambos está en
[`gdd/00-vision/high-concept.md`](gdd/00-vision/high-concept.md#capa-temática).

Lo de abajo de la línea se pega tal cual. Las **preguntas para el grupo** del final **no**.

---

## Movimiento entre habitaciones

El mapa es un **grafo**: habitaciones conectadas por pasajes. La distancia se mide en pasajes
recorridos, no en metros, y dos habitaciones conectadas están a distancia 1.

- Moverse cuesta **1 acción** y permite avanzar hasta **3 habitaciones**.
- Algunos pasajes pueden quedar **bloqueados** por efectos. Ciertos personajes y habilidades
  los cruzan igual: el bloqueo es por personaje, no del mapa.
- Los **portales** conectan habitaciones no contiguas y cuentan como adyacentes **para todo
  efecto**: moverse, atacar y medir distancia.
- Un pasaje bloqueado **no cuenta como adyacente** para quien no puede cruzarlo. Baja el
  número de salidas de esa habitación, y hay efectos que leen ese número.

**Los enemigos son pegajosos.** Al salir de una habitación, **todos** los enemigos que
estaban ahí te siguen. No hay forma de zafar. Esto es deliberado: convierte al movimiento en
la decisión más cara del juego, porque huir no resuelve, arrastra.

## Combate

Atacar cuesta **1 acción**.

Se tiran los dados del personaje. El dado tiene **4 caras**:

| Cara | Efecto |
|---|---|
| **Acierto** | 1 herida al objetivo |
| **Comodín** | Nada, salvo que una habilidad o carta lo lea |
| **Peaje** | **+1 Locura.** Siempre, en toda tirada |
| **Vacío** | Nada |

Algunas caras traen dos resultados a la vez (por ejemplo Acierto y Peaje juntos): se aplican
los dos.

**La regla que define el juego: el Peaje cobra en TODAS las tiradas.** Atacando,
defendiéndote, o tirando por cualquier efecto. No existe una tirada gratis, y por eso **no
hacer nada es una jugada legítima**: un turno donde te movés a una habitación limpia y
descansás es un turno donde no tocaste los dados.

**Los dados bonus no tienen cara de Peaje.** Los que ganás por habilidades, cartas o umbrales
de Locura son puro beneficio. El poder es doblemente bueno: tirás más dados, y tirás dados que
no pueden cobrarte.

Los **puntos de Estrés** permiten volver a tirar un dado, reemplazando el resultado por el
nuevo.

Al **boss** no se lo puede atacar hasta que estén destruidos los altares.

## Cartas de evento

Se roba **1 carta al terminar las 3 acciones** del personaje, y se resuelve **de arriba hacia
abajo**: el orden de los efectos dentro de la carta importa.

Las cartas resueltas van al **descarte**, y algunas llevan **símbolos de reloj**.

Cuando el descarte acumula **3 o más símbolos**, el boss **avanza un stage** y el **descarte
se remezcla entero** de vuelta al mazo. Como el contador se resetea con la remezcla, nunca se
sabe si el próximo avance llega en 3 turnos o en 6. El reloj corre solo, no se puede detener,
y su ritmo es incierto a propósito.

## Cartas de descubrimiento

Se roba **sólo si la habitación quedó limpia** (sin enemigos) al terminar el turno del
personaje. Es la recompensa por haber limpiado, y es la otra mitad de la misma regla que hace
que los enemigos te ataquen si **no** limpiaste.

Todo turno termina en **premio o en paliza**. Nunca en nada.

## Fin de turno

Esta fase falta entera en el doc y es donde vive la presión del juego. Son **5 pasos, en este
orden**, y ninguno se saltea:

1. **Efectos de fin de turno de los personajes.** Si hay varios, el jugador elige el orden.
2. **Condiciones peligrosas.** Se tira 1 dado por cada token de peligro acumulado, todos
   juntos como una sola tirada. Después se descartan todos.
3. **Chequeo de reloj.** Si hay 3 o más símbolos en el descarte: el boss avanza y el descarte
   se remezcla. **Se chequea en cada turno, no una vez por ronda** — como la remezcla resetea
   el contador, chequear por ronda tira el excedente acumulado.
4. **Chequeo de manifestación del boss.**
5. **Efectos de fin de turno del boss**, resolviendo sus stages revelados en orden.

## Sobre los "Puntos de Locura"

El doc los marca con el nombre pendiente. Más importante que el nombre son tres reglas que
todavía no están escritas ahí:

- **Sólo suben. Nunca bajan.** Descansar no los toca, curarse no los toca, ningún ítem los
  toca.
- Al cruzar un umbral: se **ignora el exceso** (el marcador para justo en el umbral), se
  dispara la **carta de Locura** del personaje, y **sube 1 nivel de habilidad a elección**.
- En 4 de los umbrales se gana además **+1 dado bonus permanente**.

La carta de Locura **no es puro castigo**: es un condicional de dos ramas que castiga o
recompensa según dónde estés parado al cruzar el umbral. Eso hace que el umbral sea una
decisión posicional y no sólo un resultado de dados.

---

## Preguntas para el grupo (no pegar)

1. **Altares.** El doc los pone como objetivo de misión, y en este GDD no existen todavía.
   ¿Cuántos hay, cuánto cuesta destruir uno, y qué pasa cuando cae el último?
2. **Intercambiar ítems.** Aparece como cuarta acción y no hay sistema de ítems escrito.
   ¿Qué son los ítems?
3. **Descansar** dice "recupera 3 puntos repartibles entre vida o estrés". Confirmar que
   **no** toca Locura — si la tocara, se cae el pilar central del juego.
4. **"Moverse (entre 3 habitaciones)"** — confirmar que es *hasta* 3 habitaciones y no
   *exactamente* 3.
5. **Temática.** El doc va en fantasy; el registro de decisiones tiene biopunk como
   provisional ([D-14](gdd/06-decisiones/registro.md)). Hay que cerrarlo en un lado.
