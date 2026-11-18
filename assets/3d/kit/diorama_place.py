import json, math, os, random
from at import call, S, M

AS = "editor_toolset.toolsets.asset.AssetTools"
O = "editor_toolset.toolsets.object.ObjectTools"
MI = "editor_toolset.toolsets.material_instance.MaterialInstanceTools"
TT = "editor_toolset.toolsets.texture.TextureTools"
ROOT = os.path.dirname(os.path.abspath(__file__)).replace("\\", "/")
KIT, MAT = "/Game/Project_C/Map/Kit", "/Game/Project_C/Map/Materials"
U = 800
MOON, SKY = 0.4, (0.04, 0.07, 0.09)
CUT_H = {"CutPlant": 404, "CutSump": 264}
ISLET_H = 1551
RING_N = 28
RING = [("RuinFacade", 3000, (0.5, 0.8)), ("Islet", ISLET_H, (0.6, 1.0)), ("RootSpire", 3000, (0.5, 0.9)),
        ("Tanks", 1800, (0.6, 0.9)), ("RuinPillars", 1400, (0.8, 1.2)), ("Islet", ISLET_H, (0.4, 0.7)),
        ("PipeGantry", 3200, (0.5, 0.7)), ("RootSpire", 3000, (0.4, 0.7))]
SECTIONS = {
    "Plant": [(0, c) for c in range(7)] + [(1, 6), (2, 6), (3, 6)],
    "Sump": [(2, 0), (2, 1), (3, 0), (3, 1), (2, 2), (2, 3), (2, 4), (3, 3), (3, 4), (3, 5)],
}
EDGE = {"E": ((0, 1), 0), "W": ((0, -1), 180), "N": ((1, 0), -90), "S": ((-1, 0), 90)}
NEIGHBOR = {"E": (0, 1), "W": (0, -1), "N": (-1, 0), "S": (1, 0)}
rng = random.Random(11)


def ref(p):
    return {"refPath": f"{p}.{p.split('/')[-1]}"}


def rock_mi():
    tex = "/Game/Project_C/Map/Textures"
    props = {"N": {"CompressionSettings": "TC_Normalmap", "SRGB": False}, "RM": {"CompressionSettings": "TC_Masks", "SRGB": False}}
    p = f"{MAT}/MI_Dio_Rock"
    if call(AS, "exists", path=p) is True:
        assert call(AS, "delete", path=p) is True, p
    inst = call(MI, "create", folder_path=MAT, asset_name="MI_Dio_Rock", parent=ref(f"{MAT}/M_Prop"))
    saved = [p]
    for suffix, param in (("BC", "BaseColor"), ("N", "Normal"), ("RM", "RM")):
        t = f"{tex}/T_Dio_Rock_{suffix}"
        if call(AS, "exists", path=t) is True:
            assert call(AS, "delete", path=t) is True, t
        r = call(TT, "import_file", folder_path=tex, asset_name=f"T_Dio_Rock_{suffix}", source_file=f"{ROOT}/textures/T_Dio_Rock_{suffix}.jpg")
        assert isinstance(r, list) and r, (suffix, r)
        if suffix in props:
            assert call(O, "set_properties", instance=ref(t), values=json.dumps(props[suffix])) is True
        call(MI, "set_texture_parameter", instance=inst, name=param, value=ref(t))
        saved.append(t)
    return p, saved


def fogcard(label, loc, scale, mi, yaw=None):
    yaw = rng.uniform(0, 360) if yaw is None else yaw
    a = place("/Engine/BasicShapes/Plane", label, loc, (0, yaw, 0), (scale, scale, 1), f"{MAT}/{mi}")
    ok = call(O, "set_properties", instance={"refPath": a["refPath"] + ".StaticMeshComponent0"}, values=json.dumps({
        "CastShadow": False, "bAffectDistanceFieldLighting": False, "bVisibleInRayTracing": False,
        "bAffectDynamicIndirectLighting": False}))
    assert ok is True, (label, ok)


