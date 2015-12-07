#ifndef __ENCRYPT_PASSWORDS_H
#define __ENCRYPT_PASSWORDS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt_decrypt.h" 
#include "generate_hashes.h" 

void encryptMd5(char *password, char *hash, int length);
void encryptSha1(char *password, char *hash, int length);
void encryptAes256(char *password, char *hash, int length);

#endif
