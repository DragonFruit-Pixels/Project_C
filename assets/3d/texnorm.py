import bpy, os, sys, math
import numpy as np
from mathutils import Matrix, Vector

SRC, SHEET = sys.argv[sys.argv.index("--") + 1:][:2]
ROOT = os.path.dirname(os.path.abspath(__file__))
TEXDIR = os.path.join(ROOT, "textures")
REFS = [("props", "SM_Prop_"), ("fill", "SM_Fill_")]
RES = 1024
RESHAPED = {"ChitinGrowth"}
EMISSIVE = {"Console", "LabBench", "CoreVat", "Fungus", "SpecimenJar"}


def bounds(objs):
    pts = [o.matrix_world @ Vector(c) for o in objs for c in o.bound_box]
    lo = Vector([min(p[i] for p in pts) for i in range(3)])
    hi = Vector([max(p[i] for p in pts) for i in range(3)])
    return lo, hi


def reset():
    bpy.ops.wm.read_factory_settings(use_empty=True)


def meshes():
    return [o for o in bpy.data.objects if o.type == "MESH"]


def ref_for(name):
    for folder, prefix in REFS:
        path = os.path.join(ROOT, folder, prefix + name + ".fbx")
        if os.path.exists(path):
            return path, prefix
    raise FileNotFoundError(name)


def pixels(path):
    img = bpy.data.images.load(path)
    img.scale(RES, RES)
    px = np.array(img.pixels[:], dtype=np.float32).reshape(RES, RES, 4)
    bpy.data.images.remove(img)
    return px


def save(px, path):
    img = bpy.data.images.new("out", RES, RES, alpha=False)
    img.pixels[:] = px.ravel()
    img.filepath_raw = path
    img.file_format = "JPEG"
    bpy.context.scene.render.image_settings.quality = 90
    img.save()
    bpy.data.images.remove(img)


def textures(name):
    src = os.path.join(SRC, name + "_textures")
    out = {}
    bc = pixels(os.path.join(src, "base_color.png"))
    save(bc, os.path.join(TEXDIR, f"T_{name}_BC.jpg"))
    out["BaseColor"] = "BC"
    save(pixels(os.path.join(src, "normal.png")), os.path.join(TEXDIR, f"T_{name}_N.jpg"))
    out["Normal"] = "N"
    rough = pixels(os.path.join(src, "roughness.png"))[..., 0]
    metal = pixels(os.path.join(src, "metallic.png"))[..., 0]
    rm = np.ones((RES, RES, 4), dtype=np.float32)
    rm[..., 1], rm[..., 2] = rough, metal
    save(rm, os.path.join(TEXDIR, f"T_{name}_RM.jpg"))
    out["RM"] = "RM"
    em_path = os.path.join(src, "emission.png")
    if name in EMISSIVE and os.path.exists(em_path):
        em = pixels(em_path)
        if em[..., :3].max() > 0.05:
            save(em, os.path.join(TEXDIR, f"T_{name}_E.jpg"))
            out["Emissive"] = "E"
    return out


os.makedirs(TEXDIR, exist_ok=True)
done = []
for f in sorted(f for f in os.listdir(SRC) if f.endswith(".glb")):
    name = f[:-4]
    ref, prefix = ref_for(name)
    reset()
    bpy.ops.import_scene.fbx(filepath=ref)
    rlo, rhi = bounds(meshes())
    rd = rhi - rlo

    reset()
    bpy.ops.import_scene.gltf(filepath=os.path.join(SRC, f))
    ms = meshes()
    bpy.ops.object.select_all(action="SELECT")
    bpy.context.view_layer.objects.active = ms[0]
    bpy.ops.object.parent_clear(type="CLEAR_KEEP_TRANSFORM")
    bpy.ops.object.transform_apply(location=True, rotation=True, scale=True)
    for o in [o for o in bpy.data.objects if o.type != "MESH"]:
        bpy.data.objects.remove(o)
    lo, hi = bounds(ms)
    d = hi - lo
    assert max(d) > 0, name + " has no geometry"
    k = rd.z / d.z if name in RESHAPED else max(rd) / max(d)
    pivot = Vector(((lo.x + hi.x) / 2, (lo.y + hi.y) / 2, lo.z))
    for o in ms:
        o.data.transform(Matrix.Scale(k, 4) @ Matrix.Translation(-pivot))
        o.matrix_world = Matrix.Identity(4)
    nd = d * k
    print("TEX %-14s ref %.2f x %.2f x %.2f  new %.2f x %.2f x %.2f" % (name, *rd, *nd))
    assert name in RESHAPED or all(abs(a - b) < 0.15 * max(rd) for a, b in zip(sorted(rd), sorted(nd))), name + " proportions differ from the reference"
    bpy.ops.object.select_all(action="SELECT")
    bpy.ops.export_scene.fbx(filepath=ref, use_selection=True, apply_unit_scale=True,
                             mesh_smooth_type="FACE", object_types={"MESH"}, path_mode="STRIP")
    maps = textures(name)
    print("MAPS", name, prefix, ",".join(f"{k}={v}" for k, v in maps.items()))
    done.append(os.path.join(SRC, f))

reset()
cols, cell = 6, 4.0
for i, path in enumerate(done):
    before = set(bpy.data.objects)
    bpy.ops.import_scene.gltf(filepath=path)
    objs = [o for o in bpy.data.objects if o not in before]
    lo, hi = bounds([o for o in objs if o.type == "MESH"])
    s = 2.5 / max(hi - lo)
    gx, gy = (i % cols) * cell, -(i // cols) * cell
    for o in objs:
        if o.parent is None:
            o.scale *= s
            o.location = o.location * s + Vector((gx, gy, 0))
scn = bpy.context.scene
scn.render.engine = "BLENDER_EEVEE"
sun = bpy.data.objects.new("Sun", bpy.data.lights.new("Sun", "SUN"))
sun.data.energy = 4
sun.rotation_euler = (math.radians(40), math.radians(20), math.radians(30))
scn.collection.objects.link(sun)
rows = (len(done) + cols - 1) // cols
scn.render.resolution_x, scn.render.resolution_y = 1800, max(500, 300 * rows)
cam = bpy.data.objects.new("Cam", bpy.data.cameras.new("Cam"))
scn.collection.objects.link(cam)
cam.data.type = "ORTHO"
cam.data.ortho_scale = cols * cell
cam.location = ((cols - 1) * cell / 2, -(rows - 1) * cell / 2 - 14, 12)
cam.rotation_euler = (math.radians(55), 0, 0)
scn.camera = cam
scn.world = bpy.data.worlds.new("W")
scn.world.use_nodes = True
scn.world.node_tree.nodes["Background"].inputs[0].default_value = (0.35, 0.35, 0.38, 1)
scn.render.filepath = SHEET
bpy.ops.render.render(write_still=True)
