"""Abre un BP, entra al grafo de una función y deja el ref del panel en panel.txt. Uso: gopen.py /Game/Path/BP Funcion"""
import sys,time,re
from at import call
from sl import snap, click
call("EditorToolset.EditorAppToolset","OpenEditorForAsset",assetPath=sys.argv[1]); time.sleep(2)
s=snap(300)
g=[re.search(r"\[ref=(\w+)\]",l).group(1) for l in s.splitlines() if re.search(r'(button|tab|checkbox) "Graph"',l)]
if g: click(g[0]); time.sleep(1); s=snap(300)
refs=[re.search(r"\[ref=(\w+)\]",l).group(1) for l in s.splitlines() if f'listitem "{sys.argv[2]}"' in l]
click(refs[0],dbl=True); time.sleep(1.2)
s=snap(300); best=None
for l in s.splitlines():
    m=re.search(r"splitter \[pos=(\d+),(\d+) size=(\d+),(\d+)\] \[ref=(\w+)\]",l)
    if m and int(m.group(3))>1200 and int(m.group(4))>700: best=m.group(5)
open("panel.txt","w").write(best or "sp52"); print(sys.argv[2],"panel",best)
