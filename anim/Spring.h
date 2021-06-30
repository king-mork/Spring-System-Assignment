#include "BaseSystem.h"
#include <shared/defs.h>
#include <util/util.h>
#include "animTcl.h"
#include <GLmodel/GLmodel.h>

class Spring {
public:
	Spring(float tk, float td, int t0, int t1, double rl);
	~Spring() {};

	float getK();
	float getD();
	int getP0();
	int getP1();
	double getRestLength();

protected:
	//Stiffness
	float k;
	//Dampness
	float d;
	//Indices of points
	int p0, p1;
	//Rest length
	double restLength;
};
