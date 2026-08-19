# Enemigos

← [Índice](../README.md)

## Taxonomía

Tres categorías, y la distinción importa porque muchos efectos apuntan a una sola.

| ID | Qué es |
|---|---|
| `Servant` | Enemigo humano, ligado al objetivo. Pool limitado a **10** |
| `Creature` | Todo enemigo que no sea `Servant` ni el `Adversary` |
| `Adversary` | El jefe. Solo cuenta como enemigo una vez manifestado |

La palabra paraguas es **enemigo**: cubre las tres. Un efecto que dice "enemigos" toca a
las tres; uno que dice `Creature` no toca `Servant`.

## Datos de cada enemigo

Cada tipo tiene:

- **Vida** — cuántas heridas aguanta antes de morir
- **Dados de ataque** — cuántos estándar y cuántos bonus tira
- **Habilidad especial** — condicional, se dispara en cierta circunstancia
- **Tamaño del pool** — cuántas figuras hay disponibles en total

Las que no están en el mapa quedan en un pool, listas para spawnear.

## Comportamiento

Los enemigos de este juego **no piensan**. Su comportamiento entero son tres reglas:

**1. Te siguen.** Cuando un personaje sale de un espacio, **todos** los enemigos que
estaban ahí se mueven con él. Todos, aunque queden otros personajes en el espacio original.

**2. Te atacan.** En la fase de referéndum, si comparten espacio con el personaje activo,
cada uno lo ataca una vez.

**3. Los mueven las cartas.** Cualquier reposicionamiento que no sea "seguir" viene de un
efecto de carta, que los mueve por el camino más corto hacia un objetivo.

Eso es todo. No patrullan, no buscan, no coordinan.

### Por qué eso es una buena noticia

**Las reglas base no necesitan IA.** Se pueden implementar con lógica directa, y funcionan
igual de bien que con un sistema complejo, porque el diseño no pide más.

La IA real —NavMesh, Behaviour Tree, EQS, Percepción— entra después, reemplazando ese
scripting sin cambiar ninguna regla. Eso permite tener el juego jugable antes y agregar la
capa de IA como una mejora medible, no como un requisito de arranque.

## La regla de "te siguen"

Es la regla espacial más importante del juego, y merece atención.

**No hay desengage.** No podés soltar a un enemigo moviéndote. Si entrás a un espacio con
tres criaturas, esas tres son tuyas hasta que las mates o uses `Stealth`.

Las consecuencias en cadena:

- Un espacio con enemigos es un espacio donde no podés `Recover` ni robar recompensa.
- Moverte no te salva: te llevás el problema puesto.
- Juntar enemigos es fácil y desjuntarlos es caro. Un personaje puede terminar arrastrando
  media pantalla.
- Y como al moverte también agarrás `Hazard`, escapar cuesta doble.

Por eso `Stealth` —la única skill que te deja dejar enemigos atrás— es más valiosa de lo
que parece a primera vista.

## Spawn

Los enemigos aparecen en las **3 `Gate`** del mapa, invocados por la tercera parte de las
cartas de presión.

Reglas:

- La carta dice **qué `Gate`** y **qué enemigo**.
- Si **no hay figura disponible** en el pool, ese spawn **se saltea**. No se sustituye por
  otra cosa.
- Si hay varios spawns en una carta, se hacen **en orden**.
- Si la carta invoca varios sin especificar orden, elige el jugador. Eso importa justo
  cuando no alcanzan las figuras.

## Límites de pool

En el juego de mesa los componentes son finitos, y eso funciona como un **tope blando de
dificultad**: cuando ya hay 10 `Servant` en el tablero, no pueden aparecer más.

**En digital ese límite deja de ser físico.** Se puede replicar, subir o sacar. Sacarlo hace
el juego más duro, y hay que re-balancear.

Qué reemplaza ese límite sigue abierto.
→ [`06-decisiones/abiertas.md`](../06-decisiones/abiertas.md)

## Muerte

Cuando las heridas acumuladas igualan o superan la vida, el enemigo muere y la figura
**vuelve al pool**, disponible para spawnear de nuevo.

El daño sobrante **no se transfiere** a otro enemigo, salvo con `Brawling` de nivel
suficiente.