def import_meshes(rock):
    paths = []
    for n in ("SM_Dio_Base_Plant", "SM_Dio_Base_Sump"):
        p = f"{KIT}/{n}"
        if call(AS, "exists", path=p) is True:
            assert call(AS, "delete", path=p) is True, p
        r = call(M, "import_file", folder_path=KIT, asset_name=n, source_file=f"{ROOT}/{n}.fbx",
                 import_materials=False, import_textures=False, combine_meshes=True)
        assert isinstance(r, list) and r, (n, r)
        for slot in call(M, "get_material_slots", mesh=ref(p)):
            assert call(M, "set_material", mesh=ref(p), slot_name=slot, material=ref(rock)) is True
        paths.append(p)
    return paths


def cleanup():
    for a in call(S, "find_actors", name="Dio_", tag="", collision_channels=[]) or []:
        call(S, "remove_from_scene", actor=a)


def place(asset, label, loc, rot=(0, 0, 0), scale=1.0, override=None):
    s = scale if isinstance(scale, tuple) else (scale,) * 3
    a = call(S, "add_to_scene_from_asset", asset_path=asset, name=label,
             xform={"location": dict(zip("xyz", loc)), "rotation": dict(zip(("pitch", "yaw", "roll"), rot)),
                    "scale": dict(zip("xyz", s))}, parent=None, snap_to_ground=False)
    assert isinstance(a, dict), (label, a)
    if override:
        ok = call(O, "set_properties", instance={"refPath": a["refPath"] + ".StaticMeshComponent0"},
                  values=json.dumps({"OverrideMaterials": [ref(override)]}))
        assert ok is True, (label, ok)
    call(S, "set_actor_folder", actor=a, folder_path="Diorama")
    return a


def light(label, loc, color, intensity, radius, scatter):
    a = call(S, "add_to_scene_from_class", actor_type={"refPath": "/Script/Engine.PointLight"}, name=label,
             xform={"location": dict(zip("xyz", loc))}, parent=None, snap_to_ground=False)
    ok = call(O, "set_properties", instance={"refPath": a["refPath"] + ".LightComponent0"}, values=json.dumps({
        "Intensity": intensity, "AttenuationRadius": radius, "Mobility": "Movable", "CastShadows": False,
        "VolumetricScatteringIntensity": scatter, "LightColor": dict(zip("rgba", (*color, 1)))}))
    assert ok is True, (label, ok)
    call(S, "set_actor_folder", actor=a, folder_path="Diorama")


def edges(section, cells):
    owned = set(cells)
    for r, c in cells:
        cx, cy = -U * r, 6000 + U * c
        for e, ((dx, dy), yaw) in EDGE.items():
            nr, nc = r + NEIGHBOR[e][0], c + NEIGHBOR[e][1]
            if (nr, nc) not in owned:
                yield (dx, dy), cx + dx * U / 2, cy + dy * U / 2, yaw + 180


