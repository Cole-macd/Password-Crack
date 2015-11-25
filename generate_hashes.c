#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt_decrypt.h"

#define SHA1_HASH_SIZE 20
#define MD5_HASH_SIZE 16
#define NUM_OF_PASSWORDS 50

char *passwords[NUM_OF_PASSWORDS];
unsigned char md5_hash[MD5_HASH_SIZE];
unsigned char sha1_hash[SHA1_HASH_SIZE]; 
char buffer[256];

void md5_to_text();
void sha1_to_text();
void aes_256_to_text();
void encrypt_md5(char *password); 
void encrypt_sha1(char *password);
void encrypt_aes_256(char *plaintext);

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
    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
	encrypt_md5(passwords[i]); 
	fprintf(f_md5, "%s\n", buffer); 
    }
   fclose(f_md5);
}

void encrypt_md5(char *password) {
    //md5 produces a 16 byte hash value (one way)
    memset(md5_hash, 0, MD5_HASH_SIZE);
    encrypt_digest(password, strlen(password), md5_hash, 0, EVP_md5());
    memset(buffer, 0, sizeof(buffer));
    int j; 
    for(j = 0; j < MD5_HASH_SIZE; j++) {
	sprintf(buffer+2*j, "%02x", md5_hash[j]);
    }
}

void sha1_to_text() {
    FILE *f_sha1 = fopen("sha1_pw.txt", "w");
    int i;
    char* encrypted; 
    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
    	encrypt_sha1(passwords[i]); 
	fprintf(f_sha1, "%s\n", buffer); 
    }
    fclose(f_sha1); 
}

void encrypt_sha1(char *password) {
    // SHA1 gives a 20 byte hash
    memset(sha1_hash, 0, SHA1_HASH_SIZE);
    encrypt_digest(password, strlen(password), sha1_hash, 0, EVP_sha1());
    memset(buffer, 0, sizeof(buffer));
    int j; 
    for(j = 0; j < SHA1_HASH_SIZE; j++){
	    sprintf(buffer+2*j, "%02x", sha1_hash[j]);
    }
}

void aes_256_to_text() {
    FILE *f_aes256 = fopen("aes256_pw.txt", "w");
    int i;
    char* encrypted; 
    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
    	encrypt_aes_256(passwords[i]); 
	fprintf(f_aes256, "%s\n", buffer); 
    }
    fclose(f_aes256); 
}

void encrypt_aes_256(char *plaintext) {
    char* encrypted; 
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";	/* 256 bit key */
    unsigned char *iv = (unsigned char *)"01234567890123456";			/* A 128 bit IV  = Initialization vector*/
    unsigned char ciphertext[128];						/* Buffer for ciphertext, make sure buffer is long enough for ciphertext*/
    unsigned char decryptedtext[128];						/* Buffer for the decrypted text */

    int decryptedtext_len, ciphertext_len;
   
    ciphertext_len = encrypt_cipher(plaintext, strlen((char *)plaintext), key, iv, ciphertext, EVP_aes_256_cbc());	/* Encrypt the plaintext */
    ciphertext[ciphertext_len] = '\0';
    memset(buffer, 0, sizeof(buffer));
    int j; 
    for(j = 0; j < ciphertext_len; j++){
	    sprintf(buffer+2*j, "%02x", ciphertext[j]);
    }
}
