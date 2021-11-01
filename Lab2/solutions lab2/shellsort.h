#ifndef SHELLSORT_H
#define SHELLSORT_H

#include "sorteermethode.h"

/** \class Shellsort
*/

template <typename T>
class ShellSort : public Sorteermethode<T>
{
public:
    std::vector<long long> tokuda(int size) const;

    void operator()(std::vector<T>& v) const override;
};

#include <iostream>

template <typename T>
std::vector<long long> ShellSort<T>::tokuda(int size) const
{
    // Tokuda: https://oeis.org/A108870

    std::vector<long long> tokuda;

    int i = 1;
    long long k_i = 1;

    while (((9.0 / 4.0) * k_i) < size)
    {
        tokuda.push_back(k_i);

        k_i = ceil((9.0 * pow((9.0 / 4.0), i) - 4.0) / 5.0);
        i++;
    }

    return tokuda;
}

template <typename T>
void ShellSort<T>::operator()(std::vector<T>& v) const
{
    std::vector<long long> tokuda_sequence = tokuda(v.size());

    for (auto it = tokuda_sequence.rbegin(); it != tokuda_sequence.rend(); ++it)
    {
        long long k = *it;

        for (long long i = k; i < v.size(); i++)
        {
            T h{std::move(v[i])};
            long long j = (i - k);

            while (j >= 0 && h < v[j])
            {
                v[j + k] = std::move(v[j]);
                j -= k;
            }

            v[j + k] = std::move(h);
        }
    }
}

#endif
