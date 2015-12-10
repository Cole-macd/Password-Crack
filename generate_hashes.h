#ifndef __GENERATE_HASHES_H
#define __GENERATE_HASHES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "encrypt_decrypt.h" 
#include "encrypt_passwords.h"

#define SHA1_HASH_SIZE 20
#define MD5_HASH_SIZE 16
#define AES256_HASH_SIZE 16
#define NUM_OF_PASSWORDS 30
#define MAX_PW_LENGTH 6

char **passwords;
void md5ToText();
void sha1ToText();
void aes256ToText();

#endif
