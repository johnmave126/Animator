 // SAMPLE_SOLUTION
#ifndef PARTICLE_H
#define PARTICLE_H
#include "vec.h"

class Particle{
public:
	Particle(float x, float y, float z)
		:velocity(Vec3f(x, y, z)), position(Vec3f(0, 0, 0)), force(Vec3f(0, 0, 0)){};

private:
	Vec3f velocity;
	Vec3f position;
	Vec3f force;
}
#endif