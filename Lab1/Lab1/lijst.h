// Ontbrekende elementen: move en copy, iterator; probleem: lijn 235 in copy operator
#ifndef __LIJST17_H
#define __LIJST17_H
//
// speciale code voor debuggen
//
#define DEBUG
//
//
//speciale code voor iteratoren
//#define ITERATOR

#include <iostream>
#include <fstream>
#include <memory>
#include <cassert>
#include <algorithm>
#include <string>
#include <sstream>
using std::string;
using std::endl;
using std::ostream;
using std::ofstream;

template<class T>
class Lijstknoop;
template<class T>
class Lijst;

template<class T>
using Lijstknoopptr=std::unique_ptr<Lijstknoop<T> >; // Een Lijstknoopptr is hier een pseudoniem voor unique_ptr<Lijstknoop<T>>
template<class T>
ostream& operator<<(ostream& os, const Lijst<T>& l);

template<class T>
class Lijst: private Lijstknoopptr<T>{
public:

    using std::unique_ptr<Lijstknoop<T>>::operator=;
    using std::unique_ptr<Lijstknoop<T>>::swap;

    // ToDo: (done)
    // - Add default constructor
    Lijst(){};
    
    // ********* Rule of 5 *********
    // - Add copy constructor
    Lijst(const Lijst<T>&);
    // - Add move constructor
    Lijst(Lijst<T>&&);
    // - Add destructor
    ~Lijst(){};
	// - Add copy assignment 
    Lijst<T>& operator=(const Lijst<T>&);		// & wijst op een lvalue; een variable
    // - Add move assignment
    Lijst<T>& operator=(Lijst<T>&&);			// && wijst op een rvalue; een soort anoniem iets, niet belangrijk genoeg om een naam te geven
	Lijst<T>& operator=(Lijstknoopptr<T>&&);	// =operator (move equal)   // om voegToe te laten werken
	// ********* Rule of 5 *********
	
    //duplicaten zijn toegelaten.
    public: void voegToe(const T&);

    //geefaantal geeft het aantal keer dat de sleutel voorkomt.
    //gebruikt de zoekfunctie om door de lijst te lopen!
    //zonder argument: geef lengte lijst
    public: int geefAantal(const T&) const;
    public: int geefAantal() const;

    //verwijder verwijdert slechts het eerste exemplaar met de gegeven
    //T, en geeft geen fout als de T niet gevonden wordt.
    //gebruik de zoekfunctie om door de lijst te lopen!
    public: void verwijder(const T&);

    //verwijder eerste knoop.
    public: void verwijderEerste();

    public: void insertionsort();

    public: bool isClone(const Lijst<T>&) const;

    // zoek geeft een pointer naar de Lijst die de sleutelwaarde bevat,
    // en geeft een pointer naar de lege lijst op het einde als de sleutel niet
    // voorkomt.
    protected: const Lijst* zoek(const T&) const;
    protected: Lijst* zoek(const T&);

    //preconditie zoekgesorteerd: lijst is gesorteerd
    //teruggeefwaarde: wijst naar Lijst waar sleutel staat/zou moeten staan.
    protected: Lijst<T>* zoekGesorteerd(const T& sleutel);

    //uitschrijf- en tekenoperaties
    //dotformaat:
    public: void teken(const char * bestandsnaam)const;
    //uitschrijven: voor elke knoop de T-waarde, gescheiden door komma's
    friend ostream& operator<< <>(ostream& os, const Lijst& l);
    public: void schrijf(ostream & os) const;



    // Iterator; member class van Lijst; gaat ervan uit dat alles const is
    public: class iterator{
        public:
            iterator(Lijstknoop<T>* l=0):it_ptr(l){};
            const T& operator*() const;
            const iterator& operator++();
            bool operator!=(const iterator& i);
            Lijstknoop<T>* it_ptr;
    };
    iterator begin() const;
    iterator end() const;

};

