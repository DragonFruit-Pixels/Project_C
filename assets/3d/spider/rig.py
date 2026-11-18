import bpy, sys, math
from mathutils import Vector, Matrix

argv = sys.argv[sys.argv.index("--") + 1:]
SRC, OUT = argv[0], argv[1]
FPS = 30

bpy.ops.wm.read_factory_settings(use_empty=True)
sc = bpy.context.scene
sc.render.fps = FPS
bpy.ops.import_scene.gltf(filepath=SRC)
mesh = [o for o in bpy.data.objects if o.type == 'MESH'][0]
mesh.name = "SK_Spider"
mesh.data.transform(Matrix.Translation((0, 0, 0.719)))

LEGS = {
    "L1": [(0.42, -0.10, 0.62), (0.68, -0.18, 0.64), (0.74, -0.20, 0.02)],
    "L2": [(0.38, 0.38, 0.62), (0.60, 0.60, 0.72), (0.67, 0.66, 0.02)],
}
CLAWS = {"CL": [(0.42, -0.36, 0.78), (0.50, -0.68, 0.80), (0.38, -0.95, 0.28)]}


def mirror(pts):
    return [(-x, y, z) for x, y, z in pts]


chains = {}
for n, p in LEGS.items():
    chains["Leg_" + n] = p
    chains["Leg_" + n.replace("L", "R")] = mirror(p)
chains["Claw_L"] = CLAWS["CL"]
chains["Claw_R"] = mirror(CLAWS["CL"])

arm_data = bpy.data.armatures.new("Armature")
arm = bpy.data.objects.new("Armature", arm_data)
sc.collection.objects.link(arm)
bpy.context.view_layer.objects.active = arm
bpy.ops.object.mode_set(mode='EDIT')
eb = arm_data.edit_bones
root = eb.new("root"); root.head = (0, 0, 0); root.tail = (0, 0, 0.3)
body = eb.new("body"); body.head = (0, 0.5, 0.9); body.tail = (0, -0.4, 0.9); body.parent = root
body.align_roll(Vector((0, 0, 1)))
for n, (a, b, c) in chains.items():
    up = eb.new(n + "_up"); up.head = a; up.tail = b; up.parent = body
    up.align_roll(Vector((0, 0, 1)))
    lo = eb.new(n + "_lo"); lo.head = b; lo.tail = c; lo.parent = up; lo.use_connect = True
    lo.align_roll(Vector((0, 0, 1)) if n.startswith("Claw") else Vector((0, 0, 1)).cross(Vector(c) - Vector(b)).normalized())
bpy.ops.object.mode_set(mode='OBJECT')

AX = {}
for b in arm_data.bones:
    M = b.matrix_local.to_3x3()
    X, Y, Z = M.col[0], M.col[1], M.col[2]
    d = (b.tail_local - b.head_local)
    out = Vector((b.head_local.x, b.head_local.y + 0.05, 0))
    out = out.normalized() if out.length > 1e-4 else Vector((0, 0, 0))
    up_eff = X.cross(d)
    if b.name.startswith("Leg") and b.name.endswith("_lo"):
        s_lift = 1 if up_eff.dot(out) >= 0 else -1
    else:
        s_lift = 1 if up_eff.z >= 0 else -1
    s_swing = 1 if -(Z.cross(d)).y >= 0 else -1
    AX[b.name] = (X, Y, Z, s_lift, s_swing)

E_C, E_R = Vector((0, 0.22, 0.92)), Vector((0.58, 0.72, 0.47))


def seg_dist(p, a, b):
    ab = b - a
    t = max(0.0, min(1.0, (p - a).dot(ab) / ab.length_squared))
    return (p - (a + ab * t)).length


segs = {}
for n, (a, b, c) in chains.items():
    segs[n] = [(n + "_up", Vector(a), Vector(b)), (n + "_lo", Vector(b), Vector(c))]

groups = {b.name: mesh.vertex_groups.new(name=b.name) for b in arm_data.bones}
for v in mesh.data.vertices:
    p = v.co
    q = p - E_C
    e = math.sqrt((q.x / E_R.x) ** 2 + (q.y / E_R.y) ** 2 + (q.z / E_R.z) ** 2)
    best, bd = None, 9
    for n, s in segs.items():
        d = min(seg_dist(p, a, b) for _, a, b in s)
        if d < bd:
            best, bd = n, d
    limb = 0.0
    if bd < 0.2:
        limb = max(0.0, min(1.0, (e - 0.92) / 0.22)) * max(0.0, min(1.0, (0.2 - bd) / 0.06))
    if limb < 1:
        groups["body"].add([v.index], 1 - limb, 'REPLACE')
    if limb > 0:
        (nu, au, bu), (nl, al, bl) = segs[best]
        du, dl = seg_dist(p, au, bu) + 1e-3, seg_dist(p, al, bl) + 1e-3
        wu, wl = 1 / du ** 4, 1 / dl ** 4
        groups[nu].add([v.index], limb * wu / (wu + wl), 'REPLACE')
        groups[nl].add([v.index], limb * wl / (wu + wl), 'REPLACE')

mesh.parent = arm
mod = mesh.modifiers.new("Armature", 'ARMATURE'); mod.object = arm

PB = arm.pose.bones
for pb in PB:
    pb.rotation_mode = 'XYZ'


def rest():
    for pb in PB:
        pb.location = (0, 0, 0); pb.rotation_euler = (0, 0, 0)


