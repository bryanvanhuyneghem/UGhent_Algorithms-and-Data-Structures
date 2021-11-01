#ifndef INVERSIETELLER_H
#define INVERSIETELLER_H

#include "mergesort.h"

#include <vector>

template <class T>
class InversieTeller
{
public:
    // Een paar elementen dat niet in de juiste volgorde staat vormt een inversie
    int geef_aantal_inversies(const std::vector<T>& v) const;

private:
    int O_n2(const std::vector<T>& v) const;
    int O_n_log_n(const std::vector<T>& v) const;
};

template <class T>
int InversieTeller<T>::geef_aantal_inversies(const std::vector<T>& v) const
{
    return O_n_log_n(v);
}

template <class T>
int InversieTeller<T>::O_n2(const std::vector<T>& v) const
{
    int aantal = 0;
    
    for (int i = 0; i < (v.size() - 1); i++)
    {
        for (int j = (i + 1); j < v.size(); j++)
        {
            if (v[i] > v[j])
            {
                aantal++;
            }
        }
    }
    
    return aantal;
}

template <class T>
int InversieTeller<T>::O_n_log_n(const std::vector<T>& v) const
{
    MergeSort<T> ms;

    std::vector<T> copy(v);

    return ms.aantal_inversies(copy);
}

#endif