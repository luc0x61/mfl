# mfl
A median filter algorithm

## Background
Some time ago, I found an old article on Embdedded.com by Phil Ekstrom, [Better Than Average](https://www.embedded.com/better-than-average/), describing a method for a fast median filter.
However, the original article had a few flaws in the proposed code that made it unpractical.
It was amended on Embeddedgurus.com by Nigel Jones, [Median filtering](https://embeddedgurus.com/stack-overflow/2010/10/median-filtering/), eventually with fully working code.
Still it didn't convince me for a couple of reasons:
- the code looked too complex for the purpose (was it possible to shorten it somehow?)
- the chaining system uses an input value as a marker (could I use all the values represented by the input type as sample data?)

## A different approach
I worked then from scratch, still trying to create a fast algorithm, improving the aspects I liked the least in the original setting.
The result was enough satisfying my intentions, then I thought to share it.
