.text
.align 2

.globl _bedday
.globl _pose
.globl _text_frame
.globl _next_page_icon
.globl _default_font

_text_frame:
.incbin "build/text_frame.apg"

_next_page_icon:
.incbin "build/down.apg"

_default_font:
.incbin "build/font_dejavu_sans.bmf"
