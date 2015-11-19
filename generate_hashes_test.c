#include "encrypt_decrypt.h"
#include <string.h>

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


void test_md5(void){
  char plaintext[]= "Hello world";
  //md5 produces a 16 byte hash value (one way)
  unsigned char hash[16]; // results
  memset(hash, 0, sizeof(hash));
  const EVP_MD *md = EVP_get_digestbyname("MD5");
  if(!md) printf("Unable to initialize MD5\n");
  encrypt_digest(plaintext, strlen(plaintext), hash, 0, EVP_md5());
  // Below output should be the same as echo -n "Hello world" | openssl md5
  printf("MD5(%s)=", plaintext);
  int i;
  for(i = 0; i < 16; i++){
    printf("%02x", (unsigned char)hash[i]);
  }
  printf("\n");


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
  
  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

  return 0;
}