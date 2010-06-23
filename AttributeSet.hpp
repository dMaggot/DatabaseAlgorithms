#ifndef ATTRIBUTESET_H_
#define ATTRIBUTESET_H_

#include <set>
#include <string>
#include <vector>

using namespace std;

template <class T>
class AttributeSet : public set<T>
{
public:
	AttributeSet();
	~AttributeSet();
	void setKey(AttributeSet &);
	AttributeSet getKey();
	AttributeSet & operator = (const AttributeSet &);

private:
	set<T> key;
};

template<class T>
AttributeSet<T>::AttributeSet()
{
}

template<class T>
AttributeSet<T>::~AttributeSet()
{
}

template<class T>
AttributeSet<T> & AttributeSet<T>::operator=(const AttributeSet<T> & as)
{
	this->clear();
	this->insert(as.begin(), as.end());

	return *this;
}

template<class T>
void AttributeSet<T>::setKey(AttributeSet<T> & attSetKey)
{
	this->key.clear();

	for (typename set<T>::iterator  i = attSetKey.begin(); i != attSetKey.end(); i++)
	{
		if (this->find(*i) != this->end())
			this->key.insert(*i);
	}

	return;
}

template<class T>
AttributeSet<T> AttributeSet<T>::getKey()
{
	AttributeSet<T> key;

	for (set<string>::iterator i = this->key.begin(); i != this->key.end(); i++)
		key.insert(*i);

	return key;
}

#endif /* ATTRIBUTESET_H_ */
