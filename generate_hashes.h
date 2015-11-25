
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void md5_to_text();
void sha1_to_text();
void aes_256_to_text();
char* encrypt_md5(char *password); 
char* encrypt_sha1(char *password);
char* encrypt_aes_256(char *password);
