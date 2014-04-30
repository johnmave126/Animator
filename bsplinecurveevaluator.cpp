#include "bsplinecurveevaluator.h"

void BsplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	std::vector<Point> B = ptvCtrlPts;

	if(!bWrap) ptvEvaluatedCurvePts.push_back(Point(0.0, B[0].y));
	if(bWrap) {
		B.push_back(Point(B[0].x + fAniLength, B[0].y));
		B.push_back(Point(B[1].x + fAniLength, B[1].y));
	}
	int size = B.size();

	for(int i=0; i+3 < size; i++) {
		//first transform bspline to bezier
		Point V[4];
		V[0].x = (B[i].x + 4*B[i+1].x + B[i+2].x)/6;
		V[0].y = (B[i].y + 4*B[i+1].y + B[i+2].y)/6;
		V[1].x = (4*B[i+1].x + 2*B[i+2].x)/6;
		V[1].y = (4*B[i+1].y + 2*B[i+2].y)/6;
		V[2].x = (2*B[i+1].x + 4*B[i+2].x)/6;
		V[2].y = (2*B[i+1].y + 4*B[i+2].y)/6;
		V[3].x = (B[i+1].x + 4*B[i+2].x + B[i+3].x)/6;
		V[3].y = (B[i+1].y + 4*B[i+2].y + B[i+3].y)/6;
		//then display bezier curve
		addBezier(ptvEvaluatedCurvePts, V);
	}
	
	if(bWrap) {
		std::vector<Point>::iterator it;
		for(it=ptvEvaluatedCurvePts.begin(); it!=ptvEvaluatedCurvePts.end(); it++) {
			if(it->x > fAniLength) it->x -= fAniLength;
		}
	} else {
		int count = ptvEvaluatedCurvePts.size();
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvEvaluatedCurvePts[count-1].y));
	}
}