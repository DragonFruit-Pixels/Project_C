"""Encuadra el grafo abierto (Home) y guarda captura del panel. Uso: gsee.py out.png [ref_panel]"""
import sys,time,re,base64
from sl import sl, click
ref=sys.argv[2] if len(sys.argv)>2 else "sp52"
click(ref); time.sleep(0.3); sl("PressKey",key="Home"); time.sleep(1.2)
t=sl("Screenshot",ref=ref)
open(sys.argv[1],"wb").write(base64.b64decode(re.search(r'"data"\s*:\s*"([A-Za-z0-9+/=]+)"',t).group(1)))
from PIL import Image
im=Image.open(sys.argv[1]); im.crop((0,60,im.size[0],1000)).save(sys.argv[1])
