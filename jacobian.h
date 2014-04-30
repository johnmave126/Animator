#ifndef __JACOBIAN_HEADER__
#define __JACOBIAN_HEADER__

#include "linearalgebra.h"
#include "euclid.h"
#include "mathfunc.h"

using namespace MathFunc;
using Linear::RealMat;
using Linear::RealVec;

enum JacobState {
	NOT_INIT = 0,
	READY
};

class Jacobian {
public:
	Jacobian();
	Jacobian(int freedom);
	~Jacobian();

	void setInitVec(double x, double y, double z);

	void pushRotV(int varid, double x, double y, double z);
	void pushRotC(double angle, double x, double y, double z);
	//void pushTransV();
	void pushTransC(double x, double y, double z);
	void setConstraint(int varid, double min, double max);

	void setTrans(int i, int j, const ExprP& e);
	
	void preprocess();

	RealVec stepDelta(const RealVec& cTheta, const RealVec& desPos, double distance, bool &finished);
	
	RealVec evalTrans(const RealVec& theta);

private:
	
	Linear::Vec<ExprP> initPos;
	Linear::Vec<ExprP> Refined;

	Linear::Mat<ExprP> rawTrans;
	Linear::Mat<ExprP>* Jacob;
	int deg_freedom;

	RealVec* max_constraint;
	RealVec* min_constraint;
	
	JacobState state;
};

#endif
