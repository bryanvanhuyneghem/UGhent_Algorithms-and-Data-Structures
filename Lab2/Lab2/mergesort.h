#ifndef MERGESORT_H
#define MERGESORT_H

#include "sorteermethode.h"

/** \class MergeSort
*/
template <class T>
class MergeSort : public Sorteermethode<T> {
	
	public:
/// \fn operator() sorteert de vector gegeven door het argument
        void operator()(vector<T> & v) const;
        
};

template <typename T>
void MergeSort<T>::operator()(vector<T> & v) const {
	
}


#endif

