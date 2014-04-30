#include "euclid.h"

using namespace Linear;

RealVec::RealVec(): Vec<double>() {}
RealVec::RealVec(int dim): Vec<double>(dim) {}
RealVec::RealVec(const RealVec& v): Vec<double>(v) {}
RealVec::RealVec(const Vec<double>& v): Vec<double>(v) {}

double RealVec::modulus() const {
	int i;
	double result = 0;
	for(i = 0; i < dimension; i++) {
		result += data[i] * data[i];
	}
	return sqrt(result);
}

bool RealVec::zero() const {
	double len = this->modulus();
	return (len <= eps) && (len >= -eps);
}

RealVec RealVec::norm() const {
	int i;
	RealVec ret(dimension);
	double len = this->modulus();
	assert(!this->zero());
	for(i = 0; i < dimension; i++) {
		ret.data[i] /= len;
	}
	return ret;
}

RealMat::RealMat(): Mat<double>() {}
RealMat::RealMat(int dim_m, int dim_n): Mat<double>(dim_m, dim_n) {}
RealMat::RealMat(const RealMat& mm): Mat<double>(mm) {}
RealMat::RealMat(const Mat<double>& mm): Mat<double>(mm) {}

RealMat RealMat::id(int dim) {
	RealMat ret(dim, dim);
	int i;
	memset(ret.data, 0, sizeof(double) * dim * dim);
	for(i = 0; i < dim; i++) {
		ret[i][i] = 1.0;
	}
	return ret;
}

RealMat RealMat::inv() const {
	assert(m == n);
	RealMat ret = RealMat::id(m);
	double* p = new double[m * m];
	double tmp, coef;
	double max;
	int max_row;
	int i, j, k;
	memcpy(p, data, sizeof(double) * m * m);
	for(i = 0; i < m; i++) {
		if(fabs(p[i * m + i]) < eps) {
			max = p[i * m + i];
			max_row = i;
			for(j = i + 1; j < m; j++) {
				if(fabs(p[j * m + i]) > max) {
					max = fabs(p[j * m + i]);
					max_row = j;
				}
			}
			assert(max >= eps);

			for(j = 0; j < m; j++) {
				SWAP(p[max_row * m + j], p[i * m + j]);
				SWAP(ret[max_row][j], ret[i][j]);
			}
		}
	}
	for(i = 0; i < m; i++) {
		for(k = i + 1; k < m; k++) {
			coef = p[k * m + i] / p[i * m + i];
			for(j = 0; j < m; j++) {
				p[k * m + j] -= coef * p[i * m + j];
				ret[k][j] -= coef * ret[i][j];
			}
		}
		
		coef = p[i * m + i];
		for(j = 0; j < m; j++) {
			p[i * m + j] /= coef;
			ret[i][j] /= coef;
		}
	}
	for(i = m - 1; i >= 0; i--) {
		for(k = i - 1; k >= 0; k--) {
			coef = p[k * m + i] / p[i * m + i];
			p[k * m + i] = 0;
			for(j = 0; j < m; j++) {
				ret[k][j] -= coef * ret[i][j];
			}
		}
	}
	delete [] p;
	return ret;
}
