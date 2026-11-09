import os
import numpy as np
from PIL import Image, ImageFilter

DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "textures")
W, H, OVERLAP, PANEL, BLUR = 4096, 1024, 0.18, 4, 8


def panel(i, w):
    im = Image.open(os.path.join(DIR, f"backdrop_src_{i}.png")).convert("RGB").resize((w, H), Image.LANCZOS)
    return np.asarray(im, np.float64) / 255


step = W // PANEL
ov = int(step * OVERLAP)
out = np.zeros((H, W, 3))
weight = np.zeros((H, W, 1))
for k in range(PANEL):
    p = panel(k + 1, step + ov)
    ramp = np.ones(step + ov)
    ramp[:ov] = np.linspace(0, 1, ov)
    ramp[-ov:] = np.linspace(1, 0, ov)
    cols = (np.arange(step + ov) + k * step - ov // 2) % W
    out[:, cols] += p * ramp[None, :, None]
    weight[:, cols] += ramp[None, :, None]
out /= np.maximum(weight, 1e-6)
fade = np.clip((np.arange(H) / H - 0.7) / 0.3, 0, 1)[:, None, None]
out = out * (1 - fade) + np.array([0.02, 0.035, 0.04]) * fade
assert weight.min() > 0.99, "every column must be covered"
assert np.abs(out[:, 0] - out[:, -1]).mean() < 0.05, "seam at u=0 must wrap"
Image.fromarray((out * 255).astype(np.uint8)).save(os.path.join(DIR, "T_Backdrop.jpg"), quality=92)

smoke = np.asarray(Image.open(os.path.join(DIR, "smoke_src.png")).convert("L").resize((512, 512), Image.LANCZOS)
                   .filter(ImageFilter.GaussianBlur(BLUR)), np.float64) / 255
y, x = np.mgrid[0:512, 0:512] / 511 * 2 - 1
edge = np.clip((1 - np.maximum(abs(x), abs(y))) / 0.15, 0, 1)
radial = np.clip(1 - np.hypot(x, y), 0, 1) ** 1.5
smoke = np.clip((smoke - 0.04) / 0.96, 0, 1) * edge * radial
assert smoke[0].max() == 0 and smoke[:, 0].max() == 0 and smoke.max() > 0.2
Image.fromarray((smoke * 255).astype(np.uint8), "L").save(os.path.join(DIR, "T_FogCloud.png"))
print("BACKDROP TEX OK")
