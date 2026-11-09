import bpy, bmesh, math, os, random, sys
from mathutils import Vector, noise

OUT = sys.argv[sys.argv.index("--") + 1]
U = 8.0
TOP = -0.1
DEPTH = 4.5
SECTIONS = {
    "SM_Dio_Base_Plant": [(0, c) for c in range(7)] + [(1, 6), (2, 6), (3, 6)],
    "SM_Dio_Base_Sump": [(2, 0), (2, 1), (3, 0), (3, 1), (2, 2), (2, 3), (2, 4), (3, 3), (3, 4), (3, 5)],
}


def cell_center(r, c):
    return -U * r, -(60.0 + U * c)


def reset():
    bpy.ops.wm.read_factory_settings(use_empty=True)


def box_uv(obj, tile=4.0):
    bm = bmesh.new()
    bm.from_mesh(obj.data)
    uv = bm.loops.layers.uv.new("UVMap")
    for f in bm.faces:
        ax = max(range(3), key=lambda i: abs(f.normal[i]))
        a, b = [i for i in range(3) if i != ax]
        for l in f.loops:
            l[uv].uv = (l.vert.co[a] / tile, l.vert.co[b] / tile)
    bm.to_mesh(obj.data)
    bm.free()


def export(obj, name):
    box_uv(obj)
    obj.data.materials.clear()
    obj.data.materials.append(bpy.data.materials.new("M_Dio_Rock"))
    bpy.ops.object.select_all(action="DESELECT")
    obj.select_set(True)
    bpy.context.view_layer.objects.active = obj
    bpy.ops.export_scene.fbx(filepath=os.path.join(OUT, name + ".fbx"), use_selection=True, apply_unit_scale=True,
                             mesh_smooth_type="FACE", object_types={"MESH"})
    lo = [min(v.co[i] for v in obj.data.vertices) for i in range(3)]
    hi = [max(v.co[i] for v in obj.data.vertices) for i in range(3)]
    print("DIO", name, [round(x, 2) for x in lo], [round(x, 2) for x in hi], len(obj.data.polygons), "faces")


def rockify(obj, keep_top, amp, scale, seed):
    off = Vector((seed * 13.1, seed * 7.7, seed * 3.3))
    for v in obj.data.vertices:
        if keep_top is not None and v.co.z > keep_top - 0.35:
            v.co.z = min(v.co.z, keep_top)
            continue
        p = v.co * scale + off
        n = noise.noise_vector(p)
        depth = 1.0 if keep_top is None else min(1.0, (keep_top - v.co.z) / 1.5)
        v.co += Vector((n.x, n.y, n.z * 1.6)) * amp * depth


def remesh(obj, voxel, ratio):
    bpy.context.view_layer.objects.active = obj
    m = obj.modifiers.new("R", "REMESH")
    m.mode, m.voxel_size = "VOXEL", voxel
    bpy.ops.object.modifier_apply(modifier="R")
    d = obj.modifiers.new("D", "DECIMATE")
    d.ratio = ratio
    bpy.ops.object.modifier_apply(modifier="D")


for name, cells in SECTIONS.items():
    reset()
    bm = bmesh.new()
    for r, c in cells:
        x, y = cell_center(r, c)
        h = U / 2 + 0.05
        geom = bmesh.ops.create_cube(bm, size=1.0)["verts"]
        for v in geom:
            v.co = Vector((x + v.co.x * 2 * h, y + v.co.y * 2 * h, TOP - DEPTH / 2 + v.co.z * DEPTH))
    mesh = bpy.data.meshes.new(name)
    bm.to_mesh(mesh)
    obj = bpy.data.objects.new(name, mesh)
    bpy.context.scene.collection.objects.link(obj)
    remesh(obj, 0.35, 0.25)
    for v in obj.data.vertices:
        if v.co.z < TOP - DEPTH + 0.6:
            v.co.z -= random.Random(hash((round(v.co.x), round(v.co.y)))).uniform(0, 2.5)
    rockify(obj, TOP, 0.55, 0.45, 1 if "Plant" in name else 2)
    assert abs(max(v.co.z for v in obj.data.vertices) - TOP) < 0.01, name + " top drifted"
    export(obj, name)

reset()
bpy.ops.mesh.primitive_cylinder_add(vertices=10, radius=2.0, depth=30.0, location=(0, 0, 15.0))
obj = bpy.context.object
bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
remesh(obj, 0.4, 0.2)
rockify(obj, None, 0.9, 0.35, 3)
for v in obj.data.vertices:
    v.co.x *= 1.0 + 0.25 * math.sin(v.co.z * 0.4)
    v.co.y *= 1.0 + 0.25 * math.cos(v.co.z * 0.3)
export(obj, "SM_Dio_Pillar")
