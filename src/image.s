.text
.globl _maruko
.globl _test
.globl _text_frame

_maruko:
.incbin "build/maruko.apg"

_test:
.incbin "build/test.apg"

_text_frame:
.incbin "build/text_frame.apg"
