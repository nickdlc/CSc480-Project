#pragma once

#include <NTL/ZZ_pX.h>
using namespace NTL;

void genParams();
int readParams();
void initNTLRandom();

mat_ZZ_p vandermonde(const vec_ZZ_p &A);