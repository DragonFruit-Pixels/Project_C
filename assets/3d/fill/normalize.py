import bpy, os, sys
from mathutils import Matrix, Vector

SRC, OUT = sys.argv[sys.argv.index("--") + 1:][:2]

SIZE = {
    "Boxes": ("h", 0.8), "Jerrycans": ("h", 0.5), "Bucket": ("h", 0.4), "Toolbox": ("h", 0.35),
    "CableSpool": ("h", 0.8), "TrashBags": ("h", 0.6), "Debris": ("w", 1.0), "GasCylinders": ("h", 1.4),
    "Extinguisher": ("h", 0.6), "Crate": ("h", 0.6), "EggSacs": ("h", 0.6), "BiomassPuddle": ("w", 1.6),
    "SpecimenJar": ("h", 0.7), "Fungus": ("h", 0.6), "Tendrils": ("h", 0.9), "Crust": ("w", 2.0),
}

for name, (axis, size) in SIZE.items():
    bpy.ops.wm.read_factory_settings(use_empty=True)
    bpy.ops.import_scene.gltf(filepath=os.path.join(SRC, name + ".glb"))
    meshes = [o for o in bpy.data.objects if o.type == "MESH"]
    bpy.ops.object.select_all(action="SELECT")
    bpy.context.view_layer.objects.active = meshes[0]
    bpy.ops.object.parent_clear(type="CLEAR_KEEP_TRANSFORM")
    bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
    for o in [o for o in bpy.data.objects if o.type != "MESH"]:
        bpy.data.objects.remove(o)
    pts = [o.matrix_world @ Vector(c) for o in meshes for c in o.bound_box]
    lo = Vector([min(p[i] for p in pts) for i in range(3)])
    hi = Vector([max(p[i] for p in pts) for i in range(3)])
    d = hi - lo
    current = d.z if axis == "h" else max(d.x, d.y)
    assert current > 0, name + " has no geometry"
    k = size / current
    pivot = Vector(((lo.x + hi.x) / 2, (lo.y + hi.y) / 2, lo.z))
    for o in meshes:
        o.data.transform(Matrix.Scale(k, 4) @ Matrix.Translation(-pivot))
    bpy.ops.object.select_all(action="SELECT")
    bpy.ops.export_scene.fbx(filepath=os.path.join(OUT, "SM_Fill_" + name + ".fbx"), use_selection=True,
                             apply_unit_scale=True, mesh_smooth_type="FACE", object_types={"MESH"})
    tris = sum(len(p.vertices) - 2 for o in meshes for p in o.data.polygons)
    print("FILL %-14s %.2f x %.2f x %.2f m  tris %d" % (name, d.x * k, d.y * k, d.z * k, tris))
