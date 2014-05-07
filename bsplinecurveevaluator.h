#ifndef BSPLINE_CURVE_EVALUATOR_H
#define BSPLINE_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "BezierCurveEvaluator.h"

//using namespace std;

class BsplineCurveEvaluator : public BezierCurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const;
	void toBezier(Point P[], Point V[]) const;
};

#endif