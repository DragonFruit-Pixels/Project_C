import json, sys
import ue

TS = {"Blueprint": "editor_toolset.toolsets.blueprint.BlueprintTools",
      "Asset": "editor_toolset.toolsets.asset.AssetTools",
      "Object": "editor_toolset.toolsets.object.ObjectTools",
      "Editor": "EditorToolset.EditorAppToolset",
      "Logs": "EditorToolset.LogsToolset", "Scene": "editor_toolset.toolsets.scene.SceneTools"}


def t(_tool, **args):
    ts, tool = _tool.split(".", 1)
    res = ue.call("tools/call", {"name": "call_tool", "arguments": {"toolset_name": TS[ts], "tool_name": tool, "arguments": args}})
    out = res.get("result", res)
    text = "".join(c.get("text", "") for c in out.get("content", []))
    try:
        return json.loads(text).get("returnValue", text)
    except (ValueError, AttributeError):
        return text


def ref(p):
    return {"refPath": p}


BPC = "/Game/Project_C/Characters/BP_Character.BP_Character"
ABP = "/Game/Project_C/Characters/ABP_Character.ABP_Character"


def dsl(graph):
    return t("Blueprint.read_graph_dsl", graph=ref(graph))


def write(graph, code):
    return t("Blueprint.write_graph_dsl", graph=ref(graph), code=code)


if __name__ == "__main__":
    sys.stdout.reconfigure(encoding="utf-8")
    exec(sys.stdin.read())
