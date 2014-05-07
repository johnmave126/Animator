#include "bsplinecurveevaluator.h"
#include "mat.h"
#include "vec.h"

void BsplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const
{
	int i, j;
	ptvEvaluatedCurvePts.clear();
	std::vector<Point> controlPts = ptvCtrlPts;

	if(bWrap) {
		int s = controlPts.size();
		controlPts.insert(controlPts.begin(), Point(controlPts[s - 1].x - fAniLength, controlPts[s - 1]. y));
		controlPts.insert(controlPts.begin(), Point(controlPts[s - 1].x - fAniLength, controlPts[s - 1]. y));
		controlPts.push_back(Point(controlPts[2].x + fAniLength, controlPts[2].y));
		controlPts.push_back(Point(controlPts[3].x + fAniLength, controlPts[3].y));
	}
	else {
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, controlPts.back().y));
		ptvEvaluatedCurvePts.push_back(Point(0.0, controlPts[0].y));
	}
	int size = controlPts.size();

	for(i=0; i+3 < size; i++) {
		//first transform bspline to bezier
		Point V[4];
		Point N[4];
		for(j = 0; j < 4; j++) {
			N[j] = controlPts[i + j];
		}
		toBezier(N, V);
		//then display bezier curve
		addBezier(ptvEvaluatedCurvePts, V, fAniLength);
	}
}

void BsplineCurveEvaluator::toBezier(Point P[], Point V[]) const {
	int i;
	Mat4<float> Mcb(1.0 / 6, 2.0 / 3, 1.0 / 6, 0.0,
					0.0    , 2.0 / 3, 1.0 / 3, 0.0,
					0.0    , 1.0 / 3, 2.0 / 3, 0.0,
					0.0    , 1.0 / 6, 2.0 / 3, 1.0 / 6);
	Vec4<float> vx, vy;
	for(i = 0; i < 4; i++) {
		vx[i] = P[i].x;
		vy[i] = P[i].y;
	}
	vx = Mcb * vx;
	vy = Mcb * vy;
	
	for(i = 0; i < 4; i++) {
		V[i].x = vx[i];
		V[i].y = vy[i];
	}
}
