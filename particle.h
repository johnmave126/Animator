 // SAMPLE_SOLUTION
#ifndef PARTICLE_H
#define PARTICLE_H
#include "vec.h"

class Particle{
public:
	Particle();
	Particle(float x, float y, float z)
		:velocity(Vec3f(x, y, z)), position(Vec3f(0, 0, 0)), mass(1.0){};
	void draw();
	void update(Vec3f velocity, Vec3f position);
	bool toofar();
	Vec3f velocity;
	Vec3f position;
	float mass;
};
#endif