#ifndef COMPLEX_H
#define COMPLEX_H
#include "main.h"

class Complex
{
public:
	Complex(float real = 0, float complex = 0) : a(real), b(complex)
	{
	}
	Complex operator+(Complex &c);
	Complex operator-(Complex &c);
	Complex operator*(Complex &c);
	Complex operator/(Complex &c);
	void operator+=(Complex &c);
	void operator-=(Complex &c);
	void operator*=(Complex &c);
	void operator/=(Complex &c);
	float length(void);
	float sqrLength(void);
	Complex conjugate(void);
	float a, b;
};

#endif