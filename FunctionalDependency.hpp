#ifndef FUNCTIONALDEPENDENCY_H_
#define FUNCTIONALDEPENDENCY_H_

#include <set>
#include <string>
#include <iostream>
#include <utility>

#include "AttributeSet.hpp"

using namespace std;

template<class T>
class FunctionalDependency
{
private:
	AttributeSet<T> lhs;
	AttributeSet<T> rhs;

public:
	FunctionalDependency();
	FunctionalDependency(AttributeSet<T> &, AttributeSet<T> &);
	virtual ~FunctionalDependency();

	AttributeSet<T> & getLhs();
	AttributeSet<T> & getRhs();
	void setLhs(AttributeSet<T> &);
	void setRhs(AttributeSet<T> &);
	bool operator == (const FunctionalDependency<T> &) const;
	bool operator < (const FunctionalDependency<T> &) const;
};

template<class T>
FunctionalDependency<T>::FunctionalDependency()
{
}

template<class T>
FunctionalDependency<T>::FunctionalDependency(AttributeSet<T> & lhs, AttributeSet<T> & rhs) :
	lhs(lhs), rhs(rhs)
{

}

template<class T>
FunctionalDependency<T>::~FunctionalDependency()
{
}

template<class T>
AttributeSet<T> & FunctionalDependency<T>::getLhs()
{
	return lhs;
}

template<class T>
AttributeSet<T> & FunctionalDependency<T>::getRhs()
{
	return rhs;
}

template<class T>
void FunctionalDependency<T>::setLhs(AttributeSet<T> & lhs)
{
	this->lhs = lhs;

	return;
}

template<class T>
void FunctionalDependency<T>::setRhs(AttributeSet<T> & rhs)
{
	this->rhs = rhs;

	return;
}

template<class T>
bool FunctionalDependency<T>::operator ==(const FunctionalDependency & op2) const
{
	if (lhs == op2.lhs && rhs == op2.rhs)
		return true;

	return false;
}

template<class T>
bool FunctionalDependency<T>::operator <(const FunctionalDependency<T> & op2) const
{
	if (lhs < op2.lhs)
		return true;
	else
	{
		if (lhs == op2.lhs)
			if (rhs < op2.rhs)
				return true;
	}

	return false;
}

#endif /* FUNCTIONALDEPENDENCY_H_ */
