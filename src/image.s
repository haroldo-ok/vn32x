.text
.globl _bedday
.globl _pose
.globl _text_frame

_bedday:
.incbin "build/bedday.apg"

_pose:
.incbin "build/pose.apg"

_text_frame:
.incbin "build/text_frame.apg"
