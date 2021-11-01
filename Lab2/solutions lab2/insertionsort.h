#ifndef INSERTIONSORT_H
#define INSERTIONSORT_H

#include "sorteermethode.h"

/** \class InsertionSort
*/
template <typename T>
class InsertionSort : public Sorteermethode<T>
{
public:
    void operator()(std::vector<T>& v) const override;
};

template <typename T>
void InsertionSort<T>::operator()(std::vector<T>& v) const
{
    for (int i = 1; i < v.size(); i++)
    {
        T h{std::move(v[i])};
        int j = (i - 1);

        while (j >= 0 && h < v[j])
        {
            v[j + 1] = std::move(v[j]);
            j--;
        }

        v[j + 1] = std::move(h);
    }
}

#endif
