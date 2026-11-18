import json, ue
M = {"bShift": False, "bCtrl": False, "bAlt": False, "bCmd": False}
def sl(tool, **a):
    r = ue.call("tools/call", {"name": "call_tool", "arguments": {"toolset_name": "SlateInspectorToolset.SlateInspectorToolset", "tool_name": tool, "arguments": a}})
    return "".join(c.get("text", "") for c in r.get("result", r).get("content", []))
def snap(depth=60):
    return json.loads(sl("Snapshot", ref="", maxDepth=depth))["returnValue"]
def click(ref, button="left", dbl=False):
    return sl("Click", ref=ref, button=button, doubleClick=dbl, modifiers=M)
def find(text, s=None):
    s = s or snap()
    return [l.strip() for l in s.splitlines() if text in l]
