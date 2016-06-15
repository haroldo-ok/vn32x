import sys, argparse, os
import rpy_parser, rpy_codegen

def main (argv):
    parser = argparse.ArgumentParser(description="Convert Ren'py scripts into C code.")
    parser.add_argument("source_rpy")
    parser.add_argument("destination_folder")
    opts = parser.parse_args()

    with open(os.path.abspath(opts.source_rpy), 'rt') as f:
        source = ''.join(f.readlines()).replace('\t', ' ')
        tree = rpy_parser.RpyParser().parse(source)

        dest_path = os.path.abspath(opts.destination_folder)

        # Generates the .c file
        c_code = rpy_codegen.CGenerator().generate(tree)
        with open(os.path.join(dest_path, 'generated_script.c'), 'wt') as outf:
            outf.write(c_code)

        # Generates the .mk file
        mk_code = rpy_codegen.CGenerator().generate(tree)
        with open(os.path.join(dest_path, 'generated_script.c'), 'wt') as outf:
            outf.write(c_code)


if __name__ == '__main__':
    main(sys.argv)
