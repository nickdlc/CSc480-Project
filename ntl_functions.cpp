//This file refers to the professor Skeith's NTL examples cpp file, necessary to generate random polynomials

#include <NTL/ZZ_pX.h>
#include <iostream>
#include <fstream>
#include "gmp_defs.h"

using std::ifstream;
using std::ofstream;


using namespace NTL;
using namespace std;

ZZ q,p,g;

/* you could use something like this to read the parameters
 * from a file.  I have placed the values q,p,g (in that order)
 * which were posted on piazza in the file ../params-q-p-g
 * NOTE: are written in base 10 for convenience. */
int readParams()
{
	ifstream fin("params-q-p-g");
	if (!fin.good()) {
		printf("couldn't open parameter file, generating new params...\n");
		generate_params();
		printf("reading new params...\n");
		fin = ifstream("params-q-p-g");
		if (!fin.good()) {
			fprintf(stderr, "something went wrong when opening parameter file, quiting...\n");
			return 1;
		}
	}
	/* NOTE: q,p,g declared above at global scope. */
	fin >> q >> p >> g;
	fin.close();
	/* Let's perform a quick sanity check: are values which are
	 * supposed to be prime actually so?  Does q in fact divide
	 * the order of the multiplicative group mod p? */
	if (!ProbPrime(q)) {
		fprintf(stderr, "q not prime!\n");
		return -1;
	}
	if (!ProbPrime(p)) {
		fprintf(stderr, "p not prime!\n");
		return -1;
	}
	if ((p-1)%q != 0) {
		fprintf(stderr, "q does not divde p-1!\n");
		return -1;
	}
	if ((p-1)%(q*q) == 0) {
		fprintf(stderr, "q^2 divides p-1!\n");
		return -1;
	}
	/* lastly, let's check on the generator: */
	if (PowerMod(g,(p-1)/q,p) == 1) {
		fprintf(stderr, "g does not generate subgroup of size q!\n");
		return -1;
	}
	/* NOTE: we can also set the modulus for the ZZ_p datatypes.
	 * This way you never have to perform any explicit reductions
	 * when doing arithmetic, e.g., X*Y means X*Y % p for X,Y of
	 * type ZZ_p. */
	
	ZZ_p::init(q);
	
	/* NOTE: for secret sharing, you could also use this, but you
	 * will likely want to use parameter q instead. */
	return 0;
}

/* you can (and should!) use something like this to initialize
 * the state of NTL's internal random number generator.  Also
 * note that unlike the rand() function from libc, NTL random
 * values are of cryptographic quality. */
void initNTLRandom()
{
	FILE* frand = fopen("/dev/urandom","rb");
	unsigned char seed[32];
	fread(seed,1,32,frand);
	fclose(frand);
	SetSeed(seed,32);
}

/* Compute Vandermonde matrix.  Input is list of n (distinct, nonzero)
 * points; output is nxn Vandermonde matrix.  NOTE: to call this, I
 * think you must have first called ZZ_p::init(...) with your prime
 * modulus of choice!  (This setup only has to be done once.) */
mat_ZZ_p vandermonde(const vec_ZZ_p& A)
{
	mat_ZZ_p V;
	size_t n = A.length();
	V.SetDims(n,n);
	for (size_t i = 0; i < n; i++) {
		V[i][0] = 1;
		for (size_t j = 1; j < n; j++) {
			V[i][j] = V[i][j-1] * A[i];
		}
	}
	return V;
}