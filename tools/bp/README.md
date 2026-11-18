# Herramientas para Blueprints

Scripts para editar Blueprints desde afuera del editor. Necesitan el editor abierto con el servidor MCP en `127.0.0.1:8000`. Corren con Python 3 desde esta carpeta.

## Qué hace cada uno

| Script | Para qué |
|---|---|
| `runpy.py <script.py>` | Corre un script dentro del editor (consola `Cmd`, comando `py`). El script recibe `SP` (esta carpeta) y escribe su resultado en `SP + "pyout.txt"`. |
| `apply.py` | Script de editor. Lee `plan.json` y crea o edita cajas de comentario, después compila y guarda. Se corre con `python runpy.py apply.py 30`. |
| `cgroups.py` | Arma el `plan.json` de comentarios: una caja por grupo de nodos exec, más sus nodos puros. |
| `tidy.py <grafo>` | Ordena un grafo: ejecución en fila recta, ramas cortas abajo, nodos puros debajo de quien los usa y reroutes en los cables largos. |
| `bpslim.py <grafo>` | Borra los nodos muertos que deja `write_graph_dsl`, pasa los literales a valores de pin y une nodos puros duplicados (menos los aleatorios). |
| `mkarray.py` | `fill(grafo, nodo, getters)`: completa un Make Array de un solo elemento, porque el DSL crea `MakeArray` con un solo pin. |
| `gopen.py <BP> <Función>` | Abre la función en el editor, pasando a la pestaña Graph si es un widget, y guarda el ref del panel en `panel.txt` para `gsee.py`. |
| `prune.py <grafo>` | Solo borra los nodos muertos. |
| `fixreturns.py <grafo>` | Cambia los marcadores `SetRetTmp` por Return Nodes reales, porque la DSL no puede crearlos en funciones nuevas. |
| `hug.py <grafo>` | Pega cada nodo puro a la izquierda del pin que alimenta, sin mover nodos con exec. |
| `surgery.py` | Clase `Graph` para cirugía de nodos: borrar, crear y conectar por nombre de pin. |
| `gshot.py <BP> <Función> out.png` | Abre una función en el editor y saca una captura. |
| `gsee.py out.png` | Encuadra el grafo abierto (Home) y captura el panel. |
| `execcheck.py` | Lista nodos que se usan pero no tienen el exec conectado. |

`ue.py`, `at.py`, `t.py`, `sl.py` y `capnow.py` son el cliente MCP y la consola que usan los demás.

## Comentarios en un BP

En el editor de Python, `unreal.BlueprintGraphEditor` crea los comentarios:

```python
g  = unreal.load_object(None, "/Game/.../BP_X.BP_X:Funcion")
ed = unreal.BlueprintGraphEditor.get_graph_editor(g)
c  = ed.add_comment_node("Texto", unreal.Vector2D(x, y), unreal.Vector2D(w, h))
c.set_comment_color(unreal.LinearColor(0.15, 0.15, 0.15, 0.5)); c.set_editor_property("font_size", 18)
```

Los comentarios van en español.
