/*rabinMiller.c*/
#include "rabinMiller.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


int decompose(unsigned long int* s, mpz_t d, mpz_t n)
{
	mpz_t ans;
	mpz_init(ans);
	unsigned long int modulo;
	

	//Tests if the input number n is odd
	if(mpz_cdiv_q_ui(ans, n, 2) == 0)
	{
		fprintf(stderr, "Error : n must be an odd number.\n");
		exit(1);
	}
	//If else tests if n is less than than 3
	else if(mpz_cmp_ui(n, 3) < 0)
	{
		fprintf(stderr, "Error : n must be greater than 2\n");
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

	*s = *s -1;
	mpz_cdiv_q_ui(d, num, 1 << *s);
	printf("s = %ld\n", *s); 
	printf("d = ");
	mpz_out_str(stdout, 10, d);
	printf("\n");
	
	mpz_clear(num);
	mpz_clear(ans);
}

int isWitness(mpz_t a, mpz_t n, mpz_t s , mpz_t d)
{

}

int isPrime(mpz_t n)
{

}
