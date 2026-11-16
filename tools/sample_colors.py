"""Sample hex colors from a screenshot.

Usage:
    python tools/sample_colors.py assets/example.png 400,280 650,200
    python tools/sample_colors.py assets/example.png 400,280 --radius 2

--radius averages a (2r+1)x(2r+1) square around each point, which smooths
JPEG/thumbnail noise. Uses Pillow if it is installed; otherwise it decodes the
image with ffmpeg/ffprobe (any format ffmpeg reads: png, jpg, webp, a video frame).
"""
import argparse
import shutil
import subprocess


def load_pillow(path):
    from PIL import Image
    im = Image.open(path).convert("RGB")
    w, h = im.size
    return w, h, lambda x, y: im.getpixel((x, y))


def load_ffmpeg(path):
    if not (shutil.which("ffmpeg") and shutil.which("ffprobe")):
        raise SystemExit("Needs Pillow (pip install pillow) or ffmpeg + ffprobe on PATH.")
    size = subprocess.run(["ffprobe", "-v", "error", "-select_streams", "v:0", "-show_entries", "stream=width,height",
                           "-of", "csv=p=0", path], capture_output=True, text=True, check=True).stdout.strip()
    w, h = (int(v) for v in size.split(",")[:2])
    raw = subprocess.run(["ffmpeg", "-v", "error", "-i", path, "-frames:v", "1", "-f", "rawvideo", "-pix_fmt", "rgb24", "-"],
                         capture_output=True, check=True).stdout
    return w, h, lambda x, y: tuple(raw[(y * w + x) * 3:(y * w + x) * 3 + 3])


def main() -> None:
    ap = argparse.ArgumentParser(description=__doc__, formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("image")
    ap.add_argument("points", nargs="+", help="x,y pixel coordinates")
    ap.add_argument("--radius", type=int, default=0)
    args = ap.parse_args()

    try:
        w, h, getpixel = load_pillow(args.image)
    except ImportError:
        w, h, getpixel = load_ffmpeg(args.image)
    print(f"{args.image}: {w}x{h}")
    for p in args.points:
        x, y = (int(v) for v in p.split(","))
        r = args.radius
        px = [getpixel(i, j)
              for i in range(max(0, x - r), min(w, x + r + 1))
              for j in range(max(0, y - r), min(h, y + r + 1))]
        avg = tuple(round(sum(c[k] for c in px) / len(px)) for k in range(3))
        print(f"{x},{y}  #{avg[0]:02x}{avg[1]:02x}{avg[2]:02x}  rgb{avg}")


if __name__ == "__main__":
    main()
