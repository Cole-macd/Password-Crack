#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt_decrypt.h"

#define SHA1_HASH_SIZE 20
#define MD5_HASH_SIZE 16
#define NUM_OF_PASSWORDS 50

char *passwords[NUM_OF_PASSWORDS];
void encrypt_md5();
void encrypt_sha1();
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
   
    encrypt_md5(); 
    encrypt_sha1();
    encrypt_aes_256("testing"); 
 
    fclose(f);
}

void encrypt_md5() {
    FILE *f_md5 = fopen("md5_pw.txt", "w");
    int i,j;
    char* encrypted; 
    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
	//md5 produces a 16 byte hash value (one way)
 	unsigned char hash[MD5_HASH_SIZE];
 	memset(hash, 0, sizeof(hash));
  	encrypt_digest(passwords[i], strlen(passwords[i]), hash, 0, EVP_md5());
  	char buffer[256];
  	memset(buffer, 0, sizeof(buffer));
  	for(j = 0; j < MD5_HASH_SIZE; j++){
   	     sprintf(buffer+2*j, "%02x", hash[j]);
  	}
	fprintf(f_md5, "%s\n", buffer); 
     }
    fclose(f_md5);
}


void encrypt_sha1() {
    FILE *f_sha1 = fopen("sha1_pw.txt", "w");
    int i,j;
    char* encrypted; 
    for (i = 0; i < NUM_OF_PASSWORDS; i++) {
 	// SHA1 gives a 20 byte hash
	unsigned char hash[SHA1_HASH_SIZE]; 
	memset(hash, 0, sizeof(hash));
	encrypt_digest(passwords[i], strlen(passwords[i]), hash, 0, EVP_sha1());
	char buffer[256];
	memset(buffer, 0, sizeof(buffer));
	for(j= 0; j < SHA1_HASH_SIZE; j++){
	    sprintf(buffer+2*j, "%02x", hash[j]);
	}
	fprintf(f_sha1, "%s\n", buffer); 
    }
    fclose(f_sha1); 
}

void encrypt_aes_256(char *plaintext) {
  FILE *f_aes256 = fopen("aes_256_pw.txt", "w");
  unsigned char *key = (unsigned char *)"01234567890123456789012345678901";	/* 256 bit key */
  unsigned char *iv = (unsigned char *)"01234567890123456";			/* A 128 bit IV  = Initialization vector*/
  //unsigned char *plaintext = (unsigned char *)"BOOBIES";	 		/* Message to be encrypted */
  unsigned char ciphertext[128];						/* Buffer for ciphertext, make sure buffer is long enough for ciphertext*/
  unsigned char decryptedtext[128];						/* Buffer for the decrypted text */

  int decryptedtext_len, ciphertext_len;
   
  ciphertext_len = encrypt_cipher(plaintext, strlen((char *)plaintext), key, iv, ciphertext, EVP_aes_256_cbc());	/* Encrypt the plaintext */
  ciphertext[ciphertext_len] = '\0';
  fprintf(f_aes256,"Cipher text: %s\n", ciphertext);

  decryptedtext_len = decrypt_cipher(ciphertext, ciphertext_len, key, iv, decryptedtext, EVP_aes_256_cbc());		/* Decrypt the ciphertext */
  decryptedtext[decryptedtext_len] = '\0';					/* Add a NULL terminator. We are expecting printable text */
  fprintf(f_aes256, "Decrypted text: %s\n", decryptedtext);


}
