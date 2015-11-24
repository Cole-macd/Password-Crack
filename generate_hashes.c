#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt_decrypt.h"

#define SHA1_HASH_SIZE 20
#define MD5_HASH_SIZE 16

int main (int argc, char *argv[]) {

    FILE *f = fopen("password.txt", "r");
    FILE *f_encrypted = fopen("encrypted_pw.txt", "w"); 
    
    char *inputs[3]; 
    inputs[0] = "ABCDEF";
    inputs[1] = "BCDEFG";
    inputs[2] = "CDEFGH";

    if (f == NULL) {
        printf("Error opening file!\n");
        return -1;
    }
    if (f_encrypted == NULL) { 
        printf("Error opening file!\n");
        return -1;
    }

    int i, j, length;
    char* encrypted; 
    for (i=0; i< 3; i++) {
	//md5 produces a 16 byte hash value (one way)
 	unsigned char hash[MD5_HASH_SIZE];
 	memset(hash, 0, sizeof(hash));
  	encrypt_digest(inputs[i], strlen(inputs[i]), hash, 0, EVP_md5());
  	char buffer[256];
  	memset(buffer, 0, sizeof(buffer));
  	sprintf(buffer, "(stdin)= ");
  	int i;
  	for(i = 0; i < MD5_HASH_SIZE; i++){
   	     sprintf(buffer+9+2*i, "%02x", hash[i]);
  	}
  	sprintf(buffer+9+2*i, "\n");
	printf("test\n");
	printf("%c\n", buffer); 
     }

     fclose(f);
     fclose(f_encrypted); 

}
