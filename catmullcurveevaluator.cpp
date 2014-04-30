#include "catmullcurveevaluator.h"

void CatmullCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	std::vector<Point> P = ptvCtrlPts;
	if(bWrap) P.push_back(Point(P[0].x + fAniLength, P[0].y));

	if(!bWrap) ptvEvaluatedCurvePts.push_back(Point(0.0, P[0].y));
	if(bWrap) P.push_back(Point(P[0].x + fAniLength, P[0].y));
	int size = P.size();

	for(int i=0; i+3 < size; i++) {
		Point V[4];
		V[0].x = (-P[i].x + 3*P[i+1].x - 3*P[i+2].x + P[i+3].x)/2;
		V[0].y = (-P[i].y + 3*P[i+1].y - 3*P[i+2].y + P[i+3].y)/2;
		V[1].x = (2*P[i].x - 5*P[i+1].x + 4*P[i+2].x - P[i+3].x)/2;
		V[1].y = (2*P[i].y - 5*P[i+1].y + 4*P[i+2].y - P[i+3].y)/2;
		V[2].x = (-P[i].x + P[i+2].x)/2;
		V[2].y = (-P[i].y + P[i+2].y)/2;
		V[3] = P[i+1];

		for(float t=0; t<1; t+= 0.01) {
			float a=t*t*t;
			float b=t*t;
			float c=t;
			float d=1;
			float x=a*V[0].x + b*V[1].x + c*V[2].x + d*V[3].x;
			float y=a*V[0].y + b*V[1].y + c*V[2].y + d*V[3].y;
			ptvEvaluatedCurvePts.push_back(Point(x, y));
		}
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
