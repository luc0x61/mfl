/*---------------------------------------------------------------*//**
 *          Median filter
 * 
 * This is marked with CC0 1.0. To view a copy of this license, 
 * visit http://creativecommons.org/publicdomain/zero/1.0 
 */
#ifndef  MFL_H
#define  MFL_H

#ifdef  _DEBUG
//  Just a debug feature: for each input data the input buffer is 
//  printed out
#define  DEBUG_MF_BUFFER
#endif


typedef int     Sample_t;       // Sample type, numerical

extern Sample_t    mfs(Sample_t sample);

#endif  // MFL_H
