#ifndef INVSQRT_H_INCLUDED
#define INVSQRT_H_INCLUDED

double InvSqrt(double x)
{
	double xhalf = 0.5f*x;
	int i = *(int *)&x;
	i = 0x5f3759df - (i >> 1);
	x = *(double *)&i;
	x = x*(1.5f - xhalf*x*x);
	return x;
}

#endif