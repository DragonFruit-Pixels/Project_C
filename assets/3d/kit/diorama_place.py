import json, math, os, random
from at import call, S, M

AS = "editor_toolset.toolsets.asset.AssetTools"
O = "editor_toolset.toolsets.object.ObjectTools"
MI = "editor_toolset.toolsets.material_instance.MaterialInstanceTools"
TT = "editor_toolset.toolsets.texture.TextureTools"
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


def fogvol(label, loc, scale, emissive, albedo=(0.02, 0.02, 0.025), radial=0.15, height=0.0):
    a = call(S, "add_to_scene_from_class", actor_type={"refPath": "/Script/Engine.LocalFogVolume"}, name=label,
             xform={"location": dict(zip("xyz", loc)), "scale": dict(zip("xyz", scale))}, parent=None, snap_to_ground=False)
    ok = call(O, "set_properties", instance={"refPath": a["refPath"] + ".LocalFogVolumeComponent"}, values=json.dumps({
        "RadialFogExtinction": radial, "HeightFogExtinction": height,
        "FogEmissive": dict(zip("rgba", (*emissive, 1))), "FogAlbedo": dict(zip("rgba", (*albedo, 1)))}))
    assert ok is True, (label, ok)
    call(S, "set_actor_folder", actor=a, folder_path="Diorama")


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
                yield (dx, dy), cx + dx * U / 2, cy + dy * U / 2, yaw + 180


def run():
    rock, saved = rock_mi()
    dark = mi("MI_Dio_Silhouette", (0.012, 0.012, 0.014, 1), 0.9, 0.2)
    saved += import_meshes(rock) + [dark]
    assert call(AS, "save_assets", asset_paths=saved) is True
    cleanup()

    for section, cells in SECTIONS.items():
        place(f"{KIT}/SM_Dio_Base_{section}", f"Dio_Base_{section}", (0, 0, 0))
        fill = (1.0, 0.65, 0.4) if section == "Plant" else (0.55, 0.85, 0.5)
        for i, ((dx, dy), x, y, yaw) in enumerate(edges(section, cells)):
            if i % 2 == 0:
                light(f"Dio_Fill_{section}{i}", (x + dx * 350, y + dy * 350, -220), fill, 22, 900, 0.5)
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
        light(f"Dio_HaloSump{i}", (x, y, -1600), (0.45, 0.9, 0.4), 22, 2600, 4)

    for i, y in enumerate(range(6200, 11400, 1700)):
        fogvol(f"Dio_FogAmber{i}", (1000, y, -250), (11, 11, 6), (0.9, 0.4, 0.1))
    for i, x in enumerate(range(-2400, 0, 800)):
        fogvol(f"Dio_FogAmberLift{i}", (x, 12000, -250), (9, 9, 6), (0.8, 0.35, 0.08))
    for i, y in enumerate(range(6400, 10400, 1600)):
        fogvol(f"Dio_FogGreenGap{i}", (-800, y, -300), (8, 9, 5), (0.3, 0.8, 0.22), radial=0.2)
    for i, y in enumerate(range(6000, 10600, 1500)):
        fogvol(f"Dio_FogGreenLow{i}", (-3300, y, -700), (12, 12, 6), (0.1, 0.32, 0.08), radial=0.12)
    for i, (x, y) in enumerate([(-5500, 5000), (-5500, 9000), (-5500, 13000), (3000, 4500), (3000, 12500), (-1200, 3000), (-1200, 14000)]):
        fogvol(f"Dio_FogMurk{i}", (x, y, -400), (30, 30, 14), (0.01, 0.015, 0.02), radial=0.06)

    fog = call(S, "add_to_scene_from_class", actor_type={"refPath": "/Script/Engine.ExponentialHeightFog"}, name="Dio_Fog",
               xform={"location": {"x": -1200, "y": 8400, "z": -600}}, parent=None, snap_to_ground=False)
    ok = call(O, "set_properties", instance={"refPath": fog["refPath"] + ".HeightFogComponent0"}, values=json.dumps({
        "FogDensity": 0.035, "FogHeightFalloff": 0.35, "FogInscatteringLuminance": {"r": 0.0015, "g": 0.002, "b": 0.003, "a": 1},
        "bEnableVolumetricFog": True, "VolumetricFogDistance": 20000, "VolumetricFogScatteringDistribution": 0.5, "VolumetricFogExtinctionScale": 1.0}))
    assert ok is True, ok
    call(S, "set_actor_folder", actor=fog, folder_path="Diorama")
    print("PLACED", len(call(S, "find_actors", name="Dio_", tag="", collision_channels=[]) or []))


run()
