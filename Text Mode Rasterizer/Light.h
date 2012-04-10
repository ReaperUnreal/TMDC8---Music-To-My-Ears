#ifndef LIGHT_H
#define LIGHT_H

class Light
{
public:
	Light();
	virtual ~Light();

	Vector pos;
	float intensity;
};

#endif