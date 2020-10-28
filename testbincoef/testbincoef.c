#include "colex.h"
#include <stdio.h>
#include <stdlib.h>
#include <openssl/bn.h>

int main(int ac, char* av[])
{
	BIGNUM *BN_n, *BN_k, *BN_bc;
    int n, k, bc;
    BN_CTX *ctx;

    ctx = BN_CTX_new();
    BN_n = BN_new();
    BN_k = BN_new();
    BN_bc = BN_new();

    if (ac < 3)
    {
        fprintf(stderr, "Usage: %s n k\n", av[0]);
        exit(1);
    }

    n = atoi(av[1]);
    k = atoi(av[2]);
    BN_dec2bn(&BN_n, av[1]);
    BN_dec2bn(&BN_k, av[2]);

    bc = BinCoef(n, k);
    BN_bincoef(BN_bc, BN_n, BN_k, ctx);

    printf("int BinCoef() = %d", bc);
    printf(" BN_bincoef() = %s", BN_bn2dec(BN_bc));
    return 0;
}