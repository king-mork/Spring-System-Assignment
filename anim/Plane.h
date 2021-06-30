#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

#include "shared/opengl.h"

class Plane : public BaseSystem {
public:
	Plane(const std::string& name);
	~Plane() {};

	virtual void getState(double* p);
	virtual void setState(double* p);
	void getNormal(double* p);
	void setNormal(double* p);

	void setKs(double ts);
	void setKd(double td);
	double getKs();
	double getKd();

	void display(GLenum mode = GL_RENDER);

protected:
	//Penalty forces
	double ks;
	double kd;
	//Point
	Vector p;
	//Normal
	Vector n;
};
