#ifndef ELGAMAL_H
#define ELGAMAL_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <gmp.h>

#include "algorithms.h"

class ElGamal
{
public:
    ElGamal();

    void GenerateKeys(FILE *PublicKey, FILE *PrivateKey, FILE *Randomize, unsigned int numBits);
    void Encrypt(FILE *Input, FILE *Output, FILE *PublicKey);
    void Decrypt(FILE *Input, FILE *Output, FILE *PrivateKey);

private:
    mpz_t p, g, x, y;

    void PrintStr(char *str, FILE *file, unsigned int numBytes);
    void ConvertStr(char *inputStr, char *outputStr, unsigned int length);

};

#endif
