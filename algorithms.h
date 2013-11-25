#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include <stdlib.h>
#include <limits.h>
#include <gmp.h>

#define PRIMELIMITS 10000

class Algorithms
{
public:
    Algorithms();
    void GeneratePrime(mpz_t number, unsigned int numBits, unsigned int randomaiser);

private:
    unsigned long *shortPrimes;
    unsigned long maxPrimeIndex;

    bool MillerRabin(mpz_t number, unsigned int numBits, unsigned int numTest);
    void Eratosthen();
};

#endif
