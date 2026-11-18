import json,base64,re,ue,sys
from at import call
def cap(out):
    res=ue.call("tools/call",{"name":"call_tool","arguments":{"toolset_name":"EditorToolset.EditorAppToolset","tool_name":"CaptureEditorImage","arguments":{}}})
    s="".join(c.get("text","") for c in res.get("result",res).get("content",[]))
    m=re.search(r'"data":\s*"([A-Za-z0-9+/=]+)"',s)
    open(out,'wb').write(base64.b64decode(m.group(1)))
def con(cmd):
    from sl import snap
    ls=snap(120).splitlines(); ref="tb2"
    for i,l in enumerate(ls):
        if 'text "Cmd"' in l:
            for j in range(i+1,min(i+6,len(ls))):
                m=re.search(r"textbox .*\[ref=(tb\d+)\]",ls[j])
                if m: ref=m.group(1); break
    from sl import click
    click(ref)
    return call("SlateInspectorToolset.SlateInspectorToolset","Type",ref=ref,text=cmd,submit=True)
