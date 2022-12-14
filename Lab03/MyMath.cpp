#include "MyMath.h"

float NormalizeAngle(float angle)
{
	while (angle > M_PI)
		angle -= 2 * M_PI;
	while (angle < -(M_PI))
		angle += 2 * M_PI;
	return angle;
}

double sgn(double val)
{
	return (0.0 < val) - (val < 0.0);
}