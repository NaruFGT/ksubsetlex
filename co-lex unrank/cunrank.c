#include "colex.h"
#include <stdio.h>
#include <stdlib.h>

int cmpfunc(const void* a, const void* b) {
    return (*(int*)a < *(int*)b) - (*(int*)a > * (int*)b);
}

int main(int ac, char* av[])
{
    int k, n, r, j;
    BIGNUM *BN_k, *BN_n, *rank, *loop, *tmp;
    size_t ploop;
    ksubset T;
    BN_ksubset BN_T;
    BN_CTX* ctx;

    BN_k = BN_new();
    BN_n = BN_new();
    rank = BN_new();
    loop = BN_new();
    tmp = BN_new();
    ctx = BN_CTX_new();

    if (ac != 4)
    {
        fprintf(stderr, "Argument mismatch, %d elements provided, 4 required.\n", ac);
        fprintf(stderr, "Usage: %s n k r\n", av[0]);
        fprintf(stderr, "  n is the total number of elements\n");
        fprintf(stderr, "  k is the number of elements in the subset\n");
        fprintf(stderr, "  r is the rank of the subset\n");
        exit(1);
    }
    n = atoi(av[1]);
    BN_dec2bn(&BN_n, av[1]);
    k = atoi(av[2]);
    BN_dec2bn(&BN_k, av[2]);
    r = atoi(av[3]);
    BN_dec2bn(&rank, av[3]);
    if (k < 0)
    {
        fprintf(stderr, "Sorry k must be greater than 0\n");
        exit(1);
    }
    if (k > n)
    {
        fprintf(stderr, "There are no %d-subsets of an %d-set?\n", k, n);
        exit(1);
    }
    T = (int*)calloc(k + 1, sizeof(int));
    BN_T = (BIGNUM **)calloc(k + 1, sizeof(BIGNUM *));

    if (!T)
    {
        fprintf(stderr, "Unable to allocate, calloc() failed.\n");
        exit(1);
    }

    ploop = 0u;
    BN_set_word(loop, ploop);
    BN_sub(tmp, BN_k, BN_value_one());
    while (BN_cmp(loop, tmp) != 1)
    {
        BN_T[ploop] = BN_new();

        ploop++;
        BN_set_word(loop, ploop);
        BN_sub(tmp, BN_k, BN_value_one());
    }
    
    setvbuf(stdout, NULL, _IONBF, 0);

    kSubsetColexUnrank(r, k, n, T);
    BN_ksubsetcolexunrank(rank, BN_k, BN_n, BN_T, ctx);

#ifdef _DEBUG
    printf("k-subset:");
    for (j = 0; j <= (k - 1); j = j + 1) printf(" %d", T[j]);
    printf("  rank: %d\n", r);
#endif

    printf("k-subset:");
    ploop = 0u;
    BN_set_word(loop, ploop);
    BN_sub(tmp, BN_k, BN_value_one());
    while (BN_cmp(loop, tmp) != 1)
    {
        printf(" %s", BN_bn2dec(BN_T[ploop]));

        ploop++;
        BN_set_word(loop, ploop);
        BN_sub(tmp, BN_k, BN_value_one());
    }
    printf("  rank: %s\n", BN_bn2dec(rank));

    free(T);
    BN_clear_free(BN_k);
    BN_clear_free(BN_n);
    BN_clear_free(rank);
    BN_clear_free(loop);
    BN_clear_free(tmp);
    BN_CTX_free(ctx);
    return(0);
}