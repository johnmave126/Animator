#ifndef BEZIER_CURVE_EVALUATOR_H
#define BEZIER_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class BezierCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const  ;
	void addBezier(std::vector<Point>& evaluatePoints, Point points[]) const;
	bool flat(Point points[]) const;
	void divide(Point P[], Point L[], Point R[]) const;
};

#endif