# -*- coding: utf-8 -*-

import sys, os, argparse, re
import Image, struct, shutil

__version__ = "0.1.0"

def main (argv):
    parser = argparse.ArgumentParser(description="Generates a bmf file from a BMFont fnt file.")
    parser.add_argument("bmf_file")
    parser.add_argument("fnt_file")
    opts = parser.parse_args()

    with open(opts.bmf_file, "wb") as out:
        write_bitmap(out, opts.fnt_file)
        write_font_offsets(out, opts.fnt_file)

def write_bitmap(out, fnt_file):
    img_file = os.path.splitext(fnt_file)[0] + '_0.png'
    img_original = Image.open(img_file)
    img_mono = img_original.convert('L')

    (w, h) = img_mono.size
    out.write(struct.pack('>HHH', w, h, w*h))

    for b in img_mono.getdata():
        out.write(struct.pack('B', b))

def write_font_offsets(out, fnt_file):
    char_offs = {}
    for i in range(32, 256):
        char_offs[i] = {'x': 0, 'width': 0}

    with open(fnt_file, "rb") as inp:
        rx_space = re.compile(r'\s+')

        for line in inp:
            parts = rx_space.split(line)
            lin_type = parts[0]
            attr_list = [p.split('=', 2) for p in parts[1:] if p]
            attrs = {a[0]:a[1] if len(a) > 1 else None for a in attr_list}

            if lin_type == 'char':
                char_offs[int(attrs['id'])] = attrs

    for i in range(32, 256):
        attrs = char_offs[i]
        out.write(struct.pack('>HH', int(attrs['x']), int(attrs['width']) ))

if __name__ == '__main__':
    main(sys.argv)
