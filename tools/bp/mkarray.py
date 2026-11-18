"""Completa un MakeArray creado por DSL con un solo elemento: agrega pines y getters.
fill(graph, makearray_node, ["Variables|WBP_GameHUD|GetDie1", ...])"""
from at import call
from surgery import Graph
B="editor_toolset.toolsets.blueprint.BlueprintTools"
def fill(G, node, getters):
    g=Graph(G); k=node if ":" in node else G+"."+node
    for n,t in enumerate(getters, start=1):
        call(B,"add_node_pin",node={"refPath":k})
        v=g.create(t)
        g.link(v, g.info(v)["output_pins"][0]["name"], k, f"[{n}]")
