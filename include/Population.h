#ifndef POPUPATION_H
#define POPUPATION_H

#include <string>
#include "FactorTree.h"


class Population
{
    public:
        Population(void);
        void init(void);
        virtual ~Population();
        Population *generation(void);
        int mutatePopulation(bool);
        void breedPopulation(Population *);
        int calcValues(void);
        int coalescePopulation(bool);
        unsigned int done;
        std::string doneString(void);
    protected:
    private:
        FactorTree **population;
};

#endif // POPUPATION_H
