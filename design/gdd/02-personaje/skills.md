# Skills

← [Índice](../README.md)

## Estructura

Cada `Character` tiene **3 skills**:

- La primera es su **skill de firma**, única de ese personaje.
- Las otras **2 salen de un pool común de 6**, así que dos personajes pueden compartirlas.

Cada skill tiene **4 niveles**. Todos arrancan en nivel 1.

**Se sube exactamente de una forma: cruzando un umbral de
[`Ratchet`](trinquete.md).** Cada umbral da 1 nivel, a elección del jugador.

## Cómo suben

Hay dos comportamientos distintos, y la diferencia importa para la estrategia:

- Algunos niveles **se suman** al anterior: el personaje conserva las dos habilidades.
- Otros **reemplazan** al anterior: la habilidad de nivel 2 desaparece al llegar a nivel 3.

Cuál es cuál está escrito en cada skill. La consecuencia de diseño es que **subir no siempre
es mejorar**: hay ramas donde conviene quedarse en un nivel intermedio, y eso hace que la
decisión del umbral no sea automática.

Un `Ally` también puede dar un nivel de skill mientras esté con el personaje. Si el
personaje no tenía esa skill, la gana en nivel 1; si ya la tenía, sube 1. Al perder el
`Ally`, baja de vuelta.

## Los 6 skills comunes

| ID | Qué hace | Eje |
|---|---|---|
| `Insight` | Convierte caras `Wild`, que son inertes, en éxitos | Economía de dados |
| `Brawling` | Pega mejor y a más enemigos dentro de tu espacio | Combate cuerpo a cuerpo |
| `Marksman` | Ataca a enemigos que no están en tu espacio | Combate a distancia |
| `Stealth` | Evita que los enemigos te sigan al moverte | Movilidad |
| `Swiftness` | Más movimiento y más acciones por turno | Economía de acciones |
| `Toughness` | Reduce daño recibido y pérdida de `Ratchet` | Defensa |

## Las 24 celdas

**6 skills × 4 niveles = 24 efectos distintos.** Esa es la superficie de maestría real del
juego: es donde el jugador aprende, compara y arma su build.

**El detalle exacto de las 24 celdas ya está transcripto** en
`docs/rulebook/rulebook.md`, páginas 24 y 25. No se duplica acá a propósito: si estuviera en
dos lugares, en algún momento van a divergir. Ese archivo es la fuente.

## Cómo se relaciona cada skill con el resto del juego

**`Insight`** es el único que le da valor a la cara `Wild`, que sin él no hace nada. Es la
skill que cambia la economía de los dados en vez de los números.

**`Brawling`** y **`Marksman`** son las dos respuestas opuestas al mismo problema: los
enemigos te siguen y te atacan si comparten tu espacio. `Brawling` acepta el abrazo y lo
convierte en ventaja; `Marksman` te deja no entrar nunca.

**`Stealth`** es la contra directa de la regla de enemigos pegajosos. En la implementación,
es la que se resuelve con AI Perception.

**`Swiftness`** compra acciones, que es el recurso más escaso del turno.

**`Toughness`** es la única que reduce pérdida de `Ratchet` desde afuera. Eso la hace
extraña: es la skill que te frena la progresión a cambio de mantenerte vivo.

Ese último punto es el más interesante del set y conviene tenerlo presente al balancear: en
un juego donde perder `Ratchet` es cómo te volvés fuerte, una skill que lo previene está
comprando supervivencia con poder futuro.
