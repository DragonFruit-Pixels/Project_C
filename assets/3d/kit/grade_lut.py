import sys
import numpy as np
from PIL import Image

SRC, DST, OUT = sys.argv[1:4]
STRENGTH = float(sys.argv[4]) if len(sys.argv) > 4 else 0.7
M = np.array([[0.4124, 0.3576, 0.1805], [0.2126, 0.7152, 0.0722], [0.0193, 0.1192, 0.9505]])
WHITE = M.sum(1)


def to_lab(rgb):
    lin = np.where(rgb <= 0.04045, rgb / 12.92, ((rgb + 0.055) / 1.055) ** 2.4)
    xyz = lin @ M.T / WHITE
    f = np.where(xyz > 216 / 24389, np.cbrt(xyz), (24389 / 27 * xyz + 16) / 116)
    return np.stack([116 * f[..., 1] - 16, 500 * (f[..., 0] - f[..., 1]), 200 * (f[..., 1] - f[..., 2])], -1)


def to_rgb(lab):
    fy = (lab[..., 0] + 16) / 116
    f = np.stack([fy + lab[..., 1] / 500, fy, fy - lab[..., 2] / 200], -1)
    xyz = np.where(f ** 3 > 216 / 24389, f ** 3, (116 * f - 16) / (24389 / 27)) * WHITE
    lin = np.clip(xyz @ np.linalg.inv(M).T, 0, 1)
    return np.where(lin <= 0.0031308, lin * 12.92, 1.055 * lin ** (1 / 2.4) - 0.055)


def stats(path):
    lab = to_lab(np.asarray(Image.open(path).convert("RGB").resize((480, 270)), np.float64) / 255).reshape(-1, 3)
    return lab.mean(0), lab.std(0)


(ms, ss), (md, sd) = stats(SRC), stats(DST)
g = np.arange(16) / 15
b, gg, r = np.meshgrid(g, g, g, indexing="ij")
ident = np.stack([r, gg, b], -1)
lab = to_lab(ident)
graded = to_rgb((lab - ms) * (sd / ss) + md)
lut = ident + STRENGTH * (graded - ident)
strip = lut.transpose(1, 0, 2, 3).reshape(16, 256, 3)
Image.fromarray(np.round(np.clip(strip, 0, 1) * 255).astype(np.uint8)).save(OUT)
check = np.asarray(Image.open(OUT), np.float64) / 255
assert check.shape == (16, 256, 3)
assert STRENGTH > 0 or np.abs(check - ident.transpose(1, 0, 2, 3).reshape(16, 256, 3)).max() < 1 / 255 + 1e-6
print("LUT", OUT, "src L %.1f->%.1f  sat a/b %.1f,%.1f -> %.1f,%.1f" % (ms[0], md[0], ss[1], ss[2], sd[1], sd[2]))
