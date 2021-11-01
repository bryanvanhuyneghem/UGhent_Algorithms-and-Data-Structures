#ifndef SHELLSORT_H
#define SHELLSORT_H

#include "sorteermethode.h"
#include <iostream>

/** \class Shellsort
*/
template <class T>
class ShellSort : public Sorteermethode<T> {
	
	public:
/// \fn operator() sorteert de vector gegeven door het argument
        void operator()(vector<T> & v) const;
	
};

template <typename T>
void ShellSort<T>::operator()(vector<T> & v) const { 		// probleempje!
	/*
	// Rangschikken met dalende incrementen
    vector<int> ciura_reeks = {701, 301, 132, 57, 23, 10, 4, 1};
    
    for(int k = 0; k < ciura_reeks.size(); k++){    // voor alle k-waarden
        while(v[k] >= v.size()/2 && v[k] >= 1){ // om te kunnen starten bij een k-waarde kleiner dan of gelijk aan de helft van de grootte van vector
            k++;
        }
        for(int i = v[k]; i < v.size(); i++){
            T hulp = move(v[i]); // sla getal (op locatie offset k) op in hulp
            int j = i-k; // te vergelijken getal offset k terug op index j
            while(j>=0 && v[j]>hulp){  // InsertionSort
                v[j+v[k]] = move(v[j]); // verplaats het linkergetal naar de offset k naar rechts
                j -= k; // ga terug
            }
            v[j+k] = move(hulp); // plaats getal in hulp op juiste locatie
        }
    }  
    */
}


#endif

