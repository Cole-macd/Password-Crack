#include "generate_hashes.h"

int main (int argc, char *argv[]) {

    FILE *f = fopen("passwords.txt", "r");

    if (f == NULL) {
        printf("Error opening file!\n");
        return -1;
    }

    char line[256];
    int lineCount = 0; 
    passwords = malloc(NUM_OF_PASSWORDS*sizeof(char*));
    int j;
    for (j = 0; j < NUM_OF_PASSWORDS; j++) {
	passwords[j] = (char*)malloc(MAX_PW_LENGTH * sizeof(char));
    }

    while (fgets(line, sizeof(line), f)) {
	strcpy(passwords[lineCount], line);
	passwords[lineCount][strlen(passwords[lineCount])-1] = '\0';
	lineCount++; 
    }
   
    md5ToText();
    sha1ToText();
    aes256ToText();

    fclose(f);
    return 1;
}


void md5ToText() {
    FILE *f_md5 = fopen("md5_hashes.txt", "w");
    int i;
    char* md5_encr = (char*) malloc(sizeof(char)*MD5_HASH_SIZE*2+1);

    for (i = 0; i < NUM_OF_PASSWORDS; i++) {    
	encryptMd5(passwords[i], md5_encr, strlen(passwords[i])); 
	fprintf(f_md5, "%s\n", md5_encr); 
    }
   fclose(f_md5);
   free(md5_encr);
}

void sha1ToText() {
    FILE *f_sha1 = fopen("sha1_hashes.txt", "w");
    int i;
    char* sha1_encr = (char*) malloc(sizeof(char)*SHA1_HASH_SIZE*2+1);

    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
    	encryptSha1(passwords[i], sha1_encr, strlen(passwords[i])); 
	fprintf(f_sha1, "%s\n", sha1_encr); 
    }

    fclose(f_sha1); 
    free(sha1_encr);

}

void aes256ToText() {
    FILE *f_aes256 = fopen("aes256_hashes.txt", "w");
    int i;
    char* aes256_encr = (char*) malloc(sizeof(char)*AES256_HASH_SIZE*2+1);
    
    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
    	encryptAes256(passwords[i], aes256_encr, strlen(passwords[i])); 
	fprintf(f_aes256, "%s\n", aes256_encr); 
    }
    printf("finished!\n"); 
    fclose(f_aes256);
    free(aes256_encr);

}


