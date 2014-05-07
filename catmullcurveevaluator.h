#ifndef CATMULL_CURVE_EVALUATOR_H
#define CATMULL_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "BezierCurveEvaluator.h"

//using namespace std;

class CatmullCurveEvaluator : public BezierCurveEvaluator
{
public:
	CatmullCurveEvaluator(): BezierCurveEvaluator() {tension = 0.5; }
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const  ;
	void toBezier(Point P[], Point V[]) const;
	void setTension(float t) { tension = t; }

private:
	float tension;
};

#endif