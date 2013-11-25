#include "algorithms.h"

Algorithms::Algorithms()
{
    shortPrimes = new unsigned long[PRIMELIMITS];
    Eratosthen();
}

void Algorithms::Eratosthen()
{
    unsigned long i, j, k;
    bool *prime = new bool[PRIMELIMITS];
    for(i = 0; i < PRIMELIMITS; i++)
        prime[i] = true;
    prime[0] = prime[1] = false;
    for(i = 2, k = 0; i < PRIMELIMITS; i++)
        if(prime[i])
        {
            for(j = i + i; j < PRIMELIMITS; j += i)
                prime[j] = false;
            shortPrimes[k++] = i;
        }
    shortPrimes[k] = 0;
    maxPrimeIndex = k - 1;
}

void Algorithms::GeneratePrime(mpz_t number, unsigned int numBits, unsigned int randomaiser)
{
    unsigned int i;
    bool isComposite;
    char *prime_str = new char[numBits + 1];

    mpz_t buf;
    mpz_init(buf);

    srand(randomaiser);
    prime_str[0] = '1';

    while(true)
    {

        for(i = 1; i < numBits - 1; i++)
            prime_str[i] = (int)(2.0 * rand() / (RAND_MAX + 1.0)) + '0';
        prime_str[numBits - 1] = '1';
        prime_str[numBits] = '\0';
        mpz_set_str(number,prime_str,2);
        for(isComposite = false, i = 0; i <= maxPrimeIndex; i++)
        {
            if(mpz_cmp_ui(number,shortPrimes[i]) <= 0)//////////////////
                break;
            mpz_mod_ui(buf, number, shortPrimes[i]);/////////////////////
            if(mpz_cmp_ui(buf,(unsigned long int)0) == 0)////////////////////
            {
                isComposite = true;
                break;
            }
        }
        if(isComposite)
            continue;
            if(MillerRabin(number, numBits, 10))
                break;
    }

    mpz_clear(buf);
    free(prime_str);
}

bool Algorithms::MillerRabin(mpz_t number, unsigned int numBits, unsigned int numTest)
{
    unsigned int i, j, k;
    char *a_str;

    mpz_t buf, t, gcd, a, c;
    mpz_init(buf);
    mpz_init(t);
    mpz_init(gcd);
    mpz_init(a);
    mpz_init(c);

    if(mpz_cmp_ui(number,(unsigned long)2) == 0)
         return true;

    mpz_mod_ui(buf, number, (unsigned long)2);
    if(mpz_cmp_ui(buf,(unsigned long)0) == 0)
        return false;

    unsigned long s = 0;
    mpz_sub_ui(t,number,(unsigned long)1);

    mpz_mod_ui(buf, t, (unsigned long)2);
    while(mpz_cmp_ui(buf,(unsigned long)0) == 0)
    {
         mpz_tdiv_qr_ui(t, buf, t, (unsigned long)2);
         mpz_mod_ui(buf, t, (unsigned long)2);
         s++;
    }

    a_str = new char[numBits];

    for(i = 0; i < numTest; i++)
    {
        for(k = 0; k < numBits - 1; k++)
            a_str[k] = (int)(2.0 * rand() / (RAND_MAX + 1.0)) + '0';
        a_str[k] = '\0';

        mpz_set_str(a,a_str,2);

        mpz_gcd(gcd,a,number);
        if(mpz_cmp_ui(gcd,(unsigned long)1) != 0)
            return false;

        mpz_powm(c, a, t, number);

        if(mpz_cmp_ui(c,(unsigned long)1) == 0)
            continue;

        mpz_sub_ui(buf,number,(unsigned long)1);
        if(mpz_cmp(c,buf) == 0)
            continue;

        bool f = true;
        for(j = 0; j < s - 1; j++)
        {
            mpz_powm_ui(c, c,(unsigned long)2, number);

            if(mpz_cmp(c,buf) == 0)
                f = false;
        }

        if(f)
            return false;
    }
    mpz_clear(buf);
    mpz_clear(t);
    mpz_clear(gcd);
    mpz_clear(a);
    mpz_clear(c);

    free(a_str);

    return true;
}
