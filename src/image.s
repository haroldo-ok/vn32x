.text
.globl _bedday
.globl _test
.globl _text_frame

_bedday:
.incbin "build/bedday.apg"

_test:
.incbin "build/test.apg"

_text_frame:
.incbin "build/text_frame.apg"
