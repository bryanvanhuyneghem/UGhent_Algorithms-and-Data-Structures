#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include "sorteermethode.h"

/** \class InsertionSort
*/
template <class T>
class InsertionSort : public Sorteermethode<T> {
	
	public:
/// \fn operator() sorteert de vector gegeven door het argument
        void operator()(vector<T> & v) const;
        
};

template <typename T>
void InsertionSort<T>::operator()(vector<T> & v) const {
	for(int i=1; i<v.size(); i++){
        T hulp;
        hulp = move(v[i]);
        int j = i - 1;
        while(j >= 0 && hulp < v[j]){
            v[j+1] = move(v[j]);
            j--;
        }
        v[j+1] = move(hulp);
    }
}


#endif

