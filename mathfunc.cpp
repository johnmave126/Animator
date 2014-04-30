#include "mathfunc.h"
#include <math.h>

using namespace MathFunc;

Expr::~Expr() {}
double Expr::operator()(const RealVec& v) const {
	return this->eval(v);
}

ExprP::ExprP(): expr(NULL) {}
ExprP::ExprP(const Expr& e): expr(e.nSelf()) {}
ExprP::ExprP(const ExprP& e): expr(e.expr->nSelf()) {}
ExprP::ExprP(Expr* e): expr(e) {}
ExprP::~ExprP() {
	if(expr) {
		delete expr;
	}
}

ExprP& ExprP::operator=(const ExprP& e) {
	if(expr) {
		delete expr;
	}
	expr = e.expr->nSelf();
	return (*this);
}

ExprP& ExprP::operator=(const Expr& e) {
	if(expr) {
		delete expr;
	}
	expr = e.nSelf();
	return (*this);
}

ExprP& ExprP::operator=(double scalar) {
	if(expr) {
		delete expr;
	}
	expr = new ConstFunc(scalar);
	return (*this);
}

ExprP& ExprP::operator+=(const ExprP& e) {
	expr = new Add(expr, e.expr->nSelf());
	return (*this);
}

ExprP& ExprP::operator+=(const Expr& e) {
	expr = new Add(expr, e.nSelf());
	return (*this);
}

ExprP& ExprP::operator+=(double scalar) {
	expr = new Add(expr, new ConstFunc(scalar));
	return (*this);
}

ExprP& ExprP::operator*=(const ExprP& e) {
	expr = new Mult(expr, e.expr->nSelf());
	return (*this);
}

ExprP& ExprP::operator*=(const Expr& e) {
	expr = new Mult(expr, e.nSelf());
	return (*this);
}

ExprP& ExprP::operator*=(double scalar) {
	expr = new Mult(expr, new ConstFunc(scalar));
	return (*this);
}

ExprP& ExprP::operator-=(const ExprP& e) {
	expr = new Minus(expr, e.expr->nSelf());
	return (*this);
}

ExprP& ExprP::operator-=(const Expr& e) {
	expr = new Minus(expr, e.nSelf());
	return (*this);
}

ExprP& ExprP::operator-=(double scalar) {
	expr = new Minus(expr, new ConstFunc(scalar));
	return (*this);
}

double ExprP::eval(const RealVec& v) const {
	return expr->eval(v);
}

double ExprP::operator()(const RealVec& v) const {
	return expr->eval(v);
}

ExprP ExprP::pd(int idx) const {
	return ExprP(expr->pd(idx));
}

ConstFunc::ConstFunc(): constant(0) {}
ConstFunc::ConstFunc(double cons): constant(cons) {}

double ConstFunc::eval(const RealVec& v) const {
	return constant;
}
Expr* ConstFunc::nSelf() const {
	return new ConstFunc(constant);
}
Expr* ConstFunc::pd(int idx) const {
	return new ConstFunc(0);
}

VarFunc::VarFunc(): var_id(0) {}
VarFunc::VarFunc(int id): var_id(id) {}

double VarFunc::eval(const RealVec& v) const {
	return this->eval(v[var_id]);
}

Expr* VarFunc::pd(int idx) const {
	if(var_id == idx) {
		return this->d();
	}
	else {
		return new ConstFunc(0);
	}
}

SinFunc::SinFunc(): VarFunc() {}
SinFunc::SinFunc(int id): VarFunc(id) {}

double SinFunc::eval(double x) const {
	return sin(x);
}

Expr* SinFunc::nSelf() const {
	return new SinFunc(var_id);
}

Expr* SinFunc::d() const {
	return new CosFunc(var_id);
}

CosFunc::CosFunc(): VarFunc() {}
CosFunc::CosFunc(int id): VarFunc(id) {}

double CosFunc::eval(double x) const {
	return cos(x);
}

Expr* CosFunc::nSelf() const {
	return new CosFunc(var_id);
}

Expr* CosFunc::d() const {
	return new unaryMinus(SinFunc(var_id));
}


XFunc::XFunc(): VarFunc() {}
XFunc::XFunc(int id): VarFunc(id) {}

double XFunc::eval(double x) const {
	return x;
}

Expr* XFunc::nSelf() const {
	return new XFunc(var_id);
}

Expr* XFunc::d() const {
	return new ConstFunc(1.0);
}

Minus::Minus(): l(NULL), r(NULL) {}
Minus::Minus(const Expr& a, const Expr& b): l(a.nSelf()), r(b.nSelf()) {}
Minus::Minus(Expr* a, Expr* b): l(a), r(b) {}
Minus::~Minus() {
	if(l) {
		delete l;
	}
	if(r) {
		delete r;
	}
}

double Minus::eval(const RealVec& v) const {
	return l->eval(v) - r->eval(v);
}

Expr* Minus::nSelf() const {
	return new Minus(*l, *r);
}

Expr* Minus::pd(int idx) const {
	return new Minus(l->pd(idx), r->pd(idx));
}

Add::Add(): l(NULL), r(NULL) {}
Add::Add(const Expr& a, const Expr& b): l(a.nSelf()), r(b.nSelf()) {}
Add::Add(Expr* a, Expr* b): l(a), r(b) {}
Add::~Add() {
	if(l) {
		delete l;
	}
	if(r) {
		delete r;
	}
}

