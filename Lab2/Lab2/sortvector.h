#ifndef __SORTVECTOR
#define __SORTVECTOR
/**
 \class sortvector
 \brief is een klasse van sorteerbare vectoren
 Bovenop de vectoreigenschappen zijn er hulpfuncties om sorteervergelijkingen
 te vergemakkelijken.
*/
#include <iostream>
using std::istream;
using std::ostream;
using std::move;
using std::swap;
#include <iomanip>   // voor setw
#include <cstdlib>   // voor rand - opletten!! 
#include <vector>
#include <random>
#include <algorithm>
using std::vector;

template<class T>
class Sortvector:public vector<T>{
  public:

    /// \fn Constructor: het argument geeft de grootte aan
    /// bij constructie zal de tabel opgevuld worden met
    /// n verschillende Ts in random volgorde
    /// (zie hulplidfuncties)
    Sortvector(int);
    
    // onderstaande constructoren zijn dus niet relevant en worden = delete
    Sortvector(const Sortvector<T>& v) = delete;
    Sortvector<T>& operator=(const Sortvector<T>& v) = delete;
    Sortvector<T>& operator=(Sortvector<T>&& v) = delete;
    Sortvector(Sortvector<T>&& v) = delete;

    /// \fn vul_range vul vector met de waarden T(0)...T(size()-1) in volgorde
    void vul_range();
    void draai_om();
    void vul_omgekeerd();
    void shuffle();
    void vul_random_zonder_dubbels();
    void vul_random();
    
    bool is_gesorteerd() const;
    /// \fn is_range controleert of *this eruit ziet als het resultaat van vul_range(), d.w.z.
    /// dat, voor alle i, (*this)[i]==T(i);
    bool is_range() const;

    friend ostream& operator<<(ostream& os, const Sortvector<T>& s){
        s.schrijf(os);
        return os;
    }

  private: 
    void schrijf(ostream & os)const;
    
};

// ******************************************************************************************************

// gegeven
template <class T>
Sortvector<T>::Sortvector(int grootte) : std::vector<T>(grootte)
{
    if (grootte > 0)
    {
        vul_random();
    }
}

//added*
template <class T>
void Sortvector<T>::vul_range(){
	this->clear(); // leegmaken, want je weet niet op voorhand wat de lengte/size is, dus kan variabel zijn
	for(int i = 0; i < this->capacity(); i++){ // size is 0 na een clear() dus capacity() gebruiken
		this->emplace_back(i);
	}
}

// added*
template <class T>
void Sortvector<T>::draai_om(){
	int start_index = 0;
	int end_index = this->size()-1;
	while (start_index < end_index){
		std::swap(this->operator[](start_index), this->operator[](end_index));
		start_index++;
		end_index--;
	}
}


// added*
template <class T>
void Sortvector<T>::vul_omgekeerd(){
	this->clear(); // leegmaken, want je weet niet op voorhand wat de lengte/size is, dus kan variabel zijn
	for(int i = this->capacity()-1; i >= 0; i--){ // size is 0 na een clear() dus capacity() gebruike
		this->emplace_back(i);
	}
}


// added*
template <class T>
void Sortvector<T>::shuffle(){
	std::random_device rd;
    std::mt19937 eng{rd()};
	std::shuffle(this->begin(), this->end(), eng);
}


// added*
template <class T>
void Sortvector<T>::vul_random_zonder_dubbels(){
	this->vul_range();
	this->shuffle();
}

// gegeven*
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

// added*
template<class T>
bool Sortvector<T>::is_gesorteerd() const{
	int i = 1; // voor lijst van 1
	while (i < this->size() && this->operator[](i-1) <= this->operator[](i)){
		i++;
	}
	return i == this->size(); // dit zal true teruggeven als hij de volledige lus heeft kunnen doorlopen (en de lijst dus sorted is)
}

// added*
template<class T>
bool Sortvector<T>::is_range() const{
	int i = 1; // voor lijst van 1
	while(i < this->size() && T(i) == this->operator[](i)){
		i++;
	}
	return i == this->size(); // geeft true als while loop volledig heeft kunnen lopen
}

// gegeven
template <class T>
void Sortvector<T>::schrijf(ostream & os)const{
    for(auto&& t : *this){
        os<<t<<" ";
    }
    os<<"\n";
}



#endif
