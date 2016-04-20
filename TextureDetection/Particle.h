#pragma once
class Particle {
public:
	double x;
	double y;
	double weight;

	Particle();
	Particle(int, int);
	Particle(int, int, int, int);
};