import json
from at import call, M

MT = "editor_toolset.toolsets.material.MaterialTools"
MI = "editor_toolset.toolsets.material_instance.MaterialInstanceTools"
TT = "editor_toolset.toolsets.texture.TextureTools"
O = "editor_toolset.toolsets.object.ObjectTools"
AS = "editor_toolset.toolsets.asset.AssetTools"
DIR = "/Game/Project_C/Map/Materials"
TEXDIR = "/Game/Project_C/Map/Textures"
SRC = "C:/Users/sebas/UnrealProjects/Project_C/assets/3d/kit/textures"
saved = []


def ref(p):
    return {"refPath": f"{p}.{p.split('/')[-1]}"}


def fresh(name):
    p = f"{DIR}/{name}"
    if call(AS, "exists", path=p) is True:
        assert call(AS, "delete", path=p) is True, p
    m = call(MT, "create_material", folder_path=DIR, asset_name=name)
    saved.append(p)
    return m


class G:
    def __init__(self, mat):
        self.mat = mat

    def e(self, cls, x, y, **props):
        e = call(MT, "add_expression", material_or_function=self.mat,
                 expression_class={"refPath": f"/Script/Engine.MaterialExpression{cls}"}, x=x, y=y)
        assert isinstance(e, dict), e
        if props:
            ok = call(O, "set_properties", instance=e, values=json.dumps(props))
            assert ok is True, (cls, props, ok)
        return e

    def link(self, a, out, b, inp):
        r = call(MT, "connect_expressions", from_expression=a, from_output_name=out, to_expression=b, to_input_name=inp)
        assert r is not False, (out, inp, r)

    def out(self, e, out, prop):
        r = call(MT, "connect_to_output", expression=e, output_name=out, material_property=prop)
        assert r is not False, (out, prop, r)

    def done(self):
        call(MT, "recompile", material_or_function=self.mat)


def tex(p):
    return f"/Script/Engine.Texture2D'{p}.{p.split('/')[-1]}'"


room = G(fresh("M_Room"))
floor = room.e("TextureSampleParameter2D", -900, -300, ParameterName="FloorTex", Texture=tex("/Engine/EngineMaterials/DefaultDiffuse"))
wall = room.e("TextureSampleParameter2D", -900, -50, ParameterName="WallTex", Texture=tex("/Engine/EngineMaterials/DefaultDiffuse"))
top = room.e("VectorParameter", -900, 200, ParameterName="TopColor", DefaultValue={"r": 0.08, "g": 0.08, "b": 0.09, "a": 1})
nrm = room.e("VertexNormalWS", -1300, 400)
nz = room.e("ComponentMask", -1150, 400, R=False, G=False, B=True, A=False)
up = room.e("Subtract", -1000, 400, ConstB=0.5)
upk = room.e("Multiply", -850, 400, ConstB=10.0)
upm = room.e("Saturate", -700, 400)
wp = room.e("WorldPosition", -1300, 600)
wz = room.e("ComponentMask", -1150, 600, R=False, G=False, B=True, A=False)
low = room.e("Subtract", -1000, 600, ConstA=20.0)
lowk = room.e("Multiply", -850, 600, ConstB=0.1)
lowm = room.e("Saturate", -700, 600)
fmask = room.e("Multiply", -550, 450)
high = room.e("OneMinus", -550, 650)
tmask = room.e("Multiply", -400, 600)
lerp1 = room.e("LinearInterpolate", -350, -150)
lerp2 = room.e("LinearInterpolate", -150, 0)
rough = room.e("ScalarParameter", -350, 300, ParameterName="Roughness", DefaultValue=0.85)
tc = room.e("TextureCoordinate", -1400, -200)
ft = room.e("ScalarParameter", -1400, -100, ParameterName="FloorTiling", DefaultValue=0.5)
wt = room.e("ScalarParameter", -1400, 50, ParameterName="WallTiling", DefaultValue=1.0)
fuv = room.e("Multiply", -1150, -250)
wuv = room.e("Multiply", -1150, -50)
room.link(tc, "", fuv, "A")
room.link(ft, "", fuv, "B")
room.link(tc, "", wuv, "A")
room.link(wt, "", wuv, "B")
room.link(fuv, "", floor, "UVs")
room.link(wuv, "", wall, "UVs")
room.link(nrm, "", nz, "")
room.link(nz, "", up, "A")
room.link(up, "", upk, "A")
room.link(upk, "", upm, "")
room.link(wp, "", wz, "")
room.link(wz, "", low, "B")
room.link(low, "", lowk, "A")
room.link(lowk, "", lowm, "")
room.link(upm, "", fmask, "A")
room.link(lowm, "", fmask, "B")
room.link(lowm, "", high, "")
room.link(upm, "", tmask, "A")
room.link(high, "", tmask, "B")
room.link(wall, "RGB", lerp1, "A")
room.link(floor, "RGB", lerp1, "B")
room.link(fmask, "", lerp1, "Alpha")
room.link(lerp1, "", lerp2, "A")
room.link(top, "", lerp2, "B")
room.link(tmask, "", lerp2, "Alpha")
room.out(lerp2, "", "MP_BaseColor")
room.out(rough, "", "MP_Roughness")
room.done()

