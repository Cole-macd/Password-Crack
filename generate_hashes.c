#include "generate_hashes.h"

int main (int argc, char *argv[]) {

    FILE *f = fopen("password.txt", "r");

    if (f == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    char line[256];
    int lineCount = 0; 

    while (fgets(line, sizeof(line), f)) {
	passwords[lineCount] = malloc(strlen(line));
	strcpy(passwords[lineCount], line);
	lineCount++; 
    }
   
    md5_to_text();
    sha1_to_text();
    aes_256_to_text();
 
    fclose(f);
}


void md5_to_text() {
    FILE *f_md5 = fopen("md5_pw.txt", "w");
    int i;
    char* encrypted;
    char* encr;
    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
	encrypt_md5(passwords[i], encr, strlen(passwords[i])); 
	printf("%s\n", encr); 
	fprintf(f_md5, "%s\n", buffer); 
    }
   fclose(f_md5);
}

void sha1_to_text() {
    FILE *f_sha1 = fopen("sha1_pw.txt", "w");
    int i;
    char* encrypted; 
    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
    	char* encr = encrypt_sha1(passwords[i]); 
	fprintf(f_sha1, "%s\n", buffer); 
    }
    fclose(f_sha1); 
}

void aes_256_to_text() {
    FILE *f_aes256 = fopen("aes256_pw.txt", "w");
    int i;
    char* encrypted; 
    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
    	char* encr = encrypt_aes_256(passwords[i]); 
	fprintf(f_aes256, "%s\n", buffer); 
    }
    fclose(f_aes256); 
}


