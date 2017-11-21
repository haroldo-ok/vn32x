# -*- coding: utf-8 -*-

import sys, os, argparse

__version__ = "0.1.0"

ACCEPTABLE_SIZES_KB = [256, 512, 1024, 2048, 3072, 4096]
ACCEPTABLE_SIZES_BYTES = [x * 1024 for x in ACCEPTABLE_SIZES_KB]

def main (argv):
    parser = argparse.ArgumentParser(description="Pads a ROM image to some standard size.")
    parser.add_argument("rom_file")
    opts = parser.parse_args()
    
    statinfo = os.stat(opts.rom_file)
    current_size = statinfo.st_size
	
    target_size = next(x for x in ACCEPTABLE_SIZES_BYTES if x >= current_size)
    padding_size = target_size - current_size
    
    if padding_size > 0:
        with open(opts.rom_file, 'ab') as f:
            f.write(b'\xFF' * padding_size)


if __name__ == '__main__':
    main(sys.argv)
