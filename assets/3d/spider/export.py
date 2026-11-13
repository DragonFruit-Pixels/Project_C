import bpy, sys
out=sys.argv[sys.argv.index("--")+1]
sc=bpy.context.scene; arm=bpy.data.objects["Armature"]; m=bpy.data.objects["SK_Spider"]
for o in bpy.data.objects: o.select_set(o in (arm,m))
bpy.context.view_layer.objects.active=arm
common=dict(use_selection=True, object_types={'ARMATURE','MESH'}, add_leaf_bones=False, use_armature_deform_only=False,
            apply_scale_options='FBX_SCALE_NONE', mesh_smooth_type='FACE', path_mode='STRIP')
arm.animation_data.action=None
for pb in arm.pose.bones: pb.location=(0,0,0); pb.rotation_euler=(0,0,0)
bpy.ops.export_scene.fbx(filepath=f"{out}/Spider_Mesh.fbx", bake_anim=False, **common)
for a in bpy.data.actions:
    arm.animation_data.action=a; arm.animation_data.action_slot=a.slots[0]
    s,e=a.frame_range; sc.frame_start=int(s); sc.frame_end=int(e)
    bpy.ops.export_scene.fbx(filepath=f"{out}/Spider_{a.name}.fbx", bake_anim=True, bake_anim_use_all_actions=False,
        bake_anim_use_nla_strips=False, bake_anim_force_startend_keying=True, bake_anim_simplify_factor=0.0, **common)
    print("EXP",a.name,s,e)
