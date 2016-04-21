#pragma once
class Particle {
public:
	double x;
	double y;
	double weight;
	double scale = 0;

	Particle();
	Particle(int, int);
	Particle(int, int, int, int);
	Particle(int, int, bool);
};