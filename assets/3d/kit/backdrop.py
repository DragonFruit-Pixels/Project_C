import bpy, bmesh, math, os

OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "SM_Dio_Backdrop.fbx")
RADIUS, SEG, RINGS = 200.0, 96, 24

bpy.ops.wm.read_factory_settings(use_empty=True)
mesh = bpy.data.meshes.new("Backdrop")
bm = bmesh.new()
uv = bm.loops.layers.uv.new()
grid = [[bm.verts.new((RADIUS * math.cos(t) * math.sin(p), RADIUS * math.sin(t) * math.sin(p), -RADIUS * math.cos(p)))
         for t in (2 * math.pi * i / SEG for i in range(SEG + 1))]
        for p in (math.pi / 2 * (1 - j / RINGS) for j in range(RINGS + 1))]
for j in range(RINGS):
    for i in range(SEG):
        corners = ((i, j), (i + 1, j), (i + 1, j + 1), (i, j + 1))
        face = bm.faces.new([grid[b][a] for a, b in corners])
        for loop, (a, b) in zip(face.loops, corners):
            loop[uv].uv = (a / SEG, 1 - b / RINGS)
bmesh.ops.remove_doubles(bm, verts=[v for row in grid for v in row], dist=1e-4)
bm.to_mesh(mesh)
bm.free()
obj = bpy.data.objects.new("SM_Dio_Backdrop", mesh)
bpy.context.scene.collection.objects.link(obj)
center = sum((v.co for v in mesh.vertices), mesh.vertices[0].co * 0) / len(mesh.vertices)
inward = sum(1 for p in mesh.polygons if p.normal.dot(center - p.center) > 0)
assert inward == len(mesh.polygons), f"{inward}/{len(mesh.polygons)} faces point inward"
bpy.ops.object.select_all(action="SELECT")
bpy.ops.export_scene.fbx(filepath=OUT, use_selection=True, apply_unit_scale=True, mesh_smooth_type="FACE",
                         object_types={"MESH"}, path_mode="STRIP")
print("BACKDROP", OUT, len(mesh.polygons))
