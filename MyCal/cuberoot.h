#ifndef CUBEROOT_H_INCLUDED
#define CUBEROOT_H_INCLUDED

#include <math.h>

double cuberoot(double a)
{
	double x, y;
	x = a / 2;
	y = (x + x + a / (x*x)) / 3.0;
	do{
		x = y;
		y = (x + x + a / (x*x)) / 3.0;
	} while (fabs(x - y) >= 1.0e-5);
	return x;
}

#endif 
