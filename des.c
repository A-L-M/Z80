#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *getInstruction(int, char[]);
void getByte(char [], char []);
void complete(const char *, char [], char []);

int i;  //contador
char buffer[20];

int main(int argvc, char **argv) {
    FILE *file;
    char line[45];
    char byte[3];
    int dbytes_in_line;  //cantidad de bytes que representan codigos de operacion en una linea del archivo
    int opcode;  //codigo de operacion, guardado en base 10
    char *mnemonico;

    file = fopen("test.txt", "r");
    if (file == NULL){
        puts("Unable to open the file");
    } 
	else 
	{
        while ( fgets(line, sizeof(line), file) != NULL ) 
		{
            byte[0] = line[1];
            byte[1] = line[2];
            dbytes_in_line = (int) strtol(byte, NULL, 16);  //Toma la cadena almacenada en byte[] y la convierte a int, leyendo en base 16
            dbytes_in_line = (dbytes_in_line * 2) + 9;  //dos digitos por byte, +9 digitos que se ignoran al inicio
            for(i=9; i < dbytes_in_line;)
			{
				getByte(byte, line);
				opcode = (int) strtol(byte, NULL, 16);
				mnemonico = getInstruction(opcode, line);
				printf("%s\n", mnemonico);
			}
			dbytes_in_line = 0;
            printf("\n");
        }
        fclose(file);
    }
    return EXIT_SUCCESS;
}

void getByte(char argument[], char line[]){
	argument[0] = line[i];
	argument[1] = line[i+1];
	i += 2;
}

void complete(const char *arg1, char arg2[], char arg3[]){  //junta primera parte de mnemonico con n o nn, inserta H al final
	char aux[20];
	strcpy(aux, arg1);
	strcat(arg2, arg3);
	strcat(aux, arg2);
	strcat(aux, "H");
	strcpy(buffer, aux);
}

