# Terminar el HUD a mano

Tres cosas que el MCP no puede hacer y hay que hacer en el editor. Están en orden: cada una se
puede probar antes de pasar a la siguiente.

Lo que ya está hecho y no hay que tocar:

- `WBP_MissionHUD` existe en `/Game/Project_C/UI/`, con `TxtPhase`, `TxtActions` y `TxtRefusal`.
- `ShowRefusal(Reason)` ya escribe en `TxtRefusal`.
- `BP_PlayerController_Mission` ya lo crea en `BeginPlay`, lo guarda en `MissionHud` y lo pone en
  pantalla.
- `AMissionPlayerController::OnOrderRefused` es un dispatcher de C++ que dispara con dos motivos
  distintos: *"Too far"* y *"No actions left"*.

---

## 1. El cuerpo de `Refresh()`

Abrí `WBP_MissionHUD` → panel **My Blueprint** → doble click en **Refresh**.

Seis nodos:

```
  ┌──────────────┐   ┌────────────────────────┐   ┌──────────────────┐
  │ Get Game Mode│──▶│ Cast To                │──▶│ ActionsRemaining │
  └──────────────┘   │ BP_GameMode_Mission    │   └────────┬─────────┘
                     └────────────────────────┘            │
                                                           ▼
                                                  ┌──────────────┐
                                                  │  To Text (int)│
                                                  └───────┬──────┘
                                                          ▼
                                            ┌──────────────────────┐
                                            │ Set Text             │
                                            │ target: Txt Actions  │
                                            └──────────────────────┘
```

Paso a paso:

1. Click derecho en el grafo → `Get Game Mode`.
2. Arrastrá desde su pin **Return Value** → `Cast To BP_GameMode_Mission`.
   Conectá el exec del **nodo de entrada de la función** a la entrada del Cast.
3. Arrastrá desde **As BP Game Mode Mission** → `Get Actions Remaining`.
4. Arrastrá desde ese pin → `To Text (Integer)`.
5. Del panel **Variables**, arrastrá **Txt Actions** al grafo como *Get*.
6. Arrastrá desde **Txt Actions** → `Set Text`. Conectá el `To Text` a su pin **In Text**, y el
   exec de la salida del Cast a su entrada.

**Por qué lo hacés vos y no yo:** casting, variables y conversión de tipos son tres temas de la
clase 2 que `course-alignment.md` marca con **cero nodos** en todo el proyecto, y el primer parcial
(24/9) cubre clases 1 a 7. No es un parche: es la parte del HUD que te suma.

Si además querés la fase, agregá en la misma función: desde el Cast → `Get Phase` → conectalo a un
`Set Text` con target **Txt Phase**. `Phase` es un enum, así que el pin de texto va a necesitar un
nodo de conversión que Unreal ofrece solo al arrastrar.

---

## 2. Los tres cables

En el **Event Graph** de `WBP_MissionHUD`, colgando de `Event Construct`.

| Qué escuchar | Dónde está | Qué llamar |
|---|---|---|
| `OnPhaseChanged` | `BP_GameMode_Mission` | `Refresh` |
| `OnFigureMoved` | `BP_GameMode_Mission` | `Refresh` |
| `OnOrderRefused` | `BP_PlayerController_Mission` | `ShowRefusal` |

Para cada uno:

1. Conseguí el objeto: `Get Game Mode` + `Cast To BP_GameMode_Mission` para los dos primeros;
   `Get Owning Player` + `Cast To BP_PlayerController_Mission` para el tercero.
2. Arrastrá desde el pin del objeto y buscá **Bind Event to OnPhaseChanged**. Unreal crea el nodo
   de bind con un pin rojo **Event**.
3. Arrastrá desde ese pin rojo → **Add Custom Event**. Ponele un nombre.
4. Del custom event, llamá a `Refresh` (o a `ShowRefusal`, pasándole el `Reason` que llega).

Al final de la cadena, llamá a `Refresh` una vez más para que el HUD arranque con valores en vez
de vacío.

**Este es el tema que el MCP no puede escribir:** el DSL de grafos no tiene forma de expresar un
bind de dispatcher.

---

## 3. El diseño

Ninguna propiedad del árbol de widgets acepta escritura por MCP — probé número, booleano, texto y
tooltip, y los cuatro fallan. Todo esto es a mano, en la pestaña **Designer**.

| Widget | Qué hacer |
|---|---|
| `TurnBox` (el Vertical Box) | anclarlo arriba a la izquierda, con ~32 de margen |
| `TxtPhase`, `TxtActions` | tamaño de fuente ~20, color claro |
| `TxtRefusal` | anclarlo abajo al centro, alineación 0.5 / 1, fuente ~24 |

---

## Cómo saber que funciona

En PIE:

1. Seleccioná una figura y movela → **Actions** tiene que bajar de 3 a 2.
2. Poné la figura en el espacio **8** y clickeá el **0** o el **2** → tiene que aparecer
   *"Too far: that space is more than one move away."*
   Esos dos están a 4 pasos del 8 contra un presupuesto de 3. Es la regla funcionando, y hasta
   ahora era muda: por eso el 8 parecía roto.
3. Barra espaciadora → **Actions** vuelve a 3.

El paso 3 es el que hoy no se puede comprobar de otra forma. La tecla está bien conectada —el
mapeo, el asset y el binding están verificados— pero como el turno también termina solo al gastar
la última acción, apretarla recarga de 3 a 3 y no se nota.
