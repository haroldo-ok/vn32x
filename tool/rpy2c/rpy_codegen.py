import textwrap

class CGenerator(object):
    def __init__(self):
        pass

    def generate(self, script):
        method = getattr(self, 'generate_' + script.__class__.__name__)
        return method(script)

    def generate_RpyScript(self, script):
        label_codes = map(self.generate, script.labels)
        return '\n'.join(['#include "script.h"'] + label_codes)

    def generate_Label(self, label):
        return self.prepare_template(r"""
        void *vn_%s() {
        }
        """) % label.name

    def prepare_template(self, template):
        return textwrap.dedent(template).strip()
