# 🗺️ El mapa y los espacios

← [Índice](../README.md)

## El mapa es un grafo, no una grilla

Esta es la decisión de implementación más importante del mapa, y equivocarla es caro.

El manual original define los espacios **por paredes y pasajes**, y dice explícitamente que
un espacio es adyacente a otro si es posible moverse entre ellos. No hay coordenadas, no hay
casillas cuadradas, no hay vecindad implícita por posición.

Entonces el mapa es un **grafo de nodos `Space`**:

- Un **nodo** es un `Space`.
- Una **arista** es un pasaje transitable entre dos `Space`.
- Una **pared** es la ausencia de arista. No se modela como objeto; simplemente no hay
  conexión.

Una grilla obligaría a inventar reglas para todo lo que sigue. Un grafo lo resuelve gratis.

## Salas

Las **salas** son la unidad de autoría y de streaming, no la unidad de juego. Una sala
contiene **1, 2 o 3 espacios** y expone puntos de conexión hacia otras salas.

En la implementación, cada sala es un **SubLevel**. Eso da tres cosas a la vez: composición
del mapa por armado, streaming para performance, y la posibilidad de que varias personas
trabajen en salas distintas sin pisarse los archivos.

## Adyacencia y distancia

**Adyacente** = existe una arista directa entre dos espacios.

**Distancia** = cantidad de aristas del **camino más corto**. Es un BFS sobre el grafo, y
lo usan un montón de reglas: "el enemigo más cercano", "movete N espacios hacia X",
"el objeto más lejano".

Reglas de distancia que salen del manual y hay que respetar:

- La distancia de un espacio a sí mismo es **0**. Si algo está en tu espacio, es lo más
  cercano que existe.
- Los **pasajes bloqueados** cuentan: el camino no puede atravesarlos.
- **Excepción:** si la figura que se va a mover puede ignorar un bloqueo, entonces ese
  bloqueo tampoco cuenta al calcular quién es "el más cercano" para ella.
- Si hay **empate** entre varios candidatos a la misma distancia, elige el jugador.

> Ese último punto aparece decenas de veces en el manual y es un costo real de la
> adaptación: cada empate es o un prompt al jugador (tedioso) o una regla automática
> (pierde agencia). Está anotado como decisión abierta.

## Aristas especiales

**Escaleras y túneles.** Conectan dos espacios lejanos y los vuelven adyacentes **para
todo efecto**, incluido el cálculo de distancias. En el grafo son simplemente una arista
más. Si el token que las genera se remueve, la arista desaparece.

**Paredes destructibles.** Romper una **agrega una arista permanente** al grafo. Eso cambia
de golpe todas las distancias, las rutas de escape y quién es el enemigo más cercano.

Esto convierte la destrucción en **una mecánica, no en un efecto visual**. Quién puede
romper qué sigue abierto: candidatos son el adversario al cambiar de stage, una criatura
pesada, o una de las 2 acciones propias de la misión.

## Ocupación

**No hay límite de figuras por espacio.** Un espacio puede tener los 4 personajes, seis
enemigos y el adversario a la vez. Eso es deliberado: es lo que hace posible que un espacio
se vuelva una trampa mortal.

## Estado `Clear`

Un espacio está **`Clear`** cuando no hay enemigos en él.

Es un estado calculado, no un flag: se evalúa en el momento en que una regla lo pregunta.
Dos acciones del juego dependen de él —`Recover` y el referéndum de fin de turno— así que
se consulta muchas veces por turno.

## Puertas

El mapa tiene **3 `Gate`**, cada una identificada por color. Son los puntos donde las cartas
de presión invocan enemigos.

Su ubicación es fija en el mapa y conocida por el jugador. Eso es información con la que se
juega: se sabe de dónde va a venir el problema, aunque no cuándo.

## Superficie y subterráneo

El mapa tiene **dos secciones**: una exterior donde arranca la partida y una subterránea
donde está el objetivo.

Es una división de producción además de una de ficción: la superficie se construye con
Landscape y el subterráneo con meshes, y la iluminación de las dos cambia a medida que el
adversario avanza en su reloj.

Las dos temáticas candidatas son sobre descender, así que la estructura funciona para
cualquiera de las dos.

## Resumen para implementar

| Concepto | Estructura |
|---|---|
| `Space` | Nodo del grafo |
| Pasaje | Arista bidireccional |
| Pared | Ausencia de arista |
| Sala | Subgrafo + SubLevel |
| Escalera / túnel | Arista extra, removible |
| Pared destructible | Arista que se agrega en runtime |
| Distancia | BFS respetando bloqueos |
| `Clear` | Consulta, no estado guardado |
| `Gate` | 3 nodos marcados, posición fija |
