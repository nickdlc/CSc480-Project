/* defines adapted from Piazza posts */
#define ISPRIME(x) mpz_probab_prime_p(x,10)
/* macro to declare a new long integer: */
#define NEWZ(x) mpz_t x; mpz_init(x)
/* these will read/write integers from byte arrays where the
 * least significant byte is first (little endian bytewise). */
#define BYTES2Z(x,buf,len) mpz_import(x,len,-1,1,0,0,buf)
#define Z2BYTES(buf,len,x) mpz_export(buf,&len,-1,1,0,0,x)