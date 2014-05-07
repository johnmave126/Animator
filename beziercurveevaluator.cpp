#include "beziercurveevaluator.h"

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const
{
	ptvEvaluatedCurvePts.clear();
	std::vector<Point> controlPts = ptvCtrlPts;
	int size = ptvCtrlPts.size();
	int i;
	if(bWrap && size % 3 == 0) {
		int s = controlPts.size();
		controlPts.push_back(Point(controlPts[0].x + fAniLength, controlPts[0].y));
		for(i = 0; i < 3; i++) {
			controlPts.insert(controlPts.begin(), Point(controlPts[s - 1].x - fAniLength, controlPts[s - 1]. y));
		}
	}

	//for every four points, display bezier
	for(i=3; i<controlPts.size(); i+=3) {
		Point points[4]={ controlPts[i-3], controlPts[i-2], controlPts[i-1], controlPts[i] };
		addBezier(ptvEvaluatedCurvePts, points, fAniLength);
	}

	i -= 4;
	while(++i<controlPts.size()) ptvEvaluatedCurvePts.push_back(controlPts[i]);

	if(bWrap) {
		if(size % 3 != 0) {
			//draw linear wrap
			float y1 = (ptvCtrlPts[0].y * (fAniLength - ptvCtrlPts[size - 1].x) + 
				  ptvCtrlPts[size - 1].y * ptvCtrlPts[0].x) /
				 (ptvCtrlPts[0].x + fAniLength - ptvCtrlPts[size- 1].x);
			ptvEvaluatedCurvePts.push_back(Point(0.0, y1));
			ptvEvaluatedCurvePts.push_back(Point(fAniLength, y1));
		}
	} else {
		ptvEvaluatedCurvePts.push_back(Point(0, ptvCtrlPts[0].y));
		ptvEvaluatedCurvePts.push_back(Point(fAniLength, ptvCtrlPts[size-1].y));
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

void BezierCurveEvaluator::addBezier(std::vector<Point>& evaluatePoints, Point points[], float rightBorder) const
{
	if(flat(points)) {
		for(int i=0; i<4; i++) {
			if(points[0].x < 0 && points[i - 1].x < 0 && points[i].x >= 0) {
				//Interpolate
				float dx = points[i].x - points[i - 1].x;
				Point p;
				p.x = 0.0;
				p.y = points[i].x / dx * points[i - 1].y  - points[i - 1].x / dx * points[i].y;
				evaluatePoints.push_back(p);
			}
			if(points[3].x > rightBorder && points[i + 1].x > rightBorder && points[i].x <= rightBorder) {
				//Interpolate
				float dx = points[i + 1].x - points[i].x;
				float q = rightBorder - points[i].x;
				Point p;
				p.x = 0.0;
				p.y = q / dx * points[i + 1].y + (1 - q / dx) * points[i].y;
				evaluatePoints.push_back(p);
			}
			if(points[i].x >= 0 && points[i].x <= rightBorder) {
				evaluatePoints.push_back(points[i]);
			}
		}
	} else {
		Point left[4], right[4];
		divide(points, left, right);
		addBezier(evaluatePoints, left);
		addBezier(evaluatePoints, right);
	}
}
