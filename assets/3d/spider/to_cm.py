import bpy, sys
out=sys.argv[sys.argv.index("--")+1]
arm=bpy.data.objects["Armature"]; m=bpy.data.objects["SK_Spider"]
arm.animation_data.action=None
for o in bpy.data.objects: o.select_set(o in (arm,m))
bpy.context.view_layer.objects.active=arm
bpy.context.scene.unit_settings.scale_length=0.01
arm.scale=(90,90,90)
bpy.ops.object.transform_apply(location=False, rotation=False, scale=True, properties=False)
for a in bpy.data.actions:
    for layer in a.layers:
        for strip in layer.strips:
            for cb in strip.channelbags:
                for fc in cb.fcurves:
                    if fc.data_path.endswith("location"):
                        for k in fc.keyframe_points:
                            k.co.y*=90; k.handle_left.y*=90; k.handle_right.y*=90
print("DIMS", tuple(round(x,1) for x in m.dimensions), tuple(arm.scale), tuple(m.scale))
bpy.ops.wm.save_as_mainfile(filepath=out)
