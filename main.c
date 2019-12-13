#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include <time.h>
#include <sys/time.h>

double getRand(unsigned int *seed) {
    return (double) rand_r(seed) * 2 / (double) (RAND_MAX) - 1;
}

long double Calculate_Pi_Sequential(long long number_of_tosses) {
    long long number_in_circle = 0;              
    unsigned int seed = (unsigned int) time(NULL);
    // generates random x,y cordinates for every toss
    for (long long int toss = 0; toss < number_of_tosses; toss++) {
        double x = getRand(&seed);                                   // random x gerator
        double y = getRand(&seed);                                   // radom y generator
        double distance_squared = x*x + y*y;
        if (distance_squared <= 1) number_in_circle++;
    }
    return 4 * number_in_circle/((double) number_of_tosses);
}

long double Calculate_Pi_Parallel(long long number_of_tosses) {             //holds the number of tosses 
    long long number_in_circle = 0;                                         //holds the number of point in the cirlce
#pragma omp parallel num_threads(20) //sets number of threads to 20
{
    unsigned int seed = (unsigned int) time(NULL) + (unsigned int) omp_get_thread_num();
    #pragma omp for reduction(+: number_in_circle)              //summing with the reduction clause and saving it number_in_cicle
     // generates random x,y cordinates for every toss
        for (long long int toss = 0; toss < number_of_tosses; toss++) {
            double x = getRand(&seed);                            //generates random points
            double y = getRand(&seed);
            double distance_squared = x*x + y*y;
            if (distance_squared <= 1) number_in_circle++;         //increameants numbers_in_cicle when disance squared is less than 1(when in the cicle
        }
    }
    return 4 * number_in_circle/((double) number_of_tosses);     //returns an estimated value of pi using 
}

int main() {
    struct timeval start, end;

    long long num_tosses = 10000000;

    printf("Timing sequential...\n");
    gettimeofday(&start, NULL);
    long double sequential_pi = Calculate_Pi_Sequential(num_tosses);
    gettimeofday(&end, NULL);
    printf("Took %f seconds\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    printf("Timing parallel...\n");
    gettimeofday(&start, NULL);
    long double parallel_pi = Calculate_Pi_Parallel(num_tosses);
    gettimeofday(&end, NULL);
    printf("Took %f seconds\n\n", end.tv_sec - start.tv_sec + (double) (end.tv_usec - start.tv_usec) / 1000000);

    // This will print the result to 10 decimal places
    printf("π = %.10Lf (sequential)\n", sequential_pi);
    printf("π = %.10Lf (parallel)\n", parallel_pi);

    return 0;
}
