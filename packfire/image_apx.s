.text
.globl _maruko
.globl _maruko_end
.globl _palette
.globl _palette_end

_maruko:
.incbin "maruko.apx"
_maruko_end:

_palette:
.incbin "maruko.pal"
_palette_end:

