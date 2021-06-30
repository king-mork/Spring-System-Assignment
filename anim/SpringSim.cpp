#include "SpringSim.h"


SpringSim::SpringSim(const std::string& name, BaseSystem* target) :
	BaseSimulator(name),
	ground((Plane*)target),
	particles(NULL),
	springNum(0),
	g(-9.8),
	kdrag(0.1),
	fixed{},
	springs(NULL),
	first(0.01),
	dt(0.01)
{ 
	method = Euler;
}

//Returns whether or not a particle at index i is fixed or not
boolean SpringSim::isFixed(int i) {
	if (fixed[i] == 1)
		return true;
	return false;
}


//***Integration Methods

void SpringSim::eulerVelocity(double* vel, double* iVel, double Fx, double Fy, double Fz, double m, double dt) {
	//New velocity
	vel[0] = iVel[0] + dt * Fx / m;
	vel[1] = iVel[1] + dt * Fy / m;
	vel[2] = iVel[2] + dt * Fz / m;
}

void SpringSim::eulerPosition(double* pos, double* iPos, double* iVel, double dt) {
	//New position
	pos[0] = iPos[0] + dt * iVel[0];
	pos[1] = iPos[1] + dt * iVel[1];
	pos[2] = iPos[2] + dt * iVel[2];
}

void SpringSim::verletVelocity(double* vel, double* pos, double* pPos, double dt) {
	//New velocity
	vel[0] = (pos[0] - pPos[0]) / (2.0 * dt);
	vel[1] = (pos[1] - pPos[1]) / (2.0 * dt);
	vel[2] = (pos[2] - pPos[2]) / (2.0 * dt);
}


void SpringSim::verletPosition(double* pos, double* iPos, double* pPos, double Fx, double Fy, double Fz, double m, double dt) {

	//New position
	pos[0] = (2.0 * iPos[0]) - pPos[0] + ((Fx / m) * pow(dt, 2));
	pos[1] = (2.0 * iPos[1]) - pPos[1] + ((Fy / m) * pow(dt, 2));
	pos[2] = (2.0 * iPos[2]) - pPos[2] + ((Fz / m) * pow(dt, 2));
}


//***Command Functions

void SpringSim::linkCommand(std::string sname, int s) {
	//Set number of springs
	springNum = s;
	//Link particles
	BaseSystem* tmp = GlobalResourceManager::use()->getSystem(sname);
	particles = (Particles*)tmp;
}

void SpringSim::gravityCommand(float tmp) {
	g = (double)tmp;
}

void SpringSim::dragCommand(float tmp) {
	kdrag = (double)tmp;
}

void SpringSim::springCommand(int i1, int i2, float s, float d, double rLen) {
	//If supplied restlength is negative, set to distance between the two particles
	if (rLen < 0) {
		Vector p0, p1;
		particles->getPosition(p0, i1);
		particles->getPosition(p1, i2);
		rLen = sqrt(pow(p1[0] - p0[0], 2) + pow(p1[1] - p0[1], 2) + pow(p1[2] - p0[2], 2));
	}
	//Create Spring object
	Spring* tmp= new Spring( s, d, i1, i2, rLen );
	//Add to list of springs
	springs.push_back(tmp);
}


