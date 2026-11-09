import bpy, bmesh, os, sys

OUT = sys.argv[sys.argv.index("--") + 1]
os.makedirs(OUT, exist_ok=True)
UV_M = 2.0

PIECES = {
    "SM_Kit_Floor": [((-4, -4, -0.1), (4, 4, 0), "Floor")],
    "SM_Kit_Wall": [((-4, 0, 0), (4, 0.2, 1.5), "Wall")],
    "SM_Kit_WallDoor": [((-4, 0, 0), (-1, 0.2, 1.5), "Wall"), ((1, 0, 0), (4, 0.2, 1.5), "Wall")],
    "SM_Kit_Corner": [((0, 0, 0), (0.3, 0.3, 1.7), "Trim")],
    "SM_Kit_DoorFrame": [((-1.15, 0, 0), (-1.0, 0.2, 2.2), "Trim"), ((1.0, 0, 0), (1.15, 0.2, 2.2), "Trim"),
                         ((-1.15, 0, 2.0), (1.15, 0.2, 2.2), "Trim")],
    "SM_Kit_Hatch": [((-0.8, -0.8, 0), (0.8, -0.6, 0.06), "Trim"), ((-0.8, 0.6, 0), (0.8, 0.8, 0.06), "Trim"),
                     ((-0.8, -0.6, 0), (-0.6, 0.6, 0.06), "Trim"), ((0.6, -0.6, 0), (0.8, 0.6, 0.06), "Trim"),
                     ((-0.6, -0.6, 0), (0.6, 0.6, 0.02), "Hatch")],
}


def box(bm, lo, hi, mat_index):
    v = [bm.verts.new((x, y, z)) for x in (lo[0], hi[0]) for y in (lo[1], hi[1]) for z in (lo[2], hi[2])]
    for idx in ((0, 1, 3, 2), (4, 6, 7, 5), (0, 4, 5, 1), (2, 3, 7, 6), (0, 2, 6, 4), (1, 5, 7, 3)):
        bm.faces.new([v[i] for i in idx]).material_index = mat_index


def world_uv(bm):
    uv = bm.loops.layers.uv.new("UVMap")
    for f in bm.faces:
        n = f.normal
        ax = max(range(3), key=lambda i: abs(n[i]))
        a, b = [(1, 2), (0, 2), (0, 1)][ax]
        for loop in f.loops:
            co = loop.vert.co
            loop[uv].uv = (co[a] / UV_M, co[b] / UV_M)


for name, boxes in PIECES.items():
    bpy.ops.wm.read_factory_settings(use_empty=True)
    mats = []
    for *_, m in boxes:
        if m not in mats:
            mats.append(m)
    mesh = bpy.data.meshes.new(name)
    bm = bmesh.new()
    for lo, hi, m in boxes:
        box(bm, lo, hi, mats.index(m))
    bmesh.ops.recalc_face_normals(bm, faces=bm.faces)
    world_uv(bm)
    bm.to_mesh(mesh)
    bm.free()
    for m in mats:
        mesh.materials.append(bpy.data.materials.new("M_Kit_" + m))
    obj = bpy.data.objects.new(name, mesh)
    bpy.context.scene.collection.objects.link(obj)
    obj.select_set(True)
    bpy.context.view_layer.objects.active = obj
    path = os.path.join(OUT, name + ".fbx")
    bpy.ops.export_scene.fbx(filepath=path, use_selection=True, apply_unit_scale=True,
                             mesh_smooth_type="FACE", bake_space_transform=False)
    d = obj.dimensions
    print("OK", name, round(d.x, 2), round(d.y, 2), round(d.z, 2), len(mesh.polygons), "faces")
