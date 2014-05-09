#ifndef C2_CURVE_EVALUATOR_H
#define C2_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "beziercurveevaluator.h"

//using namespace std;

#define GAUSSIAN_EPS (1e-6)
#define GAUSSIAN_MAXN (100)

class C2CurveEvaluator : public BezierCurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const  ;
	void toBezier(Point P[], Point V[]) const;
	bool gaussTPivot(int n, double a[][GAUSSIAN_MAXN], double b[]) const;
};

#endif