template<class T>
class Lijstknoop{
    friend class Lijst<T>;
    friend ostream& operator<< <>(ostream& os, const Lijst<T>& l);
    public:
        Lijst<T> volgend;
        Lijstknoop(const T&);
#ifdef DEBUG
        ~Lijstknoop();
#endif
    protected:
        T sleutel;
#ifdef DEBUG
public:
        static bool controle (int gemaakt, int verwijderd);
protected:
        static int aantalGemaakt;
        static int aantalVerwijderd;
#endif
};


template<class T>
int Lijstknoop<T>::aantalGemaakt=0;
template<class T>
int Lijstknoop<T>::aantalVerwijderd=0;

template<class T>
Lijstknoop<T>::Lijstknoop(const T& _sl):sleutel(_sl){
//    std::cerr<<"Knoop met sleutel "<<sleutel<<" wordt gemaakt\n";
    aantalGemaakt++;
}
#ifdef DEBUG

template<class T>
Lijstknoop<T>::~Lijstknoop(){
//    std::cerr<<"Knoop met sleutel "<<sleutel<<" wordt verwijderd\n";
    aantalVerwijderd++;
}
template<class T>
bool Lijstknoop<T>::controle (int gemaakt, int verwijderd){
    if (aantalGemaakt==gemaakt && aantalVerwijderd==verwijderd){
        //std::cout<<"Aantal gemaakte knopen   :" <<gemaakt<< "\n";
        //std::cout<<"Aantal verwijderde knopen: "<<verwijderd<< "\n";
		return true;
	}
    else{
        std::cerr<<"Fout bij controle:\n";
        std::cerr<<"Aantal gemaakte knopen   : "<<aantalGemaakt<<" (moet zijn: "<<gemaakt<<")\n";
        std::cerr<<"Aantal verwijderde knopen: "<<aantalVerwijderd<<" (moet zijn: "<<verwijderd<<")\n";
//ook naar cout schrijven: sommige ontwikkelingsomgevingen dumpen cerr.
        std::cout<<"Fout bij controle:\n";
        std::cout<<"Aantal gemaakte knopen   : "<<aantalGemaakt<<" (moet zijn: "<<gemaakt<<")\n";
        std::cout<<"Aantal verwijderde knopen: "<<aantalVerwijderd<<" (moet zijn: "<<verwijderd<<")\n";
        throw "Mislukte controle";
    };

};
#endif

template<class T>
ostream& operator<<(ostream& os,const Lijst<T>& l){
#ifdef ITERATOR
    for (auto&& sleutel: l)
        os<<sleutel<<", ";
#else
    if (l.get()){
        os<<l.get()->sleutel<<", ";
        os<<l.get()->volgend;
    }
#endif
    return os;
}

template<class T>
void Lijst<T>::schrijf(ostream & os) const{
#ifdef ITERATOR
    if (this->get()!=0){
        os<<this->get()->sleutel;
        std::for_each (++begin(),end(),[&](const T& sleutel){ os<<" . "<<sleutel;} );
    }
#else
	{
        Lijstknoop<T> * kn=this->get();
        if (kn!=0){
            os<<kn->sleutel;
            kn=kn->volgend.get();
        };
        while (kn != 0){
            os<<" . "<<kn->sleutel;
            kn=kn->volgend.get();
        };
    }
#endif
}

// ToDo: 
// ------------------------------------------------------------------------------
// Implementatie van member-constructors en operatoren van Lijst<T>
// Copy-constructor
template<class T>
Lijst<T>::Lijst(const Lijst<T>& lijst){
	this = lijst;
}

// Move-constructor
template<class T>
Lijst<T>::Lijst(Lijst<T>&& lijst){
	*this = std::move(lijst);	// use built-in std::move function
}

