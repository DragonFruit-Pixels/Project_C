"""Calcula cajas de comentario por grupos de nodos exec (+ sus nodos puros) y fusiona las que se pisan.
Uso: from cgroups import plan; plan(graph, [(texto, [nodos exec]), ...]) -> lista de (texto, rect)"""
import json
from tidy import Tidy
def plan(G, groups):
    t = Tidy(G)
    pos = {k: (v["position"]["x"], v["position"]["y"]) for k, v in t.info.items()}
    full = lambda n: G + "." + n
    def tree(k, acc):
        for src in t.data_inputs(k):
            if src not in acc and "Knot" not in src: acc.add(src); tree(src, acc)
        return acc
    boxes = []; taken = set()
    for text, nodes in groups:
        ks = set()
        for n in nodes: ks.add(full(n)); tree(full(n), ks)
        ks -= taken; taken |= ks
        xs = [pos[k][0] for k in ks]; ys = [pos[k][1] for k in ks]
        x2 = [pos[k][0] + t.size(k)[0] for k in ks]; y2 = [pos[k][1] + t.size(k)[1] for k in ks]
        boxes.append([text, [min(xs) - 32, min(ys) - 72, max(x2) + 32, max(y2) + 32]])
    merged = True
    while merged:
        merged = False
        for i in range(len(boxes)):
            for j in range(i + 1, len(boxes)):
                a, b = boxes[i][1], boxes[j][1]
                if a[0] < b[2] and b[0] < a[2] and a[1] < b[3] and b[1] < a[3]:
                    boxes[i] = [boxes[i][0] + " · " + boxes[j][0], [min(a[0], b[0]), min(a[1], b[1]), max(a[2], b[2]), max(a[3], b[3])]]
                    del boxes[j]; merged = True; break
            if merged: break
    return [{"graph": G, "text": tx, "rect": r} for tx, r in boxes]
