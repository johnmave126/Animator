#include "beziercurveevaluator.h"

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	std::vector<Point> controlPts = ptvCtrlPts;
	if(bWrap) controlPts.push_back(Point(controlPts[0].x + fAniLength, controlPts[0].y));
	int size = controlPts.size();

	//for every four points, display bezier
	int i;
	for(i=3; i<controlPts.size(); i+=4) {
		Point points[4]={ controlPts[i-3], controlPts[i-2], controlPts[i-1], controlPts[i] };
		addBezier(ptvEvaluatedCurvePts, points);
	}

	i -= 4;
	while(++i<controlPts.size()) ptvEvaluatedCurvePts.push_back(controlPts[i]);

	if(bWrap) {
		if(size%4 != 0) {
			//draw linear wrap
			float y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[size - 1].x) + 
				  ptvCtrlPts[size - 1].y * ptvCtrlPts[0].x) /
				 (ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[size- 1].x);
			ptvEvaluatedCurvePts.push_back(Point(0.0, y1));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, y1));
		} else {
			//draw Bezier wrap
			std::vector<Point>::iterator it;
			for(it=ptvEvaluatedCurvePts.begin(); it!=ptvEvaluatedCurvePts.end(); it++) {
				if(it->x > fAniLength) it->x -= fAniLength;
			}
		}
	} else {
		int count = ptvEvaluatedCurvePts.size();
		ptvEvaluatedCurvePts.push_back(Point(0, ptvEvaluatedCurvePts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvEvaluatedCurvePts[size-1].y));
	}
}

bool BezierCurveEvaluator::flat(Point points[]) const
{
	float dis = 0.0;
	for(int i=0; i<3; i++) {
		dis += points[i].distance(points[i+1]);	
	}
	return (dis <= (1+s_fFlatnessEpsilon) * points[0].distance(points[3]));
}

void BezierCurveEvaluator::divide(Point P[], Point L[], Point R[]) const
{
	L[0] = P[0];
	L[1] = midPoint(P[0], P[1]);
	Point H = midPoint(P[1], P[2]);
	L[2] = midPoint(L[1], H);
	
	R[3] = P[3];
	R[2] = midPoint(P[2], P[3]);
	R[1] = midPoint(P[2], H);
	R[0] = midPoint(L[2], R[1]);
	L[3] = R[0];
}

void BezierCurveEvaluator::addBezier(std::vector<Point>& evaluatePoints, Point points[]) const
{
	if(flat(points)) {
		for(int i=0; i<4; i++) evaluatePoints.push_back(points[i]);
	} else {
		Point left[4], right[4];
		divide(points, left, right);
		addBezier(evaluatePoints, left);
		addBezier(evaluatePoints, right);
	}
}
