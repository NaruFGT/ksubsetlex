#include "colex.h"
#include <stdio.h>
#include <stdlib.h>

int cmpfunc(const void* a, const void* b) {
    return (*(int*)a < * (int*)b) - (*(int*)a > *(int*)b);
}

/*
** TODO: debug
** Doesn't seem to work with qsort
*/
int BN_cmpfunc(const void* a, const void* b) {
    int cmpvalue;
    cmpvalue = BN_cmp((BIGNUM *)a, (BIGNUM *)b);
    switch (cmpvalue) {
        case -1 :
            return 1;
        case 0 :
            return 0;
        case 1 :
            return -1;
    }
    exit(1);
}

int main(int ac, char* av[])
{
    int k, j, s;
    ksubset T;
    BIGNUM *BN_k, *rank, *BN_loop, *tmp;
    size_t ploop;
    BIGNUM** BN_T;
    BN_CTX *ctx;
    ctx = BN_CTX_new();
    BN_k = BN_new();
    rank = BN_new();
    BN_loop = BN_new();
    tmp = BN_new();
    if (ac < 3)
    {
        fprintf(stderr, "Usage: %s k T[1] .. T[k]\n", av[0]);
        exit(1);
    }
    k = atoi(av[1]);
    BN_dec2bn(&BN_k, av[1]);
    if (k < 0)
    {
        fprintf(stderr, "Sorry k must be greater than 0\n");
        exit(1);
    }
    if (ac != k + 2)
    {
        fprintf(stderr, "Argument mismatch, k=%d elements requested but %d provided.", k, ac - 2);
        exit(1);
    }
    T = (int *)calloc(k + 1, sizeof(int));
    BN_T = (BIGNUM **)calloc(k + 1, sizeof(BIGNUM *));

    if (!T)
    {
        fprintf(stderr, "Unable to allocate, calloc() failed.\n");
        exit(1);
    }
    for (j = 0; j <= (k - 1); j = j + 1) T[j] = atoi(av[j+2]);
    
    ploop = 0u;
    BN_set_word(BN_loop, ploop);
    BN_sub(tmp, BN_k, BN_value_one());
    while (BN_cmp(BN_loop, tmp) != 1)
    {
        BN_T[ploop] = BN_new();
        BN_dec2bn(&BN_T[ploop], av[2+ploop]);

        ploop++;
        BN_set_word(BN_loop, ploop);
        BN_sub(tmp, BN_k, BN_value_one());
    }

    qsort(T, k, sizeof(int), cmpfunc);
    if (!BN_checksort(BN_T, BN_k))
    {
        printf("k-subset:");
        ploop = 0u;
        BN_set_word(BN_loop, ploop);
        BN_sub(tmp, BN_k, BN_value_one());
        while (BN_cmp(BN_loop, tmp) != 1)
        {
            printf(" %s", BN_bn2dec(BN_T[ploop]));

            ploop++;
            BN_set_word(BN_loop, ploop);
            BN_sub(tmp, BN_k, BN_value_one());
        }
        printf("\nProvided k-subset not sorted in decending order, cannot continue.\n\n");
        exit(1);
    }


    setvbuf(stdout, NULL, _IONBF, 0);

    s = kSubsetColexRank(T, k);
    BN_ksubsetcolexrank(rank, BN_T, BN_k, ctx);

    printf("k-subset:");
    for (j = 0; j <= (k - 1); j = j + 1) printf(" %d", T[j]);
    printf("  rank: %d\n", s);

    printf("k-subset:");
    ploop = 0u;
    BN_set_word(BN_loop, ploop);
    BN_sub(tmp, BN_k, BN_value_one());
    while (BN_cmp(BN_loop, tmp) != 1)
    {
        printf(" %s", BN_bn2dec(BN_T[ploop]));

        ploop++;
        BN_set_word(BN_loop, ploop);
        BN_sub(tmp, BN_k, BN_value_one());
    }
    printf("  rank: %s\n", BN_bn2dec(rank));

    return(0);
}