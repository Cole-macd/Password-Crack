#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "string.h"
#include "encrypt_passwords.h"

#define MIN_LENGTH 6
#define MAX_LENGTH 6
#define NUM_VALID_CHARS 26//62
#define FILENAME "Boobies.txt"

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
	clock_t start = clock(), diff;

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

		// Brute Force Loop
		for (current_length = MIN_LENGTH; current_length <= MAX_LENGTH; current_length++) {
			attempted_string = malloc(current_length * sizeof(char));
			current_values = malloc(current_length * sizeof(int));
			getFirstValues(current_values, current_length);
	
			unsigned long total_permutations = 1;
	
			int i, j;
			for (i = 1; i < current_length + 1; i++) {
				total_permutations = total_permutations * NUM_VALID_CHARS;
			}
	
			printf("total permutations are %lu for length %d\n", total_permutations, current_length);
			printf("trying to find hash %s\n", next_hash);

			unsigned long cur;
			for (cur = 0; cur < total_permutations; cur++) {
				getStringForValues(attempted_string, current_values, current_length);
				
				found_match = isMatch(attempted_string, next_hash, current_length);
				if (found_match == 1) {
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
	diff = clock() - start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
}

void writeToFile() {
        FILE *output_file;
        int i;
        output_file = fopen("passwords.txt","w");
        for (i = 0; i < number_of_passwords; i++) {
                fprintf(output_file, "%s\n", found_passwords[i]);
        }

        fclose(output_file);
}

void allocatePasswordList(int length) {
	found_passwords = malloc(length * sizeof(char*));
	int i;
	for (i = 0; i < length; i++) {
		found_passwords[i] = (char*)malloc(MAX_LENGTH * sizeof(char));
	}
}

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
	}

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

int isMatch(char *attempted_string, char *next_hash, int length) {
	char *attempted_hash = malloc(strlen(next_hash) * sizeof(char));
	encrypt_md5(attempted_string, attempted_hash, length);
	int to_return = 0;

	if (strncmp(attempted_string, "HCDAXH", 6) == 0) {
		printf("%s length %d hash is %s\n", attempted_string, length, attempted_hash);
	}

	if (strncmp(attempted_hash, next_hash, strlen(next_hash)) == 0) {
		to_return = 1;
	}

	free(attempted_hash);
	return to_return;
}