// Copy operator ---> er ontbreekt nog iets
template<class T>
Lijst<T>& Lijst<T>::operator=(const Lijst<T>& lijst){
	if(&lijst != this){ // check to ensure that "this" is not the parameter "lijst"
		this->reset(nullptr); // saves copy of the old pointer, overwrites current pointer with the argument (nullptr) and if the old pointer was non-empty, it deletes the previously managed object
		
		Lijst<T>* nieuweLijst = this; // point this to the head of "nieuweLijst" (which is a ptr to a ptr)
		const Lijst<T>* oudeLijst = &lijst; // this is a ptr to the start of my parameter "lijst". Lijst<T>* a ptr to a ptr, because Lijst<T> is already a unique_ptr; hence the &
		
		Lijstknoop<T>* knoop = oudeLijst->get(); // get the first node of the old list "oudeLijst". get() asks a unique pointer for its raw ptr and assigns this ptr to knoop
		while(knoop){ // iterate over all nodes in the old list "oudeLijst"; as long as not null do...
			nieuweLijst->voegToe(knoop->sleutel); // add a new node to the new list "nieuweLijst" with the appropriate node key value "sleutel", from the old list "oudeLijst"
			oudeLijst = &(knoop->volgend); // move to the next node in the old list "oudeLijst"
			knoop = oudeLijst->get(); //get the next node in the old list "oudeLijst"
			nieuweLijst = &((*nieuweLijst)->volgend); // in the "nieuweLijst" go to the next node (-> volgend); this way voegToe doesn't reverse my list
		}
	}
	return *this; // dereference this and return the new list "nieuweLijst"
}

// Move operator
template<class T>
Lijst<T>& Lijst<T>::operator=(Lijst<T>&& lijst){
	// move van lijst is gewoon de eerst pointer verplaatsen en dan heb je al de rest direct
	Lijstknoopptr<T>::operator=(move(lijst)); 
	return *this;
}
// ------------------------------------------------------------------------------
// Implementatie van Move operator met een Lijstknoopptr<T>
template<class T>
Lijst<T>& Lijst<T>::operator=(Lijstknoopptr<T>&& knoop){
	// move van lijst is gewoon de eerst pointer verplaatsen en dan heb je al de rest direct
	Lijstknoopptr<T>::operator=(move(knoop)); 
	return *this; // adres naar this teruggeven (dereferentie)
};


// ToDo: 
// ------------------------------------------------------------------------------
// Implementatie van iterator memberfuncties (class iterator is een member class van Lijst)
// handig om zelf te doen, indien je de elementen niet naast elkaar hebt staan in memory
template<class T>
const T& Lijst<T>::iterator::operator*() const {  
	return it_ptr->sleutel;
}

template<class T>
const typename Lijst<T>::iterator& Lijst<T>::iterator::operator++() { 
    this->it_ptr = this->it_ptr->volgend.get();
    return *this;
}

template<class T>
bool Lijst<T>::iterator::operator!=(const typename Lijst<T>::iterator &i) {
    return i.it_ptr != (*this).it_ptr;
}

template<class T>
typename Lijst<T>::iterator Lijst<T>::begin() const {
	Lijstknoop<T>* knoop = this->get();
	return Lijst<T>::iterator(knoop);
}


template<class T>
typename Lijst<T>::iterator Lijst<T>::end() const {
	return nullptr;
}
// ------------------------------------------------------------------------------

template<class T>
bool Lijst<T>::isClone(const Lijst<T>& ander) const{
    const Lijst<T>* l1=this, *l2=&ander;//twee lopers
    while (*l1 && *l2 && (*l1)->sleutel == (*l2)->sleutel){
        l1=&((*l1)->volgend);
        l2=&((*l2)->volgend);
    };
    return(!(*l1) && !(*l2));
};

template<class T>
const Lijst<T>* Lijst<T>::zoek(const T& sleutel) const{
    const Lijst<T>* pl=this;
    while (*pl && pl->get()->sleutel !=sleutel)
        pl=&(pl->get()->volgend);
    return pl;
}


template<class T>
int Lijst<T>::geefAantal(const T& sleutel) const{
    int aantal=0;
    const Lijst<T>* pl=this;
    while (*pl){
        if (sleutel=(*pl)->sleutel)
            ++aantal;
        pl=&(pl->get()->volgend);
    };
    return aantal;
};

template<class T>
int Lijst<T>::geefAantal() const{
    int aantal=0;
    const Lijst<T>* pl=this;
    while (*pl){
        ++aantal;
        pl=&(pl->get()->volgend);
    };
    return aantal;
};

