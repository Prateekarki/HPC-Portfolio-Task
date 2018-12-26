#include <time.h>

/****************************************************************************
  Functions for capturing and reporting times. Need to compile with -lrt 

  Dr Kevan Buckley, University of Wolverhampton, 2018
*****************************************************************************/

int time_difference(struct timespec *start, struct timespec *finish, long long int *difference) ;
void capture_start_time(struct timespec *start);
void capture_finish_time(struct timespec *finish);

