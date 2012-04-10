#include "main.h"

Complex Complex::operator +(Complex &c)
{
	Complex ret(a + c.a, b + c.b);
	return ret;
}

Complex Complex::operator -(Complex &c)
{
	Complex ret(a - c.a, b - c.b);
	return ret;
}

Complex Complex::operator *(Complex &c)
{
	Complex ret(a * c.a - b * c.b, a * c.b + b * c.a);
	return ret;
}

Complex Complex::operator /(Complex &c)
{
	float div = c.a * c.a + c.b * c.b;
	Complex ret((a * c.a + b * c.b)/div, (b * c.a - a * c.b)/div);
	return ret;
}

void Complex::operator +=(Complex &c)
{
	a += c.a;
	b += c.b;
}

void Complex::operator -=(Complex &c)
{
	a -= c.a;
	b -= c.b;
}

void Complex::operator *=(Complex &c)
{
	a = a * c.a - b * c.b;
	b = a * c.b + b * c.a;
}

void Complex::operator /=(Complex &c)
{
	float div = c.a * c.a + c.b * c.b;
	a = (a * c.a + b * c.b)/div;
	b = (b * c.a - a * c.b)/div;
}

float Complex::length(void)
{
	return sqrtf(a*a + b*b);
}

float Complex::sqrLength(void)
{
	return a*a + b*b;
}

Complex Complex::conjugate(void)
{
	Complex ret(a, -b);
	return ret;
}