char * getInstruction(int opcode, char line[]) {
	char argument1[20];  //byte mas significativo
	char argument2[20];  //byte menos significativo
	
	/*AGREGAR H AL FINAL DE HEXADECIMAL*/
	/*SI LA INSTRUCCION SE CORTA PORQUE SE ACABO LA LINEA FALLA, se puede poner getNewLine()*/
	switch(opcode){
		
		case 0x00:									//	NOP
			return "NOP";
		case 0x01:									//	LD BC, nn
			getByte(argument2, line);
			getByte(argument1, line);
			complete("LD BC, ", argument2, argument1);
			return buffer;
		case 0x02: 									//	LD (BC), A
			return "LD (BC),A";
		case 0x03:									//	INC BC
			return "INC BC";									
		case 0x04:									//	INC B
			return "INC B";									
		case 0x05:									//	DEC B
			return "DEC B";									
		case 0x06:									//	LD B, n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("LD B, ", argument1, argument2);
			return buffer;
		case 0x07:									//	RLCA
			return "RLCA";
		case 0x08:          						//	EX AF, AF'
			return "EX AF, AF'";
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
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("LD C, ", argument1, argument2);
			return buffer;
		case 0x0F:									//	RRCA
			return "RRCA";
		case 0x10:									//	DJNZ, e
			// funcion getEti()
			return "Incompleto";
		case 0x11:									//	LD DE, nn
			getByte(argument2, line);
			getByte(argument1, line);
			complete("LD DE, ", argument2, argument1);
			return buffer;
		case 0x12:									//	LD (DE), A
			return "LD (DE), A";
		case 0x13:									//	INC DE
			return "INC DE";
		case 0x14:									//	INC D 
			return "INC D";
		case 0x15:									//	DEC D
			return "DEC D";
		case 0x16:									//	LD D, n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("LD D, ", argument1, argument2);
			return buffer;
		case 0x17:									//	RLA
			return "RLA";
		case 0x18:									//  JR e
			// funcion getEti()
			return "Incompleto";
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
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("LD E, ", argument1, argument2);
			return buffer;
		case 0x1F:									//	RRA
			return "RRA";
		case 0x20:									//  JR NZ, e
			// funcion getEti()
			return "Incompleto";
		case 0x21:									//  LD HL, nn
			getByte(argument2, line);
			getByte(argument1, line);
			complete("LD HL, ", argument2, argument1);
			return buffer;
		case 0x22:									//  LD (nn), HL
			getByte(argument2, line);
			getByte(argument1, line);
			complete("LD (", argument2, argument1);
			strcat(buffer, "), HL");
			return buffer;
		case 0x23:									//  INC HL
			return "INC HL";
		case 0x24:									//  INC H
			return "INC H";
		case 0x25:									//  DEC H
			return "DEC H";
		case 0x26:									//  LD H, n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("LD H, ", argument1, argument2);
			return buffer;
		case 0x27:									//  DAA
			return "DAA";
		case 0x28:									//  JR Z, e
			//funcion getEti()
			return "Incompleto";
		case 0x29:									//  ADD HL, HL
			return "ADD HL, HL";
		case 0x2A:									//  LD HL, (nn)
			getByte(argument2, line);
			getByte(argument1, line);
			complete("LD HL, (", argument2, argument1);
			strcat(buffer, ")");
			return buffer;
		case 0x2B:									//  DEC HL
			return "DEC HL";
		case 0x2C:									//  INC L
			return "INC L";
		case 0x2D:									//  DEC L
			return "DEC L";
		case 0x2E:									//  LD L, n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("LD L, ", argument1, argument2);
			return buffer;
		case 0x2F:									//  CPL
			return "CPL";
		case 0x30:									//  JR NC, e
			//funcion getEti()
			return "Incompleto";
		case 0x31:									//  LD SP, nn
			getByte(argument2, line);
			getByte(argument1, line);
			complete("LD SP, ", argument2, argument1);
			return buffer;
		case 0x32: 									//  LD (nn), A
			getByte(argument2, line);
			getByte(argument1, line);
			complete("LD (", argument2, argument1);
			strcat(buffer, "), A");
			return buffer;
		case 0x33:									//  INC SP
			return "INC SP";
		case 0x34:									//  INC (HL)
			return "INC (HL)";
		case 0x35:									//  DEC (HL)
			return "DEC (HL)";
		case 0x36:									//  LD (HL), n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("LD (HL), ", argument1, argument2);
			return buffer;
		case 0x37:									//  SCF
			return "SCF";
		case 0x38:									//  JR C, e
			//funcion getEti()
			return "Incompleto";
		case 0x39:									//	ADD HL, SP
			return "ADD HL, SP";
		case 0x3A:									//  LD A, (nn)
			getByte(argument2, line);
			getByte(argument1, line);
			complete("LD A, (", argument2, argument1);
			strcat(buffer, ")");
			return buffer;
		case 0x3B:									//  DEC SP
			return "DEC SP";
		case 0x3C:									//  INC A
			return "INC A";
		case 0x3D:									//  DEC A
			return "DEC A";
		case 0x3E:									//  LD A, n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("LD A, ", argument1, argument2);
			return buffer;
		case 0x3F:									//  CCF
			return "CCF";
		case 0x40:									//  LD B, B
			return "LD B, B";
		case 0x41:									//  LD B, C
			return "LD B, C";
		case 0x42: 									//  LD B, D
			return "LD B, D";
		case 0x43:									//  LD B, E
			return "LD B, E";
		case 0x44:									//  LD B, H
			return "LD B, H";
		case 0x45:									//  LD B, L
			return "LD B, L";
		case 0x46:									//  LD B, (HL)
			return "LD B, (HL)";
		case 0x47:									//  LD B, A
			return "LD B, A";
		case 0x48:									//  LD C, B
			return "LD C, B";
		case 0x49:									//  LD C, C
			return "LD C, C";
		case 0x4A:									//  LD C, D
			return "LD C, D";
		case 0x4B:									//  LD C, E
			return "LD C, E";
		case 0x4C:									//  LD C, H
			return "LD C, H";
		case 0x4D:									//  LD C, L
			return "LD C, L";
		case 0x4E:									//  LD C, (HL)
			return "LD C, (HL)";
		case 0x4F:									//  LD C, A
			return "LD C, A";
		case 0x50:									//  LD D, B
			return "LD D, B";
		case 0x51:									//  LD D, C
			return "LD D, C";
		case 0x52: 									//  LD D, D
			return "LD D, D";
		case 0x53:									//  LD D, E
			return "LD D, E";
		case 0x54:									//  LD D, H
			return "LD D, H";
		case 0x55:									//  LD D, L
			return "LD D, L";
		case 0x56:									//  LD D, (HL)
			return "LD D, (HL)";
		case 0x57:									//  LD D, A
			return "LD D, A";
		case 0x58:									//  LD E, B
			return "LD E, B";
		case 0x59:									//  LD E, C
			return "LD E, C";
		case 0x5A:									//  LD E, D
			return "LD E, D";
		case 0x5B:									//  LD E, E
			return "LD E, E";
		case 0x5C:									//  LD E, H
			return "LD E, H";
		case 0x5D:									//  LD E, L
			return "LD E, L";
		case 0x5E:									//  LD E, (HL)
			return "LD E, (HL)";
		case 0x5F:									//  LD E, A
			return "LD E, A";
		case 0x60:									//  LD H, B
			return "LD H, B";
		case 0x61:									//  LD H, C
			return "LD H, C";
		case 0x62: 									//  LD H, D
			return "LD H, D";
		case 0x63:									//  LD H, E
			return "LD H, E";
		case 0x64:									//  LD H, H
			return "LD H, H";
		case 0x65:									//  LD H, L
			return "LD H, L";
		case 0x66:									//  LD H, (HL)
			return "LD H, (HL)";
		case 0x67:									//  LD H, A
			return "LD H, A";
		case 0x68:									//  LD L, B
			return "LD L, B";
		case 0x69:									//  LD L, C
			return "LD L, C";
		case 0x6A:									//  LD L, D
			return "LD L, D";
		case 0x6B:									//  LD L, E
			return "LD L, E";
		case 0x6C:									//  LD L, H
			return "LD L, H";
		case 0x6D:									//  LD L, L
			return "LD L, L";
		case 0x6E:									//  LD L, (HL)
			return "LD L, (HL)";
		case 0x6F:									//  LD L, A
			return "LD L, A";
		case 0x70:									//  LD (HL), B
			return "LD (HL), B";
		case 0x71:									//  LD (HL), C
			return "LD (HL), C";
		case 0x72: 									//  LD (HL), D
			return "LD (HL), D";
		case 0x73:									//  LD (HL), E
			return "LD (HL), E";
		case 0x74:									//  LD (HL), H
			return "LD (HL), H";
		case 0x75:									//  LD (HL), L
			return "LD (HL), L";
		case 0x76:									//  HALT
			return "HALT";
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
			return;
		default:
			printf("Error: formato incorrecto");
			return "ERROR";
	}
}
