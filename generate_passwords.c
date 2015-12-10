/*
* This class is used to create a text file of random passwords of length 4-6 
* alphanumeric characters. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define GEN_NUM_OF_PASSWORDS 30 

int main (int argc, char *argv[]) {

    FILE *f = fopen("passwords.txt", "w");

    if (f == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    int i, j, length;
    char* text; 
    for (i = 0; i < GEN_NUM_OF_PASSWORDS; i++) {
	length = 4 + (random() % 2); 
	text = malloc(length * sizeof(char)); 
	for (j = 0; j < length; j++) { 
		int characterOption = random() % 3; 
		char randomChar; 
		// random combines alphanumeric characters
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
