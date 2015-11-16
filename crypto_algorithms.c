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
  ciphertext_len = encrypt(plaintext, strlen((char *)plaintext), key, iv, ciphertext);

   /* Do something useful with the ciphertext here */
  printf("Ciphertext is:\n");
  
  BIO_dump_fp (stdout, (const char *)ciphertext, ciphertext_len);

  /* Decrypt the ciphertext */
  decryptedtext_len = decrypt(ciphertext, ciphertext_len, key, iv,
    decryptedtext);

  /* Add a NULL terminator. We are expecting printable text */
  decryptedtext[decryptedtext_len] = '\0';

  /* Show the decrypted text */
  printf("Decrypted text is:\n");
  printf("%s\n", decryptedtext);

}




int main(void){
	
	
  /* Initialise the library */
  ERR_load_crypto_strings();
  OpenSSL_add_all_algorithms();
  OPENSSL_config(NULL);

  test_aes_256_cbc();

  /* Clean up */
  EVP_cleanup();
  ERR_free_strings();

  return 0;
}