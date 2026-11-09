# Plan de llamadas a la API de ClickUp

Generado por `export-clickup.py`. Objetivo: dejar el doc `2ky4j7vt-8099` 1:1 con el GDD
del repo, **con la menor cantidad de llamadas posible**.

| Total de llamadas de escritura | 19 |
|---|---|
| Updates (reusan pagina existente) | 13 |
| Creates (paginas nuevas) | 6 |

Es el minimo: una llamada por pagina. Reusar las 13 paginas existentes en vez de
crear un doc nuevo cuesta lo mismo y no deja el viejo tirado — no hay tool para
borrar docs ni paginas.

**Correrlas en tandas de 4 con pausa entre tandas.** El rate limit ya se agoto una
vez con 77 llamadas en 6 minutos, y el error que devuelve viene roto ("wait NaN
minutes"), asi que no informa cuanto falta.

## Updates — `clickup_update_document_page`

`document_id` = `2ky4j7vt-8099`, `content_format` = `text/md`, `content_edit_mode` = `replace`

| # | page_id | nombre nuevo | nombre viejo | fuente |
|---|---|---|---|---|
| 02 | `2ky4j7vt-9119` | 02. Concepto | 01. Que es el juego | `00-vision/concepto.md` |
| 04 | `2ky4j7vt-9219` | 04. Vocabulario neutro | 02. Vocabulario neutro | `01-fundamentos/vocabulario.md` |
| 05 | `2ky4j7vt-9279` | 05. Mapa y espacios | 03. El mapa es un grafo | `01-fundamentos/mapa-y-espacios.md` |
| 06 | `2ky4j7vt-9079` | 06. Barras y recursos | 04. El personaje | `02-personaje/barras-y-recursos.md` |
| 07 | `2ky4j7vt-9199` | 07. Trinquete | 05. El trinquete | `02-personaje/trinquete.md` |
| 08 | `2ky4j7vt-9039` | 08. Skills | 11. Skills | `02-personaje/skills.md` |
| 09 | `2ky4j7vt-9259` | 09. Dados | 06. Dados | `03-resolucion/dados.md` |
| 10 | `2ky4j7vt-9099` | 10. Turno | 07. Secuencia de turno | `03-resolucion/turno.md` |
| 13 | `2ky4j7vt-9159` | 13. Reloj y avance | 08. El reloj | `04-oposicion/reloj-y-avance.md` |
| 14 | `2ky4j7vt-9179` | 14. Objetivo y adversario | 09. Objetivo y adversario | `04-oposicion/objetivo-y-adversario.md` |
| 15 | `2ky4j7vt-9239` | 15. Victoria y derrota | 10. Victoria y derrota | `05-partida/victoria-y-derrota.md` |
| 16 | `2ky4j7vt-9059` | 16. Contenido de una mision | 12. Contenido de una mision | `05-partida/contenido-de-una-mision.md` |
| 19 | `2ky4j7vt-9139` | 19. Decisiones abiertas | 13. Decisiones abiertas | `06-decisiones/abiertas.md` |

## Creates — `clickup_create_document_page`

`document_id` = `2ky4j7vt-8099`, `content_format` = `text/md`

| # | nombre | fuente |
|---|---|---|
| 01 | 01. Indice | `README.md` |
| 03 | 03. Alcance | `00-vision/alcance.md` |
| 11 | 11. Vocabulario de efectos | `03-resolucion/vocabulario-de-efectos.md` |
| 12 | 12. Enemigos | `04-oposicion/enemigos.md` |
| 17 | 17. Perillas y constantes | `07-balance/perillas-y-constantes.md` |
| 18 | 18. Registro de decisiones | `06-decisiones/registro.md` |

## Por que los nombres van numerados

ClickUp muestra las paginas en orden de creacion y no hay tool para reordenarlas.
Como 13 paginas se reusan en su orden viejo y 6 se crean al final, el orden
visual no va a coincidir con el logico. El numero en el nombre es lo que deja al
lector seguir la secuencia igual.
