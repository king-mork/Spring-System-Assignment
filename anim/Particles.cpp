#include "Particles.h"

Particles::Particles(const std::string& name) :
	BaseSystem(name),
	m_sx(1.0f),
	m_sy(1.0f),
	m_sz(1.0f),
	n(0)
{
	//Initialize first particle
	m[0] = 0.0f;
	setVector(pos[0], 0, 0, 0);
	setVector(vel[0], 0, 0, 0);
	setVector(pPos[0], 0, 0, 0);
}	//Constructor

void Particles::setPosition(double* p, int i)
{
	VecCopy(pos[i], p);
}	// Set position

void Particles::reset(double time)
{
	for (int i = 0; i < n; i++) {
		setVector(pos[i], 0, 0, 0);
		setVector(vel[i], 0, 0, 0);
	}
		
}	// Reset position and velocity

void Particles::getVelocity(double* p, int i) {
	VecCopy(p, vel[i]);
}

void Particles::setVelocity(double* p, int i) {
	VecCopy(vel[i], p);
}

void Particles::getPosition(double* p, int i) {
	VecCopy(p, pos[i]);
}

GLMmodel* Particles::getModel() {
	return &m_model;
}

float Particles::getMass(int i) {
	return m[i];
}

void Particles::setMass(float t, int i) {
	m[i] = t; 
}

int Particles::getN() {
	return n;
}


void Particles::getPrevPosition(double* p, int i) {
	VecCopy(p, pPos[i]);
}

void Particles::setPrevPosition(double* p, int i) {
	VecCopy(pPos[i], p);
}


//****Command functions

void Particles::dimCommand(int t) {
	n = t;
	//Initialize particles
	for (int i = 0; i < n; i++) {
		m[i] = 0.0f;
		setVector(pos[i], 0, 0, 0);
		setVector(vel[i], 0, 0, 0);
	}
}


int Particles::command(int argc, myCONST_SPEC char** argv)
{
	if (argc < 1)
	{
		animTcl::OutputMessage("system %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "read") == 0)
	{
		if (argc == 2)
		{
			m_model.ReadOBJ(argv[1]);
			glmFacetNormals(&m_model);
			glmVertexNormals(&m_model, 90);
		}
		else
		{
			animTcl::OutputMessage("Usage: read <file_name>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "scale") == 0)
	{
		if (argc == 4)
		{
			m_sx = (float)atof(argv[1]);
			m_sy = (float)atof(argv[2]);
			m_sz = (float)atof(argv[3]);
		}
		else
		{
			animTcl::OutputMessage("Usage: scale <sx> <sy> <sz> ");
			return TCL_ERROR;

		}
	}
	else if (strcmp(argv[0], "pos") == 0)
	{
		if (argc == 5)
		{
			pos[std::stoi(argv[1])][0] = atof(argv[2]);
			pos[std::stoi(argv[1])][1] = atof(argv[3]);
			pos[std::stoi(argv[1])][2] = atof(argv[4]);
		}
		else
		{
			animTcl::OutputMessage("Usage: pos <index> <x> <y> <z> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "flipNormals") == 0)
	{
		flipNormals();
	}
	else if (strcmp(argv[0], "reset") == 0)
	{
		double p[3] = { 0,0,0 };
		for(int i = 0; i < n; i++)
			setPosition(p, i);
	}
	else if (strcmp(argv[0], "dim") == 0) {
		if (argc == 2)
		{
			dimCommand(std::stoi(argv[1]));
			animTcl::OutputMessage("dim: %s particles created!", argv[1]);
		}
		else {
			animTcl::OutputMessage("Usage: dim <numOfParticles> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "particle") == 0) {
		if (argc == 9)
		{
			int i = std::stoi(argv[1]);
			setMass(std::atof(argv[2]), i);
			Vector p = { std::atof(argv[3]), std::atof(argv[4]), std::atof(argv[5]) };
			setPosition(p, i);
			Vector v = { std::atof(argv[6]), std::atof(argv[7]), std::atof(argv[8]) };
			setVelocity(v, i);
			animTcl::OutputMessage("Particle: %s particles set!", argv[1]);
		}
		else {
			animTcl::OutputMessage("Usage: particle <index> <mass> <x y z vx vy vz> ");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "all_velocities") == 0) {
		if (argc == 4)
		{
			Vector v = { std::atof(argv[6]), std::atof(argv[7]), std::atof(argv[8]) };
			for(int i = 0; i < n; i++)
				setVelocity(v, i);
			animTcl::OutputMessage("all_velocities: velocities set!");
		}
		else {
			animTcl::OutputMessage("Usage: all_velocities <vx vy vz> ");
			return TCL_ERROR;
		}
	}

	glutPostRedisplay();
	return TCL_OK;

}	// Command Interpretter



void Particles::display(GLenum mode)
{
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	//glBegin(GL_POINTS);
	set_colour(0.0f, 1.0f, 0.0f);

	for (int i = 0; i < n; i++) {
		//glVertex3f(pos[i][0], pos[i][1], pos[i][2]);
		glPushMatrix();
		glTranslated(pos[i][0], pos[i][1], pos[i][2]);
		glutSolidSphere(0.1, 5, 10);
		glPopMatrix();
	}
	//glEnd();

	glPopAttrib();
}