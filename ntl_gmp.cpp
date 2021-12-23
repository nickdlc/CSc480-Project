// This file refers to the professor Skeith's NTL examples cpp file, necessary to generate random polynomials

// #include <NTL/ZZ_pX.h>
#include <fstream>
#include <gmp.h>
#include "ntl_gmp.h"

// #include <iostream>
// using std::ofstream;
// using namespace std;

ZZ q, p, g;

using std::ifstream;
// defines adapted from Piazza posts
#define ISPRIME(x) mpz_probab_prime_p(x, 10)
/* macro to declare a new long integer: */
#define NEWZ(x) \
	mpz_t x;    \
	mpz_init(x)
/* these will read/write integers from byte arrays where the
 * least significant byte is first (little endian bytewise). */
#define BYTES2Z(x, buf, len) mpz_import(x, len, -1, 1, 0, 0, buf)

// #define Z2BYTES(buf, len, x) mpz_export(buf, &len, -1, 1, 0, 0, x)

void genParams()
{
	/* select random prime q of the right number of bits, then multiply
	 * by a random even integer, add 1, check if that is prime.  If so,
	 * we've found q and p respectively. */
	// const size_t qBitlen = 256;
	// const size_t pBitlen = 2048;
	const size_t qBitlen = 512;
	const size_t pBitlen = 4096;
	/* lengths in BYTES: */
	size_t qLen = qBitlen / 8 + (qBitlen % 8 != 0);
	size_t pLen = pBitlen / 8 + (pBitlen % 8 != 0);
	size_t rLen = pLen - qLen;
	unsigned char *qCand = (unsigned char *)malloc(qLen);
	unsigned char *rCand = (unsigned char *)malloc(rLen);
	NEWZ(q);
	NEWZ(p);
	NEWZ(r); /* holds (p-1)/q */
	NEWZ(t); /* scratch space */
	NEWZ(g); /* generator of subgroup */
	FILE *f = fopen("/dev/urandom", "rb");
	do
	{
		do
		{
			fread(qCand, 1, qLen, f);
			BYTES2Z(q, qCand, qLen);
		} while (!ISPRIME(q));
		/* now try to get p */
		fread(rCand, 1, rLen, f);
		rCand[0] &= 0xfe; /* set least significant bit to 0 (make r even) */
		BYTES2Z(r, rCand, rLen);
		mpz_mul(p, q, r);	 /* p = q*r */
		mpz_add_ui(p, p, 1); /* p = p+1 */
		/* should make sure q^2 doesn't divide p-1.
		 * suffices to check if q|r */
		mpz_mod(t, r, q); /* t = r%q */
		/* now check if t is 0: */
		if (mpz_cmp_ui(t, 0) == 0)
			continue; /* really unlucky! */
	} while (!ISPRIME(p));
	/* now find a generator of the subgroup of order q.
	 * Turns out just about anything to the r power will work: */
	size_t tLen = qLen; /* qLen somewhat arbitrary. */
	unsigned char *tCand = (unsigned char *)malloc(tLen);
	do
	{
		fread(tCand, 1, tLen, f);
		BYTES2Z(t, tCand, tLen);
		if (mpz_cmp_ui(t, 0) == 0)
			continue;				 /* really unlucky! */
		mpz_powm(g, t, r, p);		 /* efficiently do g = t**r % p */
	} while (mpz_cmp_ui(g, 1) == 0); /* since q prime, any such g /= 1
										will actually be a generator of
										the subgroup. */
	fclose(f);

	FILE *file = fopen("params-q-p-g", "w");
	gmp_fprintf(file, "%Zd\n%Zd\n%Zd", q, p, g);
	fclose(file);
	free(qCand);
	free(rCand);
	free(tCand);
}

/* you could use something like this to read the parameters
 * from a file.  I have placed the values q,p,g (in that order)
 * which were posted on piazza in the file ../params-q-p-g
 * NOTE: are written in base 10 for convenience. */
int readParams()
{
	ifstream fin("params-q-p-g");
	if (!fin.good())
	{
		fprintf(stderr, "couldn't open parameter file.\n");
		return 1;
	}
	/* NOTE: q,p,g declared above at global scope. */
	fin >> q >> p >> g;
	fin.close();
	/* Let's perform a quick sanity check: are values which are
	 * supposed to be prime actually so?  Does q in fact divide
	 * the order of the multiplicative group mod p? */
	if (!ProbPrime(q))
	{
		fprintf(stderr, "q not prime!\n");
		return -1;
	}
	if (!ProbPrime(p))
	{
		fprintf(stderr, "p not prime!\n");
		return -1;
	}
	if ((p - 1) % q != 0)
	{
		fprintf(stderr, "q does not divde p-1!\n");
		return -1;
	}
	if ((p - 1) % (q * q) == 0)
	{
		fprintf(stderr, "q^2 divides p-1!\n");
		return -1;
	}
	/* lastly, let's check on the generator: */
	if (PowerMod(g, (p - 1) / q, p) == 1)
	{
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
	FILE *frand = fopen("/dev/urandom", "rb");
	unsigned char seed[32];
	fread(seed, 1, 32, frand);
	fclose(frand);
	SetSeed(seed, 32);
}

/* Compute Vandermonde matrix.  Input is list of n (distinct, nonzero)
 * points; output is nxn Vandermonde matrix.  NOTE: to call this, I
 * think you must have first called ZZ_p::init(...) with your prime
 * modulus of choice!  (This setup only has to be done once.) */
mat_ZZ_p vandermonde(const vec_ZZ_p &A)
{
	mat_ZZ_p V;
	size_t n = A.length();
	V.SetDims(n, n);
	for (size_t i = 0; i < n; i++)
	{
		V[i][0] = 1;
		for (size_t j = 1; j < n; j++)
			V[i][j] = V[i][j - 1] * A[i];
	}
	return V;
}