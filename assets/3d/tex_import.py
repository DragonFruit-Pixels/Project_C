import json, os, sys
from at import call, M, S

AS = "editor_toolset.toolsets.asset.AssetTools"
O = "editor_toolset.toolsets.object.ObjectTools"
MI = "editor_toolset.toolsets.material_instance.MaterialInstanceTools"
TT = "editor_toolset.toolsets.texture.TextureTools"
ROOT = os.path.dirname(os.path.abspath(__file__))
TEXDIR = os.path.join(ROOT, "textures")
MASTER = {"refPath": "/Game/Project_C/Map/Materials/M_Prop.M_Prop"}
MATDIR, TEXOUT = "/Game/Project_C/Map/Materials", "/Game/Project_C/Map/Textures"
KINDS = {"BC": ("BaseColor", {}), "N": ("Normal", {"CompressionSettings": "TC_Normalmap", "SRGB": False}),
         "RM": ("RM", {"CompressionSettings": "TC_Masks", "SRGB": False}), "E": ("Emissive", {})}
TARGETS = [("props", "SM_Prop_", "/Game/Project_C/Map/Props"), ("fill", "SM_Fill_", "/Game/Project_C/Map/Fill"),
           ("kit", "SM_Dio_", "/Game/Project_C/Map/Kit")]


def ref(path):
    return {"refPath": f"{path}.{path.split('/')[-1]}"}


def target(name):
    for folder, prefix, dst in TARGETS:
        fbx = os.path.join(ROOT, folder, prefix + name + ".fbx")
        if os.path.exists(fbx):
            return fbx, f"{dst}/{prefix}{name}"
    raise FileNotFoundError(name)


def users():
    out = {}
    for prefix in ("Prop_", "Fill_"):
        for a in call(S, "find_actors", name=prefix, tag="", collision_channels=[]) or []:
            if "StaticMeshActor" not in a["refPath"]:
                continue
            comp = {"refPath": a["refPath"] + ".StaticMeshComponent0"}
            mesh = json.loads(call(O, "get_properties", instance=comp, properties=["StaticMesh"]))["StaticMesh"]
            out.setdefault(mesh["refPath"], []).append(comp)
    return out


def run(name, placed):
    fbx, mesh_path = target(name)
    comps = placed.get(ref(mesh_path)["refPath"], [])
    if call(AS, "exists", path=mesh_path) is True:
        assert call(AS, "delete", path=mesh_path) is True, (name, "delete failed")
    r = call(M, "import_file", folder_path=mesh_path.rsplit("/", 1)[0], asset_name=mesh_path.rsplit("/", 1)[1],
             source_file=fbx.replace("\\", "/"), import_materials=False, import_textures=False, combine_meshes=True)
    assert isinstance(r, list) and r, (name, r)
    for c in comps:
        ok = call(O, "set_properties", instance=c, values=json.dumps({"StaticMesh": ref(mesh_path)}))
        assert ok is True, (name, c, ok)
    mi_path = f"{MATDIR}/MI_{name}"
    mi = ref(mi_path) if call(AS, "exists", path=mi_path) is True else call(MI, "create", folder_path=MATDIR,
                                                                            asset_name=f"MI_{name}", parent=MASTER)
    saved = [mesh_path, mi_path]
    for suffix, (param, props) in KINDS.items():
        src = os.path.join(TEXDIR, f"T_{name}_{suffix}.jpg")
        if not os.path.exists(src):
            continue
        tpath = f"{TEXOUT}/T_{name}_{suffix}"
        if call(AS, "exists", path=tpath) is True:
            assert call(AS, "delete", path=tpath) is True, (name, suffix, "delete failed")
        t = call(TT, "import_file", folder_path=TEXOUT, asset_name=f"T_{name}_{suffix}", source_file=src.replace("\\", "/"))
        assert isinstance(t, list) and t, (name, suffix, t)
        if props:
            ok = call(O, "set_properties", instance=ref(tpath), values=json.dumps(props))
            assert ok is True, (name, suffix, ok)
        call(MI, "set_texture_parameter", instance=mi, name=param, value=ref(tpath))
        saved.append(tpath)
    for slot in call(M, "get_material_slots", mesh=ref(mesh_path)):
        assert call(M, "set_material", mesh=ref(mesh_path), slot_name=slot, material=mi) is True, (name, slot)
    assert call(AS, "save_assets", asset_paths=saved) is True, name
    print("OK", name, len(saved) - 2, "maps", len(comps), "actors")


placed = users()
for n in sys.argv[1:]:
    run(n, placed)
