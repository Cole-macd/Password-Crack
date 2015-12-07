#include "encrypt_decrypt.h"

void handleErrors(void)
{
  ERR_print_errors_fp(stderr);
  abort();
}

int encrypt_digest(unsigned char *plaintext, int plaintext_len, unsigned char *hash, int hash_len, const EVP_MD *digest_encryption_algorithm){
  
  EVP_MD_CTX ctx;
  EVP_MD_CTX_init(&ctx);
  EVP_DigestInit_ex(&ctx, digest_encryption_algorithm, NULL);
  EVP_DigestUpdate(&ctx, plaintext, plaintext_len);
  EVP_DigestFinal_ex(&ctx, hash, &hash_len);
  EVP_MD_CTX_cleanup(&ctx);
  return 0;

}

int encrypt_cipher(unsigned char *plaintext, int plaintext_len, unsigned char *key,
  unsigned char *iv, unsigned char *ciphertext, const EVP_CIPHER *cipher_encryption_algorithm)
{
  
  int len;
  int ciphertext_len;
  
    EVP_CIPHER_CTX *ctx;
    // Create and initialise the context 
    if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

    // Initialise the encryption operation.
    if(1 != EVP_EncryptInit_ex(ctx, cipher_encryption_algorithm, NULL, key, iv))
      handleErrors();
    
    // Provide the message to be encrypted, and obtain the encrypted output.
    if(1 != EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len))
      handleErrors();
    ciphertext_len = len;

    // Finalize Encryption
    if(1 != EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) handleErrors();
    ciphertext_len += len;

    // Clean up 
    EVP_CIPHER_CTX_free(ctx);

  return ciphertext_len;
}

int decrypt_cipher(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
  unsigned char *iv, unsigned char *plaintext, const EVP_CIPHER *cipher_encryption_algorithm)
{

  EVP_CIPHER_CTX *ctx;
  int len, plaintext_len;

  //Create and initialise the context 
  if(!(ctx = EVP_CIPHER_CTX_new())) handleErrors();

  //Initialise the decryption operation
  if(1 != EVP_DecryptInit_ex(ctx, cipher_encryption_algorithm, NULL, key, iv))
       handleErrors();

  // Provide the message to be decrypted, and obtain the plaintext output.
  if(1 != EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len))
    handleErrors();
  plaintext_len = len;

  // Finalise the decryption  
  if(1 != EVP_DecryptFinal_ex(ctx, plaintext + len, &len)) handleErrors();
  plaintext_len += len;

  // Clean up 
  EVP_CIPHER_CTX_free(ctx);

  return plaintext_len;
}
