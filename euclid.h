#ifndef __EUCLID_HEADER__
#define __EUCLID_HEADER__

#include "linearalgebra.h"
#include <math.h>

#define SWAP(x, y) (tmp = x, x = y, y = tmp)

namespace Linear {
	const double eps = 1e-6f;

	class RealVec: public Vec<double> {
	public:
		RealVec();
		RealVec(int dim);
		RealVec(const RealVec& v);
		RealVec(const Vec<double>& v);

		double modulus() const;
		bool zero() const;
		RealVec norm() const;

	};
	
	class RealMat: public Mat<double> {
	public:
		RealMat();
		RealMat(int dim_m, int dim_n);
		RealMat(const RealMat& mm);
		RealMat(const Mat<double>& mm);

		static RealMat id(int dim);
		RealMat inv() const;
	};
};

#endif
