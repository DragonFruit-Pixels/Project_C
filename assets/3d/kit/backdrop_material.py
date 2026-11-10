import json, os
from at import call, M

MT = "editor_toolset.toolsets.material.MaterialTools"
TT = "editor_toolset.toolsets.texture.TextureTools"
O = "editor_toolset.toolsets.object.ObjectTools"
AS = "editor_toolset.toolsets.asset.AssetTools"
ROOT = os.path.dirname(os.path.abspath(__file__)).replace("\\", "/")
KIT, MAT, TEX = "/Game/Project_C/Map/Kit", "/Game/Project_C/Map/Materials", "/Game/Project_C/Map/Textures"
INTENSITY = 0.04


def ref(p):
    return {"refPath": f"{p}.{p.split('/')[-1]}"}


def fresh(p):
    if call(AS, "exists", path=p) is True:
        assert call(AS, "delete", path=p) is True, p


fresh(f"{TEX}/T_Backdrop")
assert call(TT, "import_file", folder_path=TEX, asset_name="T_Backdrop", source_file=f"{ROOT}/textures/T_Backdrop.jpg")
assert call(O, "set_properties", instance=ref(f"{TEX}/T_Backdrop"), values=json.dumps({"MaxTextureSize": 4096})) is True

fresh(f"{MAT}/M_Backdrop")
mat = call(MT, "create_material", folder_path=MAT, asset_name="M_Backdrop")
assert call(O, "set_properties", instance=mat, values=json.dumps({"ShadingModel": "MSM_Unlit"})) is True
tex = call(MT, "add_expression", material_or_function=mat,
           expression_class={"refPath": "/Script/Engine.MaterialExpressionTextureSampleParameter2D"}, x=-600, y=0)
assert call(O, "set_properties", instance=tex, values=json.dumps(
    {"ParameterName": "Paint", "Texture": f"/Script/Engine.Texture2D'{TEX}/T_Backdrop.T_Backdrop'"})) is True
gain = call(MT, "add_expression", material_or_function=mat,
            expression_class={"refPath": "/Script/Engine.MaterialExpressionScalarParameter"}, x=-600, y=250)
assert call(O, "set_properties", instance=gain, values=json.dumps({"ParameterName": "Intensity", "DefaultValue": INTENSITY})) is True
mul = call(MT, "add_expression", material_or_function=mat,
           expression_class={"refPath": "/Script/Engine.MaterialExpressionMultiply"}, x=-300, y=100)
for a, out, b, inp in ((tex, "RGB", mul, "A"), (gain, "", mul, "B")):
    assert call(MT, "connect_expressions", from_expression=a, from_output_name=out, to_expression=mul, to_input_name=inp) is not False
assert call(MT, "connect_to_output", expression=mul, output_name="", material_property="MP_EmissiveColor") is not False
call(MT, "recompile", material_or_function=mat)

mesh = f"{KIT}/SM_Dio_Backdrop"
fresh(mesh)
r = call(M, "import_file", folder_path=KIT, asset_name="SM_Dio_Backdrop", source_file=f"{ROOT}/SM_Dio_Backdrop.fbx",
         import_materials=False, import_textures=False, combine_meshes=True)
assert isinstance(r, list) and r, r
for slot in call(M, "get_material_slots", mesh=ref(mesh)):
    assert call(M, "set_material", mesh=ref(mesh), slot_name=slot, material=ref(f"{MAT}/M_Backdrop")) is True
assert call(M, "remove_collisions", mesh=ref(mesh)) is True
assert call(AS, "save_assets", asset_paths=[f"{TEX}/T_Backdrop", f"{MAT}/M_Backdrop", mesh]) is True
print("BACKDROP OK")
