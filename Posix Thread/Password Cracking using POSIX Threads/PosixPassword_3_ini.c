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
    gcc -o PosixPassword3 PosixPassword_3_ini.c -lcrypt

  If you want to analyse the results then use the redirection operator
to send
  output to a file that you can view using an editor or the less
utility:

    ./PosixPassword3 > results.txt

  Dr Kevan Buckley, University of Wolverhampton, 2018
************************************************************************
******/
int n_passwords = 4;
// encrypted password hash function in array
char *encrypted_passwords[] = {
    "$6$KB$k3KYGaqaSW9LW6Ujzaw3BDPWxXRDHCdC8wWaAmpuTVad/VXKrTY0orC2G2AG.TWGVoUTFFAlh8z4BreF01O5S/",

    "$6$KB$NlinjoyviPEYXm2/71xGdTe4X62c5iifguAP4YcZDZZwk/UDlpgI2vvCKql.LctiA.rG5wbnlFtnjQtnbsbn60",

    "$6$KB$Z.kSGv3Dfc2uMHhtDLFGhEdN0l2EMSFFAdiQIFXIgFJ8mBIgu8rnDyjLnu53FSR3o5qoegEJwzPs4O4PIGYyU1",

    "$6$KB$TJZXPcjgmdaYOy7d3fXqJTJ7H46yEXDw9U2LnYLuKveyzy4vjlbH48t6wq9AwC4JeTGwfxIyIoP3E4LWBn9Cu0"

};

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

void crack(char *salt_and_encrypted){
    int i1, i2, i3, digit;     // Loop counters
    char salt[7];    // String used in hashing the password. Need space
    char plain[7];   // The combination of letters currently being checked
    char *enc;       // Pointer to the encrypted password
    int count = 0;   // The number of combinations explored so far

    substr(salt, salt_and_encrypted, 0, 6);

    for(i1='A'; i1<='Z'; i1++){ // loop for first initial 
        for(i2='A'; i2<='Z'; i2++){// loop for second initial
            for(i3='A'; i3<='Z'; i3++){ // loop for third initial 
                for(digit=0; digit<=99; digit++){ //loop for last 2 digits 00 to 99
                    sprintf(plain, "%c%c%c%02d", i1, i2, i3, digit);
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

    }

    printf("%d solutions explored\n", count);
}
//time calculation fucnction
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
