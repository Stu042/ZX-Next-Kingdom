
SECTION GAMEPLAY_SEG
PUBLIC _GamePlayTest

    include "includes.inc"

_GamePlayTest:
    push    af
    ld      a,1
    nop
    ld      a,2
    pop     af
    ret

    dw _GamePlayTest
    db "Hello"



