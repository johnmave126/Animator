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

	Vec3f gravity(0,-1,0);
	for(i=0; i<number; i++) {
		Particle p;
		p.position = init_position;
		p.velocity = init_velocity;
		p.attachForce(Force(gravity));
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
		Vec3f F(0.0, 0.0, 0.0);
		std::vector<Force>::iterator itf;
		for(itf = it->force.begin(); itf!= it->force.end(); itf++) {
			F += itf->f;
		}

		for(int j=0; j<3; j++)
		{
			it->position[j] += dt * it->velocity[j];
			it->velocity[j] += dt * F[j] / it->mass;
		}
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

