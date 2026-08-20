# 🔄 Secuencia de turno

← [Índice](../README.md)

> Este es el documento que más se va a consultar durante la implementación.

**Un turno por personaje.** Con 4 personajes, una ronda son 4 turnos completos, uno tras
otro. No hay fase simultánea.

Las cuatro fases van **siempre en este orden**, y ninguna se puede saltear:

```
1. Tres acciones
2. Carta de presión
3. Referéndum
4. Fin de turno
```

---

## Fase 1 — Tres acciones

El personaje activo tiene **3 acciones**. Puede repetir la misma.

Hay **4 acciones base**, más **2 propias de la misión** definidas por el escenario.

### `Move`

Movés hasta **3 espacios** por aristas del grafo.

Dos cosas pasan al salir de un espacio:

- **Todos los enemigos que había te siguen** al espacio nuevo. Todos, aunque haya otros
  personajes ahí.
- Si el espacio tenía `Hazard`, **te llevás uno por cada token**. Los del espacio quedan
  donde estaban; los tuyos se resuelven a fin de turno.

> Si un efecto te mueve fuera de una acción `Move`, aplica lo mismo: los enemigos siguen y
> agarrás `Hazard`. La única excepción son los efectos que **colocan** al personaje en otro
> espacio: ahí aparece directo, sin arrastrar nada.

### `Attack`

Elegís **1 enemigo en tu espacio** y hacés una tirada. Cada `Hit` es 1 herida.

Si las heridas igualan o superan la vida del enemigo, muere. El daño sobrante se pierde.

Sin la skill `Brawling`, el objetivo se elige **antes** de tirar, y todas las heridas van a
ese enemigo aunque sobren.

El adversario solo se puede atacar después de romper el objetivo, incluso si ya está en el
mapa. → [`04-oposicion/objetivo-y-adversario.md`](../04-oposicion/objetivo-y-adversario.md)

### `Recover`

**Solo en un espacio `Clear`.** Curás **3 puntos totales** repartidos entre `Wounds` y
`Reserve` como quieras. Se puede repetir en el mismo turno.

**`Ratchet` no se cura.** Nunca.

### `Trade`

Intercambiás objetos con los personajes que estén en tu espacio. Los `Burden` no se
intercambian.

### Acciones libres y especiales

Algunos efectos son **acciones libres**: se usan en esta fase sin gastar acción.

Otros son **acciones especiales**: se usan en esta fase y **sí** gastan una de las 3.

Las dos solo se pueden usar durante la fase 1.

---

## Fase 2 — Carta de presión

Se roba **1 carta** del `Pressure Deck` y se resuelve **de arriba hacia abajo**. Después va
a una pila de descarte **boca arriba**.

Las cartas tienen una **estructura fija de tres partes**:

**1. `Doom Symbol`.** No hace nada al robarse. Se cuenta a fin de turno. Si la carta lo
tiene, al descartarla **tiene que quedar visible**.

**2. Efecto especial.** Mover enemigos, dañar, forzar tiradas, lo que sea. Salvo que diga
otra cosa, **afecta solo al personaje activo** e ignora a los demás. Toda decisión que haya
que tomar la toma el jugador activo.

Cuando un efecto mueve un enemigo hacia algo, se mueve por el **camino más corto**, todo lo
que le permita el efecto, y **para al llegar**.

**3. Spawn.** Indica una `Gate` y qué enemigo aparece ahí. Si hay varios, se hacen en orden.

### La regla de ejecución

**Un paso que no se puede ejecutar se saltea, no falla.**

Si no hay figuras disponibles para invocar, se saltea ese spawn. Si no hay enemigos del tipo
que la carta quiere mover, se saltea ese movimiento. La carta sigue resolviéndose.

Esta semántica vale para todo el juego y hay que implementarla una sola vez.

---

## Fase 3 — Referéndum

La fase más corta y la que más define el juego. Depende de una sola pregunta: **¿tu espacio
está `Clear`?**

| Estado del espacio | Qué pasa |
|---|---|
| `Clear` | Robás una carta del `Reward Deck` |
| Con enemigos | **Todos te atacan**, uno por uno, en el orden que elijas |

**No hay punto medio.** Cada turno termina en premio o en paliza, y lo decide únicamente si
limpiaste tu espacio.

Detalles:

- Investigar es **obligatorio** si estás limpio, aunque no quieras.
- Los enemigos te atacan **solo a vos**, ignoran a los otros personajes.
- Si un efecto mete un enemigo en tu espacio **durante** esta fase, ese enemigo también
  ataca.
- Si un enemigo se va durante la fase, deja de atacar.
- Si el espacio queda limpio a mitad de la fase, los enemigos dejan de atacar, **pero no
  podés investigar**: lo que cuenta es cómo estaba al empezar la fase.

---

## Fase 4 — Fin de turno

Cinco pasos, en orden.

**1. Efectos de fin de turno.** Todos los que haya, menos los del adversario. El jugador
elige el orden.

**2. `Hazard`.** Tirás **1 dado estándar por cada token** que tengas encima, como una sola
tirada. Podés gastar `Reserve` para rerollear. Cada `Hit` es 1 herida, cada `Toll` es +1
`Ratchet`. Después **se descartan todos** los tokens.

> Los enemigos no agarran `Hazard` al moverse. Solo los personajes.

**3. Chequear el descarte.** Si hay **3 o más `Doom Symbol`** en la pila:

- El adversario **avanza** en el `Doom Track`
- Se resuelven los efectos del avance
- **Todo el descarte se remezcla** de vuelta en el mazo

→ [`04-oposicion/reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md)

**4. Chequear la manifestación** del adversario.

**5. Efectos de fin de turno del adversario**, resolviendo los stages revelados en orden.

El turno terminó. Pasa al siguiente personaje.

---

## Notas de implementación

**El descarte tiene que ser visible.** En la mesa, ver la pila crecer es información pública
y gratis: es lo que genera la tensión de la fase 4. En pantalla, si no se construye esa UI,
**se pierde el mecanismo de presión entero**.

**Los empates los decide el jugador.** El manual repite decenas de veces que ante varias
formas de resolver algo, elige el jugador activo. Cada una de esas es o un prompt (tedioso)
o una regla automática (pierde agencia). Cuántas se automatizan sigue abierto.

**El orden de las fases es rígido.** No hay atajos, no hay fases opcionales, no hay
reacciones fuera de turno. Eso hace que la máquina de estados sea chica y verificable.
