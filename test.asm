     LD A, 00H
     LD (2000H), A
     LD A, 01H
     LD (2001H), A
     LD D, 20H
     LD IX, 2031H
     LD A, (2090H)
     DEC A
     LD (IX+00H), A
     CALL SUB1
     HALT
     LD A, (IX+00H)
     CP 01H
     JP Z, ETI1
     CP 00H
     JP Z, ETI1
     DEC A
     INC IX
     LD (IX+00H), A
     CALL SUB1
     LD A, (IX+00H)
     DEC A
     INC IX
     LD (IX+00H), A
     CALL SUB1
     LD E, (IX+00H)
     LD A, (DE)
     LD B, A
     DEC IX
     LD E, (IX+00H)
     LD A, (DE)
     ADD A, B
     LD C, A
     DEC IX
     LD A, (IX+00H)
     LD E, A
     LD A, C
     LD (DE), A
     RET
