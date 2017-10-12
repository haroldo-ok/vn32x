.text

.globl _bedday
.globl _pose
.globl _text_frame
.globl _next_page_icon
.globl _default_font

.align 2
.word 0
_text_frame:
.incbin "build/text_frame.apg"

.align 2
.word 0
_next_page_icon:
.incbin "build/down.apg"

.align 2
.word 0
_default_font:
.incbin "build/font_dejavu_sans.bmf"
