.text
.globl _maruko
.globl _maruko_end
.globl _palette
.globl _palette_end
.globl _test
.globl _test_end
.globl _test_palette
.globl _test_palette_end

_maruko:
.incbin "build/maruko.apx"
_maruko_end:

_palette:
.incbin "build/maruko.pal"
_palette_end:

_test:
.incbin "build/test.apx"
_test_end:

_test_palette:
.incbin "build/test.pal"
_test_palette_end:
