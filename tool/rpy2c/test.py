import unittest
import textwrap, io, re
import rpy_parser, rpy_ast, rpy_codegen

class TestParser(unittest.TestCase):

    def test_code(self):
        code = r"""
        # Declare images used by this game.
        image bg lecturehall = "lecturehall.jpg"
        image bg uni = "uni.jpg"
        define s = Character("Sylvie")
        define s = Character("Sylvie", color="#c8ffc8")
        define s = Character('Sylvie', color="#c8ffc8")
        #define m = Character('Me', color="#c8c8ff")

        # The game starts here.
        label start:
            scene bg lecturehall
            "Well, professor Eileen's lecture was interesting."
            "Sylvie" "Oh, hi, do we walk home together?"
            m "Yes..."
            menu:
                "It's a story with pictures.":
                    "Text inside a menu option."
                    jump vn

                "It's a hentai game.":
                    jump hentai

        label another:
            "This is a test."

        #Comment here
        """

        tree = rpy_parser.RpyParser().parse(textwrap.dedent(code))
        self.assertIsInstance(tree, rpy_ast.RpyScript)



class TestCodeGen(unittest.TestCase):

    def test_empty(self):
        script = rpy_ast.RpyScript([], [])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertEqual('#include "script.h"\n', c_code)

    def test_one_label(self):
        script = rpy_ast.RpyScript([], [
            rpy_ast.Label('test_label', [])
        ])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertSameCode(r"""
        extern void *vn_test_label();

        void *vn_test_label() {
        }
        """, c_code)

    def test_two_labels(self):
        script = rpy_ast.RpyScript([], [
            rpy_ast.Label('test_foo', []),
            rpy_ast.Label('test_bar', [])
        ])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertSameCode(r"""
        extern void *vn_test_foo();
        extern void *vn_test_bar();

        void *vn_test_foo() {
        }
        void *vn_test_bar() {
        }
        """, c_code)

    def test_say(self):
        script = rpy_ast.RpyScript([], [
            rpy_ast.Label('test_something', [
                rpy_ast.SayCmd(None, "Okay, let's see.")
            ]),
        ])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertSameCode(r"""
        extern void *vn_test_something();

        void *vn_test_something() {
        vnText("Okay, let's see.");
        }
        """, c_code)

    def assertSameCode(self, expected, actual):
        expected = self.prepareCode(expected)
        actual = self.prepareCode(actual)
        self.assertEqual(expected, actual)

    def prepareCode(self, code):
        steps = [
            (self.RX_INCLUDES, ''),
            (self.RX_BLANK_LINES, '\n'),
            (self.RX_LEADING_TRAILING_BLANKS, r'\1')
        ]
        return reduce(lambda s, cmd: cmd[0].sub(cmd[1], s), steps, code)

    RX_LEADING_TRAILING_BLANKS = re.compile(r'^\s*(.*?)\s*$', re.MULTILINE)
    RX_INCLUDES = re.compile(r'^#include .*$', re.MULTILINE)
    RX_BLANK_LINES = re.compile(r'\n\s*\n+')




if __name__ == '__main__':
    unittest.main()
