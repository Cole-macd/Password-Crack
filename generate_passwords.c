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
    for (i = 0; i < 10; i++) {
	length = 4 + (random() % 2); 
	text = malloc(length * sizeof(char)); 
	for (j = 0; j < length; j++) { 
		int characterOption = random() % 3; 
		char randomChar; 
 		if (characterOption == 0) {
		    randomChar = ('A' + (random() % 26));
		} else if (characterOption == 1) {
		    randomChar = ('0' + (random() % 10));
		} else {
		    randomChar = ('a' + (random() % 26));
		}

		text[j] = randomChar; 
	}
	fprintf(f, "%s\n", text); 
    }
    fclose(f);
}
