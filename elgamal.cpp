#include "elgamal.h"

ElGamal::ElGamal()
{
}

void ElGamal::GenerateKeys(FILE *PublicKey, FILE *PrivateKey, FILE *Randomize, unsigned int numBits)
{
    printf("Start\n");

    Algorithms algo;

    char *p_str = new char[numBits / 4 + 2];
    char *g_str = new char[numBits / 4 + 2];
    char *x_str = new char[numBits / 4 + 2];
    char *y_str = new char[numBits / 4 + 2];
    char *buf_binstr = new char[numBits + 1];

    unsigned int initRandom = 0;
    unsigned int i;

    bool ispPrime = false;
    bool isgPrimitive = false;

    mpz_init(p);
    mpz_init(g);
    mpz_init(x);
    mpz_init(y);


    mpz_t q, n, N;
    mpz_t buf1, buf2, buf3;

    mpz_init(q);
    mpz_init(n);
    mpz_init(N);

    mpz_init(buf1);
    mpz_init(buf2);
    mpz_init(buf3);

    char *data = new char[1000];
    while(!ispPrime)
    {
        initRandom = ((unsigned int)fgetc(Randomize) << 8) | fgetc(Randomize);
        algo.GeneratePrime(q, numBits / 2, initRandom);
        algo.GeneratePrime(n, numBits / 2 - 1, initRandom);

        mpz_mul_ui(N, n, (unsigned long)2);
        mpz_mul(buf1, N, q);
        mpz_add_ui(p, buf1, (unsigned long)1);
        mpz_set_ui(buf3, (unsigned long)2);
        mpz_powm(buf2, buf3, buf1 , p);
        if(mpz_cmp_ui(buf2, (unsigned long)1) == 0)
        {
            mpz_powm(buf2, buf3, N , p);
            if(mpz_cmp_ui(buf2, (unsigned long)1) != 0)
                ispPrime = true;
        }
    }
    mpz_get_str(p_str,16,p);

    initRandom ^= ((unsigned int)fgetc(Randomize) << 8) | fgetc(Randomize);

    while(!isgPrimitive)
    {
        mpz_set_ui(g, (unsigned long)1);
        while(mpz_cmp_ui(g, 3) == -1 || mpz_cmp(g, p) >= 0)
        {
            for(i = 0; i < numBits; i++)
                buf_binstr[i] = (int)(2.0 * rand() / (RAND_MAX + 1.0)) + '0';
            buf_binstr[numBits] = '\0';
            mpz_set_str(g,buf_binstr,2);
        }

        mpz_div(buf2, buf1, q);
        mpz_powm(buf3, g, buf2 , p);
        if(mpz_cmp_ui(buf3, (unsigned long)1) != 0)
        {
            mpz_div(buf2, buf1, n);
            mpz_powm(buf3, g, buf2 , p);
            if(mpz_cmp_ui(buf3, (unsigned long)1) != 0)            {
                mpz_div_ui(buf2, buf1, (unsigned long)2);
                mpz_powm(buf3, g, buf2 , p);
                if(mpz_cmp_ui(buf3, (unsigned long)1) !=0)
                    isgPrimitive = true;
            }
        }
    }
    mpz_get_str(g_str,16,g);
    mpz_set_ui(x, (unsigned long)1);
    while(mpz_cmp_ui(x, 1) <= 0 || mpz_cmp(x, buf1) >= 0)
    {
        for(i = 0; i < numBits; i++)
            buf_binstr[i] = (int)(2.0 * rand() / (RAND_MAX + 1.0)) + '0';
        buf_binstr[numBits] = '\0';
        mpz_set_str(x,buf_binstr,2);
    }
    mpz_get_str(x_str,16,x);
    mpz_powm(y, g, x, p);
    mpz_get_str(y_str,16,y);

    fprintf(PublicKey, "%x\n%s\n%s\n%s", numBits, p_str, g_str, y_str);
    fprintf(PrivateKey, "%x\n%s\n%s", numBits, p_str, x_str);
    printf("Stop\n");

    mpz_clear(buf1);
    mpz_clear(buf2);
    mpz_clear(buf3);
    mpz_clear(q);
    mpz_clear(n);
    mpz_clear(N);
    mpz_clear(p);
    mpz_clear(g);
    mpz_clear(x);
    mpz_clear(y);
}

