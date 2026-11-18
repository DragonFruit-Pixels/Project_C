from at import call
B="editor_toolset.toolsets.blueprint.BlueprintTools"
class Graph:
    def __init__(s,G): s.G=G
    def N(s,k): return {"refPath":k if ":" in k else s.G+"."+k}
    def info(s,k): return call(B,"get_node_infos",nodes=[s.N(k)])[0]
    def pin(s,k,d,name):
        for p in s.info(k)["input_pins" if d=="in" else "output_pins"]:
            if p["name"]==name: return p["pin_id"]
        raise Exception(f"pin {k}.{name} ({d})")
    def delete(s,*ks):
        for k in ks: call(B,"delete_node",node=s.N(k))
    def create(s,type_id,x=0,y=0):
        r=call(B,"create_node",graph={"refPath":s.G},type_id=type_id,pos={"x":x,"y":y})
        return r["refPath"] if isinstance(r,dict) else r
    def link(s,a,ap,b,bp):
        r=call(B,"connect_pins",output_pin=s.pin(a,"out",ap),input_pin=s.pin(b,"in",bp))
        if r not in (None,True,""): print("link",a,ap,b,bp,r)