def rot(name, lift=0.0, swing=0.0, roll=0.0):
    X, Y, Z, sl, ss = AX[name]
    PB[name].rotation_euler = (math.radians(lift * sl), math.radians(roll), math.radians(swing * ss))


def body(z=0.0, back=0.0, pitch=0.0, roll=0.0):
    X, Y, Z, sl, ss = AX["body"]
    w = Vector((0, back, z))
    PB["body"].location = (w.dot(X), w.dot(Y), w.dot(Z))
    PB["body"].rotation_euler = (math.radians(pitch * sl), math.radians(roll), 0)


def key(f):
    for pb in PB:
        pb.keyframe_insert("location", frame=f)
        pb.keyframe_insert("rotation_euler", frame=f)


def action(name):
    arm.animation_data_create()
    act = bpy.data.actions.new(name)
    act.use_fake_user = True
    arm.animation_data.action = act
    rest()
    return act


LEGS4 = ["Leg_L1", "Leg_R1", "Leg_L2", "Leg_R2"]
PHASE = {"Leg_L1": 0.0, "Leg_R2": 0.0, "Leg_R1": 0.5, "Leg_L2": 0.5}


def gait(name, frames, A, L, bob):
    action(name)
    for f in range(0, frames + 1, 2):
        t = f / frames
        for leg in LEGS4:
            p = (t + PHASE[leg]) % 1.0
            if p < 0.5:
                sw, lf = A * (1 - 4 * p), 0.0
            else:
                q = (p - 0.5) * 2
                sw, lf = -A + 2 * A * q, L * math.sin(math.pi * q)
            rot(leg + "_up", lift=lf, swing=sw)
            rot(leg + "_lo", lift=0.6 * lf)
        rot("Claw_L_up", lift=8 + 6 * math.sin(2 * math.pi * t))
        rot("Claw_R_up", lift=8 - 6 * math.sin(2 * math.pi * t))
        rot("Claw_L_lo", lift=4 * math.sin(2 * math.pi * t))
        rot("Claw_R_lo", lift=-4 * math.sin(2 * math.pi * t))
        body(z=bob * math.sin(4 * math.pi * t), roll=2 * math.sin(2 * math.pi * t))
        key(f + 1)


gait("Walk", 24, 18, 28, 0.015)
gait("Run", 16, 24, 34, 0.025)

action("Idle")
for f in range(0, 61, 4):
    t = f / 60
    s = math.sin(2 * math.pi * t)
    body(z=0.012 * s, pitch=2 * s)
    rot("Claw_L_up", lift=5 * s); rot("Claw_R_up", lift=5 * math.sin(2 * math.pi * t + 1.3))
    rot("Claw_L_lo", lift=4 * s); rot("Claw_R_lo", lift=4 * math.sin(2 * math.pi * t + 1.3))
    for leg in LEGS4:
        rot(leg + "_up", lift=1.5 * s)
    key(f + 1)


def pose_keys(name, poses):
    action(name)
    for f, fn in poses:
        rest(); fn(); key(f)


def atk_raise():
    body(back=0.06, pitch=12)
    for c in ("Claw_L", "Claw_R"):
        rot(c + "_up", lift=55); rot(c + "_lo", lift=35)


def atk_strike():
    body(back=-0.1, pitch=-14, z=-0.04)
    for c in ("Claw_L", "Claw_R"):
        rot(c + "_up", lift=-25); rot(c + "_lo", lift=-30)


pose_keys("Attack", [(1, rest), (9, atk_raise), (15, atk_raise), (20, atk_strike), (27, atk_strike), (37, rest)])


def hit_peak():
    body(back=0.12, pitch=9, z=-0.03, roll=-4)
    for c in ("Claw_L", "Claw_R"):
        rot(c + "_up", lift=22); rot(c + "_lo", lift=15)
    for leg in LEGS4:
        rot(leg + "_up", lift=6)


def hit_settle():
    body(back=0.03, pitch=-2)


pose_keys("Hit", [(1, rest), (5, hit_peak), (13, hit_settle), (25, rest)])


def die_rear():
    body(pitch=14, z=0.03)
    for c in ("Claw_L", "Claw_R"):
        rot(c + "_up", lift=35)


def die_down():
    body(z=-0.36, pitch=-6, roll=8)
    for leg in LEGS4:
        rot(leg + "_up", lift=30); rot(leg + "_lo", lift=-55)
    for c in ("Claw_L", "Claw_R"):
        rot(c + "_up", lift=-15); rot(c + "_lo", lift=-45)


def die_curl():
    die_down()
    body(z=-0.38, pitch=-8, roll=10)
    for leg in LEGS4:
        rot(leg + "_up", lift=38); rot(leg + "_lo", lift=-75)


pose_keys("Death", [(1, rest), (9, die_rear), (25, die_down), (40, die_curl), (51, die_curl)])

for act in bpy.data.actions:
    for fc in getattr(act, "fcurves", []):
        for k in fc.keyframe_points:
            k.interpolation = 'BEZIER'

arm.animation_data.action = None
rest()
bpy.ops.wm.save_as_mainfile(filepath=OUT)
print("SAVED", OUT, [a.name for a in bpy.data.actions])
print("WEIGHTS", {g.name: sum(1 for v in mesh.data.vertices for gg in v.groups if gg.group == g.index and gg.weight > 0.5) for g in mesh.vertex_groups})
