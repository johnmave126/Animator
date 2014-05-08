#ifndef FORCE_H
#define FORCE_H
#include "vec.h"

class Force{
public:
	Force();
	Force(float x, float y, float z)
		:f(Vec3f(x, y, z)) {};
	Force(Vec3f fv)
		:f(fv) {};
	Vec3f f;
};
#endif