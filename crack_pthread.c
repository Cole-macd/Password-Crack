#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
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
void getFirstValues(int *values, int length, int rank);
void getNextHash(char *filename, char *next_hash, int current_password_index);
int getNumberOfPasswords(char *filename);
void allocatePasswordList(int length);
void writeToFile();

int num_processes, number_of_passwords;
int global_current_password;
pthread_rwlock_t password_lock;

char **found_passwords;
char valid_chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";//"0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

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
            		getFirstValues(current_values, current_length, rank);
	
            		unsigned long total_permutations = 1;
	
            		int i, j;
            		for (i = 1; i < current_length + 1; i++) {
            		        total_permutations = total_permutations * NUM_VALID_CHARS;
            		}

            		printf("length %d\nprocess %d trying to find hash %s\n", current_length, rank, next_hash);

            		unsigned long cur;
            		for (cur = rank; cur < total_permutations; cur++) {
				// TODO: Figure out a way to lock read without slowing down?
                		// Get reader lock
                		//pthread_rwlock_rdlock(&password_lock);
                		//copy_global_current_password = global_current_password;
                		//pthread_rwlock_unlock(&password_lock);
	
	                	if (global_current_password > current_password_index) {	
                	    		found_match = 1;                    
                	    		break;
                		}
		
		                getStringForValues(attempted_string, current_values, current_length);
		
		                found_match = isMatch(attempted_string, next_hash, current_length);
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
	clock_t start = clock(), diff;

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

	diff = clock() - start;
	int msec = diff * 1000 / CLOCKS_PER_SEC;
	printf("Time taken %d seconds %d milliseconds\n", msec/1000, msec%1000);
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

void getFirstValues(int *values, int length, int offset) {
        char first_values[length];

        int i;
        for (i = 0; i < length; i++) {
                if (offset >= NUM_VALID_CHARS) {
                    values[i] = NUM_VALID_CHARS - 1;
                    offset -= NUM_VALID_CHARS;
                } else {
                    values[i] = offset;
                }
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
        int i = 0;
        int offset = num_processes;

        for (i = 0; i < current_length; i++) {
                if (values[i] + offset < NUM_VALID_CHARS) {
                        break;
                }

                if (values[i] + offset % NUM_VALID_CHARS < NUM_VALID_CHARS) {
                        values[i] += offset % NUM_VALID_CHARS;
                        offset = offset / NUM_VALID_CHARS;
                        continue;
                } else {
                    values[i] = (offset + values[i]) % NUM_VALID_CHARS;
                    offset = offset / NUM_VALID_CHARS + 1;
                }
        }

        if (i < current_length) values[i] += offset;
}

int isMatch(char *attempted_string, char *next_hash, int length) {
        char *attempted_hash = (char*)malloc(strlen(next_hash) * sizeof(char));
	encrypt_md5(attempted_string, attempted_hash, length);
	int to_return = 0;

	if (strncmp(attempted_string, "HCDAXH", 6) == 0) {
		printf("%s length %d hash is %s\n", attempted_string, length, attempted_hash);
	}
	//printf("%s\n", attempted_hash);

	if (strncmp(attempted_hash, next_hash, strlen(next_hash)) == 0) {
		to_return = 1;
	}

	free(attempted_hash);
	return to_return;
}
