import json
from at import call, S, A

O = "editor_toolset.toolsets.object.ObjectTools"
AS = "editor_toolset.toolsets.asset.AssetTools"
MT = "editor_toolset.toolsets.material.MaterialTools"
LEVEL = "/Game/Project_C/Levels/Laboratorio"
OVERLAY = "/Game/Project_C/Map/Space/M_SpaceOverlay"
GLOW_SCALE = 0.05
SPACE = "/Game/Project_C/Map/Space/BP_Space.BP_Space_C"
HERO = "/Game/Project_C/Characters/Player/BP_{0}.BP_{0}_C"
COOK = "/Game/Project_C/Characters/Enemies/BP_Cook.BP_Cook_C"
SPACE_Z, UNIT_Z, YAW, SLOT = -7.5, 89.6, 180, 150
SPACE_YAW = 90
SPACES = {"Dock": (0, 7850), "Lockers": (0, 6050), "Decon": (0, 6800), "Control": (-25, 10000),
          "Office": (0, 10800), "Lift": (-1600, 10800), "Gallery": (-2400, 9200), "Vat": (-1775, 9150),
          "Cold": (-1575, 8400), "Cook": (-1600, 7600), "Batch": (-1700, 6250)}
EDGES = [("Dock", "Decon"), ("Dock", "Control"), ("Lockers", "Decon"), ("Control", "Office"),
         ("Office", "Lift"), ("Lift", "Gallery"), ("Gallery", "Cold"), ("Gallery", "Vat"), ("Vat", "Cold"),
         ("Cold", "Cook"), ("Cook", "Batch")]
HEROES = ("Shiv", "Brace", "Spike", "Maul")
COOKS = ("Cold", "Gallery", "Batch")


def cleanup():
    for prefix in ("Space_", "Unit_"):
        for a in call(S, "find_actors", name=prefix, tag="", collision_channels=[]) or []:
            if call(A, "get_label", actor=a).startswith(prefix):
                call(S, "remove_from_scene", actor=a)


def overlay_material():
    if call(AS, "exists", path=OVERLAY) is True:
        assert call(AS, "delete", path=OVERLAY) is True
    mat = call(MT, "create_material", folder_path=OVERLAY.rsplit("/", 1)[0], asset_name=OVERLAY.rsplit("/", 1)[1])
    assert call(O, "set_properties", instance=mat, values=json.dumps(
        {"BlendMode": "BLEND_Translucent", "ShadingModel": "MSM_Unlit"})) is True

    def e(cls, x, y, **props):
        node = call(MT, "add_expression", material_or_function=mat,
                    expression_class={"refPath": f"/Script/Engine.MaterialExpression{cls}"}, x=x, y=y)
        assert isinstance(node, dict), node
        if props:
            assert call(O, "set_properties", instance=node, values=json.dumps(props)) is True, (cls, props)
        return node

    def link(a, b, inp):
        assert call(MT, "connect_expressions", from_expression=a, from_output_name="", to_expression=b,
                    to_input_name=inp) is not False

    colour = e("VectorParameter", -800, -200, ParameterName="Colour", DefaultValue={"r": 0.22, "g": 0.23, "b": 0.26, "a": 1})
    glow = e("ScalarParameter", -800, 0, ParameterName="Glow", DefaultValue=0.0)
    mix = e("ScalarParameter", -800, 200, ParameterName="HighlightMix", DefaultValue=0.0)
    scale = e("Constant", -800, 100, R=GLOW_SCALE)
    bias = e("Add", -600, 0, ConstB=1.0)
    lit = e("Multiply", -450, 0)
    out = e("Multiply", -300, -100)
    link(glow, bias, "A")
    link(bias, lit, "A")
    link(scale, lit, "B")
    link(colour, out, "A")
    link(lit, out, "B")
    assert call(MT, "connect_to_output", expression=out, output_name="", material_property="MP_EmissiveColor") is not False
    assert call(MT, "connect_to_output", expression=mix, output_name="", material_property="MP_Opacity") is not False
    call(MT, "recompile", material_or_function=mat)
    assert call(AS, "save_assets", asset_paths=[OVERLAY]) is True


def spawn(cls, label, x, y, z, yaw):
    a = call(S, "add_to_scene_from_class", actor_type={"refPath": cls}, name=label,
             xform={"location": {"x": x, "y": y, "z": z}, "rotation": {"pitch": 0, "yaw": yaw, "roll": 0}},
             parent=None, snap_to_ground=False)
    assert isinstance(a, dict), (label, a)
    call(A, "set_label", actor=a, label=label)
    call(S, "set_actor_folder", actor=a, folder_path="Board")
    return a


def run():
    cleanup()
    assert call(AS, "save_assets", asset_paths=[LEVEL]) is True
    overlay_material()
    spaces = {n: spawn(SPACE, f"Space_{n}", x, y, SPACE_Z, SPACE_YAW) for n, (x, y) in SPACES.items()}
    for n, a in spaces.items():
        ok = call(O, "set_properties", instance={"refPath": a["refPath"] + ".Tile"}, values=json.dumps(
            {"OverrideMaterials": [{"refPath": f"{OVERLAY}.{OVERLAY.rsplit('/', 1)[1]}"}], "CastShadow": False}))
        assert ok is True, (n, ok)
    links = {n: [] for n in SPACES}
    for a, b in EDGES:
        links[a].append(spaces[b])
        links[b].append(spaces[a])
    for n, refs in links.items():
        assert call(O, "set_properties", instance=spaces[n], values=json.dumps({"Neighbours": refs})) is True, n
        got = json.loads(call(O, "get_properties", instance=spaces[n], properties=["Neighbours"]))["Neighbours"]
        assert sorted(r["refPath"] for r in got) == sorted(r["refPath"] for r in refs), (n, got)
    x, y = SPACES["Dock"]
    for i, h in enumerate(HEROES):
        spawn(HERO.format(h), f"Unit_{h}", x, y + (i - 1.5) * SLOT, UNIT_Z, YAW)
    for i, room in enumerate(COOKS, 1):
        sx, sy = SPACES[room]
        spawn(COOK, f"Unit_Cook{i}", sx, sy, UNIT_Z, YAW)
    assert call(AS, "save_assets", asset_paths=[LEVEL]) is True
    print("BOARD OK", len(spaces), "spaces", len(EDGES), "edges", len(HEROES), "heroes", len(COOKS), "cooks")


assert len(EDGES) == 11 and all(a in SPACES and b in SPACES for a, b in EDGES)
run()
