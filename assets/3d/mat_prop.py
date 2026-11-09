import json
from at import call

MT = "editor_toolset.toolsets.material.MaterialTools"
O = "editor_toolset.toolsets.object.ObjectTools"
AS = "editor_toolset.toolsets.asset.AssetTools"
DIR = "/Game/Project_C/Map/Materials"
NAME = "M_Prop"
path = f"{DIR}/{NAME}.{NAME}"

if call(AS, "exists", path=f"{DIR}/{NAME}") is True:
    assert call(AS, "delete", path=f"{DIR}/{NAME}") is True
mat = call(MT, "create_material", folder_path=DIR, asset_name=NAME)
print("MAT", mat)


def expr(cls, x, y, **props):
    e = call(MT, "add_expression", material_or_function=mat,
             expression_class={"refPath": f"/Script/Engine.MaterialExpression{cls}"}, x=x, y=y)
    assert isinstance(e, dict), e
    if props:
        ok = call(O, "set_properties", instance=e, values=json.dumps(props))
        assert ok is True, (cls, props, ok)
    return e


def link(a, out, b, inp):
    r = call(MT, "connect_expressions", from_expression=a, from_output_name=out, to_expression=b, to_input_name=inp)
    assert r is not False, (out, inp, r)


def to_out(e, out, prop):
    r = call(MT, "connect_to_output", expression=e, output_name=out, material_property=prop)
    assert r is not False, (out, prop, r)


def tex(p):
    return f"/Script/Engine.Texture2D'{p}.{p.split('/')[-1]}'"
bc = expr("TextureSampleParameter2D", -700, -200, ParameterName="BaseColor",
          Texture=tex("/Engine/EngineMaterials/DefaultDiffuse"))
tint = expr("VectorParameter", -700, -20, ParameterName="Tint", DefaultValue={"r": 1, "g": 1, "b": 1, "a": 1})
mul = expr("Multiply", -350, -150)
nrm = expr("TextureSampleParameter2D", -700, 150, ParameterName="Normal", SamplerType="SAMPLERTYPE_Normal",
           Texture=tex("/Engine/EngineMaterials/DefaultNormal"))
rm = expr("TextureSampleParameter2D", -700, 400, ParameterName="RM", SamplerType="SAMPLERTYPE_LinearColor",
          Texture=tex("/Engine/EngineResources/WhiteSquareTexture"))
em = expr("TextureSampleParameter2D", -700, 650, ParameterName="Emissive",
          Texture=tex("/Engine/EngineResources/Black"))
ems = expr("ScalarParameter", -700, 850, ParameterName="EmissiveStrength", DefaultValue=1.0)
emul = expr("Multiply", -350, 700)

print("BC outs", call(MT, "get_expression_output_names", expression=bc))
print("MUL ins", call(MT, "get_expression_input_names", expression=mul))
link(bc, "RGB", mul, "A")
link(tint, "", mul, "B")
to_out(mul, "", "MP_BaseColor")
to_out(nrm, "RGB", "MP_Normal")
to_out(rm, "G", "MP_Roughness")
to_out(rm, "B", "MP_Metallic")
link(em, "RGB", emul, "A")
link(ems, "", emul, "B")
to_out(emul, "", "MP_EmissiveColor")
print("COMPILE", call(MT, "recompile", material_or_function=mat))
print("SAVE", call(AS, "save_assets", asset_paths=[f"{DIR}/{NAME}"]))
