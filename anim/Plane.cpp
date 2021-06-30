#include "Plane.h"

Plane::Plane(const std::string& name) : BaseSystem(name),
ks(1.0),
kd(0.0)
{
	setVector(p, 0, -5, 0);
	setVector(n, 0, 1, 0);
}

void Plane::getState(double* p) {
	VecCopy(p, this->p);
};

void Plane::setState(double* p) {
	VecCopy(this->p, p);
}

void Plane::getNormal(double* p) {
	VecCopy(p, n);
}

void Plane::setNormal(double* p) {
	VecCopy(n, p);
}

void Plane::setKs(double ts) {
	ks = ts;
}

void Plane::setKd(double td) {
	kd = td;
}

double Plane::getKs() {
	return ks;
}

double Plane::getKd() {
	return kd;
}

void Plane::display(GLenum mode) {
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glBegin(GL_LINES);
	glVertex3f(p[0], p[1], p[2]);
	glVertex3f(p[0]+100, p[1], p[2]);
	glVertex3f(p[0], p[1], p[2]);
	glVertex3f(p[0]-100, p[1], p[2]);
	glEnd();

	glPopMatrix();
	glPopAttrib();
}