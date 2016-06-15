import textwrap, os
import rpy_ast

class CGenerator(object):
    """Generates the main .c file from the AST"""

    def __init__(self):
        pass

    def generate(self, script):
        method = getattr(self, 'generate_' + script.__class__.__name__)
        return method(script)

    def generate_RpyScript(self, script):
        image_files = ['extern uint16 vg_%s[];' % n for n in script.image_names()]
        image_images = ['const uint16 *vi_%s_%s = vg_%s;' % (n.name, n.state, n.image_name()) for n in script.images()]

        label_forwards = ['extern void *vn_%s();' % x.name for x in script.labels]
        label_functions = map(self.generate, script.labels)

        code_lists = [
            ['#include "script.h"'],
            image_files,
            image_images,
            label_forwards,
            label_functions
        ]

        return '\n\n'.join(('\n'.join(x) for x in code_lists if x)) + '\n'

    def generate_Label(self, label):
        commands = map(self.generate, label.commands);

        return self.prepare_template(r"""
        void *vn_%s() {
            %s

            return vn_start;
        }
        """) % (label.name, '\n\t'.join(commands))

    def generate_SayCmd(self, say):
        return self.prepare_template(r"""
        vnText("%s");
        """ % self.escape_string(say.text))

    def generate_SceneCmd(self, scene):
        return self.prepare_template(r"""
        vnScene(vi_%s_%s);
        """ % (scene.name, scene.state))

    def generate_ShowCmd(self, show):
        return self.prepare_template(r"""
        vnShow(vi_%s_%s);
        """ % (show.name, show.state))

    def prepare_template(self, template):
        return textwrap.dedent(template).strip()

    def escape_string(self, string):
        return string.replace('"', r'\"')



class MkIncludeGenerator(object):
    """Generates a Makefile .mk include from the AST."""

    def __init__(self):
        pass

    def generate(self, script):
        return textwrap.dedent("""
        IMGS := %s
        """ % self.generate_image_list(script))

    def generate_image_list(self, script):
        images = [n + '.apg' for n in script.image_names()]

        if not images:
            return ''

        return textwrap.dedent(r"""
        $(addprefix $(OBJDIR)/, %s)
        """.strip()) % ' '.join(images)



class ImageAsmGenerator(object):
    """Generates an .s assembly  from the AST, pointing to the images."""

    def __init__(self):
        pass

    def generate(self, script):
        images = script.image_names()
        img_globals = ['.globl _vg_%s' % i for i in images]
        img_includes = [textwrap.dedent(r"""
        _vg_%s:
        .incbin "build/%s.apg"
        """) % (i, i) for i in images]

        return textwrap.dedent("""
        .text

        %s

        %s

        """) % ('\n'.join(img_globals), '\n\n'.join(img_includes))
