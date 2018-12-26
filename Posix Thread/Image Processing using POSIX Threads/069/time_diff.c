#include <time.h>
#include <stdio.h>
#include <stdlib.h>
/****************************************************************************
  Functions for capturing and reporting times. Need to compile with -lrt 

  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

/* 
  Calculates the time elapsed between a start and a finish time.
  Returns 1 on failure or 0 on success. Fails if the start time is after the
  finish time. The result is stored in nano seconds
*/
 
int time_difference(struct timespec *start, struct timespec *finish, 
                    long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn +=            1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}


void capture_start_time(struct timespec *start){
  if ((clock_gettime(CLOCK_MONOTONIC, start)) != 0) { 
    fprintf(stderr, "start time could not be set\n");
    exit(EXIT_FAILURE);
  }
}

void capture_finish_time(struct timespec *finish){
  if (clock_gettime(CLOCK_MONOTONIC, finish) != 0) {
    fprintf(stderr,"finish time could not be set\n");
    exit(EXIT_FAILURE);
  }
}


