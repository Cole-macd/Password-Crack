#include "encrypt_decrypt.h"
#include <string.h>

#define SHA1_HASH_SIZE 20
#define MD5_HASH_SIZE 16

//For block ciphers, the input size is the output size
//BUT the input size must be a multiple of the blocks (or padding is needed)
//Block size for AES is 128 bits (16 bytes), but the key size can be 128, 192 or 256 bits
//Looks likes EVP takes care of this padding though
void test_aes_256_cbc(void){
  /* A 256 bit key */
  unsigned char *key = (unsigned char *)"01234567890123456789012345678901";

  /* A 128 bit IV  = Initialization vector */
  unsigned char *iv = (unsigned char *)"01234567890123456";

  /* Message to be encrypted */
  unsigned char *plaintext = (unsigned char *)"The quick brown fox jumps over the lazy dog";

  /* Buffer for ciphertext. Ensure the buffer is long enough for the
   * ciphertext which may be longer than the plaintext, dependant on the
   * algorithm and mode
   */
  unsigned char ciphertext[128];

  /* Buffer for the decrypted text */
  unsigned char decryptedtext[128];

  int decryptedtext_len, ciphertext_len;
   /* Encrypt the plaintext */
  ciphertext_len = encrypt_cipher(plaintext, strlen((char *)plaintext), key, iv, ciphertext, EVP_aes_256_cbc());

   /* Do something useful with the ciphertext here */
  //printf("Ciphertext is:\n");
  
  //BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  /* Decrypt the ciphertext */
  decryptedtext_len = decrypt_cipher(ciphertext, ciphertext_len, key, iv,
    decryptedtext, EVP_aes_256_cbc());

  /* Add a NULL terminator. We are expecting printable text */
  decryptedtext[decryptedtext_len] = '\0';

  /* Show the decrypted text */
  //printf("Decrypted text is:\n");
  //printf("%s\n", decryptedtext);

  if(strcmp(plaintext, decryptedtext) == 0){
    printf(" AES 256 CBC PASSED\n");
  }
}

// Returns a string representing the command line output froming running "cmd"
char* get_command_output(char* cmd, int max_output) {
    char* data = malloc(sizeof(char)*max_output);
    memset(data, 0, sizeof(data));
    FILE* stream = popen(cmd, "r");
    if (stream) {
        if (fgets(data, max_output, stream) == NULL){
            printf("Failed to get output\n");
        } 
        pclose(stream);
    }
    else{
      printf("Failed to open process %s\n",cmd);
    }
    return data;
}

void test_md5(void){
  char plaintext[]= "Hello world";
  //md5 produces a 16 byte hash value (one way)
  unsigned char hash[MD5_HASH_SIZE]; 
  memset(hash, 0, sizeof(hash));
  encrypt_digest(plaintext, strlen(plaintext), hash, 0, EVP_md5());
  // Below output should be the same as echo -n "Hello world" | openssl md5
  char buffer[256];
  memset(buffer, 0, sizeof(buffer));
  sprintf(buffer, "(stdin)= ");
  int i;
  for(i = 0; i < MD5_HASH_SIZE; i++){
    sprintf(buffer+9+2*i, "%02x", hash[i]);
  }
  sprintf(buffer+9+2*i, "\n");
  char *linux_call = get_command_output("echo -n \"Hello world\" | openssl md5 2>&1", 256);
  if(strcmp(linux_call, buffer) == 0){
    printf(" MD5 PASSED\n");
  }
}


void test_sha_1(void){
  char plaintext[]= "Hello world";
  // SHA1 gives a 20 byte hash
  unsigned char hash[SHA1_HASH_SIZE]; 
  memset(hash, 0, sizeof(hash));
  encrypt_digest(plaintext, strlen(plaintext), hash, 0, EVP_sha1());
  char buffer[256];
  memset(buffer, 0, sizeof(buffer));
  sprintf(buffer, "(stdin)= ");
  int i;
  for(i = 0; i < SHA1_HASH_SIZE; i++){
    sprintf(buffer+9+2*i, "%02x", hash[i]);
  }
  sprintf(buffer+9+2*i, "\n");
  char *linux_call = get_command_output("echo -n \"Hello world\" | openssl sha1 2>&1", 256);
  if(strcmp(linux_call, buffer) == 0){
    printf(" SHA1 PASSED\n");
  }

}

int main(void){
	
	
  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OpenSSL_add_all_digests();
  OPENSSL_config(NULL);

  // Test algorithms
  // Digest algorithm --> generates a hashcode but cannot be decrypted
  // Cipher algorithm --> generates encrypted text using a key and then decrypt the encrypted text
  // with the same key
  test_aes_256_cbc();
  test_md5();
  test_sha_1();
  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

  return 0;
}