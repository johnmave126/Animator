#ifndef __LINEARALGEBRA_HEADER__
#define __LINEARALGEBRA_HEADER__

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#define SWAP(x, y) (tmp = x, x = y, y = tmp)

namespace Linear{
	template<typename T>
	class Vec;
	template<typename T>
	class Mat;

	template<typename T>
	class Vec {
	public:
		Vec(): dimension(0), data(NULL) {}
		Vec(int dim): dimension(dim), data(new T[dim]) {}
		Vec(const Vec& v): dimension(v.dimension), data(new T[v.dimension]) {
			int i;
			for(i = 0; i < dimension; i++) {
				data[i] = v.data[i];
			}
		}
		Vec& operator=(const Vec& v) {
			int i;
			if(data) {
				delete [] data;
			}
			dimension = v.dimension;
			data = new T[dimension];
			for(i = 0; i < dimension; i++) {
				data[i] = v.data[i];
			}
			return *this;
		}
		~Vec() {
			if(data) {
				delete [] data;
			}
		}

		Vec& operator+=(const Vec& v) {
			int i;
			assert(dimension == v.dimension);
			for(i = 0; i < dimension; i++) {
				data[i] += v.data[i];
			}
			return *this;
		}
		Vec& operator-=(const Vec& v) {
			int i;
			assert(dimension == v.dimension);
			for(i = 0; i < dimension; i++) {
				data[i] -= v.data[i];
			}
			return *this;
		}
		Vec& operator*=(const T scalar) {
			int i;
			for(i = 0; i < dimension; i++) {
				data[i] *= scalar;
			}
			return *this;
		}
		Vec& operator/=(const T scalar) {
			int i;
			for(i = 0; i < dimension; i++) {
				data[i] /= scalar;
			}
			return *this;
		}
		T& operator[](int idx) {
			assert(idx >= 0 && idx < dimension);
			return data[idx];
		}
		const T& operator[](int idx) const {
			assert(idx >= 0 && idx < dimension);
			return data[idx];
		}

		int dim() const {
			return dimension;
		}

		//Friends
		friend Vec operator-(const Vec& v) {
			Vec ret(v.dimension);
			int i;
			for(i = 0; i < v.dimension; i++) {
				ret.data[i] = -v.data[i];
			}
			return ret;
		}

		friend Vec operator-(const Vec& v1, const Vec& v2) {
			Vec ret(v1.dimension);
			assert(v1.dimension == v2.dimension);
			int i;
			for(i = 0; i < v1.dimension; i++) {
				ret.data[i] = v1.data[i] - v2.data[i];
			}
			return ret;
		}

		friend Vec operator+(const Vec& v1, const Vec& v2) {
			Vec ret(v1.dimension);
			assert(v1.dimension == v2.dimension);
			int i;
			for(i = 0; i < v1.dimension; i++) {
				ret.data[i] = v1.data[i] + v2.data[i];
			}
			return ret;
		}

		friend T operator*(const Vec& v1, const Vec& v2) {
			T ret = 0;
			assert(v1.dimension == v2.dimension);
			int i;
			for(i = 0; i < v1.dimension; i++) {
				ret = v1.data[i] * v2.data[i];
			}
			return ret;
		}

		friend Vec operator*(const T scalar, const Vec& v) {
			Vec ret(v.dimension);
			int i;
			for(i = 0; i < v.dimension; i++) {
				ret.data[i] = scalar * v.data[i];
			}
			return ret;
		}

		friend Vec operator*(const Vec& v, const T scalar) {
			Vec ret(v.dimension);
			int i;
			for(i = 0; i < v.dimension; i++) {
				ret.data[i] = scalar * v.data[i];
			}
			return ret;
		}

		friend Vec operator/(const Vec& v, const T scalar) {
			Vec ret(v.dimension);
			int i;
			for(i = 0; i < v.dimension; i++) {
				ret.data[i] = v.data[i] / scalar;
			}
			return ret;
		}

	protected:
		T* data;
		int dimension;
	};

	template<typename T>
	class Mat {
	public:
		Mat(): m(0), n(0), data(NULL) {}
		Mat(int dim_m, int dim_n): m(dim_m), n(dim_n), data(new T[dim_m * dim_n]) {}
		Mat(const Mat& mm): m(mm.m), n(mm.n), data(new T[mm.m * mm.n]) {
			int i;
			for(i = 0; i < mm.m * mm.n; i++) {
				data[i] = mm.data[i];
			}
		}

		~Mat() {
			if(data) {
				delete [] data;
			}
		}

		Mat& operator=(const Mat& mm) {
			int i;
			if(data) {
				delete [] data;
			}
			m = mm.m; n = mm.n;
			data = new T[m * n];
			for(i = 0; i < m * n; i++) {
				data[i] = mm.data[i];
			}
			return (*this);
		}

		T* operator[](const int idx) {
			return data + m * idx;
		}

		const T* operator[](const int idx) const {
			return data + m * idx;
		}

		Mat operator*(const Mat& mm) const {
			int i, j, k;
			assert(m == mm.n);
			Mat ret(mm.m, n);
			for(i = 0; i < n; i++) {
				for(j = 0; j < mm.m; j++) {
					assert(m > 0);
					ret[i][j] = data[i * m] * mm[0][j];
					for(k = 1; k < m; k++) {
						ret[i][j] += data[i * m + k] * mm[k][j];
					}
				}
			}
			return ret;
		}

		Mat& operator*=(const Mat& mm) {
			int i;
			Mat r = (*this) * mm;
			for(i = 0; i < m * n; i++) {
				data[i] = r.data[i];
			}
			return (*this);
		}

		Mat transpose() const {
			int i, j;
			Mat ret(n, m);
			for(i = 0; i < n; i++) {
				for(j = 0; j < m; j++) {
					ret[j][i] = data[i * m + j];
				}
			}
			return ret;
		}

		friend Vec<T> operator*(const Mat& m, const Vec<T>& v) {
			assert(m.m == v.dim());
			Vec<T> ret(m.n);
			int i, j;
			for(i = 0; i < m.n; i++) {
				assert(m.m > 0);
				ret[i] = m[i][0] * v[0];
				for(j = 1; j < m.m; j++) {
					ret[i] += m[i][j] * v[j];
				}
			}
			return ret;
		}

	protected:
		int m, n;
		T *data;
	};
	/*
	template<typename T>
	Vec<T> operator*(const Mat<T>& m, const Vec<T>& v) {
		assert(m.m == v.dimension);
		Vec<T> ret(m.n);
		int i, j;
		for(i = 0; i < m.n; i++) {
			assert(m.m > 0);
			ret[i] = m[i][0] * v[0];
			for(j = 1; j < m.m; j++) {
				ret[i] += m[i][j] * v[j];
			}
		}
		return ret;
	}*/

};

#endif
