import sys
from at import call
from prune import prune
B="editor_toolset.toolsets.blueprint.BlueprintTools"
def nodes(G): return call(B,"find_nodes",graph={"refPath":G},title="",node_class=None,entry_points_only=False)
def slim(G):
    prune(G)
    I=call(B,"get_node_infos",nodes=nodes(G)); n=0
    for i in I:
        if "Literal" not in i["type_id"]: continue
        ins=[p for p in i["input_pins"] if p["type_id"]!="Exec"]
        if len(ins)!=1 or ins[0]["connected_pins"]: continue
        v=ins[0].get("value")
        targets=[c for p in i["output_pins"] for c in p["connected_pins"]]
        call(B,"delete_node",node=i["node"])
        for t in targets: call(B,"set_pin_value",pin=t,value=v)
        n+=1
    print(G.split(":")[-1],"literales:",n,"nodos:",len(nodes(G)))


def dedupe(G):
    """Une nodos puros idénticos (mismo tipo, mismas entradas). Deja afuera getters y aleatorios."""
    total = 0
    while True:
        I = call(B, "get_node_infos", nodes=nodes(G))
        sig = {}
        dup = None
        for i in I:
            k = i["node"]["refPath"]
            if any(p["type_id"] == "Exec" for p in i["input_pins"] + i["output_pins"]): continue
            if "VariableGet" in k or "Knot" in k or "Random" in i["type_id"] or not i["input_pins"]: continue
            s = (i["type_id"], tuple((p["name"], p.get("value"), tuple(sorted((c["node"]["refPath"], c["index_id"]) for c in p["connected_pins"]))) for p in i["input_pins"]))
            if s in sig: dup = (sig[s], i); break
            sig[s] = i
        if not dup: break
        keep, d = dup
        for p in d["output_pins"]:
            kp = next(q for q in keep["output_pins"] if q["name"] == p["name"])
            for c in p["connected_pins"]:
                call(B, "connect_pins", output_pin=kp["pin_id"], input_pin=c)
        call(B, "delete_node", node=d["node"])
        total += 1
    print(G.split(":")[-1], "duplicados unidos:", total, "nodos:", len(nodes(G)))


if __name__=="__main__":
    for g in sys.argv[1:]: slim(g); dedupe(g)
