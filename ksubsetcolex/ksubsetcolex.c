/*
**  ksubsetcolex.c
*/
/*
**  September 9, 1997
**  this program implements Algorithm 2.9 and 2.10
**  ranking and unranking for colex ordering of k-subsets of an n-set
**  assumes k-subsets in decreasing order
*/
/*
** 2020 October 20th
** OpenSSL BIGNUM implementations of 2.9 and 2.10
** BN_CTX must be provided by caller
*/
#include "colex.h"


void COLEX_API BN_bincoef(BIGNUM *rv, const BIGNUM *n, const BIGNUM *k, BN_CTX *ctx)
/*
**  Computes the binomial coefficient "n choose k"
*/
{
    // initialize variables
    BIGNUM *loop, *zero, *tmp, *BN_localk;
    loop = BN_new();
    zero = BN_new();
    tmp = BN_new();
    BN_localk = BN_new();
    BN_copy(BN_localk, k);
    // if ((k < 0) || (n < k)) return(0);
    BN_zero(zero);
    if (BN_cmp(k, zero) == -1 || BN_cmp(n, k) == -1)
    {
        BN_zero(rv);
        return;
    }
    // if ((2 * k) > n) k = n - k;
    BN_set_word(tmp, 2);
    BN_mul(tmp, tmp, k, ctx);
    if (BN_cmp(tmp, n)) BN_sub(BN_localk, n, k);
    // rv = 1
    BN_set_word(rv, 1);
    // if (k > 0)
    if (BN_cmp(BN_localk, zero))
    {
        // for (loop = 0; loop <= (k - 1); loop++)
        BN_zero(loop);
        BN_sub(tmp, k, BN_value_one());
        while (BN_cmp(loop, tmp) != 1)
        {
            // rv = (rv * (n - loop)) / (loop + 1);
            BIGNUM *divisor;
            divisor = BN_new();
            BN_sub(tmp, n, loop);
            BN_mul(tmp, rv, tmp, ctx);
            BN_add(divisor, loop, BN_value_one());
            BN_div(rv, NULL, tmp, divisor, ctx);
            BN_clear_free(divisor);

            BN_add(loop, loop, BN_value_one());
            BN_sub(tmp, BN_localk, BN_value_one());
        }
    }
    // free variables
    BN_clear_free(loop);
    BN_clear_free(zero);
    BN_clear_free(tmp);
    BN_clear_free(BN_localk);
    return;
}

int COLEX_API BinCoef(int n, int r)
/*
**  Computes the binomial coefficient "n choose r"
*/
{
    int i, b;
    if ((r < 0) || (n < r)) return(0);
    if ((2 * r) > n) r = n - r;
    b = 1;
    if (r > 0)
        for (i = 0; i <= (r - 1); i = i + 1)
            b = (b * (n - i)) / (i + 1);
    return(b);
}

int COLEX_API BN_checksort(const BN_ksubset T, const BIGNUM* k)
{
    size_t ploop;
    BIGNUM* BN_loop, * tmp;
    int rv;
    ploop = 0u;
    BN_loop = BN_new();
    tmp = BN_new();
    BN_set_word(BN_loop, ploop);
    BN_sub(tmp, k, BN_value_one());
    BN_sub(tmp, tmp, BN_value_one());
    while (BN_cmp(BN_loop, tmp) != 1)
    {
        rv = BN_cmp(T[ploop], T[ploop + 1]);
        if (rv != 1) return 0;

        ploop++;
        BN_set_word(BN_loop, ploop);
        BN_sub(tmp, k, BN_value_one());
        BN_sub(tmp, tmp, BN_value_one());
    }
    return 1;
}

