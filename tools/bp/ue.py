import json, sys, urllib.request

URL = "http://127.0.0.1:8000/mcp"
SID_FILE = __file__ + ".sid"


def post(payload, sid=None):
    headers = {"Content-Type": "application/json", "Accept": "application/json, text/event-stream"}
    if sid:
        headers["Mcp-Session-Id"] = sid
    req = urllib.request.Request(URL, json.dumps(payload).encode(), headers)
    with urllib.request.urlopen(req, timeout=300) as r:
        body = r.read().decode("utf-8", "replace")
        new_sid = r.headers.get("Mcp-Session-Id")
    msgs = []
    for line in body.splitlines():
        if line.startswith("data:"):
            msgs.append(json.loads(line[5:]))
    if not msgs and body.strip():
        msgs.append(json.loads(body))
    return msgs, new_sid


def session():
    try:
        return open(SID_FILE).read().strip()
    except OSError:
        pass
    msgs, sid = post({"jsonrpc": "2.0", "id": 0, "method": "initialize", "params": {
        "protocolVersion": "2025-03-26", "capabilities": {}, "clientInfo": {"name": "ue-cli", "version": "1"}}})
    post({"jsonrpc": "2.0", "method": "notifications/initialized"}, sid)
    if sid:
        open(SID_FILE, "w").write(sid)
    return sid


def call(method, params):
    sid = session()
    try:
        msgs, _ = post({"jsonrpc": "2.0", "id": 1, "method": method, "params": params}, sid)
    except urllib.error.HTTPError as e:
        if e.code in (400, 404):
            import os
            os.remove(SID_FILE)
            msgs, _ = post({"jsonrpc": "2.0", "id": 1, "method": method, "params": params}, session())
        else:
            raise
    return msgs[-1] if msgs else None


if __name__ == "__main__":
    if sys.argv[1] == "list":
        res = call("tools/list", {})
        for t in res["result"]["tools"]:
            if len(sys.argv) < 3 or sys.argv[2].lower() in (t["name"] + t.get("description", "")).lower():
                print(t["name"], "::", t.get("description", "")[:300].replace("\n", " "))
                if len(sys.argv) > 3:
                    print(json.dumps(t.get("inputSchema"), indent=1))
    else:
        raw = sys.stdin.read() if len(sys.argv) > 2 and sys.argv[2] == "-" else (sys.argv[2] if len(sys.argv) > 2 else "{}")
        args = json.loads(raw)
        name = sys.argv[1]
        if "." in name:
            toolset, tool = name.rsplit(".", 1)
            prefixes = {"Blueprint": "editor_toolset.toolsets.blueprint.BlueprintTools",
                        "Asset": "editor_toolset.toolsets.asset.AssetTools",
                        "Object": "editor_toolset.toolsets.object.ObjectTools",
                        "SkeletalMesh": "editor_toolset.toolsets.skeletal_mesh.SkeletalMeshTools",
                        "Scene": "editor_toolset.toolsets.scene.SceneTools",
                        "Editor": "EditorToolset.EditorAppToolset",
                        "Logs": "EditorToolset.LogsToolset"}
            args = {"toolset_name": prefixes.get(toolset, toolset), "tool_name": tool, "arguments": args}
            name = "call_tool"
        res = call("tools/call", {"name": name, "arguments": args})
        out = res.get("result", res)
        for c in out.get("content", []) if isinstance(out, dict) else []:
            print(c.get("text", c))
        if not (isinstance(out, dict) and out.get("content")):
            print(json.dumps(out, indent=1))
