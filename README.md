# mfl
A median filter algorithm

## Background
Some time ago, I found an old article on Embedded.com by Phil Ekstrom, [Better Than Average](https://www.embedded.com/better-than-average/), describing a method for a fast median filter.
However, the original article had a few flaws in the proposed code that made it impractical.
It was amended on Embeddedgurus.com by Nigel Jones, [Median filtering](https://embeddedgurus.com/stack-overflow/2010/10/median-filtering/), eventually with fully working code.

Still, it didn't convince me for a couple of reasons:
- the code looked too complex for the purpose (was it possible to shorten it somehow?)
- the chaining system uses an input value as a marker (could I use all the values represented by the input type as sample data?)

## A different approach
I worked then from scratch, still trying to create a fast algorithm, improving the aspects I liked the least in the original setting.
The result was enough to satisfy my intentions, so I thought to share it.

## Test code
I run the test code in two parts. The first one is to verify that the new code gives the same results as the original algorithm.
The second test is a speed check, which can result even better than the original.

The source code has been compiled in Windows with a few compilers.
On **MSVC** (Visual Studio) I just created a project and put everything in it.
With **GCC** and **Clang** you can simply launch
```
  gcc -Wall -Wextra -O2 *.c -o mf.exe
```
  
