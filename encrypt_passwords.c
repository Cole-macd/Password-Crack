#include "encrypt_passwords.h"

void encrypt_md5(char *password, char *hash, int length) {
	unsigned char md5_hash_two[MD5_HASH_SIZE];
	char buffer_two[256];

    //md5 produces a 16 byte hash value (one way)
    memset(md5_hash_two, 0, MD5_HASH_SIZE);
    encrypt_digest(password, length, md5_hash_two, 0, EVP_md5());
    memset(buffer_two, 0, sizeof(buffer));
    int j; 
    for(j = 0; j < MD5_HASH_SIZE; j++) {
	sprintf(buffer_two+2*j, "%02x", md5_hash_two[j]);
    } 
    //char *encrypted = (char *) malloc(sizeof(char) * 256);
    strncpy(hash, buffer_two, strlen(buffer_two));
    //return encrypted;
}

char* encrypt_sha1(char *password) {
    // SHA1 gives a 20 byte hash
    memset(sha1_hash, 0, SHA1_HASH_SIZE);
    encrypt_digest(password, strlen(password), sha1_hash, 0, EVP_sha1());
    memset(buffer, 0, sizeof(buffer));
    int j; 
    for(j = 0; j < SHA1_HASH_SIZE; j++){
	    sprintf(buffer+2*j, "%02x", sha1_hash[j]);
    }
    char *encrypted = (char *) malloc(sizeof(char) * 256); 
    strcpy(encrypted, buffer); 
    return encrypted;
}

char* encrypt_aes_256(char *password) {
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901";	/* 256 bit key */
    unsigned char *iv = (unsigned char *)"01234567890123456";			/* A 128 bit IV  = Initialization vector*/
    unsigned char ciphertext[128];						/* Buffer for ciphertext, make sure buffer is long enough for ciphertext*/
    unsigned char decryptedtext[128];						/* Buffer for the decrypted text */

    int decryptedtext_len, ciphertext_len;
   
    ciphertext_len = encrypt_cipher(password, strlen((char *)password), key, iv, ciphertext, EVP_aes_256_cbc());	/* Encrypt the plaintext */
    ciphertext[ciphertext_len] = '\0';
    memset(buffer, 0, sizeof(buffer));
    int j; 
    for(j = 0; j < ciphertext_len; j++){
	    sprintf(buffer+2*j, "%02x", ciphertext[j]);
    }
    char *encrypted = (char *) malloc(sizeof(char) * 128); 
    strcpy(encrypted, buffer); 
    return encrypted;
}
