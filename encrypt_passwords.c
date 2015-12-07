#include "encrypt_passwords.h"

void encryptMd5(char *password, char *hash, int length) {
    unsigned char md5_hash[MD5_HASH_SIZE];
    char md5_buffer[256];

    //md5 produces a 16 byte hash value (one way)
    memset(md5_hash, 0, MD5_HASH_SIZE);
    encrypt_digest(password, length, md5_hash, 0, EVP_md5());
    memset(md5_buffer, 0, sizeof(md5_buffer));
    int j; 
    for(j = 0; j < MD5_HASH_SIZE; j++) {
	sprintf(md5_buffer+2*j, "%02x", md5_hash[j]);
    } 
    strncpy(hash, md5_buffer, strlen(md5_buffer));
}

void encryptSha1(char *password, char *hash, int length) {
    unsigned char sha1_hash[SHA1_HASH_SIZE];
    char sha1_buffer[256];

    // SHA1 gives a 20 byte hash
    memset(sha1_hash, 0, SHA1_HASH_SIZE);
    encrypt_digest(password, length, sha1_hash, 0, EVP_sha1());
    memset(sha1_buffer, 0, sizeof(sha1_buffer));

    int j; 
    for(j = 0; j < SHA1_HASH_SIZE; j++){
	    sprintf(sha1_buffer+2*j, "%02x", sha1_hash[j]);
    }

    strcpy(hash, sha1_buffer);
}

void encryptAes256(char *password, char *hash, int length) {
    char aes256_buffer[256];

    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";	/* 256 bit key */
    unsigned char *iv = (unsigned char *)"01234567890123456";			/* A 128 bit IV  = Initialization vector*/
    unsigned char ciphertext[128];						/* Buffer for ciphertext, make sure buffer is long enough for ciphertext*/
    unsigned char decryptedtext[128];						/* Buffer for the decrypted text */

    int decryptedtext_len, ciphertext_len;
  
    ciphertext_len = encrypt_cipher(password, length, key, iv, ciphertext, EVP_aes_256_cbc());	/* Encrypt the plaintext */
    memset(aes256_buffer, 0, sizeof(aes256_buffer));
    int j; 
    for(j = 0; j < ciphertext_len; j++){
	    sprintf(aes256_buffer+2*j, "%02x", ciphertext[j]);
    }

    strcpy(hash, aes256_buffer);
}
