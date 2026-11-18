"""Cambia cada marcador (Variables|Default|SetRetTmp X) por un Return Node real con X en su salida Valid.
La DSL no puede crear Return Nodes en esta versión: busca |AddReturnNode... y el tipo es ||AddReturnNode...
Antes: un local bool RetTmp, la DSL escrita con los marcadores, la salida Valid agregada con add_function_param y el BP compilado.
Uso: python fixreturns.py <grafo>..."""
import sys
from at import call
B = "editor_toolset.toolsets.blueprint.BlueprintTools"


def node_infos(G):
    ns = call(B, "find_nodes", graph={"refPath": G}, title="", node_class=None, entry_points_only=False)
    I = {}
    for i in range(0, len(ns), 40):
        for f in call(B, "get_node_infos", nodes=ns[i:i + 40]): I[f["node"]["refPath"]] = f
    return I

def fix_returns(G):
    I = node_infos(G)
    sets = [k for k, f in I.items() if "VariableSet" in k and f["type_id"].endswith("SetRetTmp")]
    for k in sets:
        f = I[k]; pos = f["position"]
        ex_in = next(p for p in f["input_pins"] if p["type_id"] == "Exec")
        dat = next(p for p in f["input_pins"] if p["type_id"] != "Exec")
        r = call(B, "create_node", graph={"refPath": G}, type_id="||AddReturnNode...", pos=pos)
        ri = call(B, "get_node_infos", nodes=[r])[0]
        rex = next(p for p in ri["input_pins"] if p["type_id"] == "Exec")["pin_id"]
        rv = next(p for p in ri["input_pins"] if p["name"] == "Valid")["pin_id"]
        srcs_exec = list(ex_in["connected_pins"]); srcs_dat = list(dat["connected_pins"])
        call(B, "delete_node", node={"refPath": k})
        for c in srcs_exec: call(B, "connect_pins", output_pin=c, input_pin=rex)
        for c in srcs_dat: call(B, "connect_pins", output_pin=c, input_pin=rv)
        if not srcs_dat and str(dat["value"]).lower() == "true":
            call(B, "set_pin_value", pin=rv, value="true")
    return len(sets)


if __name__ == "__main__":
    for g in sys.argv[1:]:
        print(g.split(":")[-1], "returns", fix_returns(g))
