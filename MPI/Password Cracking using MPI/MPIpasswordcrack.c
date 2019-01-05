#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

int n_passwords = 4;

char *encrypted_passwords[] = {

	"$6$KB$LmvMGhIR4PnYKlPXAOo2K4zt.BEjUN/9fKPMYT1zyisqo4WxY.FKfiCGz15GAoyNV5dW3GP.uQSiIaBQ3zxgo0",

	"$6$KB$7rLS8BU8lh76q9iZ3Ogb8w1G45hmJUMoHdmOyHuQFUBqyr7XnEMUEs2wF4xGJRgQob7nC/RD9e1AKQZr/CKI30",

	"$6$KB$.Qkp7HQ5GAeBt446HV4LjAiZinuF6Wip1EcW31dYak0s.sFZxmpD4bEnpD9BjjAS9vcuC51xqDMG80I8evnyL/",

	"$6$KB$jM4o2O3EJI9OCoHvf8Jo0YG4JcnwEPFqpJINXb4RGEahSL5JRIQt1s2djLbGHThVv9IGzrYsS18XICkn5074./"
};


/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
	memcpy(dest, src + start, length);
	*(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All
combinations
 that are tried are displayed and when the password is found, #, is put
at the 
 start of the line. Note that one of the most time consuming operations
that 
 it performs is the output of intermediate results, so performance
experiments 
 for this kind of program should not include this. i.e. comment out the
printfs.
*/

void function_1(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='A'; x<='M'; x++){
  	for(y='A'; y<='Z'; y++){
  		for(z=0; z<=99; z++){
  			printf("Instance 1");
  			sprintf(plain, "%c%c%02d", x, y, z); 
  			enc = (char *) crypt(plain, salt);
  			count++;
  			if(strcmp(salt_and_encrypted, enc) == 0){
  				printf("#%-8d%s %s\n", count, plain, enc);
  			} else {
  				printf(" %-8d%s %s\n", count, plain, enc);
  			}
  		}
  	}
  }
  printf("%d solutions explored\n", count);
}

void function_2(char *salt_and_encrypted){
  int x, y, z;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space

  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(x='N'; x<='Z'; x++){
  	for(y='A'; y<='Z'; y++){
  		for(z=0; z<=99; z++){
  			printf("Instance 2");
  			sprintf(plain, "%c%c%02d", x, y, z); 
  			enc = (char *) crypt(plain, salt);
  			count++;
  			if(strcmp(salt_and_encrypted, enc) == 0){
  				printf("#%-8d%s %s\n", count, plain, enc);
  			} else {
  				printf(" %-8d%s %s\n", count, plain, enc);
  			}
  		}
  	}
  }
  printf("%d solutions explored\n", count);
}

int time_difference(struct timespec *start, 
	struct timespec *finish, 
	long long int *difference) {
	long long int ds =  finish->tv_sec - start->tv_sec; 
	long long int dn =  finish->tv_nsec - start->tv_nsec; 

	if(dn < 0 ) {
		ds--;
		dn += 1000000000; 
	} 
	*difference = ds * 1000000000 + dn;
	return !(*difference > 0);
}

int main(int argc, char *argv[]){
  //int account = 0;
	int size, rank;

	struct  timespec start, finish;
	long long int time_elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);

	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	if(size != 3) {
		if(rank == 0) {
			printf("This program needs to run on exactly 3 processes\n");
		}
	} else {
		if(rank ==0){
			int x;
			int y;
			int i;
			MPI_Send(&x, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
			MPI_Send(&y, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);

		} else {
			if(rank == 1){
				int i;
				int number = rank + 10;
				MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				for ( i = 0; i<n_passwords;i<i++){
					function_1(encrypted_passwords[i]);
				}
			}
			else if(rank == 2){
				int i;
				int number = rank + 10;
				MPI_Recv(&number, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
				for ( i = 0; i<n_passwords;i<i++){
					function_2(encrypted_passwords[i]);
				}
			}
		}
	}
	MPI_Finalize(); 

	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_difference(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		(time_elapsed/1.0e9)); 

	return 0;
}


