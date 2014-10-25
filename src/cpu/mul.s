;add mirriors cpu asm source
;date 2014年10月24日11:37:46

        xor r0,r0,r0
        addi r0,r0,10
        jmp mul

mul  :  xor r1,r1,r1
        xor r2,r2,r2
        addi r2,r2,1
loop:   addi r1,r1,1
        mul r2,r2,r1
        less r1,r0,loop
halt:   jmp halt
