all: crack_pthread crack_sequential generate_hashes

crack_pthread: crack_pthread.c
	gcc crack_pthread.c encrypt_decrypt.c encrypt_passwords.c -o crack_pthread -lpthread -lcrypto -lssl

crack_sequential: crack_sequential.c
	gcc crack_sequential.c encrypt_decrypt.c encrypt_passwords.c -o crack_sequential -lcrypto -lssl

generate_passwords: generate_passwords.c
	gcc generate_passwords.c -o generate_passwords 

generate_hashes: generate_hashes.c
	gcc encrypt_decrypt.c generate_hashes.c encrypt_passwords.c -o generate_hashes -lcrypto -lssl

generate_hashes_test: generate_hashes_test.c
	gcc encrypt_decrypt.c generate_hashes_test.c -o generate_hashes_test -lcrypto -lssl

clean: 
	rm -rf crack_pthread
	rm -rf crack_sequential
	rm -rf generate_hashes
	rm -rf generate_hashes_test
