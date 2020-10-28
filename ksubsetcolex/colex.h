#ifndef COLEX_H
#define COLEX_H

#ifdef COLEX_EXPORTS
#define COLEX_API __declspec(dllexport)
#else
#define COLEX_API __declspec(dllimport)
#endif

#include <openssl/bn.h>

#define true 1
#define false 0
typedef int* ksubset;
typedef BIGNUM **BN_ksubset;

int COLEX_API kSubsetColexRank(ksubset T, int k);
void COLEX_API BN_ksubsetcolexrank(BIGNUM* rv, const BN_ksubset T, const BIGNUM* k, BN_CTX* ctx);
void COLEX_API kSubsetColexUnrank(int r, int k, int n, ksubset T);
void COLEX_API BN_ksubsetcolexunrank(const BIGNUM* rank, const BIGNUM* k, const BIGNUM* n, BN_ksubset T, BN_CTX* ctx);

int COLEX_API BinCoef(int n, int k);
void COLEX_API BN_bincoef(BIGNUM* rv, const BIGNUM* n, const BIGNUM* k, BN_CTX* ctx);

int COLEX_API BN_checksort(const BN_ksubset T, const BIGNUM* k);

#endif /* COLEX_H*/