#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

char *getInstruction(int, char[], char[]);
void getByte(char [], char []);
void complete(const char *, char [], char []);
void completeNewSwitch(const char *, char[], char [], char []);
char *newSwitch(char [], char []);
int countLines(char *);
char *getLines(char *, int);
char *getEti_p(uint16_t);

int i;  					//contador
int num_total_bytes = 0; 	//numero total de bytes que abarca el programa
char buffer[20];  			//almacenamiento temporal para retorno en getInstruction
uint16_t symbols[100];		//Tabla de simbolos
uint16_t cl;				//Contador de localidades asociado a las etiquetas
char eti[6];				//Nombres de las etiquetas



int main(int argvc, char **argv) {
	int num_lines;  // num_lines guardar� la cantidad de lineas del archivo, i es contador
	int opcode;  // guardara un byte en base 10
	char byte[3]; // almacena un byte individual
	char *mnemonico; //almacena la instruccion completa
	int aux;		//auxiliar para calcular contador de localidades
    uint16_t CL_n = 0x0000;
    uint16_t CL_p = 0x0000;

	num_lines = countLines("test.txt");  // countLines regresa el numero de lineas CON INSTRUCCIONES (no cuenta la ultima)
	
	/*se crea un arreglo que almacenara todos los bytes que representan codigo de operacion del programa. Cada byte son 2 caracteres,
	en una linea puede haber maximo 16 bytes, por el total de lineas: se tiene el tamano maximo que debe tener el arreglo*/
	char total_bytes[(num_lines*32)+1];  // se suma uno para tener espacio para la copia de temp a total_bytes
	char *temp = getLines("test.txt", num_lines);
	strcpy(total_bytes, temp);
	free(temp); // se libera memoria asignada dentro de la funcion getLines()

	for(i = 0; i < 100; i++){			//Se inicializa tabla de simbolos
		symbols[i] = 0x0000;
	}

	for(i = 0; i < num_total_bytes*2;){
        aux = i;
        CL_p = CL_n;
		getByte(byte, total_bytes);
		opcode = (int) strtol(byte, NULL, 16);
		mnemonico = getInstruction(opcode, total_bytes, byte);
        aux = (i - aux)/2;
        CL_n = CL_n + aux;
        printf("%X\t", CL_p);
		printf("%s\n", mnemonico);
	}
		
    return EXIT_SUCCESS;
}


