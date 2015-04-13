#include "include/Population.h"
#include "include/globals.h"

#include <stdlib.h>
#include <iostream>
#include <random>
#include <chrono>
#include <array>


//when we start working on multithreding
//distribution performance etc, I'm thinking
//one way to do it would be coarse grained GA
//with inter node communication in MPI
//and local multithreading with pthreads

Population::Population(void)
{
	population = new FactorTree *[popSize];
	done = -1;
}

void Population::init(void)
{
    for (unsigned int i = 0; i < popSize; i++)
    {
        population[i] = new FactorTree();
        population[i]->init();
    }
}

Population *Population::generation(void)
{
//    std::cout << "now in generation" << std::endl;
    int isDone;
    Population *newer = new Population();

    isDone = calcValues();
    if (isDone != -1)
    {
        done = isDone;
        return this;
    }

    mutatePopulation(false);

    isDone = coalescePopulation(true);
    if (isDone != -1)
    {
        done = isDone;
        return this;
    }

    breedPopulation(newer);

    return newer;
}

//TODO there are a lot of ways to do this
//for the sake of simplicity let's go ahead
//and make elitism 'n shtuff separate,
//this will be a function that just breeds based on
//unFitness and maybe some randomness

//if another type of breeding comes up the preference
//would be to write a seperate method, not try to do
//it all here
/**
creates a new Population from an old one by generating a discrete
distribution based on each member of the population's unFitnessRatio
modulated by selectivePressure
**/
void Population::breedPopulation(Population *newer)
{
    mpq_t tempCalc;
    std::array<double, popSize> probs;
 //   probs.fill(1.0);
    mpq_inits(tempCalc, NULL);

    for (unsigned int i = 0; i < popSize; i++)
    {
 //       probs.at(i) += selectivePressure/population[i]->unFitnessRatio;
        mpq_set_d(tempCalc, selectivePressure);
//        mpq_set_ui(tempOne, 1, 1);
        mpq_div(tempCalc, tempCalc, population[i]->unFitnessRatio);
//        mpq_div(tempCalc, population[i]->unFitnessRatio, tempCalc);

//        mpq_add(tempCalc, tempOne, tempCalc);
//        mpq_div(tempCalc, tempOne, tempCalc);
        probs.at(i) = 1 + mpq_get_d(tempCalc);
    }
    mpq_clear(tempCalc);

    std::discrete_distribution<int> mateChoice(probs.begin(), probs.end());


    std::default_random_engine generator;
    auto clock = std::chrono::system_clock::now().time_since_epoch();
    generator.seed(clock.count());


    //the idea is that each run through
    //generates one member, so when multithreading
    //we can just have each thread set the appropriate
    //index in the array with "no" possibilty of conflict
    for(unsigned int i = 0; i < popSize; i++)
        newer->population[i] = population[mateChoice(generator)]->breed(population[mateChoice(generator)]);

}

/**
hopefully this method will never have to be used;
synchronizing around progress through a list a is a huge
pain in the ass and sucks not only performance
but the very life out of my already gray and
joyless existence, well it sucks performance anyways
**/
int Population::calcValues(void)
{
//    std::cout << "now in calcValues()" << std::endl;

    for (unsigned int i = 0; i < popSize; i++)
    {
        if (population[i]->calcValue())
            return i;
    }

    return -1;
}

int Population::coalescePopulation(bool recalc)
{
    std::default_random_engine generator;
    std::bernoulli_distribution distribution(coalesceOdds);
    auto clock = std::chrono::system_clock::now().time_since_epoch();
    generator.seed(clock.count());

    for (unsigned int i = 0; i < popSize; i++)
        if(distribution(generator))
        {
            population[i]->coalesce();
            if (recalc)
                if(population[i]->calcValue())
                    return i;
        }

    return -1;
}

/**
this method mutates all the FactorTrees and recalculates
value and (un)fitness
**/
//This sort of approch seems like it should work much better
//for multiple threads each thread can just go ahead and
//calculate fitness after mutating rather than havig to
//either wait for the whole process to be finished
//or sync around every element, either of which would SUCK
int Population::mutatePopulation(bool recalc)
{
    for (unsigned int i = 0; i < popSize; i++)
    {
        population[i]->mutate();
        if (recalc)
            if(population[i]->calcValue())
                return i;
    }
    return -1;
}

Population::~Population()
{
    for (unsigned int i = 0; i < popSize; i++)
        delete population[i];

    delete [] population;
}

std::string Population::doneString(void)
{
    return population[done]->toString();
}
