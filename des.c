#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *getByte();
char *getInstruction();

int main(int argvc, char **argv) {
    FILE *archive;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    archive = fopen(argv[1], "r");
    if (archive == NULL){
        puts("Unable to open the file");
    } else {
        
        while ( (read = getline(&line, &len, archive)) != EOF ) {
            printf("%s", line);
            char *size;
            size[0] = line[1];
            size[1] = line[2];
            uint8_t lineSize = (uint8_t) size;

        }
        fclose(archive);
        if (line)   
            free(line);
    }
    return EXIT_SUCCESS;
}





char  *getInstruction(uint8_t opcode) {
	char *argument1;
	char *argument2;

	switch(opcode){
		
		case 0x00:									//	NOP
			return "NOP";
		case 0x01:									//	LD BC, nn
			argument1 = getByte();
			argument2 = getByte();
			strcat(argument2, argument1);
			strcpy(argument1, "LD BC, ");
			strcat(argument1, argument2);
			return argument1;
		case 0x02: 									//	LD (BC), A
			return "LD (BC),A";
		case 0x03:									//	INC BC
			return "INC BC";									
		case 0x04:									//	INC B
			return "INC B";									
		case 0x05:									//	DEC B
			return "DEC B";									
		case 0x06:									//	LD B, n
			argument1 = getByte();
			strcpy(argument2, "LD B, ");
			strcat(argument2, argument1);
			return argument2;
		case 0x07:									//	RLCA
			return "RLCA";
		case 0x08:          						//	EX AF, AF'
			return "EX AF, AF'"
		case 0x09:									//	ADD HL, BC
			return "ADD HL, BC";
		case 0x0A:									//	LD A,(BC)
			return "LD A,(BC)";
		case 0x0B:									//  DEC BC
			return "DEC BC";
		case 0x0C:									//	INC C
			return "INC C";
		case 0x0D:									//	DEC C
			return "DEC C";
		case 0x0E:									//	LD C, n 
			argument1 = getByte();
			strcpy(argument2, "LD C, ");
			strcat(argument2, argument1);
			return argument2;
		case 0x0F:									//	RRCA
			return "RRCA";
		case 0x10:									//	DJNZ, e
			// funcion getEti()
			return;
		case 0x11:									//	LD DE, nn
			argument1 = getByte();
			argument2 = getByte();
			strcat(argument2, argument1);
			strcpy(argument1, "LD DE, ");
			strcat(argument1, argument2);
			return argument1;
		case 0x12:									//	LD (DE), A
			return "LD (DE), A";
		case 0x13:									//	INC DE
			return "INC DE";
		case 0x14:									//	INC D 
			return "INC D";
		case 0x15:									//	DEC D
			return "DEC D";
		case 0x16:									//	LD D, n
			argument1 = getByte();
			strcpy(argument2, "LD D, ");
			strcat(argument2, argument1);
			return argument2;
		case 0x17:									//	RLA
			return "RLA";
		case 0x18:									//  JR e
			// funcion getEti()
			return;
		case 0x19:									//  ADD HL, DE
			return "ADD HL, DE";
		case 0x1A:									//	LD A, (DE)
			return "LD A, (DE)";
		case 0x1B:									//	DEC DE
			return "DEC DE";
		case 0x1C:									// 	INC E
			return "INC E";
		case 0x1D:									//	DEC E
			return "DEC E";
		case 0x1E:									//	LD E, n
			argument1 = getByte();
			strcpy(argument2, "LD E, ");
			strcat(argument2, argument1);
			return argument2;
		case 0x1F:									//	RRA
			return "RRA";
		/*case 0x20:									//  JR NZ, e
			return;
		case 0x21:									//  LD HL, nn
			return;
		case 0x22:									//  LD (nn), HL
			return;
		case 0x23:									//  INC HL
			return;
		case 0x24:									//  INC H
			return;
		case 0x25:									//  DEC H
			return;
		case 0x26:									//  LD H, n
			return;
		case 0x27:									//  DAA
			return;
		case 0x28:									//  JR Z, e
			return;
		case 0x29:									//  ADD HL,HL
			return;
		case 0x2A:									//  LD HL, (nn)
			return;
		case 0x2B:									//  DEC HL
			return;
		case 0x2C:									//  INC L
			return;
		case 0x2D:									//  DEC L
			return;
		case 0x2E:									//  LD L, n
			return;
		case 0x2F:									//  CPL
			return;
		case 0x30:									//  JR NC, e
			return;
		case 0x31:									//  LD SP, nn
			return;
		case 0x32: 									//  LD (nn), A
			return;
		case 0x33:									//  INC SP
			return;
		case 0x34:									//  INC (HL)
			return;
		case 0x35:									//  DEC (HL)
			return;
		case 0x36:									//  LD (HL), n
			return;
		case 0x37:									//  SCF
			return;
		case 0x38:									//  JR C, e
			return;
		case 0x39:									//	ADD HL, SP
			return;
		case 0x3A:									//  LD A, (nn)
			return;
		case 0x3B:									//  DEC SP
			return;
		case 0x3C:									//  INC A
			return;
		case 0x3D:									//  DEC A
			return;
		case 0x3E:									//  LD A, n
			return;
		case 0x3F:									//  CCF
			return;
		case 0x40:									//  LD B, B
			return;
		case 0x41:									//  LD B, C
			return;
		case 0x42: 									//  LD B, D
			return;
		case 0x43:									//  LD B, E
			return;
		case 0x44:									//  LD B, H
			return;
		case 0x45:									//  LD B, L
			return;
		case 0x46:									//  LD B, (HL)
			return;
		case 0x47:									//  LD B, A
			return;
		case 0x48:									//  LD C, B
			return;
		case 0x49:									//  LD C, C
			return;
		case 0x4A:									//  LD C, D
			return;
		case 0x4B:									//  LD C, E
			return;
		case 0x4C:									//  LD C, H
			return;
		case 0x4D:									//  LD C, L
			return;
		case 0x4E:									//  LD C, (HL)
			return;
		case 0x4F:									//  LD C, A
			return;
		case 0x50:									//  LD D, B
			return;
		case 0x51:									//  LD D, C
			return;
		case 0x52: 									//  LD D, D
			return;
		case 0x53:									//  LD D, E
			return;
		case 0x54:									//  LD D, H
			return;
		case 0x55:									//  LD D, L
			return;
		case 0x56:									//  LD D, (HL)
			return;
		case 0x57:									//  LD D, A
			return;
		case 0x58:									//  LD E, B
			return;
		case 0x59:									//  LD E, C
			return;
		case 0x5A:									//  LD E, D
			return;
		case 0x5B:									//  LD E, E
			return;
		case 0x5C:									//  LD E, H
			return;
		case 0x5D:									//  LD E, L
			return;
		case 0x5E:									//  LD E, (HL)
			return;
		case 0x5F:									//  LD E, A
			return;
		case 0x60:									//  LD H, B
			return;
		case 0x61:									//  LD H, C
			return;
		case 0x62: 									//  LD H, D
			return;
		case 0x63:									//  LD H, E
			return;
		case 0x64:									//  LD H, H
			return;
		case 0x65:									//  LD H, L
			return;
		case 0x66:									//  LD H, (HL)
			return;
		case 0x67:									//  LD H, A
			return;
		case 0x68:									//  LD L, B
			return;
		case 0x69:									//  LD L, C
			return;
		case 0x6A:									//  LD L, D
			return;
		case 0x6B:									//  LD L, E
			return;
		case 0x6C:									//  LD L, H
			return;
		case 0x6D:									//  LD L, L
			return;
		case 0x6E:									//  LD L, (HL)
			return;
		case 0x6F:									//  LD L, A
			return;
		case 0x70:									//  LD (HL), B
			return;
		case 0x71:									//  LD (HL), C
			return;
		case 0x72: 									//  LD (HL), D
			return;
		case 0x73:									//  LD (HL), E
			return;
		case 0x74:									//  LD (HL), H
			return;
		case 0x75:									//  LD (HL), L
			return;
		case 0x76:									//  HALT
			return;
		case 0x77:									//  LD (HL), A
			return;
		case 0x78:									//  LD A, B
			return;
		case 0x79:									//  LD A, C
			return;
		case 0x7A:									//  LD A, D
			return;
		case 0x7B:									//  LD A, E
			return;
		case 0x7C:									//  LD A, H
			return;
		case 0x7D:									//  LD A, L
			return;
		case 0x7E:									//  LD A, (HL)
			return;
		case 0x7F:									//  LD A, A
			return;
		case 0x80:									//  ADD A, B
			return;
		case 0x81:									//  ADD A, C
			return;
		case 0x82:									//  ADD A, D
			return;
		case 0x83:									//  ADD A, E
			return;
		case 0x84:									//  ADD A, H
			return;
		case 0x85:									//  ADD A, L
			return;
		case 0x86:									//  ADD A, (HL)
			return;
		case 0x87:									//  ADD A, A
			return;
		case 0x88:									//  ADC A, B
			return;
		case 0x89:									//  ADC A, C
			return;
		case 0x8A:									//  ADC A, D
			return;
		case 0x8B:									//  ADC A, E
			return;
		case 0x8C:									//  ADC A, H
			return;
		case 0x8D:									//  ADC A, L
			return;
		case 0x8E:									//  ADC A, (HL)
			return;
		case 0x8F:									//  ADC A, A
			return;
		case 0x90:									//  SUB A, B
			return;
		case 0x91:									//  SUB A, C
			return;
		case 0x92: 									//  SUB A, D
			return;
		case 0x93:									//  SUB A, E
			return;
		case 0x94:									//  SUB A, H
			return;
		case 0x95:									//  SUB A, L
			return;
		case 0x96:									//  SUB A, (HL)
			return;
		case 0x97:									//  SUB A, A
			return;
		case 0x98:									//  SBC A, B
			return;
		case 0x99:									//  SBC A, C
			return;
		case 0x9A:									//  SBC A, D
			return;
		case 0x9B:									//  SBC A, E
			return;
		case 0x9C:									//  SBC A, H
			return;
		case 0x9D:									//  SBC A, L
			return;
		case 0x9E:									//  SBC A, (HL)
			return;
		case 0x9F:									//  SBC A, A
			return;
		case 0xA0:									//  AND B
			return;
		case 0xA1:									//  AND C
			return;
		case 0xA2: 									//  AND D
			return;
		case 0xA3:									//  AND E
			return;
		case 0xA4:									//  AND H
			return;
		case 0xA5:									//  AND L
			return;
		case 0xA6:									//  AND (HL)
			return;
		case 0xA7:									//  AND A
			return;
		case 0xA8:									//  XOR B
			return;
		case 0xA9:									//  XOR C
			return;
		case 0xAA:									//  XOR D
			return;
		case 0xAB:									//  XOR E
			return;
		case 0xAC:									//  XOR H
			return;
		case 0xAD:									//  XOR L
			return;
		case 0xAE:									//  XOR (HL)
			return;
		case 0xAF:									//  XOR A
			return;
		case 0xB0:									//  OR B
			return;
		case 0xB1:									//  OR C
			return;
		case 0xB2: 									//  OR D
			return;
		case 0xB3:									//  OR E
			return;
		case 0xB4:									//  OR H
			return;
		case 0xB5:									//  OR L
			return;
		case 0xB6:									//  OR (HL)
			return;
		case 0xB7:									//  OR A
			return;
		case 0xB8:									//  CP B
			return;
		case 0xB9:									//  CP C
			return;
		case 0xBA:									//  CP D
			return;
		case 0xBB:									//  CP E
			return;
		case 0xBC:									//  CP H
			return;
		case 0xBD:									//  CP L
			return;
		case 0xBE:									//  CP (HL)
			return;
		case 0xBF:									//  CP A
			return;
		case 0xC0:									//  RET NZ
			return;
		case 0xC1:									//  POP BC
			return;
		case 0xC2: 									//  JP NZ, nn
			return;
		case 0xC3:									//  JP nn
			return;
		case 0xC4:									//  CALL NZ, nn
			return;
		case 0xC5:									//  PUSH BC
			return;
		case 0xC6:									//  ADD A, n
			return;
		case 0xC7:									//  RST 00H
			return;
		case 0xC8:									//  RET Z
			return;
		case 0xC9:									//  RET
			return;
		case 0xCA:									//  JP Z, nn
			return;
		case 0xCB:									//  ** CB **
			return;
		case 0xCC:									//  CALL Z, nn
			return;
		case 0xCD:									//  CALL nn
			return;
		case 0xCE:									//  ADC A, n
			return;
		case 0xCF:									//  RST 08H
			return;
		case 0xD0:									//  RET NC
			return;
		case 0xD1:									//  POP DE
			return;
		case 0xD2: 									//  JP NC, nn
			return;
		case 0xD3:									//  OUT (n), A
			return;
		case 0xD4:									//  CALL NC, nn
			return;
		case 0xD5:									//  PUSH DE
			return;
		case 0xD6:									//  SUB A, n
			return;
		case 0xD7:									//  RST 10H
			return;
		case 0xD8:									//  RET C
			return;
		case 0xD9:									//  EXX
			return;
		case 0xDA:									//  JP C, nn
			return;
		case 0xDB:									//  IN A, (n)
			return;
		case 0xDC:									//  CALL C, nn
			return;
		case 0xDD:									//  ** DD **
			return;
		case 0xDE:									//  SBC A, n
			return;
		case 0xDF:									//  RST 18H
			return;
		case 0xE0:									//  RET PO
			return;
		case 0xE1:									//  POP HL
			return;
		case 0xE2: 									//  JP PO, nn
			return;
		case 0xE3:									//  EX (SP), HL
			return;
		case 0xE4:									//  CALL PO, nn
			return;
		case 0xE5:									//  PUSH HL
			return;
		case 0xE6:									//  AND n
			return;
		case 0xE7:									//  RST 20H
			return;
		case 0xE8:									//  RET PE
			return;
		case 0xE9:									//  JP (HL)
			return;
		case 0xEA:									//  PE, nn
			return;
		case 0xEB:									//  EX DE, HL
			return;
		case 0xEC:									//  CALL PE, nn
			return;
		case 0xED:									//  ** ED **
			return;
		case 0xEE:									//  XOR n
			return;
		case 0xEF:									//  RST 28H
			return;
		case 0xF0:									//  RET P
			return;
		case 0xF1:									//  POP AF
			return;
		case 0xF2: 									//  JP P, nn
			return;
		case 0xF3:									//  DI
			return;
		case 0xF4:									//  CALL P, nn
			return;
		case 0xF5:									//  PUSH AF
			return;
		case 0xF6:									//  OR n
			return;
		case 0xF7:									//  RST 30H
			return;
		case 0xF8:									//  RET M
			return;
		case 0xF9:									//  LD SP, HL
			return;
		case 0xFA:									//  JP M, nn
			return;
		case 0xFB:									//  EI
			return;
		case 0xFC:									//  CALL M, nn
			return;
		case 0xFD:									//  ** FD **
			return;
		case 0xFE:									//  CP n
			return;
		case 0xFF:									//  RST 38H
			return;*/
		default:
			printf("que es estoo");
			return "ERROR CRITICO";
	}
}
