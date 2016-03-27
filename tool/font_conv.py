# -*- coding: utf-8 -*-

import sys, os, argparse
import Image, struct, shutil

__version__ = "0.1.0"

def main (argv):
    parser = argparse.ArgumentParser(description="Generates a bmf file from a BMFont fnt file.")
    parser.add_argument("bmf_file")
    parser.add_argument("fnt_file")
    opts = parser.parse_args()

    with open(opts.bmf_file, "wb") as out:
        img_file = os.path.splitext(opts.fnt_file)[0] + '_0.png'
        img_original = Image.open(img_file)
        img_mono = img_original.convert('L')

        (w, h) = img_mono.size
        out.write(struct.pack('>HHH', w, h, w*h))

        for b in img_mono.getdata():
            out.write(struct.pack('B', b))

if __name__ == '__main__':
    main(sys.argv)
