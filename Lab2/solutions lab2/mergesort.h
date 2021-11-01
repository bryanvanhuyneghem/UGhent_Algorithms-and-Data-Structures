
#ifndef MERGESORT_H
#define MERGESORT_H

#include "sorteermethode.h"

template <typename T>
class MergeSort : public Sorteermethode<T>
{
public:
    void operator()(std::vector<T>& v) const override;

private:
    int mergesort(std::vector<T>& v) const;
    int mergesort(std::vector<T>& v, int l, int r, std::vector<T>& h) const;
    int merge(std::vector<T>& v, int l, int m, int r, std::vector<T>& h) const;
};

template <typename T>
void MergeSort<T>::operator()(std::vector<T>& v) const
{
    mergesort(v);
}

template <typename T>
int MergeSort<T>::mergesort(std::vector<T>& v) const
{
    std::vector<T> h(v.size() / 2);
    return mergesort(v, 0, v.size(), h);
}

template <typename T>
int MergeSort<T>::mergesort(std::vector<T>& v, int l, int r, std::vector<T>& h) const
{
    if (l >= (r - 1)) // r points past the last index (see below)
    {
        return 0;
    }

    int m = (l + (r - l) / 2); // Prevent overflow, safer than (l+r)/2

    int aantal_inversies = mergesort(v, l, m, h);
    aantal_inversies += mergesort(v, m, r, h);
    aantal_inversies += merge(v, l, m, r, h);

    return aantal_inversies;
}

template <typename T>
int MergeSort<T>::merge(std::vector<T>& v, int l, int m, int r, std::vector<T>& h) const
{
    int aantal_inversies = 0;

    // Copy to h
    //
    // By using m in the same way as end() (e.g. pointing to the value past the latest we process),
    // the left part will always be the smallest.
    //
    // {0, 1, 2, 3, 4} (size 5)
    // l = 0, r = 5 <=> m = (0 + (5 - 0) / 2) = (5 / 2) = (int) 2.5 = 2
    // thus L_array = {0, 1} and R_array = {2, 3, 4}
    //

    int h_i = 0;
    int v_i = l;

    while (v_i < m)
    {
        h[h_i] = std::move(v[v_i]);

        h_i++;
        v_i++;
    }

    // Merge

    h_i = 0;
    v_i = m;
    int res_i = l;
    const int aantal_h = (m - l);

    while (v_i < r && h_i < aantal_h)
    {
        if (h[h_i] <= v[v_i]) // = garantuees stability
        {
            v[res_i] = std::move(h[h_i]);
            h_i++;
        }
        else
        {
            v[res_i] = std::move(v[v_i]);
            v_i++;

            aantal_inversies += (m - (h_i + l)); // Add how many are left in the lesser half
        }

        res_i++;
    }

    while (v_i < r)
    {
        v[res_i] = std::move(v[v_i]);
        v_i++;
        res_i++;
    }

    while (h_i < aantal_h)
    {
        v[res_i] = std::move(h[h_i]);
        h_i++;
        res_i++;
    }

    return aantal_inversies;
}

#endif
