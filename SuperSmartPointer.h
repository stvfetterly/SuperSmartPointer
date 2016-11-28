#include "stdafx.h"

#include <map>
#include <iostream>

template <typename T>
class SuperSmartPointer
{
public:
	explicit SuperSmartPointer();
	explicit SuperSmartPointer(T* inPtr);					//No implicit compiler conversions while making an SSP
	SuperSmartPointer(const SuperSmartPointer<T>& src);		//Copy constructor
	
	~SuperSmartPointer();									//Destructor

	//Operator overloading
	SuperSmartPointer<T>& operator=(const SuperSmartPointer<T>& rhs);
	const T& operator*() const;
	const T* operator->() const;
	T& operator*();
	T* operator->();

	operator void*() const { return m_Ptr;	}

private:
	T* m_Ptr;										//The pointer to object that SSP is referring to
	static std::map<T*, int> s_RefCountMap;			//holds addresses of pointers and the number of pointers currently being used

	void killPointer();
	void initPointer(T* inPtr);
};

//initialize the static map for counting
template <typename T>
std::map<T*, int>SuperSmartPointer<T>::s_RefCountMap;

//Constructors
template <typename T>
SuperSmartPointer<T>::SuperSmartPointer(T* inPtr)
{
	initPointer(inPtr);
}
template <typename T>
SuperSmartPointer<T>::SuperSmartPointer(const SuperSmartPointer<T>& src)
{
	initPointer(src.m_Ptr);
}
template <typename T>
SuperSmartPointer<T>::SuperSmartPointer()
{
	T* newPtr = new T;		//Default constructor creates an object of the type supplied and then points to it
	initPointer(newPtr);
}

//Destructor
template<typename T>
SuperSmartPointer<T>::~SuperSmartPointer()
{
	killPointer();
}

//Assignment operator
template<typename T>
SuperSmartPointer<T>& SuperSmartPointer<T>::operator=(const SuperSmartPointer<T>& rhs)
{
	//Check that we're not assigning to ourself, if we are do nothing
	if (this == &rhs)
	{
		return (*this);
	}

	//Get rid of old pointer reference in (this)
	killPointer();

	//Create new pointer, pointing to what's being assigned in (this)
	initPointer(rhs.m_Ptr);

	return (*this);
}

//Const and regular operator overloading to allow the SSP to be used like a regular pointer to the object being referenced
template <typename T>
const T* SuperSmartPointer<T>::operator->() const
{
	return m_Ptr;
}
template <typename T>
const T& SuperSmartPointer<T>::operator*() const
{
	return *m_Ptr;
}
template <typename T>
T* SuperSmartPointer<T>::operator->()
{
	return m_Ptr;
}
template <typename T>
T& SuperSmartPointer<T>::operator*()
{
	return *m_Ptr;
}


template<typename T>
void SuperSmartPointer<T>::initPointer(T* inPtr)
{
	m_Ptr = inPtr;
	
	// If the key for the pointer doesn't exist yet, it will be automatically created here with a value of 1
	// If the key does exist, we will just increment the value as we're adding another one in
	s_RefCountMap[m_Ptr]++;
}

template<typename T>
void SuperSmartPointer<T>::killPointer()
{
	if (s_RefCountMap[m_Ptr] == 0)  //This should be impossible
	{
		//if somehow the pointer was not created properly in the map, we need to report an error
		std::cerr << "ERROR: No references to the object SuperSmartPointer is trying to delete" << std::endl;
		
		//erase the bad pointer from our map
		s_RefCountMap.erase(m_Ptr);
	}
	//If the last reference to the pointer is being deleted
	else if (s_RefCountMap[m_Ptr] == 1)
	{
		//This is the final reference, we need to erase this object from the map
		s_RefCountMap.erase(m_Ptr);

		//Delete the object we were pointing to since it's not being used any more
		delete m_Ptr;
	}
	//There are other pointers still using this, just decrement our count
	else
	{
		s_RefCountMap[m_Ptr]--;
	}
}





