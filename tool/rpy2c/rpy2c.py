import sys, argparse, os
import rpy_parser

def main (argv):
    parser = argparse.ArgumentParser(description="Convert Ren'py scripts into C code.")
    parser.add_argument("source_rpy")
    parser.add_argument("destination_folder")
    opts = parser.parse_args()

    with open(os.path.abspath(opts.source_rpy), 'rt') as f:
        source = ''.join(f.readlines()).replace('\t', ' ')
        tree = rpy_parser.RpyParser().parse(source)


if __name__ == '__main__':
    main(sys.argv)
