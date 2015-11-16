#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

#define MIN_LENGTH 5
#define MAX_LENGTH 7
#define NUM_ALPHANUMERICS 62

int number_of_passwords, rank, num_processes;

int main(int argc, char *argv[]) {
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &num_processes);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// Brute Force Loop
	int i;
	for (i = rank; i < (NUM_ALPHANUMERICS * MAX_LENGTH) - (NUM_ALPHANUMERICS * MIN_LENGTH); i += num_processes) {

	}

	MPI_Finalize();
}

int isMatch(char *attempted_string, char *hashed_password) {
	/*char *hashed_attempt = hash(attempted_string);
	 *
	 *if (hashed_attempt == hashed_password) return 1;
	 *return 0;
	 */
}

char ** getHashedPasswords(char *unhashed_strings_file_name) {

}


