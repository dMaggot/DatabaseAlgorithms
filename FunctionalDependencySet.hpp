#ifndef FUNCTIONALDEPENDENCYSET_H_
#define FUNCTIONALDEPENDENCYSET_H_

#include <set>
#include <utility>
#include <functional>
#include <string>
#include <map>
#include <algorithm>

#include "FunctionalDependency.hpp"
#include "FunctionalDependencyComparator.hpp"

using namespace std;

template<class T>
class FunctionalDependencySet: public set<FunctionalDependency<T> , FunctionalDependencyComparator<T> >
{
public:
	FunctionalDependencySet();
	FunctionalDependencySet(const typename set<FunctionalDependency<T> , FunctionalDependencyComparator<T> >::key_compare&);
	template<class InputIterator>
	FunctionalDependencySet(InputIterator f, InputIterator l) :
		set<FunctionalDependency<T> , FunctionalDependencyComparator<T> > (f, l)
	{
	}
	;
	template<class InputIterator>
	FunctionalDependencySet(InputIterator f, InputIterator l, const typename set<FunctionalDependency<T> , FunctionalDependencyComparator<T> >::key_compare& comp) :
		set<FunctionalDependency<T> , FunctionalDependencyComparator<T> > (f, l, comp)
	{
	}
	;

	~FunctionalDependencySet();
	void addIn(AttributeSet<T>, map<FunctionalDependency<T>, int> &, map<T, vector<FunctionalDependency<T> > > &, AttributeSet<T> &);
	AttributeSet<T> getAllAtts();
	FunctionalDependencySet<T> closure();
	AttributeSet<T> closure(AttributeSet<T> );
	FunctionalDependencySet<T> canonicalCover();
	pair<AttributeSet<T> , AttributeSet<T> > findExtraneous(FunctionalDependency<T> );
	set<AttributeSet<T> > get3NF();
	AttributeSet<T> getCandidateKey();
};

template<class T>
void FunctionalDependencySet<T>::addIn(AttributeSet<T> a, map<FunctionalDependency<T>, int> & fdcount, map<T, vector<FunctionalDependency<T> > > & appears, AttributeSet<T> & result)
{
	for (typename AttributeSet<T>::iterator i = a.begin(); i != a.end(); i++)
	{
		if (result.find(*i) == result.end())
		{
			result.insert(*i);
			for (typename vector<FunctionalDependency<T> >::iterator j = appears[*i].begin(); j != appears[*i].end(); j++)
			{
				fdcount[*j]--;

				if (!fdcount[*j])
					addIn(j->getRhs(), fdcount, appears, result);
			}
		}
	}

	return;
}

template<class T>
FunctionalDependencySet<T>::FunctionalDependencySet()
{
}

template<class T>
FunctionalDependencySet<T>::FunctionalDependencySet(const typename set<FunctionalDependency<T> , FunctionalDependencyComparator<T> >::key_compare & comp) :
	set<FunctionalDependency<T> , FunctionalDependencyComparator<T> > (comp)
{
}

template<class T>
FunctionalDependencySet<T>::~FunctionalDependencySet()
{
}

template<class T>
AttributeSet<T> FunctionalDependencySet<T>::getAllAtts()
{
	AttributeSet<T> allAtts;

	for (typename set<FunctionalDependency<T> , FunctionalDependencyComparator<T> >::iterator i = this->begin(); i != this->end(); i++)
	{
		//FIXME Why can't I insert directly?
		FunctionalDependency<T> thisFd = *i;

		allAtts.insert(thisFd.getLhs().begin(), thisFd.getLhs().end());
		allAtts.insert(thisFd.getRhs().begin(), thisFd.getRhs().end());
	}

	return allAtts;
}

template<class T>
FunctionalDependencySet<T> FunctionalDependencySet<T>::closure()
{
	FunctionalDependencySet fplus = (*this);

	return fplus;
}

template<class T>
AttributeSet<T> FunctionalDependencySet<T>::closure(AttributeSet<T> atts)
{
	AttributeSet<T> attsplus;

	map<FunctionalDependency<T>, int> fdcount;
	map<T, vector<FunctionalDependency<T> > > appears;

	for (typename set<FunctionalDependency<T> , FunctionalDependencyComparator<T> >::iterator i = this->begin(); i != this->end(); i++)
	{
		//FIXME Why can't I work directly?
		FunctionalDependency<T> thisFd = *i;
		fdcount[thisFd] = thisFd.getLhs().size();

		for (typename AttributeSet<T>::iterator j = thisFd.getLhs().begin(); j != thisFd.getLhs().end(); j++)
			appears[*j].push_back(*i);
	}

	addIn(atts, fdcount, appears, attsplus);

	return attsplus;
}

