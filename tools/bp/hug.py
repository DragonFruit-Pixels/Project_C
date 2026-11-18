"""Pone cada nodo puro pegado al pin que alimenta; no mueve nodos con exec."""
import sys
sys.path.insert(0, __import__("os").path.dirname(__import__("os").path.abspath(__file__)))
from at import call
B = "editor_toolset.toolsets.blueprint.BlueprintTools"
G = sys.argv[1]
ns = call(B, "find_nodes", graph={"refPath": G}, title="", node_class=None, entry_points_only=False)
I = {}
for i in range(0, len(ns), 40):
    for f in call(B, "get_node_infos", nodes=ns[i:i + 40]): I[f["node"]["refPath"]] = f
I = {k: v for k, v in I.items() if "Comment" not in k}
ex = lambda k: any(p["type_id"] == "Exec" for p in I[k]["input_pins"] + I[k]["output_pins"])
def size(k):
    f = I[k]; t = f["type_id"].split("|")[-1]
    if "VariableGet" in k and not f["input_pins"]: return 9 * len(t) + 40, 36
    n = max(len(f["input_pins"]), len(f["output_pins"]))
    return max(180, 10 * len(t) + 60), 56 + 28 * n
P = {k: (I[k]["position"]["x"], I[k]["position"]["y"]) for k in I}
rects = [(P[k][0], P[k][1], *size(k)) for k in I if ex(k)]
def hit(x, y, w, h):
    return any(x < rx + rw + 16 and rx < x + w + 16 and y < ry + rh + 8 and ry < y + h + 8 for rx, ry, rw, rh in rects)
placed = set()
def hug(c):
    cx, cy = P[c]
    for idx, p in enumerate(I[c]["input_pins"]):
        for con in p["connected_pins"]:
            s = con["node"]["refPath"]
            if s not in I or ex(s) or s in placed: continue
            w, h = size(s)
            x = cx - w - 48 - (0 if idx % 2 else 0)
            y = cy + 44 + 28 * idx - (18 if h == 36 else 44)
            while hit(x, y, w, h): x -= 40
            P[s] = (x, y); placed.add(s); rects.append((x, y, w, h))
            call(B, "set_node_position", node={"refPath": s}, pos={"x": int(x) // 16 * 16, "y": int(y)})
            hug(s)
for k in sorted([k for k in I if ex(k)], key=lambda k: P[k][0]): hug(k)
print("movidos", len(placed), "de", sum(1 for k in I if not ex(k)), "puros")
