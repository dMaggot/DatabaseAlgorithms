/*
 * FunctionalDependencyComparator.h
 *
 *  Created on: Jun 23, 2010
 *      Author: david
 */

#ifndef FUNCTIONALDEPENDENCYCOMPARATOR_H_
#define FUNCTIONALDEPENDENCYCOMPARATOR_H_

template<class T>
class FunctionalDependencyComparator {
public:
	bool operator () (const FunctionalDependency<T> &, const FunctionalDependency<T> &);
};

template<class T>
bool FunctionalDependencyComparator<T>::operator ()(const FunctionalDependency<T> & op1, const FunctionalDependency<T> & op2)
{
	return op1 < op2;
}

#endif /* FUNCTIONALDEPENDENCYCOMPARATOR_H_ */
