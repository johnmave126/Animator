#ifndef __MATHFUNC_HEADER__
#define __MATHFUNC_HEADER__

#include "euclid.h"

using Linear::RealVec;

namespace MathFunc {
	class ExprP;
	class Expr {
	public:
		virtual ~Expr();

		virtual double eval(const RealVec& v) const=0;
		double operator()(const RealVec& v) const;
		virtual Expr* nSelf() const=0;
		virtual Expr* pd(int idx) const=0;

		friend class ExprP;
	};

	class ExprP {
	public:
		ExprP();
		ExprP(const ExprP& e);
		ExprP(const Expr& e);
		ExprP(Expr* e);
		~ExprP();

		ExprP& operator=(const ExprP& e);
		ExprP& operator=(const Expr& e);
		ExprP& operator=(double scalar);
		
		ExprP& operator+=(const ExprP& e);
		ExprP& operator+=(const Expr& e);
		ExprP& operator+=(double scalar);
		
		ExprP& operator*=(const ExprP& e);
		ExprP& operator*=(const Expr& e);
		ExprP& operator*=(double scalar);
		
		ExprP& operator-=(const ExprP& e);
		ExprP& operator-=(const Expr& e);
		ExprP& operator-=(double scalar);

		double eval(const RealVec& v) const;
		double operator()(const RealVec& v) const;
		
		ExprP pd(int idx) const;
		
	private:
		Expr* expr;

		friend ExprP operator-(const ExprP& a, const ExprP& b);
		friend ExprP operator-(const ExprP& a);
		friend ExprP operator+(const ExprP& a, const ExprP& b);
		friend ExprP operator*(const ExprP& a, const ExprP& b);

		friend ExprP operator-(const ExprP& a, const Expr& b);
		friend ExprP operator-(const Expr& a, const ExprP& b);
		friend ExprP operator+(const ExprP& a, const Expr& b);
		friend ExprP operator+(const Expr& a, const ExprP& b);
		friend ExprP operator*(const ExprP& a, const Expr& b);
		friend ExprP operator*(const Expr& a, const ExprP& b);

		friend ExprP operator-(const ExprP& a, double b);
		friend ExprP operator-(double a, const ExprP& b);
		friend ExprP operator+(const ExprP& a, double b);
		friend ExprP operator+(double a, const ExprP& b);
		friend ExprP operator*(const ExprP& a, double b);
		friend ExprP operator*(double a, const ExprP& b);
	
		friend ExprP operator-(const Expr& a, double b);
		friend ExprP operator-(double a, const Expr& b);
		friend ExprP operator+(const Expr& a, double b);
		friend ExprP operator+(double a, const Expr& b);
		friend ExprP operator*(const Expr& a, double b);
		friend ExprP operator*(double a, const Expr& b);
	};

	//Constant Func
	class ConstFunc: public Expr {
	public:
		ConstFunc();
		ConstFunc(double cons);
		
		virtual double eval(const RealVec& v) const;
		virtual Expr* nSelf() const;
		virtual Expr* pd(int idx) const;

	private:
		double constant;
	};

	//Function assigned with one variable
	class VarFunc: public Expr {
	public:
		VarFunc();
		VarFunc(int id);
		
		virtual double eval(const RealVec& v) const;
		virtual double eval(double x) const=0;
		virtual Expr* d() const=0;
		virtual Expr* pd(int idx) const;

	protected:
		int var_id;
	};

	//Trigomatry
	class SinFunc: public VarFunc {
	public:
		SinFunc();
		SinFunc(int id);
		
		virtual double eval(double x) const;
		virtual Expr* nSelf() const;
		virtual Expr* d() const;
	};
	class CosFunc: public VarFunc {
	public:
		CosFunc();
		CosFunc(int var_id);
		
		virtual double eval(double x) const;
		virtual Expr* nSelf() const;
		virtual Expr* d() const;
	};

	// f(x) = x
	class XFunc: public VarFunc {
	public:
		XFunc();
		XFunc(int id);
		
		virtual double eval(double x) const;
		virtual Expr* nSelf() const;
		virtual Expr* d() const;
	};

	class Minus: public Expr {
	public:
		Minus();
		Minus(const Expr& a, const Expr& b);
		Minus(Expr* a, Expr* b);
		~Minus();
		
		virtual double eval(const RealVec& v) const;
		virtual Expr* nSelf() const;
		virtual Expr* pd(int idx) const;
		
	private:
		Expr *l, *r;
	};
	class Add: public Expr {
	public:
		Add();
		Add(const Expr& a, const Expr& b);
		Add(Expr* a, Expr* b);
		~Add();
		
		virtual double eval(const RealVec& v) const;
		virtual Expr* nSelf() const;
		virtual Expr* pd(int idx) const;
		
	private:
		Expr *l, *r;
	};
	class Mult: public Expr {
	public:
		Mult();
		Mult(const Expr& a, const Expr& b);
		Mult(Expr* a, Expr *b);
		~Mult();
		
		virtual double eval(const RealVec& v) const;
		virtual Expr* nSelf() const;
		virtual Expr* pd(int idx) const;
		
	private:
		Expr *l, *r;
	};
	class unaryMinus: public Expr {
	public:
		unaryMinus();
		unaryMinus(const Expr& a);
		unaryMinus(Expr* a);
		~unaryMinus();
		
		virtual double eval(const RealVec& v) const;
		virtual Expr* nSelf() const;
		virtual Expr* pd(int idx) const;
		
	private:
		Expr* expr;
	};

	ExprP operator-(const ExprP& a, const ExprP& b);
	ExprP operator-(const ExprP& a);
	ExprP operator+(const ExprP& a, const ExprP& b);
	ExprP operator*(const ExprP& a, const ExprP& b);

	ExprP operator-(const Expr& a, const Expr& b);
	ExprP operator-(const Expr& a);
	ExprP operator+(const Expr& a, const Expr& b);
	ExprP operator*(const Expr& a, const Expr& b);

	ExprP operator-(const ExprP& a, const Expr& b);
	ExprP operator-(const Expr& a, const ExprP& b);
	ExprP operator+(const ExprP& a, const Expr& b);
	ExprP operator+(const Expr& a, const ExprP& b);
	ExprP operator*(const ExprP& a, const Expr& b);
	ExprP operator*(const Expr& a, const ExprP& b);
	
	ExprP operator-(const ExprP& a, double b);
	ExprP operator-(double a, const ExprP& b);
	ExprP operator+(const ExprP& a, double b);
	ExprP operator+(double a, const ExprP& b);
	ExprP operator*(const ExprP& a, double b);
	ExprP operator*(double a, const ExprP& b);
	
	ExprP operator-(const Expr& a, double b);
	ExprP operator-(double a, const Expr& b);
	ExprP operator+(const Expr& a, double b);
	ExprP operator+(double a, const Expr& b);
	ExprP operator*(const Expr& a, double b);
	ExprP operator*(double a, const Expr& b);

}

#endif
