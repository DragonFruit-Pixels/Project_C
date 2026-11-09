import json, math, random
from at import call, S

O = "editor_toolset.toolsets.object.ObjectTools"
FILL = "/Game/Project_C/Map/Fill/SM_Fill_"
KIT = "/Game/Project_C/Map/Kit/SM_Det_"
U, H, FACE = 800, 400, 20
EDGE = {"E": ((0, H), 0), "W": ((0, -H), 180), "N": ((H, 0), -90), "S": ((-H, 0), 90)}

STAFF = {"Staff": ((0, 0), [((0, 0), dict(doors="E")), ((0, 1), dict(doors="WE"))])}
ROOMS = {
    **STAFF,
    "Dock": ((0, 2), [((0, 0), dict(open_="E", doors="W")), ((0, 1), dict(open_="WE", doors="N")), ((0, 2), dict(open_="W", doors="E"))]),
    "Offices": ((0, 5), [((0, 0), dict(doors="WE")), ((0, 1), dict(doors="WS"))]),
    "Lift": ((1, 6), [((0, 0), dict(open_="S", doors="N")), ((1, 0), dict(open_="NS")), ((2, 0), dict(open_="N", doors="W"))]),
    "Gallery": ((3, 3), [((0, 0), dict(open_="E", doors="N")), ((0, 1), dict(open_="WE", doors="N")), ((0, 2), dict(open_="W", doors="E"))]),
    "Lab": ((2, 2), [((0, 0), dict(doors="WE")), ((0, 1), dict(doors="WES")), ((0, 2), dict(doors="WS"))]),
    "Batch": ((2, 0), [((0, 0), dict(open_="ES")), ((0, 1), dict(open_="WS", doors="E")), ((1, 0), dict(open_="NE")), ((1, 1), dict(open_="NW"))]),
}

IND = ["Boxes", "Jerrycans", "Bucket", "Toolbox", "CableSpool", "TrashBags", "Debris", "GasCylinders", "Extinguisher", "Crate"]
BIO = ["EggSacs", "BiomassPuddle", "Fungus", "Tendrils", "Crust"]
FLAT = {"BiomassPuddle", "Debris", "Crust", "Tendrils"}
STYLE = {
    "Staff": dict(n=4, bio=0.0, pipes=0.3, light=(1.0, 0.82, 0.6), lux=14),
    "Dock": dict(n=5, bio=0.1, pipes=0.4, light=(1.0, 0.75, 0.45), lux=18),
    "Offices": dict(n=4, bio=0.0, pipes=0.2, light=(1.0, 0.85, 0.65), lux=14),
    "Lift": dict(n=3, bio=0.15, pipes=0.6, light=(1.0, 0.6, 0.3), lux=14),
    "Gallery": dict(n=6, bio=0.45, pipes=0.9, light=(0.7, 0.88, 1.0), lux=14),
    "Lab": dict(n=7, bio=0.6, pipes=0.6, light=(0.78, 1.0, 0.7), lux=14, extra=["SpecimenJar"]),
    "Batch": dict(n=7, bio=0.85, pipes=0.5, light=(0.72, 1.0, 0.62), lux=16),
}
HERO = [(0, 6300), (0, 6000), (-280, 6280), (220, 7400), (100, 8400), (-250, 9450), (280, 10000), (80, 10000),
        (200, 10800), (280, 11100), (-280, 11100), (-1300, 7600), (-1350, 8650), (-1600, 9200), (-2100, 8400),
        (-2550, 9850), (-2400, 6800), (-2650, 6950), (-1600, 8400), (0, 6800)]

random.seed(7)
placed = []


def rot(x, y, yaw):
    r = math.radians(yaw)
    return x * math.cos(r) - y * math.sin(r), x * math.sin(r) + y * math.cos(r)


def wall_point(cx, cy, edge, along, depth):
    (ex, ey), yaw = EDGE[edge]
    dx, dy = rot(along, -(FACE + depth), yaw)
    return cx + ex + dx, cy + ey + dy, yaw


def spawn(asset, label, x, y, z=0, yaw=0, scale=1.0, folder=""):
    a = call(S, "add_to_scene_from_asset", asset_path=asset, name=label,
             xform={"location": {"x": x, "y": y, "z": z}, "rotation": {"pitch": 0, "yaw": yaw, "roll": 0},
                    "scale": {"x": scale, "y": scale, "z": scale}}, parent=None, snap_to_ground=False)
    if not isinstance(a, dict):
        raise RuntimeError(f"{label}: {a}")
    call(S, "set_actor_folder", actor=a, folder_path=folder)
    placed.append(a)
    return a


