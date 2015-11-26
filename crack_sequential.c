#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "string.h"
#include "encrypt_passwords.h"

#define MIN_LENGTH 6
#define MAX_LENGTH 6
#define NUM_VALID_CHARS 26//62
#define FILENAME "Hashes.txt"

void getFirstString(char *string, int length);
int isMatch(char *attempted_string, char *next_hash, int length);
void getStringForValues(char *string, int *values, int length);
void incrementValues(int *values, int current_length);
void getFirstValues(int *values, int length);
void getNextHash(char *filename, char *next_hash, int current_password_index);
int getNumberOfPasswords(char *filename);
void allocatePasswordList(int length);
void writeToFile();

int number_of_passwords, rank, num_processes;
int current_password;

char **found_passwords;
char valid_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";//"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

int main(int argc, char *argv[]) {
	struct timeval tvBegin, tvEnd, tvDiff;
	gettimeofday(&tvBegin, NULL);

	number_of_passwords = getNumberOfPasswords(FILENAME);
	int current_password_index;
	int found_match;
	char *attempted_string;
	int *current_values;
	int current_length;
	int total_permutations;
	char *next_hash;

	allocatePasswordList(number_of_passwords);

	for (current_password_index = 0; current_password_index < number_of_passwords; current_password_index++) {
		found_match = 0;
		next_hash = (char*)malloc(MAX_LENGTH * sizeof(char));
		getNextHash(FILENAME, next_hash, current_password_index);

		/* Brute Force Loop */
		for (current_length = MIN_LENGTH; current_length <= MAX_LENGTH; current_length++) {
			attempted_string = malloc(current_length * sizeof(char));
			current_values = malloc(current_length * sizeof(int));
			getFirstValues(current_values, current_length);
	
			/* Calculate number of total permutations */
			unsigned long total_permutations = 1;
			int i, j;
			for (i = 1; i < current_length + 1; i++) {
				total_permutations = total_permutations * NUM_VALID_CHARS;
			}
	
			printf("total permutations are %lu for length %d\n", total_permutations, current_length);
			printf("trying to find hash %s\n", next_hash);

			/* Iterate through all permutations of strings with current_length */
			unsigned long current_permutation;
			for (current_permutation = 0; current_permutation < total_permutations; current_permutation++) {
				getStringForValues(attempted_string, current_values, current_length);
				
				found_match = isMatch(attempted_string, next_hash, current_length);
				if (found_match == 1) {
					/* Found the current hash, move to next */
					strncpy(found_passwords[current_password_index], attempted_string, current_length);
					printf("found %s\n", next_hash);
					break;
				}

				incrementValues(current_values, current_length);
			}
			free(attempted_string);
			free(current_values);
			if (found_match == 1) {
				break;
			}
		}
	}

	writeToFile();
	gettimeofday(&tvEnd, NULL);
	timevalSubtract(&tvDiff, &tvEnd, &tvBegin);
	printf("Time elapsed is %ld.%06ld\n", tvDiff.tv_sec, tvDiff.tv_usec);
}

/* Return 1 if the difference is negative, otherwise 0.  */
int timevalSubtract(struct timeval *result, struct timeval *t2, struct timeval *t1)
{
    	long int diff = (t2->tv_usec + 1000000 * t2->tv_sec) - (t1->tv_usec + 1000000 * t1->tv_sec);
    	result->tv_sec = diff / 1000000;
    	result->tv_usec = diff % 1000000;
	
	return (diff<0);
}

/* Write the found_passwords array to a file */
void writeToFile() {
        FILE *output_file;
        int i;
        output_file = fopen("passwords.txt","w");
        for (i = 0; i < number_of_passwords; i++) {
                fprintf(output_file, "%s\n", found_passwords[i]);
        }

        fclose(output_file);
}

/* Allocates memory for the found_passwords array */
void allocatePasswordList(int length) {
	found_passwords = malloc(length * sizeof(char*));
	int i;
	for (i = 0; i < length; i++) {
		found_passwords[i] = (char*)malloc(MAX_LENGTH * sizeof(char));
	}
}

/* Gets the number of passwords from a text file */
int getNumberOfPasswords(char *filename) {
	FILE *file;
	file = fopen(filename, "r");
	int rows = 0;
	char c;
	
	while(!feof(file)) {
		c = fgetc(file);
		if (c == '\n') {
			rows++;
		}
	}

	return rows;
}

/* Gets the next hash to crack from a text file */
void getNextHash(char *filename, char * next_hash, int index) {
	FILE *file;
	file = fopen(filename, "r");
	int row = 0;
	char c;

	while(!feof(file)) {
		if (row == index) {
			fscanf(file, "%s", next_hash);
			break;
		}
		c = fgetc(file);
		if (c == '\n') {
			row++;
		}
	}
} 

/* Initializes first set of values for a given length */
void getFirstValues(int *values, int length) {
	char first_values[length];
	
	int i;
	for (i = 0; i < length; i++) {
		values[i] = 0;
	}
}

/* Gets a string based on values array */
void getStringForValues(char *string, int *values, int length) {
	char new_string[length];
	int i;

	for (i = 0; i < length; i++) {
		new_string[i] = valid_chars[values[i]];
	}

	strcpy(string, new_string);
}

/* Increments values array for next permutation */
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

/* 
 * Hashes an attempted_string and checks it against next hash.
 * Returns 1 if there is a match
 */
int isMatch(char *attempted_string, char *next_hash, int length) {
	char *attempted_hash = malloc(strlen(next_hash) * sizeof(char));
	encryptMd5(attempted_string, attempted_hash, length);
	//encryptSha1(attempted_string, attempted_hash, length);
	//encryptAes256(attempetd_string, attempted_hash, length);
	int to_return = 0;

	if (strncmp(attempted_hash, next_hash, strlen(next_hash)) == 0) {
		to_return = 1;
	}

	free(attempted_hash);
	return to_return;
}

