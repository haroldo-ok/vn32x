import textwrap

class CGenerator(object):
    def __init__(self):
        pass

    def generate(self, script):
        method = getattr(self, 'generate_' + script.__class__.__name__)
        return method(script)

    def generate_RpyScript(self, script):
        label_forwards = ['extern void *vn_%s();' % x.name for x in script.labels]
        label_functions = map(self.generate, script.labels)

        code_lists = [
            ['#include "script.h"'],
            label_forwards,
            label_functions
        ]

        return '\n\n'.join(('\n'.join(x) for x in code_lists if x)) + '\n'

    def generate_Label(self, label):
        commands = map(self.generate, label.commands);

        return self.prepare_template(r"""
        void *vn_%s() {
            %s
        }
        """) % (label.name, '\n\t'.join(commands))

    def generate_SayCmd(self, say):
        return self.prepare_template(r"""
        vnText("%s");
        """ % say.text)

    def prepare_template(self, template):
        return textwrap.dedent(template).strip()
