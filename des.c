#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *getInstruction(int, char[], char[]);
void getByte(char [], char []);
void complete(const char *, char [], char []);
void attach(const char *, char [], char []);
char *newSwitch(int, char [], int);
int countLines(char *);
char *getLines(char *, int);

int i;  //contador
int num_total_bytes = 0; // numero total de bytes que abarca el programa
char buffer[20];  //almacenamiento temporal para retorno en getInstruction


int main(int argvc, char **argv) {
	int num_lines;  // num_lines guardará la cantidad de lineas del archivo, i es contador
	int opcode;  // guardara un byte en base 10
	char byte[3]; // almacena un byte individual
	char *mnemonico; //almacena la instruccion completa

	num_lines = countLines("test.txt");  // countLines regresa el numero de lineas CON INSTRUCCIONES (no cuenta la ultima)
	
	/*se crea un arreglo que almacenará todos los bytes que representan codigo de operacion del programa. Cada byte son 2 caracteres,
	en una linea puede haber maximo 16 bytes, por el total de lineas: se tiene el tamaño maximo que debe tener el arreglo*/
	char total_bytes[(num_lines*32)+1];  // se suma uno para tener espacio para la copia de temp a total_bytes
	char *temp = getLines("test.txt", num_lines);
	strcpy(total_bytes, temp);
	free(temp);

	for(i = 0; i < num_total_bytes*2;){
		getByte(byte, total_bytes);
		opcode = (int) strtol(byte, NULL, 16);
		mnemonico = getInstruction(opcode, total_bytes, byte);
		printf("%s\n", mnemonico);
	}
	
    return EXIT_SUCCESS;
}

void getByte(char argument[], char line[]){
	argument[0] = line[i];
	argument[1] = line[i+1];
	i += 2;
}

int countLines(char * filename){
	FILE *file = fopen(filename, "r");
	if (file == NULL){
        puts("Unable to open the file");
    }
	else{
		int lines = 0;
		while(!feof(file))
		{
		  	if(fgetc(file) == '\n')
		  	{
		    	lines++;
		  	}
		}
		fclose(file);
		return lines;
	}
	return 0;
}
char * getLines(char * filename, int num_lines){
	FILE *file = fopen(filename, "r");
	if (file == NULL){
        puts("Unable to open the file");
    }
	else{
		char line[45], byte[3];
		char *total_bytes = malloc((num_lines*32)*sizeof(char));
		int k,j, dbytes_in_line;
		int index = 0;
		for(k = 0; k < num_lines; k++){
			fgets(line, sizeof(line), file);
			byte[0] = line[1];
			byte[1] = line[2];
			dbytes_in_line = (int) strtol(byte, NULL, 16);  //Toma la cadena almacenada en byte[] y la convierte a int, leyendo en base 16
			num_total_bytes = num_total_bytes + dbytes_in_line;
            dbytes_in_line = (dbytes_in_line * 2) + 9;  //dos digitos por byte, +9 digitos que se ignoran al inicio
            for(j = 9; j < dbytes_in_line; j++, index++){
            	total_bytes[index] = line[j];
			}
		}
		fclose(file);
		return total_bytes;
	}
	return "Error";
}

void complete(const char *arg1, char arg2[], char arg3[]){  //junta primera parte de mnemonico con n o nn, inserta H al final
	char aux[20];
	strcpy(aux, arg1);
	strcat(arg3, arg2);
	strcat(aux, arg3);
	strcat(aux, "H");
	strcpy(buffer, aux);
}

void attach(const char *arg1, char arg2[], char arg3[]){  //junta primera parte de mnemonico con n o nn, no inserta H al final
	char aux[20];
	strcpy(aux, arg1);
	strcat(arg3, arg2);
	strcat(aux, arg3);
	strcpy(buffer, aux);
}

