
	.include "atari.inc"
	.export _mem_detect, _banks, _bsav

	;; Detect extended memory banks for XL/XE systems.
	;; Borrowed from: 
; labels
ext_b = $4000; anything in the range of $ 4000- $ 7FFF
portb = $d301

; procedure
_mem_detect:	
       lda portb
       pha

       lda #$ff
       sta portb

       lda ext_b
       pha

       ldx #$0f; remember bytes ext (from 16 blocks of 64k)
_p0:   jsr setpb
       lda ext_b
       sta _bsav, x
       dex
       bpl _p0

       ldx #$0f; reset them (in a separate loop, because it is not known
_p1:   jsr setpb; which PORTB bit combinations choose the same banks)
       lda #$00
       sta ext_b
       dex
       bpl _p1

       stx portb; elimination of basic memory
       stx ext_b
       stx $00; necessary for some extensions up to 256k

       ldy #$00; loop counting blocks 64k
       ldx #$0f
_p2:   jsr setpb
       lda ext_b; if ext_b is nonzero, the 64k block is already counted
       bne _n2

       dec ext_b; otherwise mark as counted

       lda ext_b; check if checked; if not -> something is wrong with the equipment
       bpl _n2

       lda portb; enter PORTB into the array for bank 0
       sta _banks, y
       eor #%00000100; complete the values ​​for banks 1, 2, 3
       sta _banks + 1, y
       eor #%00001100
       sta _banks + 2, y
       eor #%00000100
       sta _banks + 3, y
       iny
       iny
       iny
       iny

_n2:   dex
       bpl _p2

       ldx #$0f; restore content ext
_p3:   jsr setpb
       lda _bsav, x
       sta ext_b
       dex
       bpl _p3

       stx portb; X = $ FF

       pla
       sta ext_b

       pla
       sta portb
       tya
       rts

; subroutines
setpb: txa ; bit order change:% 0000dcba ->% cba000d0
       lsr            
       ror
       ror
       ror
       adc #$01 ; setting bit 1 depending on the state of C
       ora #$01 ; setting the OS ROM control bit to the default value
       sta portb
       rts

; variables
_banks: .res 64
_bsav:  .res 16
