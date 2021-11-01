#ifndef STLSORT_H
#define STLSORT_H

#include "sorteermethode.h"
#include <algorithm>

/** \class STLSort
*/
template <class T>
class STLSort : public Sorteermethode<T>{
	
	public:
/// \fn operator() sorteert de vector gegeven door het argument
        void operator()(vector<T> & v) const;
	
};

template <typename T>
void STLSort<T>::operator()(vector<T> & v) const {
	std::sort(v.begin(), v.end());
}


#endif

