#include "../include/FactorTree.h"
#include <random>
#include <chrono>
#include <limits>
#include <iostream>
#include <string>


FactorTree::FactorTree()
{
    factors = new mpz_t[arrayLength];
    mpz_inits(unFitness, value, NULL);
    mpq_init(unFitnessRatio);
}

/**
Initializes new FactorTree to all twos
**/
void FactorTree::init(void)
{
    int i;
    for (i = 0; i < arrayLength; i++)
        mpz_init_set_ui(factors[i], 2);
}

/**
Initializes new FactorTree to all instaces of the given value
**/
void FactorTree::init(long value)
{
    int i;
    for (i = 0; i < arrayLength; i++)
        mpz_init_set_ui(factors[i], value);
}


/**
breeds a new FactorTree from two old ones, this and a partner
**/
FactorTree *FactorTree::breed(FactorTree *partner)
{
    std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(0,1);
    auto clock = std::chrono::system_clock::now().time_since_epoch();
    int coinFlip, i;
    extern int arrayLength;
    FactorTree *ret = new FactorTree();
    ret->init();


    for (i=0;i<arrayLength;i++)
    {
        generator.seed(clock.count());
        coinFlip = distribution(generator);

        if(coinFlip)
            mpz_set(ret->factors[i], factors[i]);
        else
            mpz_set(ret->factors[i], partner->factors[i]);
    }

    return ret;
}

/**
Performs unify operation on the FactorTree according
to the relevant global values
**/
void FactorTree::coalesce(void)
{
    int i;
    std::default_random_engine generator;
    std::bernoulli_distribution distribution(unifyOdds);
    std::uniform_int_distribution<int> distribution2(0, arrayLength-1);
    auto clock = std::chrono::system_clock::now().time_since_epoch();
    generator.seed(clock.count());

    for (i = 0; i < arrayLength; i++)
        if (distribution(generator))
            unify(i, distribution2(generator));
}

/**
given two array indices
this function will combine those two factors in the tree
into one prime at the first index given and set the second
index to one
**/
void FactorTree::unify(int one, int two)
{
    mpz_mul(factors[one], factors[two], factors[one]);
    mpz_nextprime(factors[one], factors[one]);

    mpz_set_ui(factors[two], 1);
}

/**
public function to mutate a factor tree
according to the constants set at startup
**/
void FactorTree::mutate(void)
{
    int i;
    std::default_random_engine generator;
    std::bernoulli_distribution distribution(mutationOdds);
    auto clock = std::chrono::system_clock::now().time_since_epoch();
    generator.seed(clock.count());
    for (i = 0; i < arrayLength; i++)
        if(distribution(generator))
            mutate(i);
}
/**
private helper function that changes the number at a given index to a prime
nearby, how nearby is controlled by the mutation amount
**/
void FactorTree::mutate(int idx)
{
    int incDec;
    std::default_random_engine generator;
    std::bernoulli_distribution distribution1(.5);//TODO this should probably be a modifiable value
    std::uniform_int_distribution<int> distribution2(0,mutationAmount);

    auto clock = std::chrono::system_clock::now().time_since_epoch();
    generator.seed(clock.count());

    incDec = distribution2(generator);

    if(distribution1(generator))
    {
        mpz_sub_ui(factors[idx], factors[idx], incDec);
        switch(mpz_sgn(factors[idx]))
        {
        case -1:
            mpz_abs(factors[idx], factors[idx]);
            break;
        case 0:
            mpz_set_ui(factors[idx], 1);
        }
    }
    else
        mpz_add_ui(factors[idx], factors[idx], incDec);

    mpz_nextprime(factors[idx], factors[idx]);
}

//TODO, since here is where we might discover that
//we're done, so some kind of return value or global
//variable would be ideal
//global variable has to be accessed atomically and checked
//every so often, return value has to somehow percolate
//everywhere which is not awesome
//so it seems like a design decision to be made when
//we know a little more
bool FactorTree::calcValue(void)
{
//    std::cout << "now in the very beginning of calcValue" << std::endl;
    mpz_set_ui(value, 1);
    int i;

    for (i = 0; i < arrayLength; i++)
        mpz_mul(value, value, factors[i]);


    mpz_sub(unFitness, goal, value);
    mpz_abs(unFitness, unFitness);

    if (mpz_get_ui(unFitness) == 0)
    {
//        unFitnessRatio = std::numeric_limits<double>::max();
        mpq_set_ui(unFitnessRatio, 1, 0);
        return true;
    }

    mpq_set_z(unFitnessRatio, unFitness);
    mpq_set_den(unFitnessRatio, goal);

    //TODO
    //as it turns out we can in fact
    //get a type that represents a rational number
    //ie a fraction which is kinda perfect
    //for this use

//The goal is to set this equal to unFitness/goal
//but I've really no idea how to do that
    return false;
}


FactorTree::~FactorTree()
{
	mpq_clear(unFitnessRatio);
	mpz_clear(unFitness);
	mpz_clear(value);
    for (int i = 0; i < arrayLength; i++)
        mpz_clear(factors[i]);
    delete [] factors;
}

std::string FactorTree::toString(void)
{
    char *stringish;
    std::string *real;

    stringish = mpz_get_str(NULL, 10, factors[0]);

    std::string *retVal = new std::string(stringish);
    for (int i = 1; i < arrayLength; i++)
    {
        stringish = mpz_get_str(NULL, 10, factors[i]);
        real = new std::string(stringish);
        *retVal += "*";
        *retVal += *real;
    }
    return *retVal;
}
