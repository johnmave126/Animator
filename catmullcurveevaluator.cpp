#include "catmullcurveevaluator.h"
#include "mat.h"
#include "vec.h"

void CatmullCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const
{
	int i, j;
	ptvEvaluatedCurvePts.clear();
	std::vector<Point> P = ptvCtrlPts;
	if(bWrap) {
		int s = P.size();
		P.insert(P.begin(), Point(P[s - 1].x - fAniLength, P[s - 1]. y));
		P.insert(P.begin(), Point(P[s - 1].x - fAniLength, P[s - 1]. y));
		P.push_back(Point(P[2].x + fAniLength, P[2].y));
		P.push_back(Point(P[3].x + fAniLength, P[3].y));
	}
	else {
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, P.back().y));
		ptvEvaluatedCurvePts.push_back(Point(0.0, P[0].y));
	}

	//For simplicity, add 2 points at the beginning and the end
	P.insert(P.begin(), P[0]);
	P.push_back(P.back());

	int size = P.size();

	for(i = 0; i+3 < size; i++) {
		Point V[4];
		Point N[4];
		for(j = 0; j < 4; j++) {
			N[j] = P[i + j];
		}
		toBezier(N, V);
		addBezier(ptvEvaluatedCurvePts, V, fAniLength);
	}
}

void CatmullCurveEvaluator::toBezier(Point P[], Point V[]) const {
	int i;
	float t = tension / 3.0;
	Mat4<float> Mcb(0.0, 1.0, 0.0, 0.0,
					-t , 1.0, t  , 0.0,
					0.0, t  , 1.0, -t,
					0.0, 0.0, 1.0, 0.0);
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
