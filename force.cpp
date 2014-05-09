#include "force.h"
#include "particle.h"

Vec3f Force::getAcc(const Particle& p) const{
	return f / p.mass;
}
