import unreal, json
plan=json.load(open(SP+"plan.json",encoding="utf8"))
col=unreal.LinearColor(0.15,0.15,0.15,0.5)
done=0;err=[];bps=set()
for p in plan:
    try:
        g=unreal.load_object(None,p["graph"]); ed=unreal.BlueprintGraphEditor.get_graph_editor(g)
        if "edit" in p:
            c=unreal.load_object(None,p["edit"]); c.modify(); c.set_comment_text(p["text"])
        else:
            x0,y0,x1,y1=p["rect"]
            c=ed.add_comment_node(p["text"],unreal.Vector2D(x0,y0),unreal.Vector2D(x1-x0,y1-y0))
            c.set_comment_color(col); c.set_editor_property("font_size",18)
        bps.add(p["graph"].split(":")[0]); done+=1
    except Exception as e: err.append(p["graph"]+" "+str(e)[:100])
for b in bps:
    bp=unreal.load_object(None,b); unreal.BlueprintEditorLibrary.compile_blueprint(bp)
    unreal.EditorAssetLibrary.save_asset(b.split(".")[0],only_if_is_dirty=False)
open(SP+"pyout.txt","w").write(f"{done} ok, {len(err)} errores, {len(bps)} BPs\n"+"\n".join(err[:30]))
