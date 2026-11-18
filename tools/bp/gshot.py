"""Abre una función de un BP en el editor y saca captura. Uso: gshot.py /Game/Path/BP Funcion out.png"""
import sys,time
from at import call
from sl import snap, click, sl
import ue,json,base64,re
bp,fn,out=sys.argv[1],sys.argv[2],sys.argv[3]
call("EditorToolset.EditorAppToolset","OpenEditorForAsset",assetPath=bp); time.sleep(1.5)
s=snap(200)
ref=[re.search(r"\[ref=(\w+)\]",l).group(1) for l in s.splitlines() if f'listitem "{fn}"' in l][0]
click(ref,dbl=True); time.sleep(1.0)
sl("PressKey",key="Home") if False else None
r=ue.call("tools/call",{"name":"call_tool","arguments":{"toolset_name":"EditorToolset.EditorAppToolset","tool_name":"CaptureEditorImage","arguments":{}}})
t="".join(c.get("text","") for c in r.get("result",r).get("content",[]))
open(out,"wb").write(base64.b64decode(re.search(r'"data"\s*:\s*"([A-Za-z0-9+/=]+)"',t).group(1)))