template<class T>
Lijst<T>* Lijst<T>::zoek(const T& sleutel){
    Lijst* pl=this;
    while (*pl && pl->get()->sleutel !=sleutel)
        pl=&(pl->get()->volgend);
    return pl;
}

// voegt nodes toe aan head van de lijst (this), omdat nieuw->volgend naar null wijst en je swapt dat met begin van de lijst (this). nieuw (new node) komt dus vooraan de lijst (this) te staan.
template<class T>
void Lijst<T>::voegToe(const T& sleutel){
    Lijstknoopptr<T> nieuw = std::make_unique<Lijstknoop<T>>(sleutel); // maakt een Lijstknoop aan met meegegeven sleutelwaarde
    Lijstknoopptr<T>::swap(nieuw->volgend); // nieuw is een Lijstknoopptr (een unique pointer) dewelke wijst naar een lijstknoop
    //haal door -> het attr op waarnaar je wijst => volgend van lijstknoop pointer is '???' wordt verwisseld met de huidige lijst (unique pointer 2) die naar null wijst
    *this = std::move(nieuw);   // nieuw is een lvalue en std::move maakt van lvalue een rvalue en zo kan ik dus ook move operator gebruiken voor mijn lvalue
    							// "this" (of dus "nieuweLijst" (in operator=)) points to the head, which is what "nieuw" was pointing to, and "nieuw" goes out of scope
}
//OPMERKING: swap(...) = this->swap(...)

template<class T>
void Lijst<T>::verwijderEerste(){
    if (this->get()!=0){
        Lijstknoopptr<T> staart(std::move(this->get()->volgend));
        this->reset();
        Lijstknoopptr<T>::swap(staart);
    }
}

template<class T>
void Lijst<T>::verwijder(const T& sleutel){
    zoek(sleutel)->verwijderEerste();
}

template<class T>
Lijst<T>* Lijst<T>::zoekGesorteerd(const T& sleutel){
    Lijst* plaats=this;
    while (*plaats && plaats->get()->sleutel < sleutel)
        plaats=&plaats->get()->volgend;
    return plaats;
};

template<class T>
void Lijst<T>::insertionsort(){
    Lijstknoopptr<T> ongesorteerd=std::move(*this);
    while (ongesorteerd){
        Lijst *plaats=zoekGesorteerd(ongesorteerd.get()->sleutel);
        Lijstknoopptr<T> dummy=std::move(ongesorteerd);
        //vermits ongesorteerd een nullpointer is, is het equivalent van volgende lijnen ongeveer
        //ongesorteerd=std::move(dummy.get()->volgend);
        //std::swap(*plaats,dummy.get()->volgend);
        std::swap(ongesorteerd,dummy.get()->volgend);
        dummy.get()->volgend=std::move(*plaats);
        *plaats=std::move(dummy);

    };
    
    
};

template<class T>
void Lijst<T>::teken(const char * bestandsnaam) const{
    ofstream uit(bestandsnaam);
    assert(uit);
    uit<<"digraph {\nrankdir=\"LR\";\n\"0\"[label=\"\",shape=diamond];\n\"0\" -> \"1\";\n";
    int knoopteller=1;//knopen moeten een eigen nummer krijgen.
    const Lijst<T>* loper=this;
    while (*loper){
        uit<<"subgraph cluster_"<<knoopteller<<" {\nrankdir=\"LR\";\n";
        uit<<"\""<<knoopteller<<"\" [label=\""<<(*loper)->sleutel<<"\",color=white];\n";
        uit<<"\""<<knoopteller<<"v\" [shape=diamond,label=\"\"];\n";
        uit<<"\""<<knoopteller<<"\" -> \""<<knoopteller<<"v\" [color=white];\n";

        uit<<"}\n";
        uit<<"\""<<knoopteller<<"v\" -> \""<<knoopteller+1
           <<"\" [lhead=cluster_"<<knoopteller<<" ltail=cluster_"<<knoopteller+1<<"];\n";
        loper=&((*loper)->volgend);
        knoopteller++;
    };
    uit<<"\""<<knoopteller<<"\" [shape=point];\n";
    uit<<"}";
};

#endif

