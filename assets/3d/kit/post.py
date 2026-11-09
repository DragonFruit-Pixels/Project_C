import json, os
from at import call

O = "editor_toolset.toolsets.object.ObjectTools"
AS = "editor_toolset.toolsets.asset.AssetTools"
TT = "editor_toolset.toolsets.texture.TextureTools"
TEX = "/Game/Project_C/Map/Textures"
LUT = f"{TEX}/T_GradeLUT"
SRC = os.path.join(os.path.dirname(os.path.abspath(__file__)), "textures", "T_GradeLUT.png").replace("\\", "/")
PPV = "/Game/Project_C/Levels/Laboratorio.Laboratorio:PersistentLevel.PostProcessVolume_0"
GRADE = {"vignetteIntensity": 0.55, "bloomIntensity": 0.8, "colorGradingIntensity": 1.0}

if call(AS, "exists", path=LUT) is True:
    assert call(AS, "delete", path=LUT) is True
assert call(TT, "import_file", folder_path=TEX, asset_name="T_GradeLUT", source_file=SRC)
lut = {"refPath": f"{LUT}.T_GradeLUT"}
assert call(O, "set_properties", instance=lut, values=json.dumps(
    {"LODGroup": "TEXTUREGROUP_ColorLookupTable", "CompressionSettings": "TC_VectorDisplacementmap"})) is True
assert call(AS, "save_assets", asset_paths=[LUT]) is True

s = json.loads(call(O, "get_properties", instance={"refPath": PPV}, properties=["Settings"]))["Settings"]
s.update(GRADE)
s["colorGradingLUT"] = lut
for k in ("vignetteIntensity", "bloomIntensity", "colorGradingIntensity", "colorGradingLUT"):
    s["bOverride_" + k[0].upper() + k[1:]] = True
assert call(O, "set_properties", instance={"refPath": PPV}, values=json.dumps({"Settings": s})) is True
back = json.loads(call(O, "get_properties", instance={"refPath": PPV}, properties=["Settings"]))["Settings"]
assert back["autoExposureMinBrightness"] == -2.5 and back["bOverride_ColorGradingLUT"], back.get("colorGradingLUT")
print("POST OK", back["colorGradingLUT"], back["vignetteIntensity"])
