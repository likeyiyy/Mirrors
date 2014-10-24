;add mirriors cpu asm source
;date 2014年10月24日11:37:46

        xor r0,r0,r0
        addi r0,r0,100
        jmp sum

sum  :    
        xor        r1,r1,r1
        xor r2,r2,r2
loop:   addi r1,r1,1
        add r2,r2,r1
        less r1,r0,loop
halt:   jmp halt
