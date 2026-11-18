import os, sys
import numpy as np
from PIL import Image

SRC = sys.argv[1]
OUT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "textures")
CROP = 0.055
N = 1024


def seamless(a):
    t = np.linspace(0, 1, a.shape[0])
    edge = np.minimum(t, 1 - t) * 2
    w = np.clip(np.minimum.outer(edge, edge) * 3, 0, 1)[..., None]
    rolled = np.roll(np.roll(a, a.shape[0] // 2, 0), a.shape[1] // 2, 1)
    return a * w + rolled * (1 - w)


im = Image.open(SRC).convert("RGB")
m = int(im.size[0] * CROP)
im = im.crop((m, m, im.size[0] - m, im.size[1] - m)).resize((N, N), Image.LANCZOS)
col = seamless(np.asarray(im, dtype=np.float32))
Image.fromarray(col.clip(0, 255).astype(np.uint8)).save(os.path.join(OUT, "T_Dio_Rock_BC.jpg"), quality=90)

h = col.mean(-1) / 255.0
dx = (np.roll(h, -1, 1) - np.roll(h, 1, 1)) * 4
dy = (np.roll(h, -1, 0) - np.roll(h, 1, 0)) * 4
n = np.stack([-dx, dy, np.ones_like(dx)], -1)
n /= np.linalg.norm(n, axis=-1, keepdims=True)
Image.fromarray(((n * 0.5 + 0.5) * 255).astype(np.uint8)).save(os.path.join(OUT, "T_Dio_Rock_N.jpg"), quality=92)

rm = np.zeros((N, N, 3), dtype=np.uint8)
rm[..., 0] = 255
rm[..., 1] = (255 * (0.95 - 0.25 * h)).clip(0, 255).astype(np.uint8)
Image.fromarray(rm).save(os.path.join(OUT, "T_Dio_Rock_RM.jpg"), quality=90)
print("ROCK", OUT)
