/**
@file main.cpp
@brief test d'uso della classe dbuffer<int>
**/
#include <iostream>
#include <fstream>
#include "array3d.h" // array3d<int>
#include <cassert>   // assert

void test_fondamentali_int() {
	std::cout << "*** TEST METODI FONDAMENTALI ***" << std::endl;

	std::cout << "test ctor default" << std::endl;
	array3d<int> a1;
	assert(a1.getSize() == 0); // verifichiamo lo stato dell'oggetto

	std::cout << "test ctor secondario 1" << std::endl;
	array3d<int> a2(3,3,3);
	assert(a2.getSize() == 27);

	std::cout << "test ctor secondario 2" << std::endl;
	array3d<int> a3(3,3,3, -1);
	assert(a3.getSize() == 27);
	for (unsigned int i = 0; i < 3; i++)
		for (unsigned int j = 0; j < 3; j++)
			for (unsigned int k = 0; k < 3; k++)
				assert(a3(k,j,i) == -1);

	std::cout << "test copy constructor" << std::endl;
	array3d<int> a4(a3);
	// ATTENZIONE: una sintassi del tipo
	//             array3d<int> a4 = a3;
	//             EQUIVALE ALLA CHIAMATA AL COPY CONSTRUCTOR!
	assert(a4.getSize() == a3.getSize());
	for (unsigned int i = 0;i < a4.getRows(); i++)
		for (unsigned int j = 0; j < a4.getCol(); j++)
			for (unsigned int k = 0; k < a4.getDepth(); k++)
				assert(a3(k,j,i) == a4(k,j,i));

	std::cout << "test operatore assegnamento =" << std::endl;
	a1 = a3;
	for (unsigned int i = 0; i < a1.getRows(); i++)
		for (unsigned int j = 0; j < a1.getCol(); j++)
			for (unsigned int k = 0; k < a1.getDepth(); k++)
				assert(a3(k, j, i) == a1(k, j, i));
}
void test_array3d_int() {
	std::cout << "*** TEST METODI array3d<int> ***" << std::endl;

	array3d<int> a(3,3,3,0);//inizializzato tutto a 0, costruttore secondario
	std::cout << "test operator()" << std::endl;
	assert(a(0, 0, 0) == a(1, 0, 0));
	a(1, 0, 0) = 1;
	a(0, 0, 0) = a(1, 0, 0);
	assert(a(0, 0, 0) == a(1, 0, 0));
	std::cout << "test operator==" << std::endl;
	array3d<int> b(3, 3, 3, 2);
	array3d<int> c(3, 3, 3, 2);
	assert(b == c);

	std::cout << "test slice()" << std::endl;
	array3d<int> s(3, 3, 3, 1);
	std::cout << s.slice(0,1,0,1,0,1) << std::endl;
	/* 1 1
	   1 1 
	   
	   1 1
	   1 1*/
	

	std::cout << "test trasform()" << std::endl;

	/*
	 test fill in the method void test_iteratori_int()
	*/
	
}

void test_array3d_const_helper_int(const array3d<int>& m) {}
/**
  test dei metodi d'uso per un dbuffer<int> const
*/void test_array3d_const_int() {}

void serializzazione_array3d_int() {}

void test_conversione_tipo_int() {
	array3d<int> d1(3,3,3);

	array3d<int> d2;

	d2 = d1; // op assegnamento

	d2 = array3d<int>(3,3,3); // conversione di tipo esplicita

	std::swap(d1, d2);

	array3d<int>::size_type s; // uso dei typedef
}

void test_iteratori_int() {
	array3d<int> a(3, 3, 3, 0);
	array3d<int> b(3, 3, 3, 0);
	int count = 0;  //filling b
	for (int i = 0; i < b.getDepth(); i++)
		for (int j = 0; j < b.getRows(); j++)
			for (int k = 0; k < b.getCol(); k++)
				b(k, j, i) = count++;
	std::cout << a << std::endl;
	array3d<int>::iterator inizio;
	array3d<int>::iterator fine;
	inizio = b.begin();
	fine = b.end();
	a.fill(inizio, fine);
	std::cout << a << std::endl;
	inizio = a.begin();
	*inizio = 100;
	assert(a(0, 0, 0) == 100);
}

struct utente {
	std::string nome;
	std::string cognome;

	utente(const std::string& n, const std::string& c) :
		nome(n), cognome(c) {}
};

int main(int argc, char* argv[]) {
	// Test con array3d su interi: array3d<int> 
	array3d<int> b(3, 3, 3, 0);
	int count = 0;  //filling b
	for (int i = 0; i < b.getDepth(); i++)
		for (int j = 0; j < b.getRows(); j++)
			for (int k = 0; k < b.getCol(); k++)
				b(k, j, i) = count++;
	std::cout << b << std::endl << std::endl << b.slice(0, 1, 0, 1, 0, 1) << std::endl;
	//test_fondamentali_int();

	//test_array3d_int();

	//test_array3d_const_int();

	//serializzazione_array3d_int();

	//test_conversione_tipo_int();

	//test_iteratori_int();

	// Test con dbuffer su tipi custom: es. dbuffer<utente>

	// Rifare tutti i test come sopra
	//
	// test_dbuffer_utente();
	// ...
	return 0;
}