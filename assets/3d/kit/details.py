import bpy, bmesh, os, sys
from mathutils import Matrix, Vector

OUT = sys.argv[sys.argv.index("--") + 1]
os.makedirs(OUT, exist_ok=True)
UV_M = 2.0
FACE = 0.2


def box(lo, hi, mat):
    return ("box", lo, hi, mat)


def pipe(x0, x1, y, z, r, mat="Pipe"):
    return ("cyl_x", (x0, x1, y, z, r), None, mat)


def post(x, y, z0, z1, r, mat="Trim"):
    return ("cyl_z", (x, y, z0, z1, r), None, mat)


def pipes(segments):
    parts = []
    for x0, x1 in segments:
        parts += [pipe(x0, x1, FACE + 0.12, 1.05, 0.09), pipe(x0, x1, FACE + 0.08, 1.3, 0.05)]
        x = x0 + 0.3
        while x < x1 - 0.1:
            parts.append(box((x - 0.04, FACE, 0.9), (x + 0.04, FACE + 0.24, 1.38), "Trim"))
            x += 1.6
    return parts


PIECES = {
    "SM_Det_Pipes": pipes([(-3.8, 3.8)]),
    "SM_Det_PipesDoor": pipes([(-3.8, -1.25), (1.25, 3.8)]),
    "SM_Det_Vent": [box((-0.35, FACE, 0.35), (0.35, FACE + 0.08, 0.85), "Trim")]
                   + [box((-0.3, FACE + 0.08, 0.4 + i * 0.09), (0.3, FACE + 0.11, 0.44 + i * 0.09), "Wall") for i in range(5)],
    "SM_Det_Panel": [box((-0.3, FACE, 0.4), (0.3, FACE + 0.18, 1.2), "Trim"),
                     box((-0.24, FACE + 0.18, 0.95), (0.24, FACE + 0.2, 1.1), "Glow")],
    "SM_Det_WallLamp": [box((-0.05, -0.05, 1.5), (0.05, FACE + 0.6, 1.58), "Trim"),
                        box((-0.18, FACE + 0.45, 1.36), (0.18, FACE + 0.75, 1.5), "Trim"),
                        box((-0.15, FACE + 0.48, 1.34), (0.15, FACE + 0.72, 1.36), "Glow")],
    "SM_Det_WorkLamp": [post(0, 0, 0, 1.7, 0.03)]
                       + [box((dx - 0.03, dy - 0.03, 0), (dx + 0.03, dy + 0.03, 0.5), "Trim")
                          for dx, dy in ((0.3, 0), (-0.15, 0.26), (-0.15, -0.26))]
                       + [box((-0.2, -0.12, 1.65), (0.2, 0.12, 1.9), "Trim"),
                          box((-0.17, -0.13, 1.68), (0.17, -0.12, 1.87), "Glow")],
    "SM_Det_FloorGrate": [box((-0.6, -0.6, 0), (0.6, 0.6, 0.02), "Trim")]
                         + [box((-0.55 + i * 0.1, -0.55, 0.02), (-0.52 + i * 0.1, 0.55, 0.04), "Trim") for i in range(12)],
}


def add_box(bm, lo, hi, mi):
    v = [bm.verts.new((x, y, z)) for x in (lo[0], hi[0]) for y in (lo[1], hi[1]) for z in (lo[2], hi[2])]
    for idx in ((0, 1, 3, 2), (4, 6, 7, 5), (0, 4, 5, 1), (2, 3, 7, 6), (0, 2, 6, 4), (1, 5, 7, 3)):
        bm.faces.new([v[i] for i in idx]).material_index = mi


def add_cyl(bm, kind, a, mi):
    if kind == "cyl_x":
        x0, x1, y, z, r = a
        m = Matrix.Translation(((x0 + x1) / 2, y, z)) @ Matrix.Rotation(1.5708, 4, "Y")
        depth = x1 - x0
    else:
        x, y, z0, z1, r = a
        m = Matrix.Translation((x, y, (z0 + z1) / 2))
        depth = z1 - z0
    res = bmesh.ops.create_cone(bm, cap_ends=True, segments=8, radius1=r, radius2=r, depth=depth, matrix=m)
    for f in {f for v in res["verts"] for f in v.link_faces}:
        f.material_index = mi


def world_uv(bm):
    uv = bm.loops.layers.uv.new("UVMap")
    for f in bm.faces:
        n = f.normal
        ax = max(range(3), key=lambda i: abs(n[i]))
        a, b = [(1, 2), (0, 2), (0, 1)][ax]
        for loop in f.loops:
            loop[uv].uv = (loop.vert.co[a] / UV_M, loop.vert.co[b] / UV_M)


for name, parts in PIECES.items():
    bpy.ops.wm.read_factory_settings(use_empty=True)
    mats = []
    for p in parts:
        if p[3] not in mats:
            mats.append(p[3])
    mesh = bpy.data.meshes.new(name)
    bm = bmesh.new()
    for kind, a, b, m in parts:
        if kind == "box":
            add_box(bm, a, b, mats.index(m))
        else:
            add_cyl(bm, kind, a, mats.index(m))
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
    bpy.ops.export_scene.fbx(filepath=os.path.join(OUT, name + ".fbx"), use_selection=True, apply_unit_scale=True,
                             mesh_smooth_type="FACE", bake_space_transform=False)
    d = obj.dimensions
    print("OK", name, round(d.x, 2), round(d.y, 2), round(d.z, 2), len(mesh.polygons), "faces")
