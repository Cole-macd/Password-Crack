#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "string.h"

#define MIN_LENGTH 4
#define MAX_LENGTH 6
#define NUM_VALID_CHARS 62
#define FILENAME "Boobies.txt"

void getFirstString(char *string, int length);
int isMatch(char *attempted_string, char *next_hash);
void getStringForValues(char *string, int *values, int length);
void incrementValues(int *values, int current_length);
void getFirstValues(int *values, int length);
void getNextHash(char *filename, char *next_hash, int current_password_index);
int getNumberOfPasswords(char *filename);
void allocatePasswordList(int length);
void writeToFile();

int num_processes, number_of_passwords;
int global_current_password;
pthread_rwlock_t password_lock;

char **found_passwords;
char valid_chars[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

void *worker(void *arg) {
	int rank = (int) arg;

	int current_password_index;
	int copy_global_current_password;
        int found_match;
        char *attempted_string;
        int *current_values;
        int current_length;
        int total_permutations;
        char *next_hash;

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

                        printf("total permutations are %lu for length %d\nprocess %d trying to find hash %s\n", total_permutations, current_length, rank, next_hash);

                        unsigned long cur;
                        for (cur = rank; cur < total_permutations; cur++) {
				// Get reader lock
				//pthread_rwlock_rdlock(&password_lock);
				//copy_global_current_password = global_current_password;
				//pthread_rwlock_unlock(&password_lock);

				if (global_current_password > current_password_index) {
					found_match = 1;					
					break;
				}

				if (strncmp(attempted_string, "2398", 4) == 0) {
					printf("process %d string %s\n", rank, attempted_string);
				}

                                getStringForValues(attempted_string, current_values, current_length);

                                found_match = isMatch(attempted_string, next_hash);
                                if (found_match == 1) {
					// Get writer lock
					pthread_rwlock_wrlock(&password_lock);
					global_current_password++;
					strncpy(found_passwords[current_password_index], attempted_string, current_length);
					pthread_rwlock_unlock(&password_lock);
					printf("process %d found %s\n", rank, next_hash);
                                        break;
                                }

                                incrementValues(current_values, current_length);
                        }
                        free(attempted_string);
                        if (found_match == 1) {
                                break;
                        }
                }
        }
	pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
	global_current_password = 0;
        number_of_passwords = getNumberOfPasswords(FILENAME);
	printf("number of passwords is %d\n", number_of_passwords);
	allocatePasswordList(number_of_passwords);

	num_processes = atoi(argv[1]);

    	pthread_t *threads = malloc(num_processes * sizeof(pthread_t));

	printf("Thread count is %d\n", num_processes);
	if (pthread_rwlock_init(&password_lock, NULL) != 0) {
		printf("Error, init rwlock\n");
		exit(1);
	}

	int i, s;
	for (i = 0; i < num_processes; i++) {
		s = pthread_create(&threads[i], NULL, worker, (void *) i);
		if (s != 0) {
			printf("Error, creating threads\n");
	    		exit(1);
		}
	}

	for (i = 0; i < num_processes; i++) {
		s = pthread_join(threads[i], NULL);
		if (s != 0) {
	    		printf("Error, joining threads\n");
	    		exit(1);
		}
	}

	pthread_rwlock_destroy(&password_lock);
	writeToFile();

}

void writeToFile() {
        FILE *output_file;
        int i;
        output_file = fopen("passwords.txt","w");
        for (i = 0; i < number_of_passwords; i++) {
		printf("writing %s to file\n", found_passwords[i]);
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

int isMatch(char *attempted_string, char *next_hash) {
        /*char *hashed_attempt = hash(attempted_string);
         *
         *if (hashed_attempt == hashed_password) return 1;
         *return 0;
         */
        if (strncmp(attempted_string, next_hash, strlen(next_hash)) == 0) {
                //printf("Found %s\n", attempted_string);
                return 1;
        }

        return 0;
}