char *newSwitch(int opcode, char line[], int nextbyte){
    char argument1[20] = {0};
	char argument2[20] = {0};
    int opbyte;

	switch(opcode){

	    case 0xDD:
	        switch(nextbyte){

	            case 0xCB:
	                getByte(argument1, line);
	                getByte(argument2, line);
                    opbyte = (int) strtol(argument2, NULL, 16);
	                switch(opbyte){

	                    case 0x06:                                  //RLC  (IX+d)
                            strcpy(argument2, "");
                            attach("RLC (IX+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x0E:                                  //RRC  (IX+d)
                            strcpy(argument2, "");
                            attach("RRC (IX+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x16:                                  //RL   (IX+d)
                            strcpy(argument2, "");
                            attach("RL (IX+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x1E:                                  //RR   (IX+d)
                            strcpy(argument2, "");
                            attach("RR (IX+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x26:                                  //SLA  (IX+d)
                            strcpy(argument2, "");
                            attach("SLA (IX+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x2E:                                  //SRA  (IX+d)
                            strcpy(argument2, "");
                            attach("SRA (IX+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x3E:                                  //SRL  (IX+d)
                            strcpy(argument2, "");
                            attach("SRL (IX+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x46:                                  //BIT  0,(IX+d)
                        	strcpy(argument2, "");
                            attach("BIT 0, (IX+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x4E:                                  //BIT  1,(IX+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x56:                                  //BIT  2,(IX+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x5E:                                  //BIT  3,(IX+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x66:                                  //BIT  4,(IX+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x6E:                                  //BIT  5,(IX+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x76:                                  //BIT  6,(IX+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x7E:                                  //BIT  7,(IX+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x86:                                  //RES  0,(IX+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x8E:                                  //RES  1,(IX+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x96:                                  //RES  2,(IX+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x9E:                                  //RES  3,(IX+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xA6:                                  //RES  4,(IX+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xAE:                                  //RES  5,(IX+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xB6:                                  //RES  6,(IX+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xBE:                                  //RES  7,(IX+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xC6:                                  //SET  0,(IX+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xCE:                                  //SET  1,(IX+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xD6:                                  //SET  2,(IX+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xDE:                                  //SET  3,(IX+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xE6:                                  //SET  4,(IX+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xEE:                                  //SET  5,(IX+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xF6:                                  //SET  6,(IX+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xFE:                                  //SET  7,(IX+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IX+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
	                }

                case 0x09:                                  //ADD  IX,BC
                    return "ADD IX,BC";
                case 0x19:                                  //ADD  IX,DE
                    return "ADD IX,DE";
                case 0x21:                                  //LD   IX, nn
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD IX, ", argument2, argument1);
                    return buffer;
                case 0x22:                                  //LD  (nn),IX
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), IX");
                    return buffer;
                case 0x23:                                  //INC  IX
                    return "INC IX";
                case 0x29:                                  //ADD  IX,IX
                    return "ADD IX,IX";
                case 0x2A:                                  //LD  IX,(nn)
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD IX, (", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer;
                case 0x2B:                                  //DEC  IX
                    return "DEC IX";
                case 0x34:                                  //INC  (IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("INC (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x35:                                  //DEC  (IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("DEC (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x36:                                  //LD  (IX+d), n
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IX+", argument1, argument2);
                    strcat(buffer, "), ");
                    getByte(argument1, line);
                    strcat(buffer, argument1);
                    return buffer;
                case 0x46:                                  //LD   B,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD B, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x4E:                                  //LD   C,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD C, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x56:                                  //LD   D,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD D, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x5E:                                  //LD   E,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD E, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x66:                                  //LD   H,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD H, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x6E:                                  //LD   L,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD L, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x70:                                  //LD   (IX+d),B
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IX+", argument1, argument2);
                    strcat(buffer, "), B");
                    return buffer;
                case 0x71:                                  //LD   (IX+d),C
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IX+", argument1, argument2);
                    strcat(buffer, "), C");
                    return buffer;
                case 0x72:                                  //LD   (IX+d),D
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IX+", argument1, argument2);
                    strcat(buffer, "), D");
                    return buffer;
                case 0x73:                                  //LD   (IX+d),E
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IX+", argument1, argument2);
                    strcat(buffer, "), E");
                    return buffer;
                case 0x74:                                  //LD   (IX+d),H
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IX+", argument1, argument2);
                    strcat(buffer, "), H");
                    return buffer;
                case 0x75:                                  //LD   (IX+d),L
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IX+", argument1, argument2);
                    strcat(buffer, "), L");
                    return buffer;
                case 0x77:                                  //LD   (IX+d),A
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IX+", argument1, argument2);
                    strcat(buffer, "), A");
                    return buffer;
                case 0x7E:                                  //LD   A,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD A, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x86:                                  //ADD  A,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("ADD A, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x8E:                                  //ADC  A,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("ADC A, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x96:                                  //SUB  A,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("SUB A, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x9E:                                  //SBC  A,(IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("SBC A, (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xA6:                                  //AND  (IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("AND (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xAE:                                  //XOR  (IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("XOR (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xB6:                                  //OR   (IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("OR (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xBE:                                  //CP   (IX+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("CP (IX+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xE1:                                  //POP  IX
                    return "POP IX";
                case 0xE3:                                  //EX   (SP),IX
                    return "EX (SP),IX";
                case 0xE5:                                  //PUSH IX
                    return "PUSH IX";
                case 0xE9:                                  //JP   (IX)
                    return "JP (IX)";
	        }
        case 0xED:
            switch(nextbyte){
                case 0x00: 
                    strcpy(buffer, "MOS_QUIT");
                    return buffer;
                case 0x01:
                    strcpy(buffer, "MOS_CLI");
                    return buffer;
                case 0x02: 
                    strcpy(buffer, "MOS_BYTE");
                    return buffer;
                case 0x03: 
                    strcpy(buffer, "MOS_WORD");
                    return buffer;
                case 0x04: 
                    strcpy(buffer, "MOS_WRCH");
                    return buffer;
                case 0x05: 
                    strcpy(buffer, "MOS_RDCH");
                    return buffer;
                case 0x06: 
                    strcpy(buffer, "MOS_FILE");
                    return buffer;
                case 0x07: 
                    strcpy(buffer, "MOS_ARGS");
                    return buffer;
                case 0x08: 
                    strcpy(buffer, "MOS_BGET");
                    return buffer;
                case 0x09: 
                    strcpy(buffer, "MOS_BPUT");
                    return buffer;
                case 0x0A: 
                    strcpy(buffer, "MOS_GBPB");
                    return buffer;
                case 0x0B: 
                    strcpy(buffer, "MOS_FIND");
                    return buffer;
                case 0x0C:  
                    strcpy(buffer, "MOS_FF0C");
                    return buffer;
                case 0x0D:
                    strcpy(buffer, "MOS_FF0D");
                    return buffer;
                case 0x0E:  
                    strcpy(buffer, "MOS_FF0E");
                    return buffer;
                case 0x0F:
                    strcpy(buffer, "MOS_FF0F");
                    return buffer; 
                case 0x40: 
                    strcpy(buffer, "IN B,(C)");
                    return buffer; 
                case 0x41: 
                    strcpy(buffer, "OUT (C),B");
                    return buffer; 
                case 0x42: 
                    strcpy(buffer, "SBC HL,BC");
                    return buffer; 
                case 0x43: 
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), BC");
                    return buffer;
                case 0x44: 
                    strcpy(buffer, "NEG");
                    return buffer;
                case 0x45: 
                    strcpy(buffer, "RETN");
                    return buffer;
                case 0x46: 
                    strcpy(buffer, "IM 0");
                    return buffer;
                case 0x47: 
                    strcpy(buffer, "LD I,A");
                    return buffer;
                case 0x48: 
                    strcpy(buffer, "IN C,(C)");
                    return buffer;
                case 0x49: 
                    strcpy(buffer, "OUT (C),C");
                    return buffer;
                case 0x4A: 
                    strcpy(buffer, "ADC HL,BC");
                    return buffer;
                case 0x4B: 
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD BC,(", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer;
                case 0x4C:
                    strcpy(buffer, "[neg]");
                    return buffer; 
                case 0x4D: 
                    strcpy(buffer, "RETI");
                    return buffer;
                case 0x4E: 
                    strcpy(buffer, "[im0]");
                    return buffer;
                case 0x4F: 
                    strcpy(buffer, "LD R,A");
                    return buffer; 
                case 0x50: 
                    strcpy(buffer, "IN D,(C)");
                    return buffer;
                case 0x51: 
                    strcpy(buffer, "OUT (C),D");
                    return buffer; 
                case 0x52: 
                    strcpy(buffer, "SBC HL,DE");
                    return buffer;
                case 0x53: 
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), DE");
                    return buffer;
                case 0x54:
                    strcpy(buffer, "[neg]");
                    return buffer;
                case 0x55:
                    strcpy(buffer, "[retn]");
                    return buffer; 
                case 0x56: 
                    strcpy(buffer, "IM 1");
                    return buffer;
                case 0x57: 
                    strcpy(buffer, "LD A,I");
                    return buffer;
                case 0x58: 
                    strcpy(buffer, "IN E,(C)");
                    return buffer;
                case 0x59:
                    strcpy(buffer, "OUT (C),E");
                    return buffer; 
                case 0x5A:
                    strcpy(buffer, "ADC HL,DE");
                    return buffer;
                case 0x5B:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD DE,(", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer; 
                case 0x5C:
                    strcpy(buffer, "[neg]");
                    return buffer;
                case 0x5D:
                    strcpy(buffer, "[reti]");
                    return buffer; 
                case 0x5E:
                    strcpy(buffer, "IM 2");
                    return buffer; 
                case 0x5F:
                    strcpy(buffer, "LD A,R");
                    return buffer; 
                case 0x60: 
                    strcpy(buffer, "IN H,(C)");
                    return buffer; 
                case 0x61:
                    strcpy(buffer, "OUT (C),H");
                    return buffer; 
                case 0x62:
                    strcpy(buffer, "SBC HL,HL");
                    return buffer; 
                case 0x63:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), HL");
                    return buffer;
                case 0x64:
                    strcpy(buffer, "[neg]");
                    return buffer; 
                case 0x65:
                    strcpy(buffer, "[retn]");
                    return buffer;
                case 0x66:
                    strcpy(buffer, "[im0]");
                    return buffer;
                case 0x67:
                    strcpy(buffer, "RRD");
                    return buffer; 
                case 0x68:
                    strcpy(buffer, "IN L,(C)");
                    return buffer;
                case 0x69:
                    strcpy(buffer, "OUT (C),L");
                    return buffer;
                case 0x6A:
                    strcpy(buffer, "ADC HL,HL");
                    return buffer; 
                case 0x6B:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD HL,(", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer; 
                case 0x6C:
                    strcpy(buffer, "[neg]");
                    return buffer;
                case 0x6D:
                    strcpy(buffer, "[reti]");
                    return buffer;
                case 0x6E:
                    strcpy(buffer, "[im0]");
                    return buffer;
                case 0x6F:
                    strcpy(buffer, "RLD");
                    return buffer;
                case 0x70:
                    strcpy(buffer, "IN F,(C)");
                    return buffer;
                case 0x71:
                    strcpy(buffer, "OUT (C),F");
                    return buffer;
                case 0x72:
                    strcpy(buffer, "SBC HL,SP");
                    return buffer;
                case 0x73:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), SP");
                    return buffer;
                case 0x74:
                    strcpy(buffer, "[neg]");
                    return buffer;
                case 0x75:
                    strcpy(buffer, "[retn]");
                    return buffer;
                case 0x76:
                    strcpy(buffer, "[im1]");
                    return buffer;
                case 0x77:
                    strcpy(buffer, "[ld i,i?]");
                    return buffer;
                case 0x78:
                    strcpy(buffer, "IN A,(C)");
                    return buffer;
                case 0x79:
                    strcpy(buffer, "OUT (C),A");
                    return buffer;
                case 0x7A:
                    strcpy(buffer, "ADC HL,SP");
                    return buffer;
                case 0x7B:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD SP,(", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer; 
                case 0x7C:
                    strcpy(buffer, "[neg]");
                    return buffer;
                case 0x7D:
                    strcpy(buffer, "[reti]");
                    return buffer;
                case 0x7E:
                    strcpy(buffer, "[im2]");
                    return buffer;
                case 0x7F:
                    strcpy(buffer, "[ld r,r?]");
                    return buffer;
                case 0xA0:
                    strcpy(buffer, "LDI");
                    return buffer;
                case 0xA1:
                    strcpy(buffer, "CPI");
                    return buffer;
                case 0xA2:
                    strcpy(buffer, "INI");
                    return buffer;
                case 0xA3:
                    strcpy(buffer, "OTI");
                    return buffer;
                case 0xA8:
                    strcpy(buffer, "LDD");
                    return buffer;
                case 0xA9:
                    strcpy(buffer, "CPD");
                    return buffer;
                case 0xAA:
                    strcpy(buffer, "IND");
                    return buffer;
                case 0xAB:
                    strcpy(buffer, "OTD");
                    return buffer;
                case 0xB0:
                    strcpy(buffer, "LDIR");
                    return buffer;
                case 0xB1:
                    strcpy(buffer, "CPIR");
                    return buffer;
                case 0xB2:
                    strcpy(buffer, "INIR");
                    return buffer;
                case 0xB3:
                    strcpy(buffer, "OTIR");
                    return buffer;
                case 0xB8:
                    strcpy(buffer, "LDDR");
                    return buffer;
                case 0xB9:
                    strcpy(buffer, "CPDR");
                    return buffer;
                case 0xBA:
                    strcpy(buffer, "INDR");
                    return buffer;
                case 0xBB:
                    strcpy(buffer, "OTDR");
                    return buffer;
                case 0xF8:
                    strcpy(buffer, "[z80]");
                    return buffer;
                case 0xF9:
                    strcpy(buffer, "[z80]");
                    return buffer;
                case 0xFA:
                    strcpy(buffer, "[z80]");
                    return buffer;
                case 0xFB:
                    strcpy(buffer, "ED_LOAD");
                    return buffer;
                case 0xFC:
                    strcpy(buffer, "[z80]");
                    return buffer;
                case 0xFD:
                    strcpy(buffer, "[z80]");
                    return buffer;
                case 0xFE:
                    strcpy(buffer, "[z80]");
                    return buffer;
                case 0xFF:
                    strcpy(buffer, "ED_DOS");
                    return buffer;
        }
        case 0xFD:
            switch(nextbyte){

	            case 0xCB:
	                getByte(argument1, line);
	                getByte(argument2, line);
                    opbyte = (int) strtol(argument2, NULL, 16);
	                switch(opbyte){

	                    case 0x06:                                  //RLC  (IY+d)
                            strcpy(argument2, "");
                            attach("RLC (IY+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x0E:                                  //RRC  (IY+d)
                            strcpy(argument2, "");
                            attach("RRC (IY+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x16:                                  //RL   (IY+d)
                            strcpy(argument2, "");
                            attach("RL (IY+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x1E:                                  //RR   (IY+d)
                            strcpy(argument2, "");
                            attach("RR (IY+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x26:                                  //SLA  (IY+d)
                            strcpy(argument2, "");
                            attach("SLA (IY+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x2E:                                  //SRA  (IY+d)
                            strcpy(argument2, "");
                            attach("SRA (IY+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x3E:                                  //SRL  (IY+d)
                            strcpy(argument2, "");
                            attach("SRL (IY+", argument1, argument2);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x46:                                  //BIT  0,(IY+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x4E:                                  //BIT  1,(IY+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x56:                                  //BIT  2,(IY+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x5E:                                  //BIT  3,(IY+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x66:                                  //BIT  4,(IY+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x6E:                                  //BIT  5,(IY+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x76:                                  //BIT  6,(IY+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x7E:                                  //BIT  7,(IY+d)
                            attach("BIT ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x86:                                  //RES  0,(IY+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x8E:                                  //RES  1,(IY+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x96:                                  //RES  2,(IY+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0x9E:                                  //RES  3,(IY+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xA6:                                  //RES  4,(IY+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xAE:                                  //RES  5,(IY+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xB6:                                  //RES  6,(IY+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xBE:                                  //RES  7,(IY+d)
                            attach("RES ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xC6:                                  //SET  0,(IY+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xCE:                                  //SET  1,(IY+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xD6:                                  //SET  2,(IY+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xDE:                                  //SET  3,(IY+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xE6:                                  //SET  4,(IY+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xEE:                                  //SET  5,(IY+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xF6:                                  //SET  6,(IY+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
                        case 0xFE:                                  //SET  7,(IY+d)
                            attach("SET ", argument2, "");
                            strcat(buffer, ", (IY+");
                            strcat(buffer, argument1);
                            strcat(buffer, ")");
                            return buffer;
	                }

                case 0x09:                                  //ADD  IY,BC
                    return "ADD IY,BC";
                case 0x19:                                  //ADD  IY,DE
                    return "ADD IY,DE";
                case 0x21:                                  //LD   IY, nn
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD IY, ", argument2, argument1);
                    return buffer;
                case 0x22:                                  //LD  (nn),IY
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), IY");
                    return buffer;
                case 0x23:                                  //INC  IY
                    return "INC IY";
                case 0x29:                                  //ADD  IY,IY
                    return "ADD IY,IY";
                case 0x2A:                                  //LD  IY,(nn)
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD IY, (", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer;
                case 0x2B:                                  //DEC  IY
                    return "DEC IY";
                case 0x34:                                  //INC  (IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("INC (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x35:                                  //DEC  (IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("DEC (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x36:                                  //LD  (IY+d), n
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IY+", argument1, argument2);
                    strcat(buffer, "), ");
                    getByte(argument1, line);
                    strcat(buffer, argument1);
                    return buffer;
                case 0x46:                                  //LD   B,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD B, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x4E:                                  //LD   C,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD C, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x56:                                  //LD   D,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD D, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x5E:                                  //LD   E,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD E, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x66:                                  //LD   H,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD H, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x6E:                                  //LD   L,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD L, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x70:                                  //LD   (IY+d),B
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IY+", argument1, argument2);
                    strcat(buffer, "), B");
                    return buffer;
                case 0x71:                                  //LD   (IY+d),C
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IY+", argument1, argument2);
                    strcat(buffer, "), C");
                    return buffer;
                case 0x72:                                  //LD   (IY+d),D
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IY+", argument1, argument2);
                    strcat(buffer, "), D");
                    return buffer;
                case 0x73:                                  //LD   (IY+d),E
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IY+", argument1, argument2);
                    strcat(buffer, "), E");
                    return buffer;
                case 0x74:                                  //LD   (IY+d),H
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IY+", argument1, argument2);
                    strcat(buffer, "), H");
                    return buffer;
                case 0x75:                                  //LD   (IY+d),L
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IY+", argument1, argument2);
                    strcat(buffer, "), L");
                    return buffer;
                case 0x77:                                  //LD   (IY+d),A
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD (IY+", argument1, argument2);
                    strcat(buffer, "), A");
                    return buffer;
                case 0x7E:                                  //LD   A,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("LD A, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x86:                                  //ADD  A,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("ADD A, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x8E:                                  //ADC  A,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("ADC A, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x96:                                  //SUB  A,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("SUB A, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0x9E:                                  //SBC  A,(IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("SBC A, (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xA6:                                  //AND  (IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("AND (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xAE:                                  //XOR  (IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("XOR (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xB6:                                  //OR   (IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("OR (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xBE:                                  //CP   (IY+d)
                    getByte(argument1, line);
                    strcpy(argument2, "");
                    attach("CP (IY+", argument1, argument2);
                    strcat(buffer, ")");
                    return buffer;
                case 0xE1:                                  //POP  IY
                    return "POP IY";
                case 0xE3:                                  //EX   (SP),IY
                    return "EX (SP),IY";
                case 0xE5:                                  //PUSH IY
                    return "PUSH IY";
                case 0xE9:                                  //JP   (IY)
                    return "JP (IY)";
	        }
	}
    return "";
}

char * getInstruction(int opcode, char line[], char byte[]) {
	char argument1[20] = {0};  //byte mas significativo
	char argument2[20] = {0};  //byte menos significativo
	int nextbyte;

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
			return "LD (HL), A";
		case 0x78:									//  LD A, B
			return "LD A, B";
		case 0x79:									//  LD A, C
			return "LD A, C";
		case 0x7A:									//  LD A, D
			return "LD A, D";
		case 0x7B:									//  LD A, E
			return "LD A, E";
		case 0x7C:									//  LD A, H
			return "LD A, H";
		case 0x7D:									//  LD A, L
			return "LD A, L";
		case 0x7E:									//  LD A, (HL)
			return "LD A, (HL)";
		case 0x7F:									//  LD A, A
			return "LD A, A";
		case 0x80:									//  ADD A, B
			return "ADD A, B";
		case 0x81:									//  ADD A, C
			return "ADD A, C";
		case 0x82:									//  ADD A, D
			return "ADD A, D";
		case 0x83:									//  ADD A, E
			return "ADD A, E";
		case 0x84:									//  ADD A, H
			return "ADD A, H";
		case 0x85:									//  ADD A, L
			return "ADD A, L";
		case 0x86:									//  ADD A, (HL)
			return "ADD A, (HL)";
		case 0x87:									//  ADD A, A
			return "ADD A, A";
		case 0x88:									//  ADC A, B
			return "ADC A, B";
		case 0x89:									//  ADC A, C
			return "ADC A, C";
		case 0x8A:									//  ADC A, D
			return "ADC A, D";
		case 0x8B:									//  ADC A, E
			return "ADC A, E";
		case 0x8C:									//  ADC A, H
			return "ADC A, H";
		case 0x8D:									//  ADC A, L
			return "ADC A, L";
		case 0x8E:									//  ADC A, (HL)
			return "ADC A, (HL)";
		case 0x8F:									//  ADC A, A
			return "ADC A, A";
		case 0x90:									//  SUB A, B
			return "SUB A, B";
		case 0x91:									//  SUB A, C
			return "SUB A, C";
		case 0x92: 									//  SUB A, D
			return "SUB A, D";
		case 0x93:									//  SUB A, E
			return "SUB A, E";
		case 0x94:									//  SUB A, H
			return "SUB A, H";
		case 0x95:									//  SUB A, L
			return "SUB A, L";
		case 0x96:									//  SUB A, (HL)
			return "SUB A, (HL)";
		case 0x97:									//  SUB A, A
			return "SUB A, A";
		case 0x98:									//  SBC A, B
			return "SBC A, B";
		case 0x99:									//  SBC A, C
			return "SBC A, C";
		case 0x9A:									//  SBC A, D
			return "SBC A, D";
		case 0x9B:									//  SBC A, E
			return "SBC A, E";
		case 0x9C:									//  SBC A, H
			return "SBC A, H";
		case 0x9D:									//  SBC A, L
			return "SBC A, L";
		case 0x9E:									//  SBC A, (HL)
			return "SBC A, (HL)";
		case 0x9F:									//  SBC A, A
			return "SBC A, A";
		case 0xA0:									//  AND B
			return "AND B";
		case 0xA1:									//  AND C
			return "AND C";
		case 0xA2: 									//  AND D
			return "AND D";
		case 0xA3:									//  AND E
			return "AND E";
		case 0xA4:									//  AND H
			return "AND H";
		case 0xA5:									//  AND L
			return "AND L";
		case 0xA6:									//  AND (HL)
			return "AND (HL)";
		case 0xA7:									//  AND A
			return "AND A";
		case 0xA8:									//  XOR B
			return "XOR B";
		case 0xA9:									//  XOR C
			return "XOR C";
		case 0xAA:									//  XOR D
			return "XOR D";
		case 0xAB:									//  XOR E
			return "XOR E";
		case 0xAC:									//  XOR H
			return "XOR H";
		case 0xAD:									//  XOR L
			return "XOR L";
		case 0xAE:									//  XOR (HL)
			return "XOR (HL)";
		case 0xAF:									//  XOR A
			return "XOR A";
		case 0xB0:									//  OR B
			return "OR B";
		case 0xB1:									//  OR C
			return "OR C";
		case 0xB2: 									//  OR D
			return "OR D";
		case 0xB3:									//  OR E
			return "OR E";
		case 0xB4:									//  OR H
			return "OR H";
		case 0xB5:									//  OR L
			return "OR L";
		case 0xB6:									//  OR (HL)
			return "OR (HL)";
		case 0xB7:									//  OR A
			return "OR A";
		case 0xB8:									//  CP B
			return "CP B";
		case 0xB9:									//  CP C
			return "CP C";
		case 0xBA:									//  CP D
			return "CP D";
		case 0xBB:									//  CP E
			return "CP E";
		case 0xBC:									//  CP H
			return "CP H";
		case 0xBD:									//  CP L
			return "CP L";
		case 0xBE:									//  CP (HL)
			return "CP (HL)";
		case 0xBF:									//  CP A
			return "CP A";
		case 0xC0:									//  RET NZ
			return "RET NZ";
		case 0xC1:									//  POP BC
			return "POP BC";
		case 0xC2: 									//  JP NZ, e
			// funcion getEti()
			return "Incompleto";
		case 0xC3:									//  JP e
			// funcion getEti()
			return "Incompleto";
		case 0xC4:									//  CALL NZ, e
			//funcion getEti()
			return "Incompleto";
		case 0xC5:									//  PUSH BC
			return "PUSH BC";
		case 0xC6:									//  ADD A, n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("ADD A, ", argument1, argument2);
			return buffer;
		case 0xC7:									//  RST 00H
			return "RST 00H";
		case 0xC8:									//  RET Z
			return "RET Z";
		case 0xC9:									//  RET
			return "RET";
		case 0xCA:									//  JP Z, e
			// funcion getEti()
			return "Incompleto";
		case 0xCB:									//  ** CB **
			getByte(byte, line);
			opcode = (int) strtol(byte, NULL, 16);
			// switch para CB
			switch(opcode){

				case 0x00:									//	RLC B
					return "RLC B";
				case 0x01:									//	RLC C
					return "RLC C";
				case 0x02: 									//	RLC D
					return "RLC D";
				case 0x03:									//	RLC E
					return "RLC E";
				case 0x04:									//	RLC H
					return "RLC H";
				case 0x05:									//	RLC H
					return "RLC L";
				case 0x06:									//	RLC (HL)
					return "RLC (HL)";
				case 0x07:									//	RLC A
					return "RLC A";
				case 0x08:          						//	RRC B
					return "RRC B";
				case 0x09:									//	RRC C
					return "RRC C";
				case 0x0A:									//	RRC D
					return "RRC D";
				case 0x0B:									//  RRC E
					return "RRC E";
				case 0x0C:									//	RRC H
					return "RRC H";
				case 0x0D:									//	RRC L
					return "RRC L";
				case 0x0E:									//	RRC (HL)
					return "RRC (HL)";
				case 0x0F:									//	RRC A
					return "RRC A";
				case 0x10:									//	RL B
					return "RL B";
				case 0x11:									//	RL C
					return "RL C";
				case 0x12:									//	RL D
					return "RL D";
				case 0x13:									//	RL E
					return "RL E";
				case 0x14:									//	RL H
					return "RL H";
				case 0x15:									//	RL L
					return "RL L";
				case 0x16:									//	RL (HL)
					return "RL (HL)";
				case 0x17:									//	RL A
					return "RL A";
				case 0x18:									//  RR B
					return "RR B";
				case 0x19:									//  RR C
					return "RR C";
				case 0x1A:									//	RR D
					return "RR D";
				case 0x1B:									//	RR E
					return "RR E";
				case 0x1C:									// 	RR H
					return "RR H";
				case 0x1D:									//	RR L
					return "RR L";
				case 0x1E:									//	RR (HL)
					return "RR (HL)";
				case 0x1F:									//	RR A
					return "RR A";
				case 0x20:									//  SLA B
					return "SLA B";
				case 0x21:									//  SLA C
					return "SLA C";
				case 0x22:									//  SLA D
					return "SLA D";
				case 0x23:									//  SLA E
					return "SLA E";
				case 0x24:									//  SLA H
					return "SLA H";
				case 0x25:									//  SLA L
					return "SLA L";
				case 0x26:									//  SLA (HL)
					return "SLA (HL)";
				case 0x27:									//  SLA A
					return "SLA A";
				case 0x28:									//  SRA B
					return "SRA B";
				case 0x29:									//  SRA C
					return "SRA C";
				case 0x2A:									//  SRA D
					return "SRA D";
				case 0x2B:									//  SRA E
					return "SRA E";
				case 0x2C:									//  SRA L
					return "SRA H";
				case 0x2D:									//  SRA L
					return "SRA L";
				case 0x2E:									//  SRA (HL)
					return "SRA (HL)";
				case 0x2F:									//  SRA
					return "SRA A";
				case 0x30:									//  SLS B
					return "SLS B";
				case 0x31:									//  SLS C
					return "SLS C";
				case 0x32: 									//  SLS D
					return "SLS D";
				case 0x33:									//  SLS E
					return "SLS E";
				case 0x34:									//  SLS H
					return "SLS H";
				case 0x35:									//  SLS L
					return "SLS L";
				case 0x36:									//  SLS (HL)
					return "SLS (HL)";
				case 0x37:									//  SLS A
					return "SLS A";
				case 0x38:									//  SRL B
					return "SRL B";
				case 0x39:									//	SRL C
					return "SRL C";
				case 0x3A:									//  SRL D
					return "SRL D";
				case 0x3B:									//  SRL E
					return "SRL E";
				case 0x3C:									//  SRL H
					return "SRL H";
				case 0x3D:									//  SRL L
					return "SRL L";
				case 0x3E:									//  SRL (HL)
					return "SRL (HL)";
				case 0x3F:									//  SRL A
					return "SRL A";
				case 0x40:									//  BIT 0, B
					return "BIT 0, B";
				case 0x41:									//  BIT 0, C
					return "BIT 0, C";
				case 0x42: 									//  BIT 0, D
					return "BIT 0, D";
				case 0x43:									//  BIT 0, E
					return "BIT 0, E";
				case 0x44:									//  BIT 0, H
					return "BIT 0, H";
				case 0x45:									//  BIT 0, L
					return "BIT 0, L";
				case 0x46:									//  BIT 0, (HL)
					return "BIT 0, (HL)";
				case 0x47:									//  BIT 0, A
					return "BIT 0, A";
				case 0x48:									//  BIT 1, B
					return "BIT 1, B";
				case 0x49:									//  BIT 1, C
					return "BIT 1, C";
				case 0x4A:									//  BIT 1, D
					return "BIT 1, D";
				case 0x4B:									//  BIT 1, E
					return "BIT 1, E";
				case 0x4C:									//  BIT 1, H
					return "BIT 1, H";
				case 0x4D:									//  BIT 1, L
					return "BIT 1, L";
				case 0x4E:									//  BIT 1, (HL)
					return "BIT 1, (HL)";
				case 0x4F:									//  BIT 1, A
					return "BIT 1, A";
				case 0x50:									//  BIT 2, B
					return "BIT 2, B";
				case 0x51:									//  BIT 2, C
					return "BIT 2, C";
				case 0x52: 									//  BIT 2, D
					return "BIT 2, D";
				case 0x53:									//  BIT 2, E
					return "BIT 2, E";
				case 0x54:									//  BIT 2, H
					return "BIT 2, H";
				case 0x55:									//  BIT 2, L
					return "BIT 2, L";
				case 0x56:									//  BIT 2, (HL)
					return "BIT 2, (HL)";
				case 0x57:									//  BIT 2, A
					return "BIT 2, A";
				case 0x58:									//  BIT 3, B
					return "BIT 3, B";
				case 0x59:									//  BIT 3, C
					return "BIT 3, C";
				case 0x5A:									//  BIT 3, D
					return "BIT 3, D";
				case 0x5B:									//  BIT 3, E
					return "BIT 3, E";
				case 0x5C:									//  BIT 3, H
					return "BIT 3, H";
				case 0x5D:									//  BIT 3, L
					return "BIT 3, L";
				case 0x5E:									//  BIT 3, (HL)
					return "BIT 3, (HL)";
				case 0x5F:									//  BIT 3, A
					return "BIT 3, A";
				case 0x60:									//  BIT 4, B
					return "BIT 4, B";
				case 0x61:									//  BIT 4, C
					return "BIT 4, C";
				case 0x62: 									//  BIT 4, D
					return "BIT 4, D";
				case 0x63:									//  BIT 4, E
					return "BIT 4, E";
				case 0x64:									//  BIT 4, H
					return "BIT 4, H";
				case 0x65:									//  BIT 4, L
					return "BIT 4, L";
				case 0x66:									//  BIT 4, (HL)
					return "BIT 4, (HL)";
				case 0x67:									//  BIT 4, A
					return "BIT 4, A";
				case 0x68:									//  BIT 5, B
					return "BIT 5, B";
				case 0x69:									//  BIT 5, C
					return "BIT 5, C";
				case 0x6A:									//  BIT 5, D
					return "BIT 5, D";
				case 0x6B:									//  BIT 5, E
					return "BIT 5, E";
				case 0x6C:									//  BIT 5, H
					return "BIT 5, H";
				case 0x6D:									//  BIT 5, L
					return "BIT 5, L";
				case 0x6E:									//  BIT 5, (HL)
					return "BIT 5, (HL)";
				case 0x6F:									//  BIT 5, A
					return "BIT 5, A";
				case 0x70:									//  BIT 6, B
					return "BIT 6, B";
				case 0x71:									//  BIT 6, C
					return "BIT 6, C";
				case 0x72: 									//  BIT 6, D
					return "BIT 6, D";
				case 0x73:									//  BIT 6, E
					return "BIT 6, E";
				case 0x74:									//  BIT 6, H
					return "BIT 6, H";
				case 0x75:									//  BIT 6, L
					return "BIT 6, L";
				case 0x76:									//  BIT 6, (HL)
					return "BIT 6 (HL)";
				case 0x77:									//  BIT 6, A
					return "BIT 6, A";
				case 0x78:									//  BIT 7, B
					return "BIT 7, B";
				case 0x79:									//  BIT 7, C
					return "BIT 7, C";
				case 0x7A:									//  BIT 7, D
					return "BIT 7, D";
				case 0x7B:									//  BIT 7, E
					return "BIT 7, E";
				case 0x7C:									//  BIT 7, H
					return "BIT 7, H";
				case 0x7D:									//  BIT 7, L
					return "BIT 7, L";
				case 0x7E:									//  BIT 7, (HL)
					return "BIT 7, (HL)";
				case 0x7F:									//  BIT 7, A
					return "BIT 7, A";
				case 0x80:									//  RES 0, B
					return "RES 0, B";
				case 0x81:									//  RES 0, C
					return "RES 0, C";
				case 0x82:									//  RES 0, D
					return "RES 0, D";
				case 0x83:									//  RES 0, E
					return "RES 0, E";
				case 0x84:									//  RES 0, H
					return "RES 0, H";
				case 0x85:									//  RES0 , L
					return "RES 0, L";
				case 0x86:									//  RES 0, (HL)
					return "RES 0, (HL)";
				case 0x87:									//  RES 0, A
					return "RES 0, A";
				case 0x88:									//  RES 1, B
					return "RES 1, B";
				case 0x89:									//  RES 1, C
					return "RES 1, C";
				case 0x8A:									//  RES 1, D
					return "RES 1, D";
				case 0x8B:									//  RES 1, E
					return "RES 1, E";
				case 0x8C:									//  RES 1, H
					return "RES 1, H";
				case 0x8D:									//  RES 1, L
					return "RES 1, L";
				case 0x8E:									//  RES 1, (HL)
					return "RES 1, (HL)";
				case 0x8F:									//  RES 1, A
					return "RES 1, A";
				case 0x90:									//  RES 2, B
					return "RES 2, B";
				case 0x91:									//  RES 2, C
					return "RES 2, C";
				case 0x92: 									//  RES 2, D
					return "RES 2, D";
				case 0x93:									//  RES 2, E
					return "RES 2, E";
				case 0x94:									//  RES 2, H
					return "RES 2, H";
				case 0x95:									//  RES 2, L
					return "RES 2, L";
				case 0x96:									//  RES 2, (HL)
					return "RES 2, (HL)";
				case 0x97:									//  RES 2, A
					return "RES 2, A";
				case 0x98:									//  RES 3, B
					return "RES 3, B";
				case 0x99:									//  RES 3, C
					return "RES 3, C";
				case 0x9A:									//  RES 3, D
					return "RES 3, D";
				case 0x9B:									//  RES 3, E
					return "RES 3, E";
				case 0x9C:									//  RES 3, H
					return "RES 3, H";
				case 0x9D:									//  RES 3, L
					return "RES 3, L";
				case 0x9E:									//  RES 3, (HL)
					return "RES 3, (HL)";
				case 0x9F:									//  RES 3, A
					return "RES 3, A";
				case 0xA0:									//  RES 4, B
					return "RES 4, B";
				case 0xA1:									//  RES 4, C
					return "RES 4, C";
				case 0xA2: 									//  RES 4, D
					return "RES 4, D";
				case 0xA3:									//  RES 4, E
					return "RES 4, E";
				case 0xA4:									//  RES 4, H
					return "RES 4, H";
				case 0xA5:									//  RES 4, L
					return "RES 4, L";
				case 0xA6:									//  RES 4, (HL)
					return "RES 4, (HL)";
				case 0xA7:									//  RES 4, A
					return "RES 4, A";
				case 0xA8:									//  RES 5, B
					return "RES 5, B";
				case 0xA9:									//  RES 5, C
					return "RES 5, C";
				case 0xAA:									//  RES 5, D
					return "RES 5, D";
				case 0xAB:									//  RES 5, E
					return "RES 5, E";
				case 0xAC:									//  RES 5, H
					return "RES 5, H";
				case 0xAD:									//  RES 5, L
					return "RES 5, L";
				case 0xAE:									//  RES 5, (HL)
					return "RES 5, (HL)";
				case 0xAF:									//  RES 5, A
					return "RES 5, A";
				case 0xB0:									//  RES 6, B
					return "RES 6, B";
				case 0xB1:									//  RES 6, C
					return "RES 6, C";
				case 0xB2: 									//  RES 6, D
					return "RES 6, D";
				case 0xB3:									//  RES 6, E
					return "RES 6, E";
				case 0xB4:									//  RES 6, H
					return "RES 6, H";
				case 0xB5:									//  RES 6, L
					return "RES 6, L";
				case 0xB6:									//  RES 6, (HL)
					return "RES 6, (HL)";
				case 0xB7:									//  RES 6, A
					return "RES 6, A";
				case 0xB8:									//  RES 7, B
					return "RES 7, B";
				case 0xB9:									//  RES 7, C
					return "RES 7, C";
				case 0xBA:									//  RES 7, D
					return "RES 7, D";
				case 0xBB:									//  RES 7, E
					return "RES 7, E";
				case 0xBC:									//  RES 7, H
					return "RES 7, H";
				case 0xBD:									//  RES 7, L
					return "RES 7, L";
				case 0xBE:									//  RES 7, (HL)
					return "RES 7, (HL)";
				case 0xBF:									//  RES 7, A
					return "RES 7, A";
				case 0xC0:									//  SET 0, B
					return "SET 0, B";
				case 0xC1:									//  SET 0, C
					return "SET 0, C";
				case 0xC2: 									//  SET 0, D
					return "SET 0, D";
				case 0xC3:									//  SET 0, E
					return "SET 0, E";
				case 0xC4:									//  SET 0, H
					return "SET 0, H";
				case 0xC5:									//  SET 0, L
					return "SET 0, L";
				case 0xC6:									//  SET 0, (HL)
					return "SET 0, (HL)";
				case 0xC7:									//  SET 0, A
					return "SET 0, A";
				case 0xC8:									//  SET 1, B
					return "SET 1, B";
				case 0xC9:									//  SET 1, C
					return "SET 1, C";
				case 0xCA:									//  SET 1, D
					return "SET 1, D";
				case 0xCB:									//  SET 1, E
					return "SET 1, E";
				case 0xCC:									//  SET 1, H
					return "SET 1, H";
				case 0xCD:									//  SET 1, L
					return "SET 1, L";
				case 0xCE:									//  SET 1, (HL)
					return "SET 1, (HL)";
				case 0xCF:									//  SET 1, A
					return "SET 1, A";
				case 0xD0:									//  SET 2, B
					return "SET 2, B";
				case 0xD1:									//  SET 2, C
					return "SET 2, C";
				case 0xD2: 									//  SET 2, D
					return "SET 2, D";
				case 0xD3:									//  SET 2, E
					return "SET 2, E";
				case 0xD4:									//  SET 2, H
					return "SET 2, H";
				case 0xD5:									//  SET 2, L
					return "SET 2, L";
				case 0xD6:									//  SET 2, (HL)
					return "SET 2, (HL)";
				case 0xD7:									//  SET 2, A
					return "SET 2, A";
				case 0xD8:									//  SET 3, B
					return "SET 3, B";
				case 0xD9:									//  SET 3, C
					return "SET 3, C";
				case 0xDA:									//  SET 3, D
					return "SET 3, D";
				case 0xDB:									//  SET 3, E
					return "SET 3, E";
				case 0xDC:									//  SET 3, H
					return "SET 3, H";
				case 0xDD:									// 	SET 3, L
					return "SET 3, L";
				case 0xDE:									//  SET 3, (HL)
					return "SET 3, (HL)";
				case 0xDF:									//  SET 3, A
					return "SET 3, A";
				case 0xE0:									//  SET 4, B
					return "SET 4, B";
				case 0xE1:									//  SET 4, C
					return "SET 4, C";
				case 0xE2: 									//  SET 4, D
					return "SET 4, D";
				case 0xE3:									//  SET 4, E
					return "SET 4, E";
				case 0xE4:									//  SET 4, H
					return "SET 4, H";
				case 0xE5:									//  SET 4, L
					return "SET 4, L";
				case 0xE6:									//  SET 4, (HL)
					return "SET 4, (HL)";
				case 0xE7:									//  SET 4, A
					return "SET 4, A";
				case 0xE8:									//  SET 5, B
					return "SET 5, B";
				case 0xE9:									//  SET 5, C
					return "SET 5, C";
				case 0xEA:									//  SET 5, D
					return "SET 5, D";
				case 0xEB:									//  SET 5, E
					return "SET 5, E";
				case 0xEC:									//  SET 5, H
					return "SET 5, H";
				case 0xED:									//  SET 5, L
					return "SET 5, L";
				case 0xEE:									//  SET 5, (HL)
					return "SET 5, (HL)";
				case 0xEF:									//  SET 5, A
					return "SET 5, A";
				case 0xF0:									//  SET 6, B
					return "SET 6, B";
				case 0xF1:									//  SET 6, C
					return "SET 6, C";
				case 0xF2: 									//  SET 6, D
					return "SET 6, D";
				case 0xF3:									//  SET 6, E
					return "SET 6, E";
				case 0xF4:									//  SET 6, H
					return "SET 6, H";
				case 0xF5:									//  SET 6, L
					return "SET 6, L";
				case 0xF6:									//  SET 6, (HL)
					return "SET 6, (HL)";
				case 0xF7:									//  SET 6, A
					return "SET 6, A";
				case 0xF8:									//  SET 7, B
					return "SET 7, B";
				case 0xF9:									//  SET 7, C
					return "SET 7, C";
				case 0xFA:									//  SET 7, D
					return "SET 7, D";
				case 0xFB:									// 	SET 7, E
					return "SET 7, E";
				case 0xFC:									//  SET 7, H
					return "SER 7, H";
				case 0xFD:									//  SET 7, L
					return "SET 7, L";
				case 0xFE:									//  SET 7, (HL)
					return "SET 7, (HL)";
				case 0xFF:									//  SET 7, A
					return "SET 7, A";
				default:
					printf("Error: formato incorrecto");
					return "ERROR";
			}
			return "Incompleto";
		case 0xCC:									//  CALL Z, e
			// funcion getEti()
			return "Incompleto";
		case 0xCD:									//  CALL e
			// funcion getEti()
			return "Incompleto";
		case 0xCE:									//  ADC A, n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("ADC A, ", argument1, argument2);
			return buffer;
		case 0xCF:									//  RST 08H
			return "RST 08H";
		case 0xD0:									//  RET NC
			return "RET NC";
		case 0xD1:									//  POP DE
			return "POP DE";
		case 0xD2: 									//  JP NC, e
			// funcion getEti()
			return "Incompleto";
		case 0xD3:									//  OUT (n), A
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("OUT (", argument1, argument2);
			strcat(buffer, "), A");
			return buffer;
		case 0xD4:									//  CALL NC, e
			// funcion getEti()
			return "Incompleto";
		case 0xD5:									//  PUSH DE
			return "PUSH DE";
		case 0xD6:									//  SUB A, n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("SUB A, ", argument1, argument2);
			return buffer;
		case 0xD7:									//  RST 10H
			return "RST 10H";
		case 0xD8:									//  RET C
			return "RET C";
		case 0xD9:									//  EXX
			return "EXX";
		case 0xDA:									//  JP C, e
			// funcion getEti()
			return "Incompleto";
		case 0xDB:									//  IN A, (n)
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("IN A, (", argument1, argument2);
			strcat(buffer, ")");
			return buffer;
		case 0xDC:									//  CALL C, e
			// funcion getEti()
			return "Incompleto";
		case 0xDD:									//  ** DD **
			getByte(argument1, line);
		    nextbyte = (int) strtol(argument1, NULL, 16);
			return newSwitch(opcode, line, nextbyte);
		case 0xDE:									//  SBC A, n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("SBC A, ", argument1, argument2);
			return buffer;
		case 0xDF:									//  RST 18H
			return "RST 18H";
		case 0xE0:									//  RET PO
			return "RET PO";
		case 0xE1:									//  POP HL
			return "POP HL";
		case 0xE2: 									//  JP PO, e
			// funcion getEti()
			return "Incompleto";
		case 0xE3:									//  EX (SP), HL
			return "EX (SP), HL";
		case 0xE4:									//  CALL PO, e
			// funcion getEti()
			return "Incompleto";
		case 0xE5:									//  PUSH HL
			return "PUSH HL";
		case 0xE6:									//  AND n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("AND ", argument1, argument2);
			return buffer;
		case 0xE7:									//  RST 20H
			return "RST 20H";
		case 0xE8:									//  RET PE
			return "RET PE";
		case 0xE9:									//  JP (HL)
			return "JP (HL)";
		case 0xEA:									//  JP PE, e
			// funcion getEti()
			return "Incompleto";
		case 0xEB:									//  EX DE, HL
			return "EX DE, HL";
		case 0xEC:									//  CALL PE, e
			// funcion getEti()
			return "Incompleto";
		case 0xED:									//  ** ED **
			// switch para ED
			return "Incompleto";
		case 0xEE:									//  XOR n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("XOR ", argument1, argument2);
			return buffer;
		case 0xEF:									//  RST 28H
			return "RST 28H";
		case 0xF0:									//  RET P
			return "RET P";
		case 0xF1:									//  POP AF
			return "POP AF";
		case 0xF2: 									//  JP P, e
			// funcion getEti()
			return "Incompleto";
		case 0xF3:									//  DI
			return "DI";
		case 0xF4:									//  CALL P, e
			// funcion getEti()
			return "Incompleto";
		case 0xF5:									//  PUSH AF
			return "PUSH AF";
		case 0xF6:									//  OR n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("OR ", argument1, argument2);
			return buffer;
		case 0xF7:									//  RST 30H
			return "RST 30H";
		case 0xF8:									//  RET M
			return "RET M";
		case 0xF9:									//  LD SP, HL
			return "LD SP, HL";
		case 0xFA:									//  JP M, e
			// funcion getEti()
			return "Incompleto";
		case 0xFB:									//  EI
			return "EI";
		case 0xFC:									//  CALL M, e
			// funcion getEti()
			return "Incompleto";
		case 0xFD:									//  ** FD **
			getByte(argument1, line);
		    nextbyte = (int) strtol(argument1, NULL, 16);
			return newSwitch(opcode, line, nextbyte);
		case 0xFE:									//  CP n
			getByte(argument1, line);
			strcpy(argument2, "");
			complete("CP ", argument1, argument2);
			return buffer;
		case 0xFF:									//  RST 38H
			return "RST 38H";
		default:
			printf("Error: formato incorrecto");
			return "ERROR";
	}
	
}
