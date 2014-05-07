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
	bake = new std::vector<Particle>[max_bake];
}


/*************
 * Destructor
 *************/

ParticleSystem::~ParticleSystem() 
{
	delete bake;
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

	if(!simulate) {
		last_time =t; 
		return;
	}
	
	//search bake
	int bake_index;
	bake_index = t*bake_fps;
	if(bake_index < max_bake && !bake[bake_index].empty()) {
		particles = bake[bake_index];
		return;
	}

	//inital force
	Vec3f force(0, 0, -5);

	//produce 20 each time
	number = 20;
	for(int i=0; i<number; i++) {
		Particle p;
		p.position = Vec3f(-0.1, 0.1, -1.5);
		for(int j=0; j<3; j++) {
			p.position[j] += 0.1* (rand()%5);
			p.velocity[j] += force[j] / p.mass + 0.1* (rand()%10);
		}
		particles.push_back(p);
	}

	//gravity force
	Vec3f gravity(0,-1,0);
	std::vector<Particle>::iterator it;
	float dt = t - last_time;
	for(it = particles.begin(); it!=particles.end();) {
		if(it->toofar()) {
			it = particles.erase(it);
			continue;
		}

		for(int j=0; j<3; j++)
		{
			it->position[j] += dt * it->velocity[j];
			it->velocity[j] += dt * gravity[j] / it->mass;
		}
		it++;
	}

	//bake particles
	if(bake_index < max_bake) {
		bake[bake_index] = std::vector<Particle>(particles);
	}
	
	last_time = t;
}


/** Render particles */
void ParticleSystem::drawParticles(float t)
{
	if(!simulate) return;
	std::vector<Particle>::iterator it;
	for(it = particles.begin(); it!=particles.end(); it++) {
		it->draw();
	}
}


/** Adds the current configuration of particles to
  * your data structure for storing baked particles **/
void ParticleSystem::bakeParticles(float t) 
{

	// TODO
}

/** Clears out your data structure of baked particles */
void ParticleSystem::clearBaked()
{

	// TODO
}





