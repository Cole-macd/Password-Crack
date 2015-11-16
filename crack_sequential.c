#include <stdio.h>
#include <stdlib.h>
#include "string.h"

#define MIN_LENGTH 1
#define MAX_LENGTH 3
#define NUM_VALID_CHARS 10 //62

void getFirstString(char *string, int length);
void isMatch(char *attempted_string);
void getStringForValues(char *string, int *values, int length);
void incrementValues(int *values, int current_length);
void getFirstValues(int *values, int length);

int number_of_passwords, rank, num_processes;
char valid_chars[] = "0123456789";//abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, char *argv[]) {

	// Brute Force Loop
	char *attempted_string;
	int *current_values;
	int current_length;
	int total_permutations;
	for (current_length = MIN_LENGTH; current_length <= MAX_LENGTH; current_length++) {
		attempted_string = (char*)malloc(current_length * sizeof(char));
		current_values = malloc(current_length * sizeof(int));
		getFirstValues(current_values, current_length);

		int total_permutations = 1;

		int i, j;
		for (i = 1; i < current_length + 1; i++) {
			total_permutations = total_permutations * NUM_VALID_CHARS;
		}

		printf("total permutations are %d\n", total_permutations);

		for (i = 0; i < total_permutations; i++) {
			getStringForValues(attempted_string, current_values, current_length);
			isMatch(attempted_string);
			incrementValues(current_values, current_length);
		}
	}
}

void getFirstValues(int *values, int length) {
	char first_values[length];
	
	int i;
	for (i = 0; i < length; i++) {
		values[i] = 0;
	}
}

void getStringForValues(char *string, int *values, int length) {
	char new_string[length];
	int i;

	for (i = 0; i < length; i++) {
		new_string[i] = valid_chars[values[i]];
		printf("%c ", new_string[i]);
	}
	printf("\n");

	strcpy(string, new_string);
}

void incrementValues(int *values, int current_length) {
	int i;
	for (i = 0; i < current_length; i++) {
		if (values[i] + 1 < NUM_VALID_CHARS) {
			break;
		}
		if (values[i] + 1 == NUM_VALID_CHARS) {
			values[i] = 0;
		}
	}
	if (i != current_length) values[i]++;
}

void isMatch(char *attempted_string) {//, char *hashed_password) {
	/*char *hashed_attempt = hash(attempted_string);
	 *
	 *if (hashed_attempt == hashed_password) return 1;
	 *return 0;
	 */
	//printf("%s\n", attempted_string);
}
/*
char ** getHashedPasswords(char *unhashed_strings_file_name) {

}
*/

