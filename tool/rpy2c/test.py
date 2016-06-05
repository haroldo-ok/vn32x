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
        with io.StringIO() as out:
            rpy_codegen.CGenerator(out).generate(script)
            self.assertEqual('', out.getvalue())

    def test_empty(self):
        script = rpy_ast.RpyScript([], [])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertEqual('', c_code)

    def test_one_label(self):
        script = rpy_ast.RpyScript([], [
            rpy_ast.Label('test_label', [])
        ])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertSameCode(r"""
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
        void *vn_test_foo() {
        }
        void *vn_test_bar() {
        }
        """, c_code)

    def assertSameCode(self, expected, actual):
        expected = self.prepareCode(expected)
        actual = self.prepareCode(actual)
        self.assertEqual(expected, actual)

    def prepareCode(self, code):
        return self.RX_LEADING_TRAILING_BLANKS.sub(r'\1', code)

    RX_LEADING_TRAILING_BLANKS = re.compile(r'^\s*(.*?)\s*$', re.MULTILINE)




if __name__ == '__main__':
    unittest.main()
