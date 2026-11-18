import sys,time,os
from capnow import con
SP=os.path.dirname(os.path.abspath(__file__)).replace("\\","/")+"/"
def runpy(name,wait=3):
    out=SP+"pyout.txt"
    if os.path.exists(out): os.remove(out)
    path=name if os.path.isabs(name) else SP+name
    con('py exec(open(r"%s",encoding="utf8").read(),{"SP":r"%s"})'%(path,SP))
    for _ in range(wait*5):
        if os.path.exists(out): time.sleep(0.3); return open(out,encoding="utf8").read()
        time.sleep(0.2)
    return "NO OUTPUT"
if __name__=="__main__": print(runpy(sys.argv[1], int(sys.argv[2]) if len(sys.argv)>2 else 5))