char *getEti_p(uint16_t cl){
    int index = 1;
	char aux[12];	//Todos los números enteros caben en un arreglo de 12 carcateres
    while (symbols[index] != 0x0000 && index < 100){	//Busca por etiquetas ya definidas
        if (cl == symbols[index]){		//Si lo encuentra
			strcpy(eti, "ETI");
			sprintf(aux, "%d", index); 	//Se copia el entero en un arreglo
			strcat(eti, aux);			//Se concatena el entero con la palabra ETI
			return eti;
		}
        index++;
        
    }
	if(index < 100){			//Agrega etiquetas no definidas
		symbols[index] = cl;
		strcpy(eti, "ETI");
		sprintf(aux, "%d", index);
		strcat(eti, aux);
		return eti;
	}else
	return "ERROR";				//Algún error
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
		char line[46], byte[3];
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

void complete(const char *arg1, char arg2[], char arg3[]){  // junta primera parte de mnemonico con n o nn, inserta H al final
	char aux[20];
	strcpy(aux, arg1);
	strcat(arg3, arg2);
	strcat(aux, arg3);
	strcat(aux, "H");
	strcpy(buffer, aux);
}

void completeNewSwitch(const char *arg1, char option[], char arg2[], char arg3[]){
	//junta primera parte de mnemonico con IX o IY, luego con n, inserta H al final
	char aux[20];
	strcpy(aux, arg1);
	strcat(aux, option);
	strcat(arg3, arg2);
	strcat(aux, arg3);
	strcat(aux, "H");
	strcpy(buffer, aux);
}

char *newSwitch(char line[], char ins[]){
    char argument1[20] = {0};
	char argument2[20] = {0};
    int opbyte;
	
	getByte(argument1, line);
	opbyte = (int) strtol(argument1, NULL, 16);
	
	    switch(opbyte){

			case 0xCB:
	            getByte(argument1, line);
	            getByte(argument2, line);
            	opbyte = (int) strtol(argument2, NULL, 16);
	            switch(opbyte){
	                    
					case 0x06:                                  //RLC (IX+d) / RLC (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RLC (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x0E:                                  //RRC (IX+d) / RRC (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RRC (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x16:                                  //RL (IX+d) / RL (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RL (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x1E:                                  //RR (IX+d) / RR (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");    
                        completeNewSwitch("RR (", ins, argument1, argument2);
                        strcat(buffer, ")");
                    	return buffer;
                    case 0x26:                                  //SLA (IX+d) / SLA (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SLA (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
					case 0x2E:                                  //SRA (IX+d) / SRA (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SRA (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x36:                                  //SLS (IX+d) / SRA (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SLS (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
					case 0x3E:                                  //SRL (IX+d) / SRA (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SRL (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x46:                                  //BIT 0, (IX+d) / BIT 0, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("BIT 0, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x4E:                                  //BIT  1, (IX+d) / BIT 1, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("BIT 1, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x56:                                  //BIT 2,(IX+d) / BIT 2, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("BIT 2, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x5E:                                  //BIT 3,(IX+d) / BIT 3, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("BIT 3, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x66:                                  //BIT 4, (IX+d) / BIT 4, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("BIT 4, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x6E:                                  //BIT 5, IX+d) / BIT 5, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("BIT 5, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x76:                                  //BIT 6, (IX+d) / BIT 6, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("BIT 6, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x7E:                                  //BIT 7, (IX+d) / BIT 7, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("BIT 7, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x86:                                  //RES  0, (IX+d) / RES 0, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RES 0, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x8E:                                  //RES 1, (IX+d) / RES 1, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RES 1, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x96:                                  //RES 2, (IX+d) / RES 2, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RES 2, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0x9E:                                  //RES 3, (IX+d) / RES 3, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RES 3, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xA6:                                  //RES 4, (IX+d) / RES 4, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RES 4, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xAE:                                  //RES 5, (IX+d) / RES 5, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RES 5, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xB6:                                  //RES 6,(IX+d) / RES 6, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RES 6, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xBE:                                  //RES 7,(IX+d) / RES 7, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("RES 7, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xC6:                                  //SET 0, (IX+d) / SET 0, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SET 0, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xCE:                                  //SET 1, (IX+d) / SET 1, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SET 1, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xD6:                                  //SET 2, (IX+d) / SET 2, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SET 2, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xDE:                                  //SET 3, (IX+d) / SET 3, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SET 3, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xE6:                                  //SET 4, (IX+d) / SET 4, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SET 4, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xEE:                                  //SET 5, (IX+d) / SET 5, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SET 5, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xF6:                                  //SET 6, (IX+d) / SET 6, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SET 6, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
                    case 0xFE:                                  //SET 7, (IX+d) / SET 7, (IY+d)
                        strcpy(argument2, "");
                        strcat(ins, "+");
                        completeNewSwitch("SET 7, (", ins, argument1, argument2);
                        strcat(buffer, ")");
                        return buffer;
	                }
            case 0x09:                                  //ADD IX, BC / ADD IY, BC
            	strcpy(buffer, "ADD ");
            	strcat(buffer, ins);
            	strcat(buffer, ", BC");
                return buffer;
            case 0x19:                                  //ADD IX, DE / ADD IY, DE
            	strcpy(buffer, "ADD ");
            	strcat(buffer, ins);
            	strcat(buffer, ", DE");
                return buffer;
            case 0x21:                                  //LD IX, nn / LD IY, nn
                getByte(argument2, line);
                getByte(argument1, line);
                strcat(ins, ", ");
                completeNewSwitch("LD ", ins, argument2, argument1);
                return buffer;
            case 0x22:                                  //LD (nn), IX / LD (nn), IY
                getByte(argument2, line);
                getByte(argument1, line);
                complete("LD (", argument2, argument1);
                strcat(buffer, "), ");
                strcat(buffer, ins);
                return buffer;
            case 0x23:                                  //INC IX / INC IY
                strcpy(buffer, "INC ");
                strcat(buffer, ins);
                return buffer;
            case 0x29:                                  //ADD IX, IX / ADD IY, IY
                strcpy(buffer, "ADD ");
                strcat(buffer, ins);
                strcat(buffer, ", ");
                strcat(buffer, ins);
                return buffer;
            case 0x2A:                                  //LD IX, (nn) / LD IY, (nn)
                getByte(argument2, line);
                getByte(argument1, line);
                strcat(ins, ", (");
                completeNewSwitch("LD ", ins, argument2, argument1);
                strcat(buffer, ")");
                return buffer;
            case 0x2B:                                  //DEC IX, DEC IY
            	strcpy(buffer, "DEC ");
            	strcat(buffer, ins);
                return buffer;
            case 0x34:                                  //INC (IX+d) / INC (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("INC (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x35:                                  //DEC (IX+d) / DEC (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("DEC (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x36:                                  //LD (IX+d), n / LD (IY+d), n
                getByte(argument1, line);
                getByte(argument2, line);
                strcat(ins, "+");
                strcat(ins, argument1);
                strcat(ins, "), ");
                strcat(ins, argument2);
                strcpy(buffer, "LD (");
                strcat(buffer, ins);
                return buffer;
            case 0x46:                                  //LD B, (IX+d) / LD B, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD B, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x4E:                                  //LD C, (IX+d) / LD C, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD C, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x56:                                  //LD D, (IX+d) / LD D, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD D, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x5E:                                  //LD E, (IX+d) / LD E, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD E, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x66:                                  //LD H, (IX+d) / LD H, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD H, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x6E:                                  //LD L, (IX+d) / LD L, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD H, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x70:                                  //LD (IX+d), B / LD (IY+d), B
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD (", ins, argument1, argument2);
                strcat(buffer, "), B");
                return buffer;
            case 0x71:                                  //LD (IX+d), C / LD (IY+d), C
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD (", ins, argument1, argument2);
                strcat(buffer, "), C");
                return buffer;
            case 0x72:                                  //LD (IX+d), D / LD (IY+d), D
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD (", ins, argument1, argument2);
                strcat(buffer, "), D");
                return buffer;
            case 0x73:                                  //LD (IX+d), E / LD (IY+d), E
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD (", ins, argument1, argument2);
                strcat(buffer, "), E");
                return buffer;
            case 0x74:                                  //LD (IX+d), H / LD (IY+d), H
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD (", ins, argument1, argument2);
                strcat(buffer, "), E");
                return buffer;
            case 0x75:                                  //LD (IX+d), L / LD (IY+d), L
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD (", ins, argument1, argument2);
                strcat(buffer, "), L");
                return buffer;
            case 0x77:                                  //LD (IX+d), A / LD (IY+d), A
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD (", ins, argument1, argument2);
                strcat(buffer, "), A");
                return buffer;
            case 0x7E:                                  //LD A, (IX+d) / LD A, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("LD A, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x86:                                  //ADD A, (IX+d) / ADD A, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("ADD A, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x8E:                                  //ADC A, (IX+d) / ADC A, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("ADC A, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x96:                                  //SUB A, (IX+d) / SUB A, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("SUB A, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0x9E:                                  //SBC A, (IX+d) / SBC A, (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("SBC A, (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0xA6:                                  //AND (IX+d) / AND (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("AND (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0xAE:                                  //XOR (IX+d) / XOR (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("XOR (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0xB6:                                   //OR (IX+d) / OR (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("OR (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0xBE:                                  //CP (IX+d) / CP (IY+d)
                getByte(argument1, line);
                strcpy(argument2, "");
                strcat(ins, "+");
                completeNewSwitch("CP (", ins, argument1, argument2);
                strcat(buffer, ")");
                return buffer;
            case 0xE1:                                  //POP IX / POP IY
                strcpy(buffer, "POP ");
                strcat(buffer, ins);
				return buffer;
            case 0xE3:                                  //EX (SP), IX / EX (SP), IY
                strcpy(buffer, "EX (SP), ");
                strcat(buffer, ins);
				return buffer;
            case 0xE5:                                  //PUSH IX / PUSH IY
                strcpy(buffer, "PUSH ");
                strcat(buffer, ins);
				return buffer;
            case 0xE9:                                  //JP (IX) / JP (IY)
                strcpy(buffer, "JP (");
                strcat(buffer, ins);
                strcat(buffer, ")");
				return buffer;
	        }
    return "";
}

char * getInstruction(int opcode, char line[], char byte[]) {
	char argument1[20] = {0};  //byte mas significativo
	char argument2[20] = {0};  //byte menos significativo
	int nextbyte;
    char name[6];

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
		case 0x28:                                  //  JR Z, e
            /*getByte(argument1, line);									
			eti = getEti_r(argument1);
			strcpy(buffer, "JR Z, ");
            strcat(buffer, eti);*/
            return buffer;
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
			getByte(argument2, line);
            getByte(argument1, line);
			strcpy(buffer, argument2);
			strcat(buffer, argument1);
			cl = (uint16_t)strtol(buffer, NULL, 16);
            strcpy (name, getEti_p(cl));
			strcpy(buffer, "JP NZ, ");
            strcat(buffer, eti);
            return buffer;
		case 0xC3:									//  JP e
			getByte(argument2, line);
            getByte(argument1, line);
			strcpy(buffer, argument2);
			strcat(buffer, argument1);
			cl = (uint16_t)strtol(buffer, NULL, 16);
            strcpy (name, getEti_p(cl));
			strcpy(buffer, "JP ");
            strcat(buffer, eti);
            return buffer;
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
			getByte(argument2, line);
            getByte(argument1, line);
			strcpy(buffer, argument2);
			strcat(buffer, argument1);
			cl = (uint16_t)strtol(buffer, NULL, 16);
            strcpy (name, getEti_p(cl));
			strcpy(buffer, "JP Z, ");
            strcat(buffer, eti);
            return buffer;
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
			getByte(argument2, line);
            getByte(argument1, line);
			strcpy(buffer, argument2);
			strcat(buffer, argument1);
			cl = (uint16_t)strtol(buffer, NULL, 16);
            strcpy (name, getEti_p(cl));
			strcpy(buffer, "JP NC, ");
            strcat(buffer, eti);
            return buffer;
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
			getByte(argument2, line);
            getByte(argument1, line);
			strcpy(buffer, argument2);
			strcat(buffer, argument1);
			cl = (uint16_t)strtol(buffer, NULL, 16);
            strcpy (name, getEti_p(cl));
			strcpy(buffer, "JP C, ");
            strcat(buffer, eti);
            return buffer;			
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
			strcpy(argument1, "IX");
			return newSwitch(line, argument1);
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
			getByte(argument2, line);
            getByte(argument1, line);
			strcpy(buffer, argument2);
			strcat(buffer, argument1);
			cl = (uint16_t)strtol(buffer, NULL, 16);
            strcpy (name, getEti_p(cl));
			strcpy(buffer, "JP Z, ");
            strcat(buffer, eti);
            return buffer;
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
			getByte(argument2, line);
            getByte(argument1, line);
			strcpy(buffer, argument2);
			strcat(buffer, argument1);
			cl = (uint16_t)strtol(buffer, NULL, 16);
            strcpy (name, getEti_p(cl));
			strcpy(buffer, "JP PE, ");
            strcat(buffer, eti);
            return buffer;
		case 0xEB:									//  EX DE, HL
			return "EX DE, HL";
		case 0xEC:									//  CALL PE, e
			// funcion getEti()
			return "Incompleto";
		case 0xED:									//  ** ED **
			getByte(byte, line);
			opcode = (int) strtol(byte, NULL, 16);
			// switch para ED
            switch(opcode){
                case 0x00: 
                    return "MOS_QUIT";
                case 0x01:
                    return "MOS_CLI";
                case 0x02: 
                    return "MOS_BYTE";
                case 0x03: 
                    return "MOS_WORD";
                case 0x04: 
                    return "MOS_WRCH";
                case 0x05: 
                    return "MOS_RDCH";
                case 0x06: 
                    return "MOS_FILE";
                case 0x07: 
                    return "MOS_ARGS";
                case 0x08: 
                    return "MOS_BGET";
                case 0x09: 
                    return "MOS_BPUT";
                case 0x0A: 
                    return "MOS_GBPB";
                case 0x0B: 
                    return "MOS_FIND";
                case 0x0C:  
                    return "MOS_FF0C";
                case 0x0D:
                    return "MOS_FF0D";
                case 0x0E:  
                    return "MOS_FF0E";
                case 0x0F:
                    return "MOS_FF0F";
                case 0x40: 
                	return "IN B,(C)";
                case 0x41: 
                    return "OUT (C),B";
                case 0x42: 
                    return "SBC HL,BC";
                case 0x43: 
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), BC");
                    return buffer;
                case 0x44: 
                    return "NEG";
                case 0x45: 
                    return "RETN";
                case 0x46: 
                    return "IM 0";
                case 0x47: 
                    return "LD I,A";
                case 0x48: 
                    return "IN C,(C)";
                case 0x49: 
                    return "OUT (C),C";
                case 0x4A: 
                    return "ADC HL,BC";
                case 0x4B: 
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD BC,(", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer;
                case 0x4C:
                    return "[neg]"; 
                case 0x4D: 
                    return "RETI";
                case 0x4E: 
                    return "[im0]";
                case 0x4F: 
                    return "LD R,A"; 
                case 0x50: 
                    return "IN D,(C)";
                case 0x51: 
                    return "OUT (C),D";
                case 0x52: 
                    return "SBC HL,DE";
                case 0x53: 
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), DE");
                    return buffer;
                case 0x54:
                    return "[neg]";
                case 0x55:
                    return "[retn]";
                case 0x56: 
                    return "IM 1";
                case 0x57: 
                    return "LD A,I";
                case 0x58: 
                    return "IN E,(C)";
                case 0x59:
                    return "OUT (C),E";
                case 0x5A:
                    return "ADC HL,DE";
                case 0x5B:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD DE,(", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer; 
                case 0x5C:
                    return "[neg]";
                case 0x5D:
                    return "[reti]";
                case 0x5E:
                    return "IM 2"; 
                case 0x5F:
                    return "LD A,R";
                case 0x60: 
                    return "IN H,(C)";
                case 0x61:
                    return "OUT (C),H";
                case 0x62:
                    return "SBC HL,HL";
                case 0x63:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), HL");
                    return buffer;
                case 0x64:
                    return "[neg]"; 
                case 0x65:
                    return "[retn]";
                case 0x66:
                    return "[im0]";
                case 0x67:
                    return "RRD"; 
                case 0x68:
                    return "IN L,(C)";
                case 0x69:
                    return "OUT (C),L";
                case 0x6A:
                    return "ADC HL,HL";
                case 0x6B:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD HL,(", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer; 
                case 0x6C:
                    return "[neg]";
                case 0x6D:
                    return "[reti]";
                case 0x6E:
                    return "[im0]";
                case 0x6F:
                    return "RLD";
                case 0x70:
                    return "IN F,(C)";
                case 0x71:
                	return "OUT (C),F";
                case 0x72:
                    return "SBC HL,SP";
                case 0x73:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD (", argument2, argument1);
                    strcat(buffer, "), SP");
                    return buffer;
                case 0x74:
                    return "[neg]";
                case 0x75:
                    return "[retn]";
                case 0x76:
                    return "[im1]";
                case 0x77:
                    return "[ld i,i?]";
                case 0x78:
                    return "IN A,(C)";
                case 0x79:
                    return "OUT (C),A";
                case 0x7A:
                    return "ADC HL,SP";
                case 0x7B:
                    getByte(argument2, line);
                    getByte(argument1, line);
                    complete("LD SP,(", argument2, argument1);
                    strcat(buffer, ")");
                    return buffer; 
                case 0x7C:
                    return "[neg]";
                case 0x7D:
                    return "[reti]";
                case 0x7E:
                    return "[im2]";
                case 0x7F:
                    return "[ld r,r?]";
                case 0xA0:
                    return "LDI";
                case 0xA1:
                    return "CPI";
                case 0xA2:
                    return "INI";
                case 0xA3:
                    return "OTI";
                case 0xA8:
                    return "LDD";
                case 0xA9:
                    return "CPD";
                case 0xAA:
                    return "IND";
                case 0xAB:
                    return "OTD";
                case 0xB0:
                    return "LDIR";
                case 0xB1:
                    return "CPIR";
                case 0xB2:
                    return "INIR";
                case 0xB3:
                    return "OTIR";
                case 0xB8:
                    return "LDDR";
                case 0xB9:
                    return "CPDR";
                case 0xBA:
                    return "INDR";
                case 0xBB:
                    return "OTDR";
                case 0xF8:
                    return "[z80]";
                case 0xF9:
                    return "[z80]";
                case 0xFA:
                    return"[z80]";
                case 0xFB:
                    return "ED_LOAD";
                case 0xFC:
                    return "[z80]";
                case 0xFD:
                    return "[z80]";
                case 0xFE:
                    return "[z80]";
                case 0xFF:
                    return "ED_DOS";
				default:
					printf("Error: formato incorrecto");
					return "ERROR";
        }
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
			getByte(argument2, line);
            getByte(argument1, line);
			strcpy(buffer, argument2);
			strcat(buffer, argument1);
			cl = (uint16_t)strtol(buffer, NULL, 16);
            strcpy (name, getEti_p(cl));
			strcpy(buffer, "JP P, ");
            strcat(buffer, eti);
            return buffer;
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
			getByte(argument2, line);
            getByte(argument1, line);
			strcpy(buffer, argument2);
			strcat(buffer, argument1);
			cl = (uint16_t)strtol(buffer, NULL, 16);
            strcpy (name, getEti_p(cl));
			strcpy(buffer, "JP M, ");
            strcat(buffer, eti);
            return buffer;
		case 0xFB:									//  EI
			return "EI";
		case 0xFC:									//  CALL M, e
			// funcion getEti()
			return "Incompleto";
		case 0xFD:									//  ** FD **
			strcpy(argument1, "IY");
			return newSwitch(line, argument1);
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