void COLEX_API BN_ksubsetcolexrank(BIGNUM *rv, const BN_ksubset T, const BIGNUM *k, BN_CTX *ctx)
{
    BIGNUM *BN_loop, *tmp;
    size_t loop;
    BN_loop = BN_new();
    tmp = BN_new();

    BN_zero(rv);
    // for (loop = 0; loop <= (k - 1); loop++)
    loop = 0u;
    BN_set_word(BN_loop, loop);
    BN_sub(tmp, k, BN_value_one());
    while (BN_cmp(BN_loop, tmp) != 1)
    {
        BIGNUM *BN_localn, *BN_localk;
        BN_localn = BN_new();
        BN_localk = BN_new();
        // rv = rv + BinCoef(T[loop] - 1, k + 1 - (loop + 1));
        BN_sub(BN_localn, T[loop], BN_value_one());
        BN_add(BN_localk, BN_loop, BN_value_one());
        BN_sub(BN_localk, BN_value_one(), BN_localk);
        BN_add(BN_localk, k, BN_localk);
        BN_bincoef(tmp, BN_localn, BN_localk, ctx);
        BN_add(rv, rv, tmp);
        BN_clear_free(BN_localn);
        BN_clear_free(BN_localk);

        loop++;
        BN_set_word(BN_loop, loop);
        BN_sub(tmp, k, BN_value_one());
    }
    BN_clear_free(BN_loop);
    BN_clear_free(tmp);
    return;
}

int COLEX_API kSubsetColexRank(ksubset T, int k)
/*
**  Algorithm 2.9
**
**  returns r, the rank of the ksubset T
**  T must be in decreasing order
*/
{
    int i, r;
    r = 0;
    for (i = 0; i <= (k - 1); i = i + 1)
        r = r + BinCoef(T[i] - 1, k + 1 - (i + 1));
    return(r);
}

void COLEX_API BN_ksubsetcolexunrank(const BIGNUM *rank, const BIGNUM *k, const BIGNUM *n, BN_ksubset T, BN_CTX *ctx)
{
    BIGNUM *local_n, *local_rank, *loop, *tmp;
    size_t ploop;
    local_n = BN_new();
    local_rank = BN_new();
    loop = BN_new();
    tmp = BN_new();

    // local_n = n; local_rank = rank;
    BN_copy(local_n, n);
    BN_copy(local_rank, rank);

    // for (loop = 1; loop <= k; loop++)
    ploop = 1u;
    BN_set_word(loop, ploop);
    while (BN_cmp(loop, k) != 1)
    {
        // while ( (BinCoef (local_n, k + 1 - loop) ) > rank )
        BN_add(tmp, k, BN_value_one());
        BN_sub(tmp, tmp, loop);
        BN_bincoef(tmp, local_n, tmp, ctx);
        while (BN_cmp(tmp, local_rank) == 1)
        {
            // local_n--
            BN_sub(local_n, local_n, BN_value_one());

            BN_add(tmp, k, BN_value_one());
            BN_sub(tmp, tmp, loop);
            BN_bincoef(tmp, local_n, tmp, ctx);
        }
        // T[(loop - 1)] = local_n + 1;
        BN_add(T[ploop - 1], local_n, BN_value_one());
        // local_rank = local_rank - BinCoef(local_n, k + 1 - loop);
        // tmp should still be BinCoef(local_n, k + 1 - loop)
        //  as it was evaluated before and within the while loop above
        BN_sub(local_rank, local_rank, tmp);

        ploop++;
        BN_set_word(loop, ploop);
    }
    BN_free(local_n);
    BN_free(local_rank);
    BN_free(loop);
    BN_free(tmp);
}

void COLEX_API kSubsetColexUnrank(int r, int k, int n, ksubset T)
/*
**  Algorithm 2.10
**
**  returns T, the subset of rank T
**  T is in decreasing order
*/
{
    int x, i;
    x = n;
    for (i = 1; i <= k; i = i + 1)
    {
        while ((BinCoef(x, k + 1 - i)) > r)
        {
            x = x - 1;
        }
        T[(i-1)] = x + 1;
        r = r - BinCoef(x, k + 1 - i);
    }
}