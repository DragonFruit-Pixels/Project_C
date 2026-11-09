import os, sys
import numpy as np
from PIL import Image

OUT = sys.argv[1] if len(sys.argv) > 1 else os.path.join(os.path.dirname(os.path.abspath(__file__)), "textures")
N = 1024
rng = np.random.default_rng(5)


def fractal(beta, seed):
    r = np.random.default_rng(seed)
    f = np.fft.fftfreq(N)
    k = np.sqrt(f[:, None] ** 2 + f[None, :] ** 2)
    k[0, 0] = 1
    spec = np.fft.fft2(r.standard_normal((N, N))) / k ** (beta / 2)
    h = np.real(np.fft.ifft2(spec))
    return (h - h.min()) / (h.max() - h.min())


base = fractal(2.4, 1)
detail = fractal(1.6, 2)
warp = fractal(2.8, 3)
y = np.arange(N)[:, None] / N
strata = 0.5 + 0.5 * np.sin(2 * np.pi * (6 * y + 1.5 * warp))
cracks = np.clip(1 - np.abs(fractal(2.2, 4) - 0.5) * 40, 0, 1)
h = 0.45 * base + 0.25 * detail + 0.3 * strata
h = (h - h.min()) / (h.max() - h.min())

dark = np.array([34, 27, 23], dtype=np.float32)
mid = np.array([92, 74, 60], dtype=np.float32)
light = np.array([140, 118, 96], dtype=np.float32)
t = h[..., None]
col = np.where(t < 0.5, dark + (mid - dark) * (t / 0.5), mid + (light - mid) * ((t - 0.5) / 0.5))
col *= (1 - 0.65 * cracks)[..., None]
col *= (0.85 + 0.3 * detail)[..., None]
Image.fromarray(col.clip(0, 255).astype(np.uint8)).save(os.path.join(OUT, "T_Dio_Rock_BC.jpg"), quality=90)

height = h - 0.6 * cracks
dx = (np.roll(height, -1, 1) - np.roll(height, 1, 1)) * 6
dy = (np.roll(height, -1, 0) - np.roll(height, 1, 0)) * 6
n = np.stack([-dx, dy, np.ones_like(dx)], -1)
n /= np.linalg.norm(n, axis=-1, keepdims=True)
Image.fromarray(((n * 0.5 + 0.5) * 255).astype(np.uint8)).save(os.path.join(OUT, "T_Dio_Rock_N.jpg"), quality=92)

rm = np.zeros((N, N, 3), dtype=np.uint8)
rm[..., 0] = 255
rm[..., 1] = (200 + 50 * cracks).clip(0, 255).astype(np.uint8)
Image.fromarray(rm).save(os.path.join(OUT, "T_Dio_Rock_RM.jpg"), quality=90)

tile = Image.open(os.path.join(OUT, "T_Dio_Rock_BC.jpg")).resize((256, 256))
sheet = Image.new("RGB", (512, 512))
for i in range(2):
    for j in range(2):
        sheet.paste(tile, (i * 256, j * 256))
sheet.save(os.path.join(OUT, "..", "rock_tile_check.jpg"))
print("ROCK", OUT)
