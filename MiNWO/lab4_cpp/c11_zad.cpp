// Marius Rejdak
// Inf, OS1
// MiNWO, Lab4 24.04.2014

// zad1
// zapoznaj się z implementacją klasy info (klasa raportuje operacje na niej wykonywane, obiekty są automatycznie numerowane)
// dokończ implementację klasy schowek
// klasa schowek przechowuje (za pomocą wskaźnika) daną typu info, lub nic (np. po konstrukji bezargumentowej)
// klasa schowek winna posiadać zwykłe i przenoszące warianty konstruktora (kopiującego) i operatora przypisania oraz konstruktor
// z argumentem &info, konstruktor bezargumentowy i destruktor, metody te powinny na wyjście raportować wykonywane operacje;
// wywołanie metody przenoszącej schowka nie powinno powodować konstrukcji obiektu info, ani błędów przy poźniejszej destrukcji
// przeanalizuj efekty wywołania funkcji zad1 - wykonując ją krokowo oraz śledząc komunikaty wyprowadzanie na konsolę
// rozbuduj funkcję zad1, by ten sposób sprawdzić:
// -w jakich sytuacjach wywoływane są metody zwykłe, a kiedy przenoszące (w razie potrzeby dopisz nowe metody/operatory do schowka)?
// -co jest wywoływane przy konstrukcji/przypisaniu do schowka z wyrażenia zwracającego tymczasowy nienazwany schowek?
// -co jest wywoływane przy konstrukcji/przypisaniu do schowka ze zmiennej zwracanej z funkcji przez wartość?
// -wypróbuj działanie move(), co się stanie gdy użyjemy move(), a w klasie nie ma metod przenoszących(zakomentuj je chwilowo)?
// czy metody przenoszące poprawiają wydajność kodu?

// zad2
// zapoznaj się za strukturą osoba i funkcją zad2
// dokończ funkcję zad2 posługując się algorytmami stl (stable_sort for_each) i funkcjami lambda tak, aby
// - posortować listę liste osob los wg wieku osób
// - wyświetlić posortowane osoby (tylko imie i wiek każdej osoby)
// - zwiększyć wiek każdej z osób o 1 i ponownie wyświetlić listę
// - wyznaczyć średni wiek osoby, wyświetlić go i wyświetlić osoby starsze niż średnia




// c11.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h" //Nie działa na linuksie
#include <iostream>
#include <list>
#include <string>
#include <algorithm>

using namespace std;


class info
{
	static int serial;	// ++ przy konstrukcji
	const int  nr;		// =++serial przy konstrukcji
public:
	info():nr(++serial) {cout << "\ninfo() nr: " << nr;}
	info(const info &cri):nr(++serial) {cout << "\ninfo(const info &cri) nr: " << nr << " cri.nr: " << cri.nr;}
	~info() {cout << "\n~info() nr: " << nr;}
	info & operator=(const info &cri) {cout << "\ninfo & operator=(const info &cri) nr: " << nr << " cri.nr: " << cri.nr; return *this;}
	friend ostream& operator<<(ostream &os, const info& cri){return os<<" info nr: "<<cri.nr;}
};
int info::serial=0;		// static w klasie


struct osoba
{
	string imie, nazwisko;
	int wiek;
//	osoba():wiek(-1){}
	osoba(const string & imie, const string & nazwisko, const int wiek):imie(imie), nazwisko(nazwisko), wiek(wiek){}
	friend ostream& operator<<(ostream &os, const osoba& cro){return os<<endl<<cro.imie<<' '<<cro.nazwisko<<' '<<cro.wiek;}
};


class schowek
{
	info *ptr; // gdy ==nullptr, to schowek jest pusty
public:
	schowek():ptr(nullptr){cout<<"\nschowek()";}
	schowek(const info &crt):ptr(new info(crt)){cout<<"\nschowek(const info &crt)";}
	~schowek(){cout<<"\n~schowek()"; delete ptr;}
	friend ostream& operator<<(ostream &os, const schowek& crs){return crs.ptr?os<<*crs.ptr:os<<" nullptr";}

	//TODO

	//schowek(const schowek &crs);
	//schowek(schowek &&rrs);
	//schowek& operator=(const schowek &crs);
	//schowek& operator=(schowek &&rrs);
};


void zad1()
{
	info i1;
	schowek	si1(i1),
			si2(i1),
			si3;

	cout<<"\n\n zamiana s1-s2 z move(), przed "<<si1<<si2<<si3;

	si3=move(si2);
	si2=move(si1);
	si1=move(si3);

	cout<<"\n\n zamiana s1-s2 bez move(), przed "<<si1<<si2<<si3;

	si3=si2;
	si2=si1;
	si1=si3;

	cout<<"\n\n po zamianie "<<si1<<si2<<si3;

	// TODO

}


void zad2()
{
	list<osoba> los;
	los.push_back (osoba("Jan", "Kowalski", 25));
	los.push_back (osoba("Tomek", "Kowalski", 5));
	los.push_back (osoba("Anna", "Kowalska", 20));
	los.push_back (osoba("Antoni", "Kowalski", 65));
	los.push_back (osoba("Onufry", "Kowalski", 99));
	los.push_back (osoba("Zosia", "Kowalska", 3));

	cout<<"\n\n Lista osob:";
	for (auto p = los.begin(); p!=los.end(); ++p) cout<<*p;

	// od tąd bez iteratorow (deklarowanych jawnie ani auto), bez for, z algorytmami i funkcjami lambda

	//GCC wymaga vector dla stable_sort
	vector<osoba> los_sorted(los.begin(), los.end());

	cout<<"\n\n Sortowanie wg wieku...";
	stable_sort(los_sorted.begin(), los_sorted.end(), [](const osoba &lhs, const osoba &rhs){ return lhs.wiek < rhs.wiek; });

	cout<<"\n\n Lista (imie i wiek):";
	for_each(los_sorted.begin(), los_sorted.end(), [](osoba &o){ cout << "\n" << o.imie << " " << o.wiek; });

	cout<<"\n\n Zwiekszenie wieku:";
	for_each(los_sorted.begin(), los_sorted.end(), [](osoba &o){ o.wiek += 1; });

	cout<<"\n\n Lista:";
	for_each(los_sorted.begin(), los_sorted.end(), [](osoba &o){ cout << o; });

	cout<<"\n\n Sredni wiek...";
	double avg = 0;
	for_each(los.begin(), los.end(), [&avg](const osoba &o){ avg += o.wiek; });
	avg /= los.size();
	cout << "\n" << avg;

	cout<<"\n\n Starsi od sredniej:";
	for_each(los.begin(), los.end(), [avg](const osoba &o){ if(o.wiek > avg) cout << o; });
}


int main()
{
	zad1();
	zad2();
	return 0;
}












