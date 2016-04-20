#include "Particle.h"
#include <stdlib.h>     
#include <time.h>  

Particle::Particle() {
	x = 0;
	y = 0;
	weight = 1.0;
}

Particle::Particle(int Width, int Height) {
	double t1 = ((double)rand() / (double)RAND_MAX);
	double t2 = ((double)rand() / (double)RAND_MAX);

	x = t1 * Width;
	y = t2 * Height;
	weight = 1;
}

Particle::Particle(int X, int Y, int radius_x, int radius_y) {
	double t1 = 2 * ((double)rand() / (double)RAND_MAX);
	double t2 = 2 * ((double)rand() / (double)RAND_MAX);

	x = t1 * radius_x - radius_x;
	y = t2 * radius_y - radius_y;
	weight = 1;
}