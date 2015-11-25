#ifndef __ENCRYPT_PASSWORDS_H
#define __ENCRYPT_PASSWORDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt_decrypt.h" 
#include "generate_hashes.h" 

char* encrypt_md5(char *password);
char* encrypt_sha1(char *password);
char* encrypt_aes_256(char *password);

#endif