template<class T>
FunctionalDependencySet<T> FunctionalDependencySet<T>::canonicalCover()
{
	vector<FunctionalDependency<T> > fcvector(this->begin(), this->end());
	FunctionalDependencySet fc;

	bool changed;

	do
	{
		changed = false;
		for (typename vector<FunctionalDependency<T> >::iterator i = fcvector.begin(); i != fcvector.end(); i++)
			for (typename vector<FunctionalDependency<T> >::iterator j = i + 1; j != fcvector.end(); j++)
				if (i->getLhs() == j->getLhs())
				{
					AttributeSet<T> base = i->getRhs();
					AttributeSet<T> toAdd = j->getRhs();

					base.insert(toAdd.begin(), toAdd.end());

					i->setRhs(base);
					j = fcvector.erase(j) - 1;

					changed = true;
				}

		fc.clear();
		fc.insert(fcvector.begin(), fcvector.end());
		for (typename vector<FunctionalDependency<T> >::iterator i = fcvector.begin(); i != fcvector.end(); i++)
		{
			pair<AttributeSet<T> , AttributeSet<T> > extraneous = fc.findExtraneous(*i);

			if (!extraneous.first.empty())
			{
				i->getLhs().erase((*extraneous.first.begin()));
				changed = true;
			}
			else if (!extraneous.second.empty())
			{
				i->getRhs().erase((*extraneous.second.begin()));
				changed = true;
			}

			if (changed)
			{
				if (i->getLhs().empty() || i->getRhs().empty())
					i = fcvector.erase(i) - 1;

				break;
			}
		}
	} while (changed);

	return fc;
}

template<class T>
pair<AttributeSet<T> , AttributeSet<T> > FunctionalDependencySet<T>::findExtraneous(FunctionalDependency<T> fd)
{
	AttributeSet<T> extraneousLeft;
	AttributeSet<T> extraneousRight;

	//Getting extraneous on left
	for (typename AttributeSet<T>::iterator i = fd.getLhs().begin(); i != fd.getLhs().end(); i++)
	{
		AttributeSet<T> temp = fd.getLhs();
		temp.erase(*i);

		AttributeSet<T> tempclosure = closure(temp);

		bool allFound = true;

		for (typename AttributeSet<T>::iterator j = fd.getRhs().begin(); j != fd.getRhs().end(); j++)
			if (tempclosure.find(*j) == tempclosure.end())
			{
				allFound = false;
				break;
			}

		if (allFound)
			extraneousLeft.insert(*i);
	}

	//Getting extraneous on right
	for (typename AttributeSet<T>::iterator i = fd.getRhs().begin(); i != fd.getRhs().end(); i++)
	{
		AttributeSet<T> temp = fd.getRhs();
		temp.erase(*i);

		FunctionalDependencySet fprime = (*this);
		fprime.erase(fd);
		fprime.insert(FunctionalDependency<T> (fd.getLhs(), temp));

		AttributeSet<T> tempclosure = fprime.closure(fd.getLhs());

		if (tempclosure.find(*i) != tempclosure.end())
			extraneousRight.insert(*i);
	}

	return pair<AttributeSet<T> , AttributeSet<T> > (extraneousLeft, extraneousRight);
}

template<class T>
set<AttributeSet<T> > FunctionalDependencySet<T>::get3NF()
{
	set<AttributeSet<T> > relations;
	FunctionalDependencySet fc = canonicalCover();

	for (typename set<FunctionalDependency<T> , FunctionalDependencyComparator<T> >::iterator i = fc.begin(); i != fc.end(); i++)
	{
		bool containsAll = false;
		FunctionalDependency<T> fd = *i;
		AttributeSet<T> allThisAtts;

		set_union(fd.getLhs().begin(), fd.getLhs().end(), fd.getRhs().begin(), fd.getRhs().end(), insert_iterator<AttributeSet<T> > (allThisAtts, allThisAtts.begin()));
		allThisAtts.setKey(fd.getLhs());

		for (typename set<AttributeSet<T> >::iterator j = relations.begin(); j != relations.end(); j++)
		{
			if ((containsAll = includes(j->begin(), j->end(), allThisAtts.begin(), allThisAtts.end())))
				break;
		}

		if (!containsAll)
			relations.insert(allThisAtts);
	}

	bool hasCandidateKey = false;
	AttributeSet<T> allAtts = getAllAtts();
	for (typename set<AttributeSet<T> >::iterator i = relations.begin(); i != relations.end(); i++)
	{
		if ((hasCandidateKey = allAtts == closure(*i)))
			break;
	}

	if (!hasCandidateKey)
	{
		AttributeSet<T> candidateKey = getCandidateKey();
		candidateKey.setKey(candidateKey);
		relations.insert(candidateKey);
	}

	return relations;
}

template<class T>
AttributeSet<T> FunctionalDependencySet<T>::getCandidateKey()
{
	AttributeSet<T> allAtts = getAllAtts();
	AttributeSet<T> candidateKey = allAtts;

	for (typename AttributeSet<T>::iterator i = candidateKey.begin(); i != candidateKey.end(); i++)
	{
		AttributeSet<T> thisAtts = candidateKey;
		thisAtts.erase(*i);

		if (allAtts == closure(thisAtts))
			candidateKey = thisAtts;
	}

	return candidateKey;
}

#endif /* FUNCTIONALDEPENDENCYSET_H_ */
