#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[]) {

    FILE *f = fopen("password.txt", "w");

    if (f == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    int i, j, length;
    char* text; 
    for (i = 0; i < 50; i++) {
	length = 6 + (random() % 3); 
	text = malloc(length * sizeof(char)); 
	for (j = 0; j < length; j++) { 
		char randomChar = ('A' + (random() % 26));
		text[j] = randomChar; 
	}
	fprintf(f, "%s\n", text); 
    }

    fclose(f);

}
