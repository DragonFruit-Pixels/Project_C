"""Ordena un grafo BP con el estilo de las funciones que Sebastián ordenó a mano.
- El camino principal de ejecución va en una fila recta.
- Las ramas cortas (la salida de un Branch o Cast con menos nodos) van debajo, a la derecha.
- Los nodos puros van debajo y a la izquierda de quien los usa.
- Los cables de datos largos van por un carril horizontal con reroutes, debajo de la fila.
Uso: python tidy.py <graph refPath>..."""
import sys
from at import call
B = "editor_toolset.toolsets.blueprint.BlueprintTools"
GAP, SNAP = 64, 16
LANE0, LANESTEP, BAND = 150, 24, 250  # carriles de datos y comienzo de la banda de nodos puros
FAR = 420


def snap(v): return int(round(v / SNAP) * SNAP)


class Tidy:
    def __init__(s, G, heads=(), band=BAND):
        s.G = G
        s.band = band
        s.heads = {h if ":" in h else G + "." + h for h in heads}
        ns = call(B, "find_nodes", graph={"refPath": G}, title="", node_class=None, entry_points_only=False)
        s.info = {}
        for i in range(0, len(ns), 40):
            for inf in call(B, "get_node_infos", nodes=ns[i:i + 40]):
                s.info[inf["node"]["refPath"]] = inf
        s.info = {k: v for k, v in s.info.items() if "Comment" not in k}
        s.pos, s.rects, s.row_of, s.kpos = {}, [], {}, {}
        s.entry = [k for k in s.info if "FunctionEntry" in k] or \
                  [n["refPath"] for n in call(B, "find_nodes", graph={"refPath": G}, title="", node_class=None, entry_points_only=True)]

    # ---------- forma ----------
    def is_exec(s, k): return any(p["type_id"] == "Exec" for p in s.info[k]["input_pins"] + s.info[k]["output_pins"])
    def title(s, k): return s.info[k]["type_id"].split("|")[-1]

    def size(s, k):
        i, t = s.info[k], s.title(k)
        nin, nout = len(i["input_pins"]), len(i["output_pins"])
        if "Knot" in k: return 16, 16
        if "VariableGet" in k and nin == 0: return 70 + 9 * max(len(t) - 3, 4), 44
        if "PromotableOperator" in k or "CommutativeAssociative" in k or "EnumEquality" in k: return 140, 36 + 28 * nin
        if "VariableSet" in k: return max(190, 100 + 9 * len(t)), 56 + 28 * max(nin, nout)
        pin = max([len(p["name"]) for p in i["input_pins"]] or [0]) + max([len(p["name"]) for p in i["output_pins"]] or [0])
        return max(220, 10 * len(t) + 60, 8 * pin + 100), 56 + 28 * max(nin, nout)

    # ---------- enlaces ----------
    def exec_outs(s, k):
        return [(p, c["node"]["refPath"]) for p in s.info[k]["output_pins"] if p["type_id"] == "Exec" for c in p["connected_pins"]]

    def data_inputs(s, k):
        r = []
        for p in s.info[k]["input_pins"]:
            if p["type_id"] == "Exec": continue
            for c in p["connected_pins"]:
                src = c["node"]["refPath"]
                if src in s.info and not s.is_exec(src): r.append(src)
        return r

    def subtree(s, k, seen=None):
        seen = seen if seen is not None else set()
        if k in seen or k in s.pos: return 0
        seen.add(k)
        return 1 + sum(s.subtree(d, seen) for _, d in s.exec_outs(k))

    def reach(s, k, seen=None):
        seen = seen if seen is not None else set()
        if k in seen or k in s.pos: return seen
        seen.add(k)
        for _, d in s.exec_outs(k): s.reach(d, seen)
        return seen

    # ---------- colisiones ----------
    def overlap(s, x, y, w, h):
        for (rx, ry, rw, rh, k) in s.rects:
            if x < rx + rw + 32 and rx < x + w + 32 and y < ry + rh + 32 and ry < y + h + 32:
                return (rx + rw + 32 - x, ry + rh + 32 - y)
        return None

    def put(s, k, x, y):
        w, h = s.size(k)
        s.pos[k] = (snap(x), snap(y))
        s.rects.append((snap(x), snap(y), w, h, k))

    def data_tree(s, k, x, y, plan):
        cy = y
        for src in s.data_inputs(k):
            if src in s.pos or any(src == p[0] for p in plan): continue
            w, h = s.size(src)
            plan.append((src, x - GAP - w, cy))
            cy = max(cy + h + 32, s.data_tree(src, x - GAP - w, cy, plan))
        return cy

    # ---------- exec ----------
    def place_exec(s, k, x, row, slide="x"):
        if k in s.pos: return
        w, h = s.size(k)
        off = 16 if "VariableSet" in k else (32 if "FunctionResult" in k else 0)
        while True:
            plan = []
            s.data_tree(k, x, row + s.band, plan)
            if k in s.heads and s.rects:
                barrier = max(rx + rw for rx, ry, rw, rh, _ in s.rects) + 128
                minp = min([px for _, px, _ in plan] + [x])
                if minp < barrier:
                    x += barrier - minp
                    continue
            hit = None
            for src, px, py in plan + [(k, x, row + off)]:
                hit = s.overlap(px, py, *s.size(src))
                if hit: break
            if not hit: break
            if slide == "y": row += hit[1]
            else: x += hit[0]
        s.put(k, x, row + off)
        s.row_of[k] = row
        for src, px, py in plan:
            s.put(src, px, py)
            s.row_of.setdefault(src, row)
        outs = [(p, d) for p, d in s.exec_outs(k) if d not in s.pos]
        if not outs: return
        main = max(outs, key=lambda o: s.subtree(o[1]))
        mreach = s.reach(main[1])
        sides = [d for p, d in outs if d is not main[1]]
        early = [d for d in sides if not (s.reach(d) & mreach)]
        for d in early:
            s.place_exec(d, x + w + GAP, row + s.band, slide="y")
        s.place_exec(main[1], x + w + GAP, row)
        for d in sides:
            if d not in early:
                s.place_exec(d, x + w + GAP, row + s.band, slide="y")

    # ---------- reroutes ----------
    def pin_y(s, k, pin, side):
        if "Knot" in k: return s.kpos[k][1] + 8
        pins = s.info[k]["input_pins" if side == "in" else "output_pins"]
        idx = next((n for n, p in enumerate(pins) if p["name"] == pin["name"]), 0)
        if ("VariableGet" in k and not s.info[k]["input_pins"]) or "PromotableOperator" in k or "CommutativeAssociative" in k:
            return s.pos[k][1] + 22 + 28 * idx
        return s.pos[k][1] + 44 + 28 * idx

    def knot(s, x, y):
        r = call(B, "create_node", graph={"refPath": s.G}, type_id="||AddRerouteNode...", pos={"x": snap(x), "y": snap(y) - 8})
        k = r["refPath"]
        s.info[k] = call(B, "get_node_infos", nodes=[r])[0]
        s.kpos[k] = (snap(x), snap(y) - 8)
        return k

    def pid(s, k, d, name):
        for p in s.info[k]["input_pins" if d == "in" else "output_pins"]:
            if p["name"] == name: return p["pin_id"]

    def wire(s, a, ap, b, bp):
        call(B, "connect_pins", output_pin=s.pid(a, "out", ap), input_pin=s.pid(b, "in", bp))

    def far_links(s, k, p):
        far = []
        for c in p["connected_pins"]:
            d = c["node"]["refPath"]
            if d not in s.pos: continue
            dp = next(q for q in s.info[d]["input_pins"] if q["pin_id"]["index_id"] == c["index_id"])
            if s.pos[d][0] - (s.pos[k][0] + s.size(k)[0]) > FAR or s.row_of.get(d) != s.row_of.get(k):
                far.append((d, dp))
        return far

    def lanes_needed(s):
        n = {}
        for k in s.pos:
            for p in s.info[k]["output_pins"]:
                if p["type_id"] != "Exec" and s.far_links(k, p):
                    n[s.row_of[k]] = n.get(s.row_of[k], 0) + 1
        return max(n.values() or [0])

    def reroute(s):
        lanes = {}
        for k in list(s.pos):
            for p in s.info[k]["output_pins"]:
                if p["type_id"] == "Exec": continue
                far = s.far_links(k, p)
                if not far: continue
                far.sort(key=lambda t: s.pos[t[0]][0])
                row = s.row_of[k]
                n = lanes.get(row, 0); lanes[row] = n + 1
                ly = row + LANE0 + LANESTEP * n
                prev = s.knot(s.pos[k][0] + s.size(k)[0] + 32, ly)
                s.wire(k, p["name"], prev, "InputPin")
                for d, dp in far:
                    kn = s.knot(s.pos[d][0] - 48, ly)
                    s.wire(prev, "OutputPin", kn, "InputPin")
                    s.wire(kn, "OutputPin", d, dp["name"])
                    prev = kn

    def unknot(s):
        knots = [k for k in s.info if "Knot" in k]
        if not knots: return False
        def src_of(k):
            for c in s.info[k]["input_pins"][0]["connected_pins"]:
                n = c["node"]["refPath"]
                return src_of(n) if "Knot" in n else c
        def dests(k):
            r = []
            for c in s.info[k]["output_pins"][0]["connected_pins"]:
                n = c["node"]["refPath"]
                r += dests(n) if "Knot" in n else [c]
            return r
        pairs = []
        for k in knots:
            if any("Knot" in c["node"]["refPath"] for c in s.info[k]["input_pins"][0]["connected_pins"]): continue
            sc = src_of(k)
            if sc: pairs += [(sc, d) for d in dests(k)]
        for k in knots: call(B, "delete_node", node={"refPath": k})
        for a, b in pairs: call(B, "connect_pins", output_pin=a, input_pin=b)
        return True

    def run(s):
        if s.unknot():
            return Tidy(s.G, s.heads, s.band).run()
        for e in s.entry:
            s.place_exec(e, 0, 0)
        need = LANE0 + LANESTEP * s.lanes_needed() + 56
        if need > s.band:
            return Tidy(s.G, s.heads, need).run()
        left = [k for k in s.info if k not in s.pos]
        for k in left:
            low = max([ry + rh for rx, ry, rw, rh, _ in s.rects] or [0]) + 120
            s.put(k, 0, low)
        for k, (x, y) in s.pos.items():
            call(B, "set_node_position", node={"refPath": k}, pos={"x": x, "y": y})
        s.reroute()
        print(s.G.split(":")[-1], "nodos:", len(s.pos), "reroutes:", len(s.kpos), "sueltos:", len(left))


if __name__ == "__main__":
    for g in sys.argv[1:]:
        Tidy(g).run()
