.text
.globl _bedday
.globl _pose
.globl _text_frame
.globl _next_page_icon
.globl _default_font

_bedday:
.align 2
.incbin "build/bedday.apg"

_pose:
.align 2
.incbin "build/pose.apg"

_text_frame:
.align 2
.incbin "build/text_frame.apg"

_next_page_icon:
.align 2
.incbin "build/down.apg"

_default_font:
.align 2
.incbin "build/font_dejavu_sans.bmf"
