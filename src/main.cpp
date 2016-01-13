#include <stdio.h>
#include <string>
#include <iostream>

#include "../include/globals.h"
#include "../include/Population.h"
#include "../include/FactorTree.h"


mpz_t goal;
int arrayLength;
double mutationOdds;
long int mutationAmount;
double unifyOdds;
double coalesceOdds;
double selectivePressure;

using namespace std;

long int getInt(string);
double getDouble(string);

int main()
{
    string goalVal;
    mpz_t curr;

	int i = 0;

	cout << "What number would you like to factor? ";
	getline(cin, goalVal);
    int fail = mpz_init_set_str(goal, goalVal.c_str(), 10);
    if (fail != 0)
        cout << "mpz_init_set_str() failed";

    mutationOdds = getDouble("how much frequently would you like mutations to happen?");
    coalesceOdds = getDouble("How agressively would you like to coalesce?");
    unifyOdds = getDouble("When coalescing, how agressively would you like to unify?");
    selectivePressure = getDouble("How much selective pressure would you like");
    mutationAmount = getInt("what shall be the maximum magnitude for mutations? ");

    for (mpz_init_set_ui(curr, 1); mpz_cmp(curr, goal) < 1; i++)
        mpz_mul_ui(curr, curr, 2);

    arrayLength = i-1;

	cout << "the max size of any array we'll need should be " << arrayLength << endl;

    Population *temp1 = new Population();
    temp1->init();
    Population *temp2 = temp1->generation();

    for(i = 0;/* i < 128*/; i++)
    {
        if (temp1 == temp2)
        {
            cout << "After " << (i+1) << " generations" << endl;
            cout << "We successfully factored the number, the individual is in the current population at index " << temp1->done << endl;
            cout << "it can be represented as follows:" << endl;
            cout << temp1->doneString() << endl;
            break;
        }
//        cout << "Did not find our answer in generation " << i+1 << endl;
        delete temp1;
        temp1 = temp2;
        temp2 = temp1->generation();
    }
    if (temp1 != temp2)
        delete temp1;
    delete temp2;

    mpz_clears(goal, curr, NULL);
}

long int getInt(string userQuery)
{
    string temp;
    cout << userQuery << " (c int value) ";
    getline(cin, temp);
    return stol(temp);
}

double getDouble(string userQuery)
{
    string temp;
    cout << userQuery << " (between 0 and 1) ";
    getline(cin, temp);
    return stod(temp);
}
