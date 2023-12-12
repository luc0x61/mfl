/*---------------------------------------------------------------*//**
 * \file    mfl.c
 *          
 * \author  luc0x61
 * \version 1.0
 * \date    December 2023
 *
 * This is free and unencumbered software released into the public domain.
 */
#include  "mfl.h"

/*  
  Median filter algorithm - Background

  Given a stream of sample data, starting with zero sample history, it
  builds up a local circular buffer of incoming data.
  The data has to be sorted, to access the median value; however, as the 
  filter data builds up as samples are coming, even a tailored and humble 
  bubble sort can do the trick with a repeatable O(N) complexity.
  Sorting and retrieving is supported by an array of indexes to incoming 
  data. An insertion consists then of:
   - removal of the oldest value (if any) / substitution with new value
   - traversal of the ordered indexes to find a positioning for the new 
     value
   - reorder of indexes
  These purposes can be achieved (with a simple algorithm) in a single 
  pass.
  Take as an example a size FILT_SIZE == 13, and the following input
  data (note: 16 values)
    { 7,   12,    0,    7,    8,   13,   12,   12,
      9,   11,   17,   10,    7,    9,    9,    9, }
  After the 13th sample new data will start overwriting older one, 
  ending with the input sorted as follows:
   item  value  buffer_index
     0. (   7 @ 3)
     1. (   7 @ 12)
     2. (   8 @ 4)
     3. (   9 @ 8)
     4. (   9 @ 0)
     5. (   9 @ 1)
     6. (   9 @ 2) <
     7. (  10 @ 11)
     8. (  11 @ 9)
     9. (  12 @ 7)
    10. (  12 @ 6)
    11. (  13 @ 5)
    12. (  17 @ 10)
  with a median value from last mfs() = 9 in sort order 6
 */

#define  FILT_SIZE  13          // Filter size

/*---------------------------------------------------------------*//**
 *          Median filter
 * 
 * \note  This function uses static data and *no* initialization
 *        method (i.e. can't be /restarted/), this is by design.
 *
 * \param   sample      Input sample
 * \return  Sample_t    Output median value
 */
Sample_t    mfs(Sample_t sample)
{
    static Sample_t data[FILT_SIZE] = { 0, };  // Input data circular buffer
    static int      ndex[FILT_SIZE] = { 0, };  // Sorted data indexes
    static int      x_ins           = 0;       // Current insertion point
    static int      d_size          = 0;       // Current data size

    int xs = 0;                 // Sorted position
    int ir = 0;                 // Reading position
    int iw = 0;                 // Writing position

    data[x_ins] = sample;       // Sample at insertion point
    xs = x_ins;                 // Index to sort: the added one

    if (d_size < FILT_SIZE)     // Buffer not yet full
    {
        ndex[x_ins] = x_ins;    // Initial indexing
        d_size++;
    }

    do
    {
        // Reading value skips the just inserted one
        if (ndex[ir] == x_ins)
        {
            ir++;   // Skip current reading position
        }
        else
        {
            // Bubble sort a single insertion value
            if (data[xs] < data[ndex[ir]])
            {
                // Swap, exchange xs <=> ndex[i]  with separate indexes 
                // for read and write position, as ir >= iw, giving
                // xs0 := ndex[ir], ndex[iw] := xs, xs := xs0
                int xs0  = ndex[ir];
                ndex[iw] = xs;
                xs = xs0;
            }
            else
            {
                // Assure write position := read position
                ndex[iw] = ndex[ir];
            }
            ir++;   // Next reading position
            iw++;   // Next writing position
        }
    } while (ir < d_size);

    ndex[iw] = xs;  // Assign last sorted position

#ifdef  DEBUG_MF_BUFFER
    for (int j = 0; j < d_size; j++)
    {
        printf("  %4d. (%4d @ %d) %c\n",
               j, data[ndex[j]], ndex[j], ndex[j] == x_ins ? '<' : ' ');
    }
#endif  // DEBUG_MF_BUFFER

    // Advance with next insertion in circular buffer
    if (++x_ins >= FILT_SIZE)
    {
        x_ins = 0;
    }

    // Median value: simplified at buffer midpoint
    // Correct for an odd number of samples, approximated for even ones
    return  data[ndex[d_size / 2]];
    // Median value: mathematically correct with any number of samples
    // int x_med_lt = ndex[(d_size - 1) / 2];
    // int x_med_rt = ndex[d_size / 2];
    // return  (Sample_t)((data[x_med_lt] + data[x_med_rt]) / 2);
}

