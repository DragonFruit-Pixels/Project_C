# 🎣 High Concept

← [Índice](../README.md)

> Documento de venta: qué es el juego, para quién, y por qué alguien lo jugaría.
>
> **Sobre el vocabulario.** El resto del GDD escribe en IDs neutros en inglés (`Ratchet`,
> `Adversary`, `Space`…) por la regla del [índice](../README.md). Este documento es la
> **excepción declarada**: se lee en castellano —Locura, Jefe, Sala— porque lo lee gente que
> ve el proyecto por primera vez. La equivalencia con el ID de código está en
> [Capa temática](#capa-temática), y esa tabla es la que hace que la excepción no cueste nada.
>
> El desarrollo del concepto está en [`concepto.md`](concepto.md) y el recorte en
> [`alcance.md`](alcance.md). Este documento no los reemplaza: los resume.

## Resumen

**Un táctico por turnos para un jugador donde tus personajes solo se vuelven fuertes
recibiendo daño en una barra que nunca se cura, y donde llenarla los mata.**

## Ficha

| | |
|---|---|
| **Género** | Táctico por turnos con dados, escenario único rejugable |
| **Modo** | Single player. Controlás a los 4 personajes |
| **Duración de partida** | Una misión completa, ganable y perdible |
| **Experiencia buscada** | Desafío y maestría. No es narrativa |
| **Motor / plataforma** | Unreal Engine 5.8, PC |
| **Estado** | Framework, grafo del mapa y selección en C++ con 6 tests automatizados. `L_Mission_01` ya se juega: cámara, selección con el mouse y movimiento por el grafo gastando acciones |

## Pilares de diseño

Cuatro. Cualquier decisión de diseño se valida contra estos; si una mecánica nueva no
sostiene ninguno, sobra.

**1. Toda ganancia se paga con algo que no se devuelve.**
La Locura solo sube. Ni descansar, ni curarse, ni un objeto la bajan. El poder y la muerte
salen del mismo lugar. → [`trinquete.md`](../02-personaje/trinquete.md)

**2. Actuar es intrínsecamente caro, y no actuar es una jugada legítima.**
La cara 🦑 del dado suma Locura, y cobra en **toda** tirada: atacando, defendiéndote o
tirando por un efecto. Por eso un turno en el que te movés a una sala limpia y descansás es
un turno sin dados, y eso vale. → [`dados.md`](../03-resolucion/dados.md)

**3. El reloj no se detiene y su ritmo es incierto.**
El Jefe avanza cuando se acumulan símbolos en el descarte — y al avanzar, el descarte se
remezcla. Nunca sabés si el próximo avance viene en 3 turnos o en 6.
→ [`reloj-y-avance.md`](../04-oposicion/reloj-y-avance.md)

**4. El espacio es un recurso escaso.**
Los enemigos te persiguen: al salir de una sala, todos los que había te siguen. Es difícil
esquivar los conflictos, y eso convierte al movimiento en la decisión más cara del juego.
→ [`enemigos.md`](../04-oposicion/enemigos.md)

## Core loop

**El turno de un personaje son 4 fases y ninguna se saltea:**

1. **3 acciones** — moverse, atacar, descansar, y las acciones propias de la misión.
2. **Carta de presión** — se roba 1 y se resuelve de arriba hacia abajo.
3. **Resolución** — si la sala quedó limpia, cobrás recompensa; si quedaron enemigos, te
   atacan todos. El turno termina en premio o en paliza, nunca en nada.
4. **Fin de turno** — efectos, `Hazard`, chequeo de reloj, manifestación del Jefe y su
   turno. → [`turno.md`](../03-resolucion/turno.md)

Una **ronda** es un turno por personaje vivo. La **partida** son rondas hasta romper el
objetivo: ahí el Jefe se vuelve vulnerable y empieza la pelea final.

El bucle corto que el jugador siente es más chico que todo eso: **tirar dados sabiendo que
cada tirada te acerca al poder y a la muerte al mismo tiempo.**

## Qué lo hace distinto

Hay muchos tácticos por turnos y muchos juegos de dados. Lo raro es la combinación.

**El recurso de progresión es también el reloj de muerte del personaje.** La mayoría de los
juegos con progresión te dejan farmear o volver atrás. Acá cada punto es definitivo en las
dos direcciones, y el personaje que juega perfecto llega al final sin herramientas.

**Los dados cobran siempre.** Una de las cuatro caras te cuesta Locura aunque estés ganando.
Los dados bonus que ganás con los umbrales **no tienen cara 🦑**, así que el poder es
doblemente bueno: tirás más dados, y tirás dados que no pueden cobrarte.

## Referencias

| Referencia | Qué se toma |
|---|---|
| *Cthulhu: Death May Die* | El ADN mecánico. Es la base declarada, no una inspiración vaga |
| *XCOM* | Táctico por turnos y el lenguaje de cámara |

## Capa temática

**La piel es dato, no diseño.** Ninguna regla nombra un tema, así que cambiar de ambientación
no toca una línea de código.

La columna **ID en código** es lo que está escrito hoy en `Source/ProjectC/`. No es
burocracia: es lo que evita que este documento y el código se hablen de cosas distintas.

| Nombre en este doc | ID en código | Fantasy | Biopunk |
|---|---|---|---|
| Sala | `ASpace` | habitación | sector |
| Personaje | `AProjectCCharacter` | aventurero | operador |
| Locura | `URatchetComponent` / `Ratchet` | magia corrupta | contaminación |
| Símbolo (🦑) | `Toll` | corrupción | la dosis |
| Jefe | `Adversary` | el boss de la dungeon | la entidad emergente |
| Objetivo | `Objective` | altar a destruir | reactor a colapsar |
| Resolución | `EMissionTurnPhase::Reckoning` | resolución de la habitación | barrido del sector |
| Carta de presión | `Pressure Deck` | carta de Hex | carta de brote |

Está sin cerrar cuál piel se usa → [D-14](../06-decisiones/registro.md). El costo de
decidirlo tarde es bajo **a propósito**: es el motivo por el que el core se escribió neutro.

## Alcance

**1 misión y 1 Jefe.** No es un recorte arbitrario: es la unidad atómica del original, que se
juega combinando una caja de escenario con una de jefe.

Entra: los 4 personajes, 1 mapa, 1 Jefe con sus 4 stages, 1 objetivo, y **2 skills por
personaje** en lugar de 3.

> **Recorte deliberado, y provisional.** El manual trae 3 skills por personaje de 4 niveles
> cada una ([`skills.md`](../02-personaje/skills.md), págs. 22 y 24-25). Para esta versión se
> baja a **2 skills por personaje**. Qué 4 personajes entran y qué 2 skills lleva cada uno lo
> decide el diseñador a mano → [A-08](../06-decisiones/abiertas.md).
>
> **Sigue abierto cuántos niveles quedan** — ver A-08. Es lo que decide si sobran umbrales de
> Locura sin nada que dar.

**Fuera de alcance, y no por olvido:** multijugador, tiempo real, narrativa ramificada, y
progresión entre partidas que dé poder permanente.

Lo que hace que valga volver a jugar es **qué subís y en qué orden**. Por eso la Locura carga
doble función: es la tensión y es el motor de rejugabilidad. → [`alcance.md`](alcance.md)

## Riesgo principal del concepto

El pilar 1 exige que el jugador **quiera** recibir daño. Si en el playtest resulta que evitar
la Locura es la estrategia dominante, se cae el juego entero, no una mecánica.

Es la primera hipótesis a testear en cuanto haya algo jugable, y hoy es la razón real de las
cinco decisiones de balance que siguen abiertas.
→ [`abiertas.md`](../06-decisiones/abiertas.md)
