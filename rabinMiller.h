#ifndef RABINMILLER_H
#define RABINMILLER_H
#include <gmp.h>

/*Decomposes an integer n in s and d like (n-1)=(2^s)*d*/
//takes:
//	mpz_t	n
//returns:
//	unsigned long int*	s
//	mpz_t			d
//	int return code
int decompose(unsigned long int* s, mpz_t d, mpz_t n);

/*Tests if a number a is a Miller witness of n*/
//takes:
//	mpz_t	a
//	mpz_t	n
//	mpz_t	s
//	mpz_t	d
//returns:
//	int	boolean answer
int isWitness(mpz_t a, mpz_t n, mpz_t s , mpz_t d);

/*Tests if a number n is prime*/
//takes:
//	mpz_t n
//returns:
//	int boolean answer
int isPrime(mpz_t n);

#endif
