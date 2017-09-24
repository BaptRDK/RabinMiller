/*rabinMiller.c*/
#include "rabinMiller.h"

//decomposes n into s and d like (n-1) = (2**s)*d
int decompose(unsigned long int* s, mpz_t d, mpz_t n)
{
	//just a buffer variable
	mpz_t ans;
	mpz_init(ans);

	//rest of the divisions
	unsigned long int modulo;
	

	//Tests if the input number n is odd
	if(mpz_cdiv_q_ui(ans, n, 2) == 0)
	{
		//clears allocated memory
		mpz_clear(ans);

		fprintf(stderr, "decompose : Error : n must be an odd number.\n");
		exit(1);
	}
	//If else tests if n is less than than 3
	else if(mpz_cmp_ui(n, 3) < 0)
	{
		//clears allocated memory
		mpz_clear(ans);

		fprintf(stderr, "decompose : Error : n must be greater than 2\n");
		exit(2);
	}
	
	mpz_t num;
	mpz_init(num);
	*s = 1;

	//num = n-1
	mpz_sub_ui(num, n, 1);

	do
	{
		*s = *s + 1;

		//d = num / (2**s)
		modulo = mpz_cdiv_q_ui(d, num, 1 << *s);

	//do while d is a round number
	}while(modulo == 0);
	
	//s takes the previous, valid, exponent
	*s = *s -1;

	//d takes the last valid value (we recalculate it)
	mpz_cdiv_q_ui(d, num, 1 << *s);

	//We clear the local variables
	mpz_clear(num);
	mpz_clear(ans);

	return(0);
}

//returns 1 if a is a miller witness of n, 0 if it is not
int isWitness(mpz_t a, mpz_t n, unsigned long int *s , mpz_t d)
{
	//buffer variable
	mpz_t ans;
	mpz_init(ans);

	//Tests if the input number n is odd
	if(mpz_cdiv_q_ui(ans, n, 2) == 0)
	{
		//clears allocated memory
		mpz_clear(ans);

		fprintf(stderr, "isWitness : Error : n must be an odd number.\n");
		exit(-1);
	}
	//If else tests if n is less than than 3
	else if(mpz_cmp_ui(n, 3) < 0)
	{
		//clears allocated memory
		mpz_clear(ans);

		fprintf(stderr, "isWitness : Error : n must be greater than 2\n");
		exit(-2);
	}
	//if else tests if a is less than 1
	else if(mpz_cmp_ui(a, 1) < 0)
	{
		//clears allocated memory
		mpz_clear(ans);

		fprintf(stderr, "isWitness : Error : A muste be greater than 1\n");
		exit(-3);
	}	

	//makes a copy of the parameter s
	unsigned long int copyS = *s;
	mpz_t num;
	mpz_init(num);

	//num = (n-1)
	mpz_sub_ui(num, n, 1);

	//calculates ans = (a**d) % n
	mpz_powm(ans, a, d, n);

	//if ans = 1 or ans=(n-1)
	if((mpz_cmp_ui(ans, 1) == 0) || (mpz_cmp(ans, num) == 0))
	{
		//clears the allocated memory
		mpz_clears(num, ans, NULL);
		//then a is not a Miller witness
		return(0);
	}

	while (copyS > 1)
	{
		//ans = (ans**2) % n
		mpz_powm_ui(ans, ans, 2, n);

		//if ans = (n-1)
		if(mpz_cmp(ans, num) == 0)
		{
			//clearing allocated memory
			mpz_clears(num, ans, NULL);

			//then a is not a Miller witness
			return(0);
		}
		
		copyS = copyS - 1;
		
	}

	//clearing allocated memory
	mpz_clears(ans, num, NULL);

	//if we arrived here then a is a Miller witness
	return(1);

}

//returns 1 if n is prime, 0 if not, for n > 3
int isPrime(mpz_t n)
{
	//buffer variable
	mpz_t ans;
	mpz_init(ans);

	if(mpz_cmp_ui(n, 4) < 0)
	{
		//clears allocated memory
		mpz_clear(ans);

		fprintf(stderr, "isPrime : Error : n must be greater than 3\n");
		exit(-1);
	}
	//else if the input number n is odd
	else if(mpz_cdiv_q_ui(ans, n, 2) == 0)
	{
		//clears allocated memory
		mpz_clear(ans);

		return(0);
	}

	//clears ans
	mpz_clear(ans);


	mpz_t d, a;
	unsigned long int s = 0;
	mpz_inits(d, a, NULL);
	int i;

	if(decompose(&s, d, n) != 0)
	{
		//clears allocated memory
		mpz_clear(d);

		fprintf(stderr, "isPrime : Error\n");
		return(-1);
	}
	//if n is less than 68
	if(mpz_cmp_ui(n, 68) < 0)
	{
		unsigned long int sizeN = mpz_get_ui(n);
		//we test every number between [2; n-2] for witnesses
		for(i=2; i<(sizeN-2); i++)
		{
			mpz_set_ui(a, i);
			//if a is a Miller witness	
			if(isWitness(a, n, &s, d) == 1)
			{
				//clears allocated memory
				mpz_clears(d, a, NULL);

				//then n is not prime
				return(0);
			}	
		}


		//clears allocated memory
		mpz_clears(d, a, NULL);

		//if we did not find any witness
		//then n is probably prime
		return(1);
	}
	
	//if n is greater than 68 then the NIST recommend to do 64 iterations
	//we take a a randomly in [2; n-2]
	//We calculate the lenght of n in byte
	int nbBit = (strlen(mpz_get_str(NULL,2, n)))+ 1;
	char * seed;

		
	mpz_t seedMpz;
	mpz_t highLim;
	gmp_randstate_t state;
	gmp_randinit_default(state);

	mpz_inits(seedMpz, highLim, NULL);
	mpz_sub_ui(highLim, n, 3);

	//we open the /dev/urandom as our entropy source
	FILE * randomFic = NULL;
	randomFic = fopen("/dev/urandom", "r");

	//we test 64 random a for witnesses
	for(i=0; i<65; i++)
	{
		//we read a 32 bit number in /dev/urandom, to use as seed
		fread(&seed, 1, sizeof(unsigned long int), randomFic);
		mpz_set_ui(seedMpz, (unsigned long int)seed);
		gmp_randseed(state, seedMpz);

		//we generate our random number
		mpz_urandomb(a, state, nbBit);
		mpz_mod(a, a, highLim);
		mpz_add_ui(a, a, 2);
		
		//if a is a Miller witness
		if(isWitness(a, n, &s, d) == 1)
		{
			//clears allocated memory
			mpz_clears(seedMpz, highLim, d, a, NULL);
			close(randomFic);

			//then n is not prime
			return(0);
		}
		
		
	}	
	
	//clears allocated memory
	mpz_clears(a, d, highLim, seedMpz, NULL);
	close(randomFic);

	//if we arrive here, then n is probably prime
	return(1);
}
