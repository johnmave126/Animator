#include "jacobian.h"
#include "mathfunc.h"
#include "euclid.h"
#include "linearalgebra.h"
#include <vector>

#define M_PI (3.14159265)
#define CALC_EPS (1e-6)

Jacobian::Jacobian()
:deg_freedom(0), rawTrans(4, 4), Jacob(NULL), Refined(4),
max_constraint(NULL), min_constraint(NULL), initPos(4),
state(NOT_INIT) {}
Jacobian::Jacobian(int freedom)
:deg_freedom(freedom), rawTrans(4, 4), Jacob(NULL), Refined(4),
max_constraint(NULL), min_constraint(NULL), initPos(4),
state(NOT_INIT) {
	int i, j;
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			rawTrans[i][j] = (i == j);
		}
	}
	Jacob = new Linear::Mat<ExprP>(deg_freedom, 4);

	max_constraint = new RealVec(deg_freedom);
	min_constraint = new RealVec(deg_freedom);
	for(i = 0; i < deg_freedom; i++) {
		(*max_constraint)[i] = 2 * M_PI;
		(*min_constraint)[i] = 0;
	}

	initPos[0] = 0;
	initPos[1] = 0;
	initPos[2] = 0;
	initPos[3] = 1;
}

Jacobian::~Jacobian() {
	if(Jacob) {
		delete Jacob;
	}
	if(max_constraint) {
		delete max_constraint;
	}
	if(min_constraint) {
		delete min_constraint;
	}
}

void Jacobian::setInitVec(double x, double y, double z) {
	state = NOT_INIT;

	initPos[0] = x;
	initPos[1] = y;
	initPos[2] = z;
}

void Jacobian::pushRotV(int varid, double x, double y, double z) {
	RealVec axis(3);
	Linear::Mat<ExprP> rotMat(4, 4);
	ExprP c(new CosFunc(varid)), s(new SinFunc(varid));
	axis[0] = x; axis[1] = y; axis[2] = z;
	axis = axis.norm();
	x = axis[0]; y = axis[1]; z = axis[2];

	state = NOT_INIT;

	rotMat[0][0] = x * x * (1.0 - c) + c;
	rotMat[0][1] = x * y * (1.0 - c) - z * s;
	rotMat[0][2] = x * z * (1.0 - c) + y * s;
	rotMat[0][3] = 0;

	rotMat[1][0] = y * x * (1.0 - c) + z * s;
	rotMat[1][1] = y * y * (1.0 - c) + c;
	rotMat[1][2] = y * z * (1.0 - c) - x * s;
	rotMat[1][3] = 0;

	rotMat[2][0] = x * z * (1.0 - c) - y * s;
	rotMat[2][1] = y * z * (1.0 - c) + x * s;
	rotMat[2][2] = z * z * (1.0 - c) + c;
	rotMat[2][3] = 0;

	rotMat[3][0] = 0;
	rotMat[3][1] = 0;
	rotMat[3][2] = 0;
	rotMat[3][3] = 1;

	rawTrans *= rotMat;
}

void Jacobian::pushRotC(double angle, double x, double y, double z) {
	RealVec axis(3);
	Linear::Mat<ExprP> rotMat(4, 4);
	double c = cos(angle), s = sin(angle);
	axis[0] = x; axis[1] = y; axis[2] = z;
	axis = axis.norm();
	x = axis[0]; y = axis[1]; z = axis[2];
	
	state = NOT_INIT;

	rotMat[0][0] = x * x * (1.0 - c) + c;
	rotMat[0][1] = x * y * (1.0 - c) - z * s;
	rotMat[0][2] = x * z * (1.0 - c) + y * s;
	rotMat[0][3] = 0;

	rotMat[1][0] = y * x * (1.0 - c) + z * s;
	rotMat[1][1] = y * y * (1.0 - c) + c;
	rotMat[1][2] = y * z * (1.0 - c) - x * s;
	rotMat[1][3] = 0;

	rotMat[2][0] = x * z * (1.0 - c) - y * s;
	rotMat[2][1] = y * z * (1.0 - c) + x * s;
	rotMat[2][2] = z * z * (1.0 - c) + c;
	rotMat[2][3] = 0;

	rotMat[3][0] = 0;
	rotMat[3][1] = 0;
	rotMat[3][2] = 0;
	rotMat[3][3] = 1;

	rawTrans *= rotMat;
}

void Jacobian::pushTransC(double x, double y, double z) {
	Linear::Mat<ExprP> transMat(4, 4);
	
	state = NOT_INIT;

	transMat[0][0] = 1;
	transMat[0][1] = 0;
	transMat[0][2] = 0;
	transMat[0][3] = x;

	transMat[1][0] = 0;
	transMat[1][1] = 1;
	transMat[1][2] = 0;
	transMat[1][3] = y;

	transMat[2][0] = 0;
	transMat[2][1] = 0;
	transMat[2][2] = 1;
	transMat[2][3] = z;

	transMat[3][0] = 0;
	transMat[3][1] = 0;
	transMat[3][2] = 0;
	transMat[3][3] = 1;

	rawTrans *= transMat;
}

void Jacobian::setTrans(int i, int j, const ExprP& e) {
	rawTrans[i][j] = e;
}

void Jacobian::setConstraint(int varid, double min, double max) {
	assert(varid < deg_freedom && varid >= 0);
	(*max_constraint)[varid] = max;
	(*min_constraint)[varid] = min;
	state = NOT_INIT;
}

void Jacobian::preprocess() {
	int i, j;
	if(state == READY) {
		return;
	}
	Refined = rawTrans * initPos;

	for(i = 0; i < 4; i++) {
		for(j = 0; j < deg_freedom; j++) {
			(*Jacob)[i][j] = Refined[i].pd(j);
		}
	}

	state = READY;
}

RealVec Jacobian::stepDelta(const RealVec& cTheta, const RealVec& desPos, double distance, bool &finished) {
	int i, j;
	RealVec ret, ans;
	RealVec delta = desPos - evalTrans(cTheta);
	RealMat evJacob(deg_freedom, 4);
	RealMat trans;
	double mmin, mmax, l;

	assert(state == READY);

	for(i = 0; i < 4; i++) {
		for(j = 0; j < deg_freedom; j++) {
			evJacob[i][j] = (*Jacob)[i][j](cTheta);
		}
	}
	trans = evJacob.transpose() * evJacob;
	trans = trans.inv() * evJacob.transpose();

	ret = trans * delta;
	ans = cTheta + ret * distance;
	for(i = 0; i < deg_freedom; i++) {
		mmin = (*min_constraint)[i];
		mmax = (*max_constraint)[i];
		if(ans[i] > mmax + CALC_EPS) {
			if(cTheta[i] < mmax + CALC_EPS || ret[i] > CALC_EPS) {
				ret[i] = mmax - cTheta[i];
			}
		}
		else if(ans[i] < mmin - CALC_EPS) {
			if(cTheta[i] > mmin - CALC_EPS || ret[i] < -CALC_EPS) {
				ret[i] = mmin - cTheta[i];
			}
		}
	}

	l = ret.modulus();
	if(l <= CALC_EPS && l >= -CALC_EPS) {
		finished = true;
	}
	else {
		finished = false;
	}

	return cTheta + ret * distance;
}

RealVec Jacobian::evalTrans(const RealVec& theta){
	RealVec ret(4);
	int i;
	assert(state == READY);
	for(i = 0; i < 4; i++) {
		ret[i] = Refined[i](theta);
	}
	return ret;
}
