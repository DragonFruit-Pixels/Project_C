import json
from at import call, S, A

O = "editor_toolset.toolsets.object.ObjectTools"
BASE = {"Batch": (0.65, 1, 0.55), "Dock": (1, 0.6, 0.3), "Gallery": (0.7, 0.9, 1), "Lab": (0.7, 1, 0.6),
        "Lift": (1, 0.55, 0.25), "Offices": (1, 0.68, 0.4), "Staff": (1, 0.66, 0.36)}
PLANT = ("Staff", "Dock", "Offices", "Lift")
WARMTH = {"Plant": 0.9, "Sump": 0.6}
INTENSITY = {"Plant": 20, "Sump": 16}


def run():
    n = 0
    for a in call(S, "find_actors", name="Light_", tag="", collision_channels=[]) or []:
        label = call(A, "get_label", actor=a)
        if "PointLight" not in a["refPath"] or label.startswith("Dio_"):
            continue
        room = label.split("_")[1].rstrip("0123456789")
        section = "Plant" if room in PLANT else "Sump"
        col = {k: 1 - WARMTH[section] * (1 - BASE[room][i]) for i, k in enumerate("rgb")}
        ok = call(O, "set_properties", instance={"refPath": a["refPath"] + ".LightComponent0"}, values=json.dumps({
            "LightColor": {**col, "a": 1}, "SourceRadius": 40, "Intensity": INTENSITY[section]}))
        assert ok is True, (label, ok)
        n += 1
    assert n == 20, n
    print("ROOM LIGHTS", n)


run()
