# Contenido de una misión

← [Índice](../README.md)

Inventario de lo que hay que producir para tener **una partida completa**. Sirve como
checklist de producción y como estimación de esfuerzo.

## Contado

| Elemento | Cantidad | Origen |
|---|---|---|
| Cartas de presión (`Pressure Deck`) | **16** — 8 de la misión + 8 del adversario | Diseño propio |
| Cartas de recompensa (`Reward Deck`) | **15** | Diseño propio |
| Cartas de trinquete (`Ratchet Card`) | **8**, se reparte 1 por personaje | Diseño propio |
| Stages del `Adversary` | **4** | Diseño propio |
| Acciones propias de la misión | **2** | Diseño propio |
| `Gate` en el mapa | **3** | Layout |
| Pool máximo de `Servant` | **10** | Regla |
| Personajes jugables | **4** | Diseño propio |
| Skills comunes | **6**, de 4 niveles cada uno = **24 efectos** | **Ya especificado** |
| Espacios del `Doom Track` | **8** | Regla |

## Lo que NO hay que diseñar

**Las 24 celdas de skills ya están escritas.** Están en `docs/rulebook/rulebook.md`, páginas
24 y 25, transcriptas del manual original. Es el sistema más profundo del juego y viene
resuelto.

Toda la estructura de reglas —secuencia de turno, resolución de dados, distancias,
condiciones de fin— también está transcripta y verificada. Lo que se diseña es **contenido**,
no sistemas.

## Lo que sí hay que diseñar de cero

Ordenado por cuánto define el juego:

### El objetivo de la misión

Qué hay que hacer para romperlo, y cómo se hace. Es lo que da forma a la primera mitad de
toda partida. Junto con las **2 acciones propias**, que existen para servirlo.

Es la pieza más importante y la que menos ayuda tiene: el resto del contenido se puede
derivar del manual, esto no.

### El adversario

Sus 4 stages, con vida, efecto de revelación, efecto permanente y dados. Más su recurso
especial y sus minions.

Con un solo adversario, **esto es la identidad del juego**.
→ [`../04-oposicion/objetivo-y-adversario.md`](../04-oposicion/objetivo-y-adversario.md)

### Las 16 cartas de presión

Tienen [estructura fija de tres partes](../03-resolucion/turno.md), así que se pueden
componer a partir de un vocabulario chico de efectos reutilizables en vez de escribirse una
por una a mano.

**Esa es la clave de la escalabilidad**: si el vocabulario existe, la carta 17 y la 200
cuestan lo mismo que la 5. Si no, cada carta es código a medida.

El vocabulario **no se diseña de entrada**: se implementan las primeras 5 o 6 cartas a mano,
se mira qué se repite, y se extrae de ahí. Así se paga la abstracción con evidencia en vez
de con adivinanza.

### Las 15 cartas de recompensa

Objetos, `Ally` y `Burden`. Cada una con dos lados, de los que el personaje reclama uno.

### Las 8 cartas de trinquete

Una por personaje, del mazo. Se dispara **cada vez** que ese personaje cruza un umbral, así
que un efecto que parece chico se vuelve grande en un personaje muy avanzado. Diseñarlas
pensando en que se van a disparar 4 a 6 veces.

### Los 4 personajes

Cada uno con su **skill de firma** única, más 2 del pool común. La skill de firma es lo que
los diferencia.

### El mapa

Un layout con sección de superficie y sección subterránea, armado por salas de 1 a 3
espacios, con 3 `Gate` y las conexiones especiales que haga falta.

### Los tipos de enemigo

Vida, dados de ataque, habilidad especial y tamaño de pool para cada uno.

## Nota sobre el orden

El vocabulario neutro de este GDD alcanza para diseñar **todo** lo de arriba sin haber
decidido la temática. Los nombres, el arte y el texto de sabor se aplican encima al final,
a partir del glosario de la variante elegida.

Diseñar el contenido antes de elegir temática no es un problema — es el orden correcto.