solid = G(fresh("M_KitSolid"))
col = solid.e("VectorParameter", -500, -100, ParameterName="Color", DefaultValue={"r": 0.2, "g": 0.2, "b": 0.2, "a": 1})
rgh = solid.e("ScalarParameter", -500, 100, ParameterName="Roughness", DefaultValue=0.6)
met = solid.e("ScalarParameter", -500, 200, ParameterName="Metallic", DefaultValue=0.5)
emc = solid.e("VectorParameter", -500, 320, ParameterName="Emissive", DefaultValue={"r": 0, "g": 0, "b": 0, "a": 1})
solid.out(col, "", "MP_BaseColor")
solid.out(rgh, "", "MP_Roughness")
solid.out(met, "", "MP_Metallic")
solid.out(emc, "", "MP_EmissiveColor")
solid.done()

for n in ["floor_plant", "wall_plant", "floor_sump", "wall_sump"]:
    p = f"{TEXDIR}/T_Kit_{n}"
    if call(AS, "exists", path=p) is True:
        call(AS, "delete", path=p)
    r = call(TT, "import_file", folder_path=TEXDIR, asset_name=f"T_Kit_{n}", source_file=f"{SRC}/T_Kit_{n}.jpg")
    assert isinstance(r, list) and r, (n, r)
    saved.append(p)


def mi(name, parent, textures=None, vectors=None, scalars=None):
    p = f"{DIR}/{name}"
    if call(AS, "exists", path=p) is True:
        assert call(AS, "delete", path=p) is True, p
    inst = call(MI, "create", folder_path=DIR, asset_name=name, parent=ref(f"{DIR}/{parent}"))
    for k, v in (textures or {}).items():
        call(MI, "set_texture_parameter", instance=inst, name=k, value=ref(f"{TEXDIR}/{v}"))
    for k, v in (vectors or {}).items():
        call(MI, "set_vector_parameter", instance=inst, name=k, value=dict(zip("rgba", v)))
    for k, v in (scalars or {}).items():
        call(MI, "set_scalar_parameter", instance=inst, name=k, value=v)
    saved.append(p)
    return ref(p)


plant = mi("MI_Room_Plant", "M_Room", {"FloorTex": "T_Kit_floor_plant", "WallTex": "T_Kit_wall_plant"})
sump = mi("MI_Room_Sump", "M_Room", {"FloorTex": "T_Kit_floor_sump", "WallTex": "T_Kit_wall_sump"},
          {"TopColor": (0.07, 0.04, 0.06, 1)})
trim = mi("MI_Kit_Trim", "M_KitSolid", vectors={"Color": (0.09, 0.09, 0.1, 1)}, scalars={"Roughness": 0.5, "Metallic": 0.8})
pipe = mi("MI_Kit_Pipe", "M_KitSolid", vectors={"Color": (0.32, 0.17, 0.09, 1)}, scalars={"Roughness": 0.65, "Metallic": 0.7})
glow = mi("MI_Kit_Glow", "M_KitSolid", vectors={"Color": (1, 0.9, 0.7, 1), "Emissive": (6, 4.6, 2.8, 1)}, scalars={"Metallic": 0})
hatch = mi("MI_Kit_Hatch", "M_KitSolid", vectors={"Color": (0.05, 0.05, 0.05, 1)}, scalars={"Roughness": 0.4, "Metallic": 0.9})

SLOT = {"M_Kit_Trim": trim, "M_Kit_Pipe": pipe, "M_Kit_Glow": glow, "M_Kit_Hatch": hatch,
        "M_Kit_Wall": plant, "M_Kit_Floor": plant}
ROOM = {"Staff": plant, "Dock": plant, "Offices": plant, "Lift": plant, "Gallery": sump, "Lab": sump, "Batch": sump}
for p in call(AS, "find_assets", folder_path="/Game/Project_C/Map/Kit", name=""):
    mesh = {"refPath": p} if "." in p else ref(p)
    for slot in call(M, "get_material_slots", mesh=mesh):
        assert call(M, "set_material", mesh=mesh, slot_name=slot, material=SLOT[slot]) is True, (p, slot)
    saved.append(mesh["refPath"].split(".")[0])
for name, m in ROOM.items():
    p = f"/Game/Project_C/Map/Rooms/SM_Room_{name}"
    for slot in call(M, "get_material_slots", mesh=ref(p)):
        assert call(M, "set_material", mesh=ref(p), slot_name=slot, material=m) is True, (p, slot)
    saved.append(p)
print("SAVE", call(AS, "save_assets", asset_paths=saved), len(saved))
