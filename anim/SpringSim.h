#include <GLModel/GLModel.h>
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include "BaseSimulator.h"
#include "BaseSystem.h"
#include "Particles.h"
#include "Plane.h"
#include "GlobalResourceManager.h"
#include "Spring.h"

#include <vector>
#include <string>

#define Euler 0;
#define Symplectic 1;
#define Verlet 2;


class SpringSim : public BaseSimulator {
public:
	SpringSim(const std::string& name, BaseSystem* target);
	~SpringSim() {};

	int step(double time);
	int init(double time) { return 0; };

	int command(int argc, myCONST_SPEC char** argv);

	void linkCommand(std::string sname, int s);
	void gravityCommand(float tmp);
	void dragCommand(float tmp);
	void springCommand(int i1, int i2, float s, float d, double rLen);

	boolean isFixed(int i);

	void eulerPosition(double* pos, double* iPos, double* vel, double dt);
	void eulerVelocity(double* vel, double* iVel, double Fx, double Fy, double Fz, double m, double dt);
	void verletPosition(double* pos, double* iPos, double* pPos, double Fx, double Fy, double Fz, double m, double dt);
	void verletVelocity(double* vel, double* pos, double* pPos, double dt);

	void display(GLenum mode = GL_RENDER);

protected:
	//Time step
	double dt;
	//Time of the first step execution
	double first;
	//Integration Method
	int method;
	//Acceleration due to gravity
	double g;
	//Number of springs in system
	int springNum;
	//Global drag/friction coefficient
	double kdrag;
	//Fixed particles
	int fixed[1000];
	//Objects
	Particles* particles;
	Plane* ground;
	//Track springs in scene
	std::vector<Spring*> springs;
};