def light(label, x, y, z, color, lux, folder):
    a = call(S, "add_to_scene_from_class", actor_type={"refPath": "/Script/Engine.PointLight"}, name=label,
             xform={"location": {"x": x, "y": y, "z": z}}, parent=None, snap_to_ground=False)
    comp = {"refPath": a["refPath"] + ".LightComponent0"}
    ok = call(O, "set_properties", instance=comp, values=json.dumps({
        "Intensity": lux, "AttenuationRadius": 950, "Mobility": "Movable", "SourceRadius": 20,
        "LightColor": {"r": color[0], "g": color[1], "b": color[2], "a": 1}}))
    assert ok is True, f"{label}: light props not set"
    call(S, "set_actor_folder", actor=a, folder_path=folder)
    placed.append(a)


def free(x, y, taken, gap):
    return all(math.hypot(x - a, y - b) > gap for a, b in taken + HERO)


def cleanup():
    for prefix in ("Fill_", "Det_", "Light_"):
        for a in call(S, "find_actors", name=prefix, tag="", collision_channels=[]) or []:
            call(S, "remove_from_scene", actor=a)


cleanup()
counts = {}
for room, ((r0, c0), cells) in ROOMS.items():
    st = STYLE[room]
    folder = f"Dressing/{room}"
    for (dr, dc), kw in cells:
        cx, cy = -U * (r0 + dr), 6000 + U * (c0 + dc)
        tag = f"{room}{dr}{dc}"
        walls = [e for e in EDGE if e not in kw.get("open_", "")]
        doors = kw.get("doors", "")
        taken = [wall_point(cx, cy, d, 0, 60)[:2] for d in doors]

        for e in walls:
            if random.random() < st["pipes"]:
                x, y, yaw = wall_point(cx, cy, e, 0, 0)
                spawn(KIT + ("PipesDoor" if e in doors else "Pipes"), f"Det_{tag}_Pipes{e}", x, y, yaw=yaw, folder=folder)

        lamp_wall = random.choice(walls)
        along = random.choice([-2.4, 2.4]) * 100 if lamp_wall in doors else random.uniform(-1.5, 1.5) * 100
        x, y, yaw = wall_point(cx, cy, lamp_wall, along, 0)
        spawn(KIT + "WallLamp", f"Det_{tag}_Lamp", x, y, yaw=yaw, folder=folder)
        lx, ly, _ = wall_point(cx, cy, lamp_wall, along, 80)
        light(f"Light_{tag}", lx, ly, 120, st["light"], st["lux"], folder)
        taken.append((lx, ly))

        e = random.choice(walls)
        along = random.choice([-1, 1]) * random.uniform(160, 300) if e in doors else random.uniform(-300, 300)
        x, y, yaw = wall_point(cx, cy, e, along, 0)
        if free(x, y, taken, 80):
            spawn(KIT + random.choice(["Vent", "Panel"]), f"Det_{tag}_Wall", x, y, yaw=yaw, folder=folder)

        pool_size = st["n"]
        tries = 0
        n = 0
        while n < pool_size and tries < 80:
            tries += 1
            bio = random.random() < st["bio"]
            pool = (BIO + st.get("extra", [])) if bio else IND + st.get("extra", [])
            name = random.choice(pool)
            if name in FLAT and random.random() < 0.5:
                x = cx + random.uniform(-260, 260)
                y = cy + random.uniform(-260, 260)
                if math.hypot(x - cx, y - cy) < 150:
                    continue
            else:
                e = random.choice(walls) if walls else random.choice(list(EDGE))
                along = random.uniform(-330, 330)
                if e in doors and abs(along) < 160:
                    continue
                x, y, _ = wall_point(cx, cy, e, along, random.uniform(30, 70))
            if not free(x, y, taken, 95):
                continue
            if any(math.hypot(x - a, y - b) < 200 for a, b in taken[:len(doors)]):
                continue
            k = random.uniform(0.5, 0.7) if name == "Crust" else random.uniform(0.85, 1.15)
            spawn(FILL + name, f"Fill_{tag}_{n}_{name}", x, y, yaw=random.uniform(0, 360), scale=k, folder=folder)
            taken.append((x, y))
            n += 1
        counts[tag] = n

        if r0 + dr >= 2 and random.random() < 0.6:
            gx, gy = cx + random.choice([-200, 200]), cy + random.choice([-200, 200])
            if free(gx, gy, taken, 120):
                spawn(KIT + "FloorGrate", f"Det_{tag}_Grate", gx, gy, z=1, folder=folder)

for a in call(S, "find_actors", name="Prop_", tag="", collision_channels=[]) or []:
    call(S, "set_actor_folder", actor=a, folder_path="Dressing/Props")
print("placed", len(placed), counts)
