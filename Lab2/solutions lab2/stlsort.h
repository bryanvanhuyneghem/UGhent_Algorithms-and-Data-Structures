#ifndef STLSORT_H
#define STLSORT_H

#include "sorteermethode.h"

#include <algorithm>

/** \class STLSort
   \brief STANDARD TEMPLATE LIBRARY SORT
*/
template <typename T>
class STLSort : public Sorteermethode<T>
{
public:
    void operator()(std::vector<T>& v) const override;
};

template <typename T>
void STLSort<T>::operator()(std::vector<T>& v) const
{
    std::sort(v.begin(), v.end());
}

#endif