double Add::eval(const RealVec& v) const {
	return l->eval(v) + r->eval(v);
}

Expr* Add::nSelf() const {
	return new Add(*l, *r);
}

Expr* Add::pd(int idx) const {
	return new Add(l->pd(idx), r->pd(idx));
}

Mult::Mult(): l(NULL), r(NULL) {}
Mult::Mult(const Expr& a, const Expr& b): l(a.nSelf()), r(b.nSelf()) {}
Mult::Mult(Expr* a, Expr* b): l(a), r(b) {}
Mult::~Mult() {
	if(l) {
		delete l;
	}
	if(r) {
		delete r;
	}
}

double Mult::eval(const RealVec& v) const {
	return l->eval(v) * r->eval(v);
}

Expr* Mult::nSelf() const {
	return new Mult(*l, *r);
}

Expr* Mult::pd(int idx) const {
	Expr *pdl, *pdr, *ret;
	pdl = l->pd(idx);
	pdr = r->pd(idx);
	ret = new Add(Mult(*pdl, *r), Mult(*l, *pdr));
	delete pdl;
	delete pdr;
	return ret;
}


unaryMinus::unaryMinus(): expr(NULL) {}
unaryMinus::unaryMinus(const Expr& a): expr(a.nSelf()) {}
unaryMinus::unaryMinus(Expr* a): expr(a) {}
unaryMinus::~unaryMinus() {
	if(expr) {
		delete expr;
	}
}

double unaryMinus::eval(const RealVec& v) const {
	return -expr->eval(v);
}

Expr* unaryMinus::nSelf() const {
	return new unaryMinus(*expr);
}

Expr* unaryMinus::pd(int idx) const {
	return new unaryMinus(expr->pd(idx));
}

//Operators
//ExprP x ExprP
ExprP MathFunc::operator-(const ExprP& a, const ExprP& b) {
	return ExprP(new Minus(*a.expr, *b.expr));
}

ExprP MathFunc::operator-(const ExprP& a) {
	return ExprP(new unaryMinus(*a.expr));
}

ExprP MathFunc::operator+(const ExprP& a, const ExprP& b) {
	return ExprP(new Add(*a.expr, *b.expr));
}

ExprP MathFunc::operator*(const ExprP& a, const ExprP& b) {
	return ExprP(new Mult(*a.expr, *b.expr));
}

//Expr x Expr
ExprP MathFunc::operator-(const Expr& a, const Expr& b) {
	return ExprP(new Minus(a, b));
}

ExprP MathFunc::operator-(const Expr& a) {
	return ExprP(new unaryMinus(a));
}

ExprP MathFunc::operator+(const Expr& a, const Expr& b) {
	return ExprP(new Add(a, b));
}

ExprP MathFunc::operator*(const Expr& a, const Expr& b) {
	return ExprP(new Mult(a, b));
}

//ExprP x Expr
ExprP MathFunc::operator-(const ExprP& a, const Expr& b) {
	return ExprP(new Minus(*a.expr, b));
}
ExprP MathFunc::operator-(const Expr& a, const ExprP& b) {
	return ExprP(new Minus(a, *b.expr));
}
ExprP MathFunc::operator+(const ExprP& a, const Expr& b) {
	return ExprP(new Add(*a.expr, b));
}
ExprP MathFunc::operator+(const Expr& a, const ExprP& b) {
	return ExprP(new Add(a, *b.expr));
}
ExprP MathFunc::operator*(const ExprP& a, const Expr& b) {
	return ExprP(new Mult(*a.expr, b));
}
ExprP MathFunc::operator*(const Expr& a, const ExprP& b) {
	return ExprP(new Mult(a, *b.expr));
}

//Scalar
//scalar x ExprP
ExprP MathFunc::operator-(const ExprP& a, double b) {
	return ExprP(new Minus(*a.expr, ConstFunc(b)));
}
ExprP MathFunc::operator-(double a, const ExprP& b) {
	return ExprP(new Minus(ConstFunc(a), *b.expr));
}
ExprP MathFunc::operator+(const ExprP& a, double b) {
	return ExprP(new Add(*a.expr, ConstFunc(b)));
}
ExprP MathFunc::operator+(double a, const ExprP& b) {
	return ExprP(new Add(ConstFunc(a), *b.expr));
}
ExprP MathFunc::operator*(const ExprP& a, double b) {
	return ExprP(new Mult(*a.expr, ConstFunc(b)));
}
ExprP MathFunc::operator*(double a, const ExprP& b) {
	return ExprP(new Mult(ConstFunc(a), *b.expr));
}

//scalar x Expr
ExprP MathFunc::operator-(const Expr& a, double b) {
	return ExprP(new Minus(a, ConstFunc(b)));
}
ExprP MathFunc::operator-(double a, const Expr& b) {
	return ExprP(new Minus(ConstFunc(a), b));
}
ExprP MathFunc::operator+(const Expr& a, double b) {
	return ExprP(new Add(a, ConstFunc(b)));
}
ExprP MathFunc::operator+(double a, const Expr& b) {
	return ExprP(new Add(ConstFunc(a), b));
}
ExprP MathFunc::operator*(const Expr& a, double b) {
	return ExprP(new Mult(a, ConstFunc(b)));
}
ExprP MathFunc::operator*(double a, const Expr& b) {
	return ExprP(new Mult(ConstFunc(a), b));
}
