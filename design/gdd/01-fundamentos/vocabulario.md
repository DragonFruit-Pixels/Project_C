# Vocabulario neutro

← [Índice](../README.md)

## Por qué existe

La temática del juego **no está decidida**. Hay dos variantes completas y verificadas en
`docs/rulebook/`, y puede aparecer una tercera.

Si el core del juego usara los nombres de una de ellas, cambiar de temática dejaría de ser
una decisión de arte y pasaría a ser una refactorización. Por eso **el core habla en IDs
neutros y la temática es data**.

Esto ya está probado, no es una hipótesis de diseño. Los dos glosarios temáticos demuestran
que el mismo documento mecánico soporta vocabularios completamente distintos **sin cambiar
una sola regla**, y la equivalencia se verificó línea por línea.

## Cómo se usa

Los nombres de abajo son **identificadores**, no texto de jugador. El texto que ve el
jugador sale de un theme pack: una tabla que mapea cada ID a su nombre y su descripción en
la temática elegida. Los glosarios de `docs/rulebook/` son exactamente la especificación de
ese theme pack.

En código, en Data Tables y en estos documentos se usa siempre el ID.

## La tabla

| Concepto | ID neutro | En el manual original |
|---|---|---|
| Personaje jugable | `Character` | Investigator |
| El jefe | `Adversary` | Elder One |
| Enemigo humano, ligado al objetivo | `Servant` | Cultist |
| Todo otro enemigo | `Creature` | Monster |
| Barra que no se cura y da poder | `Ratchet` | Sanity |
| Moneda de reroll | `Reserve` | Stress |
| Daño | `Wounds` | Wounds |
| Nodo del mapa | `Space` | Space |
| Punto de spawn de enemigos | `Gate` | Gate |
| Reloj de fatalidad | `Doom Track` | Summoning track |
| Símbolo que hace avanzar el reloj | `Doom Symbol` | Elder One Summoning |
| Mazo de amenazas | `Pressure Deck` | Mythos deck |
| Mazo de recompensas | `Reward Deck` | Discovery deck |
| Carta que dispara en cada umbral | `Ratchet Card` | Insanity card |
| Lo que hay que romper | `Objective` | Ritual |
| Cara de éxito | `Hit` | Success |
| Cara inerte, sirve solo con skill | `Wild` | Elder Sign |
| Cara que cuesta `Ratchet` | `Toll` | Tentacle |
| Token que daña a fin de turno | `Hazard` | Fire |
| Espacio sin enemigos | `Clear` | Safe space |
| Acción de curarse | `Recover` | Rest |
| Objeto que acompaña y absorbe daño | `Ally` | Companion |
| Carta negativa persistente | `Burden` | Condition |
| Objeto con habilidad propia | `Trinket` | Relic |

## Los que se dejaron quietos

`Wounds`, `Space`, `Gate` y `Skill` no se renombraron porque ya son genéricos: no arrastran
temática de ningún lado y cualquier reemplazo sería peor.

## Reglas de nomenclatura

- **Un ID nuevo se agrega acá primero**, y recién después se usa en otro documento o en
  código. Si aparece un nombre que no está en esta tabla, es un bug de proceso.
- **Nada de nombres temáticos en el core.** Ni de las variantes existentes ni inventados.
- **Los IDs no se traducen.** Van en inglés incluso en documentos en español, porque son
  los mismos nombres que van a aparecer en Blueprints, Data Tables y Gameplay Tags.
- **El nombre describe la función, no la ficción.** `Ratchet` se llama así porque es un
  trinquete: solo gira para un lado. Ese nombre sigue siendo correcto sea cual sea la
  temática que se elija.

## Las variantes temáticas

Existen y están terminadas, pero **ninguna está elegida**:

| Variante | Glosario | Términos |
|---|---|---|
| A | `docs/rulebook/glossary-biopunk.md` | 66 |
| B | `docs/rulebook/glossary-dnd.md` | 67 |

Las dos vienen con su manual completo aplicado y verificado. Cuando se decida la temática,
el trabajo es mapear esta tabla contra el glosario elegido — no rediseñar nada.
