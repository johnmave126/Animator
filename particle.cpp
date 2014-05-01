#include "particle.h"
#include "modelerdraw.h"

Particle::Particle()
	:mass(1.0), position(Vec3f(0,0,0)), velocity(Vec3f(0,0,0)) {};

void Particle::draw() {
	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		drawSphere(0.1);
	glPopMatrix();
}

bool Particle::toofar() {
	return position.length() > 20;
}

void Particle::update(Vec3f v, Vec3f p)
{
	velocity = v;
	position = p;
}