    CPU "Z80.TBL"
    HOF "INT8"
    ADD IX, BC
    ADD IX, DE
    LD IX, 1000H
    LD (1000H), IX
    INC IX
    ADD IX, IX
    LD IX, (1000H)
    DEC IX
    INC (IX+02H)
    DEC (IX+02H)
    LD (IX+02), 55
    ADD IX, SP
    LD B, (IX+00H)
    LD C, (IX+00H)
    LD D, (IX+00H)
    LD E, (IX+00H)
    LD H, (IX+02H)
    LD L, (IX+02H)
    LD (IX+02H), B
    LD (IX+02H), C
    LD (IX+02H), D
    LD (IX+02H), E
    LD (IX+02H), H
    LD (IX+02H), L
    LD (IX+02H), A
    LD A, (IX+00H)
    ADD A, (IX+00H)
    ADC A, (IX+00H)
    SUB (IX+00H)
    AND (IX+00H)
    XOR (IX+00H)
    OR (IX+00H)
    CP (IX+00H)
    POP IX
    EX (SP), IX
    PUSH IX
    JP (IX)
    ADD IY, BC
    ADD IY, DE
    LD IY, 1000H
    LD (1000H), IY
    INC IY
    ADD IY, IY
    LD IY, (1000H)
    DEC IY
    INC (IY+02H)
    DEC (IY+02H)
    LD (IY+02), 55
    ADD IY, SP
    LD B, (IY+00H)
    LD C, (IY+00H)
    LD D, (IY+00H)
    LD E, (IY+00H)
    LD H, (IY+02H)
    LD L, (IY+02H)
    LD (IY+02H), B
    LD (IY+02H), C
    LD (IY+02H), D
    LD (IY+02H), E
    LD (IY+02H), H
    LD (IY+02H), L
    LD (IY+02H), A
    LD A, (IY+00H)
    ADD A, (IY+00H)
    ADC A, (IY+00H)
    SUB (IY+00H)
    AND (IY+00H)
    XOR (IY+00H)
    OR (IY+00H)
    CP (IY+00H)
    POP IY
    EX (SP), IY
    PUSH IY
    JP (IY)