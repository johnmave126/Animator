#pragma warning(disable : 4786)

#include "particleSystem.h"


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <limits.h>


/***************
 * Constructors
 ***************/

ParticleSystem::ParticleSystem() 
{
	simulate = false;
	bake_fps = 30; //bake 30 per seconds
	max_bake = 10000;
	number = 10;
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
}


/******************
 * Simulation fxns
 ******************/

/** Start the simulation */
void ParticleSystem::startSimulation(float t)
{
	bake_start_time = t;
	last_time = t;
	// These values are used by the UI ...
	// -ve bake_end_time indicates that simulation
	// is still progressing, and allows the
	// indicator window above the time slider
	// to correctly show the "baked" region
	// in grey.
	bake_end_time = -1;
	simulate = true;
	dirty = true;

}

/** Stop the simulation */
void ParticleSystem::stopSimulation(float t)
{
	bake_end_time = t;    
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Reset the simulation */
void ParticleSystem::resetSimulation(float t)
{
	// These values are used by the UI
	simulate = false;
	dirty = true;

}

/** Compute forces and update particles **/
void ParticleSystem::computeForcesAndUpdateParticles(float t)
{
	int i, bake_index;
	if(!simulate) {
		last_time =t; 
		return;
	}
	
	if(loadBaked(t)) {
		return;
	}

	for(i=0; i<number; i++) {
		Particle p;
		p.mass = 3.0;
		p.position = init_position;
		p.velocity = init_velocity;
		for(int j=0; j<3; j++) {
			p.position[j] += 0.1* (rand()%5 - 2.5);
			p.velocity[j] += 0.1* (rand()%10 - 5);
		}
		particles.push_back(p);
	}

	//Compute force on particles one by one
	std::vector<Particle>::iterator it;
	float dt = t - last_time;
	for(it = particles.begin(); it!=particles.end();) {
		if(it->toofar()) {
			it = particles.erase(it);
			continue;
		}

		std::vector<Force> ff;
		std::vector<Force>::iterator itf;
		for(itf = it->force.begin(); itf!= it->force.end(); itf++) {
			ff.push_back(*itf);
		}
		for(itf = fieldForce.begin(); itf!=fieldForce.end(); itf++) {
			ff.push_back(*itf);
		}

		rkProc(*it, ff, dt);
		it++;
	}

	//Bake the particles
	bakeParticles(t);
	
	last_time = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	if(!simulate) return;
	//Look for the bake index
	
	//search bake
	if(!loadBaked(t)) {
		computeForcesAndUpdateParticles(t);
	}
	std::vector<Particle>::iterator it;
	for(it = particles.begin(); it!=particles.end(); it++) {
		it->draw();
	}
}


/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{
	int bake_index = t * bake_fps;
	//bake particles
	if(storeBake.size() < max_bake) {
		storeBake[bake_index] = std::vector<Particle>(particles);
	}
}

/** Clears out your data structure of baked particles */
bool ParticleSystem::loadBaked(float t)
{
	int bake_index, i;
	//search bake
	bake_index = t * bake_fps;
	if(storeBake.count(bake_index)) {
		particles = storeBake[bake_index];
		return true;
	}
	//Or do a local search
	//Bear within 1 frames
	for(i = 1; i < 2; i++) {
		if(storeBake.count(bake_index + i)) {
			particles = storeBake[bake_index + i];
			return true;
		}
		if(storeBake.count(bake_index - i)) {
			particles = storeBake[bake_index - i];
			return true;
		}
	}
	return false;
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{
	storeBake.clear();
}

void ParticleSystem::rkProc(Particle& p, std::vector<Force>& f, float dt) {
	std::vector<Force>::iterator itf;

	Vec3f x1 = p.position;
	Vec3f v1 = p.velocity;
	Vec3f a1(0.0, 0.0, 0.0);
	for(itf = f.begin(); itf!= f.end(); itf++) {
		a1 += itf->getAcc(p);
	}
	
	Vec3f x2 = x1 + 0.5 * v1 * dt;
	Vec3f v2 = v1 + 0.5 * a1 * dt;
	Vec3f a2(0.0, 0.0, 0.0);
	p.position = x2; p.velocity = v2;
	for(itf = f.begin(); itf!= f.end(); itf++) {
		a2 += itf->getAcc(p);
	}
	
	Vec3f x3 = x2 + 0.5 * v2 * dt;
	Vec3f v3 = v2 + 0.5 * a2 * dt;
	Vec3f a3(0.0, 0.0, 0.0);
	p.position = x3; p.velocity = v3;
	for(itf = f.begin(); itf!= f.end(); itf++) {
		a3 += itf->getAcc(p);
	}
	
	Vec3f x4 = x3 + v3 * dt;
	Vec3f v4 = v3 + a3 * dt;
	Vec3f a4(0.0, 0.0, 0.0);
	p.position = x4; p.velocity = v4;
	for(itf = f.begin(); itf!= f.end(); itf++) {
		a4 += itf->getAcc(p);
	}
	
	Vec3f xf = x1 + (dt/6)*(v1 + 2*v2 + 2*v3 + v4);
	Vec3f vf = v1 + (dt/6)*(a1 + 2*a2 + 2*a3 + a4);

	p.position = x4; p.velocity = v4;
}

