#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

int charToInt(char, char);

int main(int argvc, char **argv) {
    FILE *archive;
    char line[100];
    char byte[2] = "6A";

    archive = fopen(argv[1], "r");
    if (archive == NULL){
        puts("Unable to open the file");
    } else {
        
        while ( fgets(line, sizeof(line), archive) != NULL ) {
            byte[0] = line[1];
            byte[1] = line[2];
            int intOpcode = (int) strtol(byte, NULL, 16);
            printf("%i\n", intOpcode);
            
        }

        fclose(archive);
    }
    return EXIT_SUCCESS;
}






