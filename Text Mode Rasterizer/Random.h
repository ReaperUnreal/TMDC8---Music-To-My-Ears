#ifndef RANDOM_H
#define RANDOM_H

#include "main.h"

//uses the Marsaglia algorithm
class Random
{
public:
	static unsigned int rand(void);
	static float randf(void);
	static void rand_seed(unsigned int seed1, unsigned int seed2);
private:
	static int seed_x;
	static int seed_y;
	static const float divider;
};

#endif