def run():
    rock, saved = rock_mi()
    saved += import_meshes(rock)
    assert call(AS, "save_assets", asset_paths=saved) is True
    cleanup()

    for section, cells in SECTIONS.items():
        place(f"{KIT}/SM_Dio_Base_{section}", f"Dio_Base_{section}", (0, 0, 0))
        for i, ((dx, dy), x, y, yaw) in enumerate(edges(section, cells)):
            light(f"Dio_Fill_{section}{i}", (x + dx * 300, y + dy * 300, -100), (1.0, 0.75, 0.55), 9, 800, 0.5)
            cut = "CutPlant" if section == "Plant" or i % 2 else "CutSump"
            h = CUT_H[cut]
            place(f"{KIT}/SM_Dio_{cut}", f"Dio_{cut}{i}", (x + dx * 30, y + dy * 30, -20 - h), (0, yaw - 180, 0),
                  (1 if i % 2 else -1, 1, 1))

    cx, cy = -1200, 8200
    for i in range(RING_N):
        name, height, (k0, k1) = RING[i % len(RING)]
        t = 2 * math.pi * (i + rng.uniform(-0.3, 0.3)) / RING_N
        d = rng.uniform(1800, 3000) if i % 2 else rng.uniform(3500, 6000)
        k = rng.uniform(k0, k1)
        top = rng.uniform(-1500, -200)
        place(f"{KIT}/SM_Dio_{name}", f"Dio_Ring{i}_{name}", (cx + (1600 + d) * math.cos(t), cy + (2600 + d) * math.sin(t),
              top - height * k), (0, rng.uniform(0, 360), 0), k)

    bowl = place(f"{KIT}/SM_Dio_Backdrop", "Dio_Backdrop", (-1200, 8400, 2000), (0, 45, 0))
    ok = call(O, "set_properties", instance={"refPath": bowl["refPath"] + ".StaticMeshComponent0"}, values=json.dumps({
        "CastShadow": False, "bAffectDistanceFieldLighting": False, "bAffectDynamicIndirectLighting": False}))
    assert ok is True, ok

    for i, (x, y) in enumerate([(1500, 6800), (1500, 9600), (-400, 12600), (-1700, 12600)]):
        light(f"Dio_HaloPlant{i}", (x, y, -1400), (1.0, 0.55, 0.2), 10, 2600, 2)
    for i, (x, y) in enumerate([(-2000, 6400), (-2000, 8200), (-2400, 9600), (-1600, 7300)]):
        light(f"Dio_HaloSump{i}", (x, y, -1600), (0.45, 0.9, 0.4), 4, 2400, 2)

    for i, (x, y) in enumerate([(1400, 7400), (1500, 9400), (-1200, 12500)]):
        fogcard(f"Dio_FogAmber{i}", (x, y, rng.uniform(-100, 150)), rng.uniform(28, 36), "MI_Fog_Amber")
    for i, (x, y) in enumerate([(-800, 6400), (-800, 8200), (-900, 9900), (-2300, 4400), (-1400, 11000)]):
        fogcard(f"Dio_FogGreen{i}", (x, y, rng.uniform(-350, -150)), rng.uniform(14, 20), "MI_Fog_Green")

    fog = call(S, "add_to_scene_from_class", actor_type={"refPath": "/Script/Engine.ExponentialHeightFog"}, name="Dio_Fog",
               xform={"location": {"x": -1200, "y": 8400, "z": -600}}, parent=None, snap_to_ground=False)
    ok = call(O, "set_properties", instance={"refPath": fog["refPath"] + ".HeightFogComponent0"}, values=json.dumps({
        "FogDensity": 0.006, "FogHeightFalloff": 0.35, "FogInscatteringLuminance": {"r": 0.0004, "g": 0.0007, "b": 0.001, "a": 1},
        "bEnableVolumetricFog": True, "VolumetricFogDistance": 20000, "VolumetricFogScatteringDistribution": 0.5, "VolumetricFogExtinctionScale": 1.0}))
    assert ok is True, ok
    call(S, "set_actor_folder", actor=fog, folder_path="Diorama")

    moon = call(S, "add_to_scene_from_class", actor_type={"refPath": "/Script/Engine.DirectionalLight"}, name="Dio_Moon",
                xform={"location": {"x": -1200, "y": 8400, "z": 3000}, "rotation": {"pitch": -35, "yaw": 200, "roll": 0}},
                parent=None, snap_to_ground=False)
    ok = call(O, "set_properties", instance={"refPath": moon["refPath"] + ".LightComponent0"}, values=json.dumps({
        "Intensity": MOON, "LightColor": {"r": 0.55, "g": 0.75, "b": 0.9, "a": 1}, "Mobility": "Movable",
        "VolumetricScatteringIntensity": 0.0}))
    assert ok is True, ok
    call(S, "set_actor_folder", actor=moon, folder_path="Diorama")
    sky = call(S, "add_to_scene_from_class", actor_type={"refPath": "/Script/Engine.SkyLight"}, name="Dio_Sky",
               xform={"location": {"x": -1200, "y": 8400, "z": 1000}}, parent=None, snap_to_ground=False)
    ok = call(O, "set_properties", instance={"refPath": sky["refPath"] + ".SkyLightComponent0"}, values=json.dumps({
        "Mobility": "Movable", "bRealTimeCapture": False, "Intensity": 1.0, "VolumetricScatteringIntensity": 0.0, "bLowerHemisphereIsBlack": True,
        "LowerHemisphereColor": dict(zip("rgba", (*SKY, 1)))}))
    assert ok is True, ok
    call(S, "set_actor_folder", actor=sky, folder_path="Diorama")
    print("PLACED", len(call(S, "find_actors", name="Dio_", tag="", collision_channels=[]) or []))


run()
