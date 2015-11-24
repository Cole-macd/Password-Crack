all: crack_mpi crack_sequential generate_hashes

crack_mpi: crack_mpi.c
	mpicc crack_mpi.c -o crack_mpi

crack_sequential: crack_sequential.c
	gcc crack_sequential.c -o crack_sequential

generate_passwords: generate_passwords.c
	gcc generate_passwords.c -o generate_passwords 

generate_hashes: generate_hashes.c
	gcc encrypt_decrypt.c generate_hashes.c -o generate_hashes -lcrypto -lssl

generate_hashes_test: generate_hashes_test.c
	gcc encrypt_decrypt.c generate_hashes_test.c -o generate_hashes_test -lcrypto -lssl

generate_hashes_test: generate_hashes_test.c
	gcc encrypt_decrypt.c generate_hashes_test.c -o crypto -lcrypto -lssl
clean: 
	rm -rf crack_mpi
	rm -rf crack_sequential
	rm -rf generate_hashes
