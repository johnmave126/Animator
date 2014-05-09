#ifndef PARTICLE_H
#define PARTICLE_H
#include "vec.h"
#include "camera.h"
#include <vector>

class Force;

class Particle{
public:
	Particle();
	Particle(float x, float y, float z)
		:velocity(Vec3f(x, y, z)), position(Vec3f(0, 0, 0)), mass(1.0){};
	void draw(Camera* cam);
	void update(Vec3f velocity, Vec3f position);
	bool toofar();
	void attachForce(Force f);
	Vec3f velocity;
	Vec3f position;
	float mass;
	std::vector<Force> force;
	friend class Force;
};
#endif