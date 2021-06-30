#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"

class Particles : public BaseSystem {
public:
	Particles(const std::string& name);

	virtual void getState(double* p) {
	//Unfortunately this needs to be defined
	};
	virtual void setState(double* p) {
	//Unfortunately this needs to be defined
	};
	void reset(double time);

	void display(GLenum mode = GL_RENDER);

	void readModel(char* fname) { m_model.ReadOBJ(fname); }
	void flipNormals(void) { glmReverseWinding(&m_model); }
	int command(int argc, myCONST_SPEC char** argv);

	void getVelocity(double* p, int i);
	void setVelocity(double* p, int i);
	void getPosition(double* p, int i);
	void setPosition(double* p, int i);
	float getMass(int i);
	void setMass(float t, int i);
	int getN();

	void getPrevPosition(double* p, int i);
	void setPrevPosition(double* p, int i);

	GLMmodel* getModel();

	//Assignment commands
	void dimCommand(int t);
protected:
	//Scale
	float m_sx;
	float m_sy;
	float m_sz;
	//Number of particles
	int n;
	//Mass
	float m[1000];
	//Current position
	Vector pos[1000];
	//Previous position (for verlet integration)
	Vector pPos[1000];
	//Current velocity
	Vector vel[1000];
	//Model
	GLMmodel m_model;
};