int SpringSim::command(int argc, myCONST_SPEC char** argv) {
	if (argc < 1)
	{
		animTcl::OutputMessage("simulator %s: wrong number of params.", m_name.c_str());
		return TCL_ERROR;
	}
	else if (strcmp(argv[0], "link") == 0) {
		if (argc == 3) {
			linkCommand(argv[1], std::atoi(argv[2]));
			animTcl::OutputMessage("system %s linked - %d spring(s)!", argv[1], std::atoi(argv[2]));
		}
		else {
			animTcl::OutputMessage("Usage: link <System> <Number of Springs>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "gravity") == 0) {
		if (argc == 2) {
			gravityCommand(std::stof(argv[1]));
			animTcl::OutputMessage("Gravity set to %f", std::stof(argv[1]));
		}
		else {
			animTcl::OutputMessage("Usage: gravity <g>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "drag") == 0) {
		if (argc == 2) {
			dragCommand(std::stof(argv[1]));
			animTcl::OutputMessage("Drag set to %f", std::stof(argv[1]));
		}
		else {
			animTcl::OutputMessage("Usage: drag <kdrag>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "fix") == 0) {
		if (argc == 2) {
			fixed[std::atoi(argv[1])] = 1;
			animTcl::OutputMessage("Particle %d fixed!", std::atoi(argv[1]));
		}
		else {
			animTcl::OutputMessage("Usage: fix <index>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "spring") == 0) {
		if (argc == 6) {
			springCommand(std::atoi(argv[1]), std::atoi(argv[2]), std::stof(argv[3]), std::stof(argv[4]), std::stof(argv[5]));
			animTcl::OutputMessage("Spring on particles %d and %d", std::atoi(argv[1]), std::atoi(argv[2]));
		}
		else {
			animTcl::OutputMessage("Usage: spring <index1> <index2> <ks> <kd> <restlength>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "integration") == 0) {
		if (argc == 3) {
			if (strcmp(argv[1], "euler") == 0) {
				method = Euler;
			}
			else if (strcmp(argv[1], "symplectic") == 0) {
				method = Symplectic;
			}
			else if (strcmp(argv[1], "verlet") == 0) {
				method = Verlet;
			}
			dt = std::stof(argv[2]);
			animTcl::OutputMessage("Integration method set to %s - dt = %f", argv[1], std::stof(argv[2]));
		}
		else {
			animTcl::OutputMessage("Usage: integration <euler|symplectic|verlet> <time step>");
			return TCL_ERROR;
		}
	}
	else if (strcmp(argv[0], "ground") == 0) {
		if (argc == 3) {
			ground->setKs(std::stof(argv[1]));
			ground->setKd(std::stof(argv[2]));
		}
		else {
			animTcl::OutputMessage("Usage: ground <ks> <kd>");
			return TCL_ERROR;
		}
	}

	return TCL_OK;
}

int SpringSim::step(double time)
{
	int n = particles->getN();

	//For every particle
	for (int i = 0; i < n; i++) {
		//If it is not fixed: Calculate changes
		if (!isFixed(i)) {

			Vector iPos, iVel;
			Vector pos{0,0,0}, vel{0,0,0};

			//Current position & velocity
			particles->getPosition(iPos, i);
			particles->getVelocity(iVel, i);

			//Calculate total force acting on particle
			double m = particles->getMass(i);
				//Drag + Gravity (only has a y component)
			double Fx = (-1.0 * kdrag * iVel[0]);
			double Fy = (-1.0 * kdrag * iVel[1]) + (m * g);
			double Fz = (-1.0 * kdrag * iVel[2]);
				//Springs
			for (int j = 0; j < springs.size(); j++) {
				//Check for associated springs
				if (springs[j]->getP0() == i || springs[j]->getP1() == i) {
					double FsX, FsY, FsZ;
					double FdX, FdY, FdZ;
					double ks = springs[j]->getK(), rl = springs[j]->getRestLength(), kd = springs[j]->getD();
					//Get other particle position and velocity
					Vector p1;
					Vector v1;
					if (springs[j]->getP0() == i) {
						particles->getPosition(p1, springs[j]->getP1());
						particles->getVelocity(v1, springs[j]->getP1());
					}
					else {
						particles->getPosition(p1, springs[j]->getP0());
						particles->getVelocity(v1, springs[j]->getP0());
					}

					double l = sqrt(pow(p1[0] - iPos[0], 2) + pow(p1[1] - iPos[1], 2) + pow(p1[2] - iPos[2], 2));
					//Spring force
					FsX = ks * (rl - l) * ((iPos[0] - p1[0]) / l);
					FsY = ks * (rl - l) * ((iPos[1] - p1[1]) / l);
					FsZ = ks * (rl - l) * ((iPos[2] - p1[2]) / l);
					//Damping force
					FdX = kd * ( (v1[0] - iVel[0]) * ((iPos[0] - p1[0]) / l) ) * ((iPos[0] - p1[0]) / l);
					FdY = kd * ( (v1[1] - iVel[1]) * ((iPos[1] - p1[1]) / l) ) * ((iPos[1] - p1[1]) / l);
					FdZ = kd * ( (v1[2] - iVel[2]) * ((iPos[2] - p1[2]) / l) ) * ((iPos[2] - p1[2]) / l);
					//Add to total force
					Fx += (FsX + FdX);
					Fy += (FsY + FdY);
					Fz += (FsZ + FdZ);
				}
			}

			//New Velocity and Position via Integration methods
			if (method == 1) {	//Symplectic
				eulerVelocity(vel, iVel, Fx, Fy, Fz, m, dt);
				eulerPosition(pos, iPos, vel, dt);
			}
			else if (method == 2) {	//Verlet
				Vector pPos, revVel;
				particles->getPrevPosition(pPos, i);
				if (time == first) {	//Case: first execution; no previous positions
					//Reverse the velocity
					revVel[0] = -1.0 * iVel[0];
					revVel[1] = -1.0 * iVel[1];
					revVel[2] = -1.0 * iVel[2];
					//Euler interpret previous position
					eulerPosition(pPos, iPos, revVel, dt);
				}
				verletPosition(pos, iPos, pPos, Fx, Fy, Fz, m, dt);
				verletVelocity(vel, pPos, pPos, dt);
			}
			else {					//Euler
				eulerVelocity(vel, iVel, Fx, Fy, Fz, m, dt);
				eulerPosition(pos, iPos, iVel, dt);
			}

			//Check for collision
			Vector p, n;
			ground->getState(p);
			ground->getNormal(n);
			//x-p
			Vector tmp;
			tmp[0] = pos[0] - p[0];
			tmp[1] = pos[1] - p[1];
			tmp[2] = pos[2] - p[2];
			//dot((x-p), n)
			tmp[0] = tmp[0] * n[0];
			tmp[1] = tmp[1] * n[1];
			tmp[2] = tmp[2] * n[2];
			double dProd = tmp[0] + tmp[1] + tmp[2];
			//Collision resolution
			if (dProd < 0.1) {
				//Ground penalties
				double kd = ground->getKd(), ks = ground->getKs();
				double Fground = kd + ks;

				//New Velocity and Position via Integration methods
				if (method == 1) {	//Symplectic
					eulerVelocity(vel, iVel, 0, Fground, 0, m, dt);
					eulerPosition(pos, iPos, vel, dt);
				}
				else if (method == 2) {	//Verlet
					Vector pPos;
					particles->getPrevPosition(pPos, i);
					verletPosition(pos, iPos, pPos, 0, Fground, 0, m, dt);
					verletVelocity(vel, pPos, pPos, dt);
				}
				else {					//Euler
					eulerVelocity(vel, iVel, 0, Fground, 0, m, dt);
					eulerPosition(pos, iPos, iVel, dt);
				}
				//New Velocity
				//eulerVelocity(vel, vel, 0.0, Fground, 0.0, m, dt);
				//New Position
				//eulerPosition(pos, pos, vel, dt);
			}

			//Update velocity and position
			particles->setPosition(pos, i);
			particles->setVelocity(vel, i);
			//Store previous position
			particles->setPrevPosition(iPos, i);
		}
		
	}

	return 0;

}	// Step function


void SpringSim::display(GLenum mode) {
	glEnable(GL_LIGHTING);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glPushAttrib(GL_ALL_ATTRIB_BITS);

	glBegin(GL_LINES);
	set_colour(1.0f, 1.0f, 1.0f);

	//Draw springs
	for (int i = 0; i < springs.size(); i++) {
		//Get points
		int p0 = springs[i]->getP0(), p1 = springs[i]->getP1();
		//Get point coordinates
		Vector pos0, pos1;
		particles->getPosition(pos0, p0);
		particles->getPosition(pos1, p1);
		//Draw line
		glVertex3f(pos0[0], pos0[1], pos0[2]);
		glVertex3f(pos1[0], pos1[1], pos1[2]);
	}

	glEnd();

	glPopMatrix();
	glPopAttrib();
}