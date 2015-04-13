#ifndef FACTORTREE_H
#define FACTORTREE_H

#include <gmp.h>
#include <string>
#include "globals.h"



class FactorTree
{
    //this will probably
    //become a  double at some point
    //but for now it's a lovely litte placeholder
    mpz_t unFitness;
    mpz_t value;
    mpz_t *factors;

    public:
        FactorTree();
        FactorTree *breed(FactorTree*);
        void init(void);
        void init(long);
        bool calcValue(void);
        void mutate(void);
        void coalesce(void);
        virtual ~FactorTree();
        mpq_t unFitnessRatio;
        std::string toString(void);
    protected:
    private:
        void unify(int,int);
        void mutate(int);
};

#endif // FACTORTREE_H
