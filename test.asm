    LD A, (1000H)
    LD B, A
    LD IX, 1010H
    LD D, 00H
    LD E, 00H
    LD C, 00H
ETI5: BIT 0, (IX+00H)
    JP Z, ETI1
ETI6: LD A, C
    CP 07H
    JP NZ, ETI2
    LD A, D
    CP B
    JP NZ, ETI3
    LD A, E
    CP 00H
    JP PO, ETI4
    LD (1020H), A
    LD A, 00H
    LD (1021H), A
ETI7: HALT
ETI2: RLC (IX+00H)
    INC C
    JP ETI5
ETI1: INC E
    JP ETI6
ETI3: INC IX
    INC D
    LD C, 00H
    JP ETI5
ETI4: LD (1020H), A
    LD A, 01H
    LD (1021H), A
    JP ETI7