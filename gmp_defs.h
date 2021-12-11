#include <gmp.h>

/* defines adapted from Piazza posts */
#define ISPRIME(x) mpz_probab_prime_p(x,10)
/* macro to declare a new long integer: */
#define NEWZ(x) mpz_t x; mpz_init(x)
/* these will read/write integers from byte arrays where the
 * least significant byte is first (little endian bytewise). */
#define BYTES2Z(x,buf,len) mpz_import(x,len,-1,1,0,0,buf)
#define Z2BYTES(buf,len,x) mpz_export(buf,&len,-1,1,0,0,x)

bool primetest(mpz_t p) {
    while (gmp_scanf("%Zd",p) == 1) {
		if (ISPRIME(p)) return true;
	}

    return false;
}

void generate_params() {
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
	unsigned char* qCand = (unsigned char*)malloc(qLen);
	unsigned char* rCand = (unsigned char*)malloc(rLen);
	NEWZ(q);
	NEWZ(p);
	NEWZ(r); /* holds (p-1)/q */
	NEWZ(t); /* scratch space */
	NEWZ(g); /* generator of subgroup */
	FILE* f = fopen("/dev/urandom","rb");
	do {
		do {
			fread(qCand,1,qLen,f);
			BYTES2Z(q,qCand,qLen);
		} while (!ISPRIME(q));
		/* now try to get p */
		fread(rCand,1,rLen,f);
		rCand[0] &= 0xfe; /* set least significant bit to 0 (make r even) */
		BYTES2Z(r,rCand,rLen);
		mpz_mul(p,q,r);     /* p = q*r */
		mpz_add_ui(p,p,1);  /* p = p+1 */
		/* should make sure q^2 doesn't divide p-1.
		 * suffices to check if q|r */
		mpz_mod(t,r,q);     /* t = r%q */
		/* now check if t is 0: */
		if (mpz_cmp_ui(t,0) == 0) continue; /* really unlucky! */
	} while (!ISPRIME(p));
	/* now find a generator of the subgroup of order q.
	 * Turns out just about anything to the r power will work: */
	size_t tLen = qLen; /* qLen somewhat arbitrary. */
	unsigned char* tCand = (unsigned char*)malloc(tLen);
	do {
		fread(tCand,1,tLen,f);
		BYTES2Z(t,tCand,tLen);
		if (mpz_cmp_ui(t,0) == 0) continue; /* really unlucky! */
		mpz_powm(g,t,r,p); /* efficiently do g = t**r % p */
	} while (mpz_cmp_ui(g,1) == 0); /* since q prime, any such g /= 1
									   will actually be a generator of
									   the subgroup. */
	fclose(f);
    
    FILE* file = fopen("params-q-p-g", "w");
    gmp_fprintf(file, "%Zd\n%Zd\n%Zd", q, p, g);
    fclose(file);
    free(qCand);
    free(rCand);
    free(tCand);
}