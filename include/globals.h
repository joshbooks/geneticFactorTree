#ifndef GLOBALS_INCLUDED
#define GLOBALS_INCLUDED

#include <gmp.h>
#include <iostream>
#include <string>


const unsigned int popSize = 256;
/**
arrayLength indicates the size of the array for the factorTree
it is calculated by the main method based on the size of the
number to be factored
**/
extern int arrayLength;
/**
mutationOdds determines the odds of, oh you guessed it already,
ok, well for the sake of complete documentation, the odds that
an individual will mutate in any given round of selection
**/
extern double mutationOdds;
/**
The amount that an individual will mutate if it does mutate
my intuition says that this will vastly affect the performance
and behavior so look into this
**/
extern long int mutationAmount;

extern mpz_t goal;

/**
Specifies the probablility that, when coalescing, a given pair of
factors will be unified
**/
extern double unifyOdds;

/**
Specifies the probability that, when a population is going through a generation,
a given FactorTree will have its coalesce method called
**/
extern double coalesceOdds;

/**
Specifies the degree to which partner selection is determined by fitness
of the partner
**/
extern double selectivePressure;
//so what I'm imagining is a custom integer distribution
//with higher probability of selecting other fit factorTrees
//the degree to which these fit individulas are selected
//is determined by selectivePressure
//as it turns out this is nicely taken care of by
//discrete_distribution



#endif // GLOBALS_INCLUDED