void ElGamal::Encrypt(FILE *Input, FILE *Output, FILE *PublicKey)
{
    printf("Start\n");

    mpz_init(p);
    mpz_init(g);
    mpz_init(y);

    mpz_t M, a, b, k;
    mpz_t buf;

    mpz_init(M);
    mpz_init(a);
    mpz_init(b);
    mpz_init(k);
    mpz_init(buf);

    unsigned int i, j;
    unsigned int numBits, inputBytes, outputBytes;

    fscanf(PublicKey,"%x\n", &numBits);

    outputBytes = (numBits % 8 ? numBits / 8 - 1 : numBits / 8);

    inputBytes = (numBits % 8 ? numBits / 8 :  numBits / 8 - 1);

    char *p_str = new char[numBits / 4 + 2];
    char *g_str = new char[numBits / 4 + 2];
    char *y_str = new char[numBits / 4 + 2];

    char *a_str = new char[numBits / 4 + 2];
    char *b_str = new char[numBits / 4 + 2];
    char *IOstring = new char[outputBytes + 1];
    char *buf_binstr = new char[numBits + 1];

    fscanf(PublicKey,"%s\n%s\n%s\n", p_str, g_str, y_str);
    mpz_set_str(p,p_str,16);
    mpz_set_str(g,g_str,16);
    mpz_set_str(y,y_str,16);

    mpz_set_ui(k, (unsigned long)1);
    mpz_sub_ui(buf, p, (unsigned long)1);
    while(mpz_cmp_ui(k, 1) <= 0 || mpz_cmp(k, buf) >= 0)
    {
        for(i = 0; i < numBits; i++)
            buf_binstr[i] = (int)(2.0 * rand() / (RAND_MAX + 1.0)) + '0';
        buf_binstr[numBits] = '\0';
        mpz_set_str(k,buf_binstr,2);
    }

    while(!feof(Input))
    {
        for(i = 0, j = 0; i < inputBytes && !feof(Input); i++, j++)
            IOstring[i] = fgetc(Input);
        if(i)
        {
            if(!feof(Input))
                IOstring[j] = '\0';
            else if (j - 1)
                IOstring[--j] = '\0';
            else
                break;
        }
        else
            break;

        char* m_str = new char[j * 2 + 1];
        ConvertStr(IOstring, m_str, j);
        mpz_set_str(M,m_str,16);
        mpz_powm(a,g,k,p);
        mpz_get_str(a_str,16,a);
        mpz_powm(buf,y,k,p);
        mpz_mul(b, buf, M);
        mpz_mod(b, b, p);
        mpz_get_str(b_str,16,b);

        PrintStr(a_str, Output, outputBytes);
        PrintStr(b_str, Output, outputBytes);
        free(m_str);
    }
    printf("Stop\n");

    mpz_clear(M);
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(k);
    mpz_clear(buf);
    mpz_clear(p);
    mpz_clear(g);
    mpz_clear(y);
}

void ElGamal::Decrypt(FILE *Input, FILE *Output, FILE *PrivateKey)
{
    printf("Start\n");

    mpz_init(p);
    mpz_init(x);
    mpz_t M, a, b;
    mpz_t buf;
    mpz_init(M);
    mpz_init(a);
    mpz_init(b);
    mpz_init(buf);

    unsigned int i, j;
    unsigned int numBits, inputBytes, outputBytes;

    fscanf(PrivateKey,"%x\n", &numBits);

    inputBytes = (numBits % 8 ? numBits / 8 - 1 : numBits / 8);
    outputBytes = (numBits % 8 ? numBits / 8 :  numBits / 8 - 1);

    char *p_str = new char[numBits / 4 + 2];
    char *x_str = new char[numBits / 4 + 2];
    char *m_str = new char[outputBytes + 1];
    char *IOstring = new char[inputBytes + 1];

    fscanf(PrivateKey,"%s\n%s\n", p_str, x_str);

    mpz_set_str(p,p_str,16);
    mpz_set_str(x,x_str,16);

    while(!feof(Input))
    {
        for(i = j = 0; i < inputBytes && !feof(Input); i++, j++)
            IOstring[j] = fgetc(Input);
        if(i)
        {
            if(!feof(Input))
                IOstring[j] = '\0';
            else if (j - 1)
                IOstring[--j] = '\0';
            else
                break;
        }
        else
        {
            printf("\nDecrypt error\n");
            break;
        }

        char* a_str = new char[j * 2 + 1];
        ConvertStr(IOstring, a_str, j);
        mpz_set_str(a,a_str,16);

        for(i = j = 0; i < inputBytes && !feof(Input); i++, j++)
            IOstring[j] = fgetc(Input);
        if(i)
        {
            if(!feof(Input))
                IOstring[j] = '\0';
            else if (j - 1)
                IOstring[--j] = '\0';
            else
                break;
        }
        else
            break;

        char* b_str = new char[j * 2 + 1];
        ConvertStr(IOstring, b_str, j);
        mpz_set_str(b,b_str,16);
        mpz_sub(buf, p, x);
        mpz_sub_ui(M, buf, (unsigned long)1);
        mpz_powm(buf, a, M, p);
        mpz_mul(M, buf, b);
        mpz_mod(M, M, p);

        mpz_get_str(m_str,16,M);
        PrintStr(m_str, Output, outputBytes);
        free(a_str);
        free(b_str);
    }
    printf("Stop\n");
}

void ElGamal::PrintStr(char *str, FILE *file, unsigned int numBytes)
{
    unsigned int i, tmp = numBytes * 2 - strlen(str);
    tmp -= ((numBytes * 2 - strlen(str)) % 2);
    tmp /= 2;
    for(i = 0; i < tmp; i++)
        fprintf(file,"%c", 0);
    for(i = 0; i < strlen(str);)
    {
        if(((numBytes * 2 - strlen(str)) % 2) && !i)
            tmp = 0;
        else
            tmp = str[i] - (str[i++] >= 'a' ? 'a' - 10 : '0');
        tmp <<= 4;
        tmp |= str[i] - (str[i++] >= 'a' ? 'a' - 10 : '0');
        fprintf(file,"%c", tmp);
    }
}

void ElGamal::ConvertStr(char *inputStr, char *outputStr, unsigned int length)
{
    char tmps[2];
    strcpy(outputStr,"");

    for(unsigned int i = 0, tmp; i < length; i++)
    {
        tmp = ((inputStr[i] >> 4) & 0xF);
        sprintf(tmps,"%01x",tmp);
        strcat(outputStr,tmps);

        tmp = (inputStr[i]  & 0xF);
        sprintf(tmps,"%01x",tmp);
        strcat(outputStr,tmps);
    }
}
