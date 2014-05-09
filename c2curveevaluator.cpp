#include "c2curveevaluator.h"
#include <math.h>
#include "mat.h"
#include "vec.h"

void C2CurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts, 
		std::vector<Point>& ptvEvaluatedCurvePts, 
		const float& fAniLength, 
		const bool& bWrap) const
{
	int i, j;
	double coef_mat[GAUSSIAN_MAXN][GAUSSIAN_MAXN], tmp[GAUSSIAN_MAXN][GAUSSIAN_MAXN];
	double res_vecx[GAUSSIAN_MAXN], res_vecy[GAUSSIAN_MAXN];
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

	//Construct the matrix to solve
	for(i = 0; i < size; i++) {
		for(j = 0; j < size; j++) {
			coef_mat[i][j] = 0.0f;
		}
	}
	res_vecx[0] = 3 * (controlPts[1].x - controlPts[0].x);
	res_vecy[0] = 3 * (controlPts[1].y - controlPts[0].y);
	res_vecx[size - 1] = 3 * (controlPts[size - 1].x - controlPts[size - 2].x);
	res_vecy[size - 1] = 3 * (controlPts[size - 1].y - controlPts[size - 2].y);

	for(i = 1; i < size - 1; i++) {
		res_vecx[i] = 3 * (controlPts[i + 1].x - controlPts[i - 1].x);
		res_vecy[i] = 3 * (controlPts[i + 1].y - controlPts[i - 1].y);
	}
	coef_mat[0][0] = 2.0f; coef_mat[0][1] = 1.0f;
	coef_mat[size - 1][size - 1] = 2.0f; coef_mat[size - 1][size - 2] = 1.0f;
	for(i = 1; i < size - 1; i++) {
		coef_mat[i][i - 1] = 1.0f; coef_mat[i][i] = 4.0f; coef_mat[i][i + 1] = 1.0f;
	}

	memcpy(tmp, coef_mat, sizeof(double) * GAUSSIAN_MAXN * GAUSSIAN_MAXN);

	gaussTPivot(size, tmp, res_vecx);
	gaussTPivot(size, coef_mat, res_vecy);

	for(i=0; i+1 < size; i++) {
		//transform to bezier
		Point V[4];
		Point N[4];
		N[0] = controlPts[i];
		N[3] = controlPts[i+1];
		N[1] = Point(res_vecx[i], res_vecx[i]);
		N[2] = Point(res_vecx[i + 1], res_vecx[i + 1]);
		toBezier(N, V);
		//then display bezier curve
		addBezier(ptvEvaluatedCurvePts, V, 0.0f, fAniLength);
	}
}

void C2CurveEvaluator::toBezier(Point P[], Point V[]) const {
	int i;
	Mat4<float> Mcb(1.0, 0.0    , 0.0     , 0.0,
					1.0, 1.0 / 3, 0.0     , 0.0,
					0.0, 0.0    ,-1.0 / 3 , 1.0,
					0.0, 0.0    , 0.0     , 1.0);
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

bool C2CurveEvaluator::gaussTPivot(int n, double a[][GAUSSIAN_MAXN], double b[]) const {
	int i, j, k, row, col, index[GAUSSIAN_MAXN];
	double maxp, t;
	for (i = 0; i < n; i++) {
		index[i] = i;
	}
	for (k = 0; k < n; k++) {
		for (maxp = 0, i = k; i < n; i++) {
			for (j = k; j < n; j++) {
				if (fabs(a[i][j]) > fabs(maxp)) {
					maxp = a[row = i][col = j];
				}
			}
		}
		if (fabs(maxp) < GAUSSIAN_EPS) {
			return false;
		}
		if (col != k) {
			for (i = 0; i < n; i++) {
				swap(a[i][col], a[i][k]);
			}
			swap(index[col], index[k]);
		}
		if (row != k) {
			for (j = k; j < n; j++) {
				swap(a[k][j], a[row][j]);
			}
			swap(b[k], b[row]);
		}
		for (j = k + 1; j < n; j++) {
			a[k][j] /= maxp;
			for (i = k + 1; i < n; i++) {
				a[i][j] 
				-= a[i][k] * a[k][j];
			}
		}
		b[k] /= maxp;
		for (i = k + 1; i < n; i++) {
			b[i] 
			-= b[k] * a[i][k];
		}
	}
	for (i = n 
		- 1; i >= 0; i--) {
			for (j = i + 1; j < n; j++) {
				b[i] 
				-= a[i][j] * b[j];
			}
	}
	for (k = 0; k < n; k++) {
		a[0][index[k]] = b[k];
	}
	for (k = 0; k < n; k++) {
		b[k] = a[0][k];
	}
	return true;
}