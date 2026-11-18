import json, os
from at import call

MT = "editor_toolset.toolsets.material.MaterialTools"
MI = "editor_toolset.toolsets.material_instance.MaterialInstanceTools"
TT = "editor_toolset.toolsets.texture.TextureTools"
O = "editor_toolset.toolsets.object.ObjectTools"
AS = "editor_toolset.toolsets.asset.AssetTools"
DIR, TEX = "/Game/Project_C/Map/Materials", "/Game/Project_C/Map/Textures"
SRC = os.path.join(os.path.dirname(os.path.abspath(__file__)), "textures", "T_FogCloud.png").replace("\\", "/")
CARDS = {"MI_Fog_Amber": ((1.0, 0.45, 0.12), 0.08, 1.0), "MI_Fog_Green": ((0.35, 0.85, 0.25), 0.04, 1.0)}


def ref(p):
    return {"refPath": f"{p}.{p.split('/')[-1]}"}


def fresh(path):
    if call(AS, "exists", path=path) is True:
        assert call(AS, "delete", path=path) is True, path


fresh(f"{TEX}/T_FogCloud")
assert call(TT, "import_file", folder_path=TEX, asset_name="T_FogCloud", source_file=SRC)
assert call(O, "set_properties", instance=ref(f"{TEX}/T_FogCloud"),
            values=json.dumps({"CompressionSettings": "TC_Grayscale", "SRGB": False})) is True

fresh(f"{DIR}/M_FogCard")
mat = call(MT, "create_material", folder_path=DIR, asset_name="M_FogCard")
assert call(O, "set_properties", instance=mat, values=json.dumps(
    {"BlendMode": "BLEND_Additive", "ShadingModel": "MSM_Unlit", "TwoSided": True})) is True


def e(cls, x, y, **props):
    node = call(MT, "add_expression", material_or_function=mat,
                expression_class={"refPath": f"/Script/Engine.MaterialExpression{cls}"}, x=x, y=y)
    assert isinstance(node, dict), node
    if props:
        assert call(O, "set_properties", instance=node, values=json.dumps(props)) is True, (cls, props)
    return node


def link(a, out, b, inp):
    assert call(MT, "connect_expressions", from_expression=a, from_output_name=out, to_expression=b, to_input_name=inp) is not False


tex = e("TextureSampleParameter2D", -900, 0, ParameterName="Cloud", SamplerType="SAMPLERTYPE_Grayscale",
        Texture=f"/Script/Engine.Texture2D'{TEX}/T_FogCloud.T_FogCloud'")
color = e("VectorParameter", -900, -250, ParameterName="Color", DefaultValue={"r": 1, "g": 1, "b": 1, "a": 1})
glow = e("ScalarParameter", -900, -120, ParameterName="Intensity", DefaultValue=2.0)
opac = e("ScalarParameter", -900, 250, ParameterName="Opacity", DefaultValue=0.6)
fade = e("DepthFade", -300, 250, FadeDistanceDefault=250.0)
c1 = e("Multiply", -600, -200)
c2 = e("Multiply", -400, -150)
o1 = e("Multiply", -600, 200)
link(color, "", c1, "A")
link(glow, "", c1, "B")
link(tex, "R", o1, "A")
link(opac, "", o1, "B")
link(o1, "", fade, "Opacity")
link(c1, "", c2, "A")
link(fade, "", c2, "B")
assert call(MT, "connect_to_output", expression=c2, output_name="", material_property="MP_EmissiveColor") is not False
call(MT, "recompile", material_or_function=mat)

saved = [f"{TEX}/T_FogCloud", f"{DIR}/M_FogCard"]
for name, (col, glow, alpha) in CARDS.items():
    fresh(f"{DIR}/{name}")
    inst = call(MI, "create", folder_path=DIR, asset_name=name, parent=ref(f"{DIR}/M_FogCard"))
    call(MI, "set_vector_parameter", instance=inst, name="Color", value=dict(zip("rgba", (*col, 1))))
    call(MI, "set_scalar_parameter", instance=inst, name="Intensity", value=glow)
    call(MI, "set_scalar_parameter", instance=inst, name="Opacity", value=alpha)
    saved.append(f"{DIR}/{name}")
assert call(AS, "save_assets", asset_paths=saved) is True
print("FOG MATERIAL OK")
