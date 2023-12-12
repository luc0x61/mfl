/*---------------------------------------------------------------*//**
 * \file    main.c
 *    
 * Simple median filter test set
 * 
 * \author  luc0x61
 * \version 1.0
 * \date    December 2023
 * 
 * This is free and unencumbered software released into the public domain.
 */
#include  <stdio.h>
#include  <stdlib.h>
#include  <math.h>
#include  <time.h>
#include  <assert.h>

// Testing function from "median_filter.c"
#include  <stdint.h>
extern  uint16_t median_filter(uint16_t datum);

// Testing function from "mfl.c"
#include  "mfl.h"


/*---------------------------------------------------------------*//**
 *          Generate some Gaussian noise value
 * 
 * \param   mean 
 * \param   stdDev 
 * \return  double 
 */
static double generateGaussian(double mean, double stdDev) 
{
    static double spare;
    static int hasSpare = 0;

    if (hasSpare) {
        hasSpare = 0;
        return spare * stdDev + mean;
    }
    else {
        double u, v, s, f;
        do {
            u = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
            v = (rand() / ((double)RAND_MAX)) * 2.0 - 1.0;
            s = u * u + v * v;
        } while (s >= 1.0 || s == 0.0);
        f = sqrt(-2.0 * log(s) / s);
        spare = v * f;
        hasSpare = 1;
        return mean + stdDev * u * f;
    }
}


/*---------------------------------------------------------------*//**
 *          Main test 
 * 
 * \return  int 
 */
int main(void)
{
    int     i;
    int     samples[100];

    //  1. Check for equivalent behavior on a small sample buffer

    for (i = 0; i < 100; i++)
    {
        int mf_value1, mf_value2;
        int sample;

        sample = (int)generateGaussian(10, 4) % 20;
        printf("\n");
        printf("[%d] -> sample = %d\n", i, sample);

        samples[i] = sample;

        //  Current implementation of median filter
        mf_value1 = mfs(sample);
        printf("mfs()           = %d\n", mf_value1);

        //  Referenced implementation of median filter
        mf_value2 = median_filter(sample);
        printf("median_filter() = %d\n", mf_value2);

        assert(mf_value1 == mf_value2);
    }
    
    printf("\n\nSamples = { \n");
    for (i = 0; i < 100; )
    {
        printf("%4d, ", samples[i]);
        if (++i % 8 == 0)   putchar('\n');
    }
    printf("\n};\n");


    //  2. Timing test (if enabled)

#if !defined(DEBUG_MF_BUFFER)
    //  During debug don't include the heavy test!
    clock_t     t0, t1;
    double      dt;
#define  TIME_LOOP      100000000
    
    //  2.a. Current implementation of median filter
    //  Use always the same pseudo-random stream
    srand(0);
    t0 = clock();
    for (i = 0; i < TIME_LOOP; i++)
    {
        //  Note: same data as below cycle
        (void)mfs(rand() | 0x0001);
    }
    t1 = clock();
    dt = (double)(t1 - t0) / CLOCKS_PER_SEC;
    printf("mfs() time: %fs\n", dt);

    //  2.b. Referenced implementation of median filter
    //  Use always the same pseudo-random stream
    srand(0);
    t0 = clock();
    for (i = 0; i < TIME_LOOP; i++)
    {
        //  Note: data made always != 0, as zero is an 
        //  unacceptable special case for the original filter (!)
        (void)median_filter(rand() | 0x0001);
    }
    t1 = clock();
    dt = (double)(t1 - t0) / CLOCKS_PER_SEC;
    printf("median_filter() time: %fs\n", dt);

#endif // !defined(DEBUG_MF_BUFFER)

    return  0;
}


