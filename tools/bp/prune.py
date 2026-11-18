import sys
from t import t, ref


def prune(graph, dry=False):
    ns = t("Blueprint.find_nodes", graph=ref(graph), title="", node_class=None, entry_points_only=False)
    info = {}
    for i in range(0, len(ns), 40):
        for inf in t("Blueprint.get_node_infos", nodes=ns[i:i + 40]):
            info[inf["node"]["refPath"]] = inf
    entry = [n["refPath"] for n in t("Blueprint.find_nodes", graph=ref(graph), title="", node_class=None, entry_points_only=True)]
    live = set(entry)
    stack = list(entry)
    while stack:
        inf = info.get(stack.pop())
        if not inf:
            continue
        links = [c for p in inf["output_pins"] if p["type_id"] == "Exec" for c in p["connected_pins"]]
        links += [c for p in inf["input_pins"] if p["type_id"] != "Exec" for c in p["connected_pins"]]
        for c in links:
            r = c["node"]["refPath"]
            if r not in live:
                live.add(r)
                stack.append(r)
    dead = [r for r in info if r not in live and "Comment" not in r]
    print(graph.split(":")[-1], len(info), "nodes", len(dead), "dead", [d.split(".")[-1] for d in dead])
    if not dry:
        for d in dead:
            t("Blueprint.delete_node", node=ref(d))
    return dead


if __name__ == "__main__":
    for g in sys.argv[1:]:
        if g != "--dry":
            prune(g, dry="--dry" in sys.argv)
