#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <math.h>
/***********************************************************************
*******
  Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm. Works on passwords that consist only of 2
uppercase
  letters and a 2 digit integer. Your personalised data set is included
in the
  code. 

  Compile with:
    gcc -o PosixPassword PosixPassword.c -lcrypt

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./PosixPassword > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;

// encrypted passsword using crypt function with salt..
char *encrypted_passwords[] = {

	"$6$KB$LmvMGhIR4PnYKlPXAOo2K4zt.BEjUN/9fKPMYT1zyisqo4WxY.FKfiCGz15GAoyNV5dW3GP.uQSiIaBQ3zxgo0",

	"$6$KB$7rLS8BU8lh76q9iZ3Ogb8w1G45hmJUMoHdmOyHuQFUBqyr7XnEMUEs2wF4xGJRgQob7nC/RD9e1AKQZr/CKI30",

	"$6$KB$.Qkp7HQ5GAeBt446HV4LjAiZinuF6Wip1EcW31dYak0s.sFZxmpD4bEnpD9BjjAS9vcuC51xqDMG80I8evnyL/",

	"$6$KB$jM4o2O3EJI9OCoHvf8Jo0YG4JcnwEPFqpJINXb4RGEahSL5JRIQt1s2djLbGHThVv9IGzrYsS18XICkn5074./"
};
void substr(char *dest, char *src, int start, int length){
	memcpy(dest, src + start, length);
	*(dest + length) = '\0';
}
void crack(char *salt_and_encrypted){
  int i1, i2, i3;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(i1='A'; i1<='Z'; i1++){  //iterates over first intiital 
    for(i2='A'; i2<='Z'; i2++){   //iterates over second initial 
      for(i3=0; i3<=99; i3++){      //iterates over last two initials 00 to 99
      	sprintf(plain, "%c%c%02d", i1, i2, i3); 
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
  printf("%d solutions explored\n", count); //counts total solution explored
}
//time difference code 
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
//main function 
int main(int argc, char *argv[]){
	int i;
	struct  timespec start, finish;
	long long int time_elapsed;
	clock_gettime(CLOCK_MONOTONIC, &start);

	for(i=0;i<n_passwords;i<i++) {
		crack(encrypted_passwords[i]);
	}

	clock_gettime(CLOCK_MONOTONIC, &finish);
	time_difference(&start, &finish, &time_elapsed);
	printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,
		(time_elapsed/1.0e9)); 
	return 0;
}
