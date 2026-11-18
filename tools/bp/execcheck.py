import sys
from t import t, ref
def bad_nodes(graph):
    ns=t("Blueprint.find_nodes", graph=ref(graph), title="", node_class=None, entry_points_only=False)
    out=[]
    for i in range(0,len(ns),40):
        for inf in t("Blueprint.get_node_infos", nodes=ns[i:i+40]):
            ex=[p for p in inf["input_pins"] if p["type_id"]=="Exec" and p["name"] in ("execute",)]
            used=any(p["connected_pins"] for p in inf["output_pins"] if p["type_id"]!="Exec")
            if ex and not any(p["connected_pins"] for p in ex) and used:
                out.append(inf["node"]["refPath"].split(".")[-1]+" "+str(inf.get("title","")))
    return out
if __name__=="__main__":
    BPS=sys.argv[1:]
    for bp in BPS:
        for g in t("Blueprint.list_graphs", blueprint=ref(bp)):
            b=bad_nodes(g["refPath"])
            if b: print(g["refPath"].split(":")[-1], bp.split(".")[-1], b)
    print("done")
