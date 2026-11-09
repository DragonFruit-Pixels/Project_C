import json, math, os, random
from at import call, S, M

AS = "editor_toolset.toolsets.asset.AssetTools"
O = "editor_toolset.toolsets.object.ObjectTools"
MI = "editor_toolset.toolsets.material_instance.MaterialInstanceTools"
ROOT = os.path.dirname(os.path.abspath(__file__)).replace("\\", "/")
KIT, MAT = "/Game/Project_C/Map/Kit", "/Game/Project_C/Map/Materials"
U = 800
SECTIONS = {
    "Plant": [(0, c) for c in range(7)] + [(1, 6), (2, 6), (3, 6)],
    "Sump": [(2, 0), (2, 1), (3, 0), (3, 1), (2, 2), (2, 3), (2, 4), (3, 3), (3, 4), (3, 5)],
}
EDGE = {"E": ((0, 1), 0), "W": ((0, -1), 180), "N": ((1, 0), -90), "S": ((-1, 0), 90)}
NEIGHBOR = {"E": (0, 1), "W": (0, -1), "N": (-1, 0), "S": (1, 0)}
rng = random.Random(11)


def ref(p):
    return {"refPath": f"{p}.{p.split('/')[-1]}"}


def mi(name, color, rough, metal):
    p = f"{MAT}/{name}"
    if call(AS, "exists", path=p) is True:
        assert call(AS, "delete", path=p) is True, p
    inst = call(MI, "create", folder_path=MAT, asset_name=name, parent=ref(f"{MAT}/M_KitSolid"))
    call(MI, "set_vector_parameter", instance=inst, name="Color", value=dict(zip("rgba", color)))
    call(MI, "set_scalar_parameter", instance=inst, name="Roughness", value=rough)
    call(MI, "set_scalar_parameter", instance=inst, name="Metallic", value=metal)
    return p


def import_meshes(rock):
    paths = []
    for n in ("SM_Dio_Base_Plant", "SM_Dio_Base_Sump", "SM_Dio_Pillar"):
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
                  values=json.dumps({"OverrideMaterials": [ref(override)] * 4}))
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
                yield e, cx + dx * U / 2, cy + dy * U / 2, yaw + 180


def run():
    rock = mi("MI_Dio_Rock", (0.16, 0.13, 0.11, 1), 0.95, 0.0)
    dark = mi("MI_Dio_Silhouette", (0.012, 0.012, 0.014, 1), 0.9, 0.2)
    saved = import_meshes(rock) + [rock, dark]
    assert call(AS, "save_assets", asset_paths=saved) is True
    cleanup()

    for section, cells in SECTIONS.items():
        place(f"{KIT}/SM_Dio_Base_{section}", f"Dio_Base_{section}", (0, 0, 0))
        for i, (e, x, y, yaw) in enumerate(edges(section, cells)):
            roll = rng.random()
            if roll < 0.45:
                place(f"{KIT}/SM_Det_Pipes", f"Dio_Pipes_{section}{i}", (x, y, -170 - rng.choice([0, 90])), (0, yaw, 0))
            elif section == "Sump" and roll < 0.8:
                place("/Game/Project_C/Map/Fill/SM_Fill_Tendrils", f"Dio_Roots_{section}{i}",
                      (x, y, -60), (rng.uniform(-80, -60), yaw + 90, 0), rng.uniform(1.4, 2.0))

    x0, x1, y0, y1 = -2900, 500, 5500, 11300
    ring = []
    for i in range(16):
        t = i / 16 * 2 * math.pi
        rx, ry = (x1 - x0) / 2 + rng.uniform(3500, 6000), (y1 - y0) / 2 + rng.uniform(3500, 6000)
        ring.append(((x0 + x1) / 2 + rx * math.cos(t), (y0 + y1) / 2 + ry * math.sin(t)))
    for i, (x, y) in enumerate(ring):
        near_cam = x < x0 + 200
        top = rng.uniform(-2500, -1500) if near_cam else rng.uniform(-1400, 100)
        k = rng.uniform(1.4, 2.2)
        kind = i % 4
        if kind in (0, 2):
            place(f"{KIT}/SM_Dio_Pillar", f"Dio_Pillar{i}", (x, y, top - 3000 * k), (0, rng.uniform(0, 360), 0), k, dark)
        elif kind == 1:
            place("/Game/Project_C/Map/Props/SM_Prop_CoreVat", f"Dio_Tank{i}", (x, y, top - 2200), (0, rng.uniform(0, 360), 0), 3.0, dark)
        else:
            place(f"{KIT}/SM_Det_Pipes", f"Dio_Riser{i}", (x, y, top - 2400), (90, rng.uniform(0, 360), 0), (3.0, 3.0, 3.0), dark)

    for i, (x, y) in enumerate([(1500, 6800), (1500, 9600), (-400, 12600), (-1700, 12600)]):
        light(f"Dio_HaloPlant{i}", (x, y, -1400), (1.0, 0.55, 0.2), 60, 3000, 5)
    for i, (x, y) in enumerate([(-2000, 6400), (-2000, 8200), (-2400, 9600), (-1600, 7300)]):
        light(f"Dio_HaloSump{i}", (x, y, -1600), (0.35, 1.0, 0.3), 40, 2600, 5)

    fog = call(S, "add_to_scene_from_class", actor_type={"refPath": "/Script/Engine.ExponentialHeightFog"}, name="Dio_Fog",
               xform={"location": {"x": -1200, "y": 8400, "z": -600}}, parent=None, snap_to_ground=False)
    ok = call(O, "set_properties", instance={"refPath": fog["refPath"] + ".HeightFogComponent0"}, values=json.dumps({
        "FogDensity": 0.035, "FogHeightFalloff": 0.35, "FogInscatteringLuminance": {"r": 0.0015, "g": 0.002, "b": 0.003, "a": 1},
        "bEnableVolumetricFog": True, "VolumetricFogScatteringDistribution": 0.5, "VolumetricFogExtinctionScale": 1.0}))
    assert ok is True, ok
    call(S, "set_actor_folder", actor=fog, folder_path="Diorama")
    print("PLACED", len(call(S, "find_actors", name="Dio_", tag="", collision_channels=[]) or []))


run()
