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
            show sylvie normal
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

    def test_simple_code(self):
        code = r"""
        label start:
            "Well, professor Eileen's lecture was interesting."
            "Sylvie" "Oh, hi, do we walk home together?"
            m "Yes..."
        """

        tree = rpy_parser.RpyParser().parse(textwrap.dedent(code))

        self.assertIsInstance(tree, rpy_ast.RpyScript)
        self.assertFalse(tree.declarations)
        self.assertEquals(1, len(tree.labels))
        self.assertEquals(3, len(tree.labels[0].commands))
        self.assertEquals("Well, professor Eileen's lecture was interesting.", tree.labels[0].commands[0].text)



class CodeGenTestCase(unittest.TestCase):

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



class TestCGenerator(CodeGenTestCase):

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
            return vn_start;
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
            return vn_start;
        }
        void *vn_test_bar() {
            return vn_start;
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
            return vn_start;
        }
        """, c_code)

    def test_say_special_chars(self):
        script = rpy_ast.RpyScript([], [
            rpy_ast.Label('test_something', [
                rpy_ast.SayCmd(None, "Okay, let's \"see\".")
            ]),
        ])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertSameCode(r"""
        extern void *vn_test_something();

        void *vn_test_something() {
            vnText("Okay, let's \"see\".");
            return vn_start;
        }
        """, c_code)

    def test_one_image(self):
        script = rpy_ast.RpyScript([
            rpy_ast.ImageDecl('bg', 'lecturehall', 'lecturehall.jpg')
        ], [])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertSameCode(r"""
        extern uint16 vg_lecturehall[];
        const uint16 *vi_bg_lecturehall = vg_lecturehall;
        """, c_code)

    def test_scene(self):
        script = rpy_ast.RpyScript([], [
            rpy_ast.Label('test_scene', [
                rpy_ast.SceneCmd('bg', 'lecturehall')
            ]),
        ])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertSameCode(r"""
        extern void *vn_test_scene();

        void *vn_test_scene() {
            vnScene(vi_bg_lecturehall);
            return vn_start;
        }
        """, c_code)

    def test_show(self):
        script = rpy_ast.RpyScript([], [
            rpy_ast.Label('test_show', [
                rpy_ast.ShowCmd('bg', 'lecturehall')
            ]),
        ])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertSameCode(r"""
        extern void *vn_test_show();

        void *vn_test_show() {
            vnShow(vi_bg_lecturehall);
            return vn_start;
        }
        """, c_code)

    def test_menu(self):
        script = rpy_ast.RpyScript([], [
            rpy_ast.Label('test_menu', [
                rpy_ast.MenuCmd([
                    rpy_ast.MenuOpt('First option', [
                        rpy_ast.JumpCmd('here')
                    ]),
                    rpy_ast.MenuOpt('Second option', [
                        rpy_ast.JumpCmd('there')
                    ])
                ])
            ]),
        ])
        c_code = rpy_codegen.CGenerator().generate(script)
        self.assertSameCode(r"""
        extern void *vn_test_menu();

        void *vn_test_menu() {
            int mn_choice, mn_option_1, mn_option_2;

            initMenu();
            mn_option_1 = addMenuItem("First option");
            mn_option_2 = addMenuItem("Second option");
            mn_choice = vnMenu();

        	if (mn_choice == mn_option_1) {
        		return vn_here;
        	} else if (mn_choice == mn_option_2) {
        		return vn_there;
        	}

            return vn_start;
        }
        """, c_code)



class TestMkIncludeGenerator(CodeGenTestCase):

    def test_empty(self):
        script = rpy_ast.RpyScript([], [])
        mk_code = rpy_codegen.MkIncludeGenerator().generate(script)
        self.assertSameCode('IMGS :=', mk_code)

    def test_one_image(self):
        script = rpy_ast.RpyScript([
            rpy_ast.ImageDecl('bg', 'lecturehall', 'lecturehall.jpg')
        ], [])
        mk_code = rpy_codegen.MkIncludeGenerator().generate(script)
        self.assertSameCode("""
        IMGS := $(addprefix $(OBJDIR)/, lecturehall.apg)
        """, mk_code)

    def test_two_images(self):
        script = rpy_ast.RpyScript([
            rpy_ast.ImageDecl('bg', 'lecturehall', 'lecturehall.jpg'),
            rpy_ast.ImageDecl('bg', 'uni', 'uni.jpg')
        ], [])
        mk_code = rpy_codegen.MkIncludeGenerator().generate(script)
        self.assertSameCode("""
        IMGS := $(addprefix $(OBJDIR)/, lecturehall.apg uni.apg)
        """, mk_code)



class TestImageAsmGenerator(CodeGenTestCase):

    def test_empty(self):
        script = rpy_ast.RpyScript([], [])
        asm_code = rpy_codegen.ImageAsmGenerator().generate(script)
        self.assertSameCode('.text', asm_code)

    def test_one_image(self):
        script = rpy_ast.RpyScript([
            rpy_ast.ImageDecl('bg', 'lecturehall', 'lecturehall.jpg')
        ], [])
        asm_code = rpy_codegen.ImageAsmGenerator().generate(script)
        self.assertSameCode(r"""
        .text

        .globl _vg_lecturehall

        _vg_lecturehall:
        .incbin "build/lecturehall.apg"
        """, asm_code)

    def test_two_images(self):
        script = rpy_ast.RpyScript([
            rpy_ast.ImageDecl('bg', 'lecturehall', 'lecturehall.jpg'),
            rpy_ast.ImageDecl('bg', 'uni', 'uni.jpg')
        ], [])
        asm_code = rpy_codegen.ImageAsmGenerator().generate(script)
        self.assertSameCode(r"""
        .text

        .globl _vg_lecturehall
        .globl _vg_uni

        _vg_lecturehall:
        .incbin "build/lecturehall.apg"

        _vg_uni:
        .incbin "build/uni.apg"
        """, asm_code)




if __name__ == '__main__':
    unittest.main()
