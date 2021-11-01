#ifndef SORTVECTOR_H
#define SORTVECTOR_H

/**
 \class sortvector
 \brief is een klasse van sorteerbare vectoren
 Bovenop de vectoreigenschappen zijn er hulpfuncties om sorteervergelijkingen
 te vergemakkelijken.
*/

#include <algorithm>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <limits>
#include <numeric>
#include <random>
#include <vector>

template <class T>
class Sortvector : public std::vector<T>
{
public:
    /// \fn Constructor: het argument geeft de grootte aan
    /// bij constructie zal de tabel opgevuld worden met
    /// n verschillende Ts in random volgorde
    /// (zie hulplidfuncties)

    Sortvector() = default;
    Sortvector(int);
    Sortvector(std::initializer_list<T> l);
    Sortvector(const Sortvector<T>& v);
    Sortvector<T>& operator=(const Sortvector<T>& v) = default;
    Sortvector<T>& operator=(Sortvector<T>&& v) = default;
    Sortvector(Sortvector<T>&& v) = default;
    virtual ~Sortvector() = default;

    /// \fn vul_range vul vector met de waarden T(0)...T(size()-1) in volgorde
    void vul_range();
    void draai_om();
    void vul_omgekeerd();
    void shuffle();
    void vul_random_zonder_dubbels();
    void vul_random(); //< nog niet implementeren

    bool is_gesorteerd() const;
    /// \fn is_range controleert of *this eruit ziet als het resultaat van vul_range(), d.w.z.
    /// dat, voor alle i, (*this)[i]==T(i);
    bool is_range() const;

    friend std::ostream& operator<<(std::ostream& os, const Sortvector<T>& s)
    {
        s.schrijf(os);
        return os;
    }

private:
    void schrijf(std::ostream& os) const;
};

template <class T>
Sortvector<T>::Sortvector(std::initializer_list<T> l) : std::vector<T>(l)
{
}

template <class T>
Sortvector<T>::Sortvector(int grootte) : std::vector<T>(grootte)
{
    if (grootte > 0)
    {
        vul_random();
    }
}

template <class T>
Sortvector<T>::Sortvector(const Sortvector& v) : std::vector<T>(v)
{
}

template <class T>
void Sortvector<T>::vul_range()
{
    if (this->empty())
    {
        return;
    }

    for (int i = 0; i < this->size(); i++)
    {
        (*this)[i] = i;
    }
}

template <class T>
void Sortvector<T>::draai_om()
{
    if (this->empty())
    {
        return;
    }

    std::reverse(this->begin(), this->end());
}

template <class T>
void Sortvector<T>::shuffle()
{
    if (this->empty())
    {
        return;
    }

    std::random_device rd;
    std::mt19937 eng{rd()};

    std::shuffle(this->begin(), this->end(), eng);
}

template <class T>
void Sortvector<T>::vul_omgekeerd()
{
    if (this->empty())
    {
        return;
    }

    for (int i = 0; i < this->size(); i++)
    {
        assert(this->size() < std::numeric_limits<int>::max());
        (*this)[i] = (static_cast<int>(this->size()) - i - 1);
    }
}

template <class T>
void Sortvector<T>::vul_random_zonder_dubbels()
{
    if (this->empty())
    {
        return;
    }

    vul_range();
    shuffle();
}

template <class T>
void Sortvector<T>::vul_random()
{
    if (this->empty())
    {
        return;
    }

    std::random_device rd;
    std::mt19937 eng{rd()};
    const auto max_value = (this->size() - 1);
    assert(max_value < std::numeric_limits<int>::max());
    std::uniform_int_distribution<int> dist{0, static_cast<int>(max_value)};

    std::generate(this->begin(), this->end(), [&dist, &rd]() { return dist(rd); });
}

template <class T>
bool Sortvector<T>::is_gesorteerd() const
{
    return std::is_sorted(this->begin(), this->end());
}

template <class T>
bool Sortvector<T>::is_range() const
{
    if (this->size() <= 1)
    {
        return true;
    }

    for (int i = 1; i < this->size(); i++)
    {
        if ((*this)[i] != ((*this)[i - 1] + 1))
        {
            return false;
        }
    }

    return true;
}

template <class T>
void Sortvector<T>::schrijf(std::ostream& os) const
{
    for (auto&& t : *this)
    {
        os << t << " ";
    }
    os << "\n";
}

#endif
