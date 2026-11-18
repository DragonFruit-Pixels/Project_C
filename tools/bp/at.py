import json, ue
def call(ts, tool, **args):
    res = ue.call("tools/call", {"name": "call_tool", "arguments": {"toolset_name": ts, "tool_name": tool, "arguments": args}})
    txt = "".join(c.get("text", "") for c in res.get("result", res).get("content", []))
    try: return json.loads(txt).get("returnValue", txt)
    except Exception: return txt
A = "editor_toolset.toolsets.actor.ActorTools"
S = "editor_toolset.toolsets.scene.SceneTools"
M = "editor_toolset.toolsets.static_mesh.StaticMeshTools"
