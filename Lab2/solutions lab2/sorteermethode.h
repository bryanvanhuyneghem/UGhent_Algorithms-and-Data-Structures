#ifndef SORTEERMETHODE_H
#define SORTEERMETHODE_H

#include "chrono-simple.h"
#include "csv.h"
#include "sortvector.h"

#include <algorithm>
#include <iostream>

template <typename T>
class Sortvector;

/** class Sorteermethode
    \brief abstracte klasse van methodes die een vector sorteren
*/
template <typename T>
class Sorteermethode
{
public:
    Sorteermethode() = default;
    Sorteermethode(const Sorteermethode<T>& v) = default;
    Sorteermethode(Sorteermethode<T>&& v) = default;
    Sorteermethode<T>& operator=(const Sorteermethode<T>& v) = default;
    Sorteermethode<T>& operator=(Sorteermethode<T>&& v) = default;
    virtual ~Sorteermethode() = default;

    /// \fn operator() sorteert de vector gegeven door het argument
    virtual void operator()(std::vector<T>& v) const = 0;

    /// \fn meet(int kortste, int langste, ostream& os) schrijft naar os een overzicht (met de nodige ornamenten)
    /// met de snelheid van de opgegeven sorteermethode *this. Er wordt 1 lijn uitgedrukt voor elke mogelijke
    /// grootte. Deze groottes zijn kleinste, 10*kleinste, 100*kleinste,
    /// enzovoorts, tot aan grootste.
    /// Op een lijn staat de snelheid van de methode toegepast op
    /// (1) een random tabel
    /// (2) een al gesorteerde tabel.
    /// (3) een omgekeerd gesorteerde tabel.

    /// Deze functie werkt alleen als T een toekenning van een int toelaat,
    /// zodat bv.
    ///    T a=5;
    /// geldig is.
    void meet(int kortste, int langste, std::ostream& os, CsvData& csv) const;
};

template <typename T>
void Sorteermethode<T>::meet(int kortste, int langste, std::ostream& os, CsvData& csv) const
{
    constexpr int FIELD_WIDTH = 20;
    Chrono timer;

    os << std::setw(FIELD_WIDTH) << "lengte" << std::setw(FIELD_WIDTH) << "random" << std::setw(FIELD_WIDTH)
       << "gesorteerd" << std::setw(FIELD_WIDTH) << "omgekeerd" << std::endl
       << std::endl;

    int aantal_elementen = kortste;
    while (aantal_elementen < langste)
    {

        os << std::setw(FIELD_WIDTH) << aantal_elementen;

        Sortvector<T> data(aantal_elementen);

        data.vul_random();

        timer.start();
        (*this)(data);
        timer.stop();

        double tijd_random = timer.tijd();
        os << std::setw(FIELD_WIDTH) << tijd_random;

        data.vul_range();

        timer.start();
        (*this)(data);
        timer.stop();

        double tijd_gesorteerd = timer.tijd();
        os << std::setw(FIELD_WIDTH) << tijd_gesorteerd;

        data.vul_omgekeerd();

        timer.start();
        (*this)(data);
        timer.stop();

        double tijd_omgekeerd_gesorteerd = timer.tijd();
        os << std::setw(FIELD_WIDTH) << tijd_omgekeerd_gesorteerd;

        csv.voeg_data_toe(std::vector<double>{
                static_cast<double>(aantal_elementen), tijd_random, tijd_gesorteerd, tijd_omgekeerd_gesorteerd});

        aantal_elementen *= 10;

        os << std::endl;
    }
}

#endif
