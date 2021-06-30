#include "Spring.h"

Spring::Spring(float tk, float td, int t0, int t1, double rl) : 
	k(tk),
	d(td),
	p0(t0),
	p1(t1),
	restLength(rl)
{}


float Spring::getK() {
	return k;
}

float Spring::getD() {
	return d;
}

int Spring::getP0() {
	return p0;
}

int Spring::getP1() {
	return p1;
}

double Spring::getRestLength() {
	return restLength;
}
