/**
@file main.cpp 
@brief test d'uso della classe dbuffer<int>
**/


// NOTA

#include <iostream>
#include <fstream>
#include "dbuffer.h" // dbuffer<int>
#include <cassert>   // assert


void test_fondamentali_int() {

  std::cout << "*** TEST METODI FONDAMENTALI ***" << std::endl;

  std::cout << "test ctor default" << std::endl;
  dbuffer<int> db1;
  assert(db1.size() == 0); // verifichiamo lo stato dell'oggetto

  std::cout << "test ctor secondario 1" << std::endl;
  dbuffer<int> db2(100);
  assert(db2.size() == 100);

  std::cout << "test ctor secondario 2" << std::endl;
  dbuffer<int> db3(50,-1);
  assert(db3.size() == 50);
  for(unsigned int i =0; i < 50; i++)
    assert(db3[i] == -1);

  std::cout << "test copy constructor" << std::endl;
  dbuffer<int> db4(db3); 
  // ATTENZIONE: una sintassi del tipo
  //             dbuffer<int> db4 = db3;
  //             EQUIVALE ALLA CHIAMATA AL COPY CONSTRUCTOR!
  assert(db4.size() == db3.size());
  for(unsigned int i =0 ; 
    i < db4.size(); i++)
    assert(db3[i] == db4[i]);

  std::cout << "test operatore assegnamento =" << std::endl;
  db1 = db3;
  assert(db1.size() == db3.size());
  for(unsigned int i =0 ; 
    i < db1.size(); i++)
    assert(db3[i] == db1[i]);
}

/**
  test dei metodi d'uso per un dbuffer<int> non const
*/
void test_dbuffer_int() {
  std::cout << "*** TEST METODI dbuffer<int> ***" << std::endl;

  dbuffer<int> db(5,0);

  std::cout << "test value" << std::endl;
  db.value(1) = 100;
  assert(db.value(1) == 100);

  std::cout << "test set_value" << std::endl;
  db.set_value(0,99);
  assert(db.value(0) == 99);

  std::cout << "test scrittura op[]" << std::endl;
  db[3] = 5;
  assert(db[3]==5);

  // Se compilazione in modalita' debug da' errore 
  //db[100] = 5;

  std::cout << "test lettura op[]" << std::endl;
  int a = db[3];
  assert(a==5);

  // Se compilazione in modalita' debug da' errore 
  //int tmp = db[8];

  std::cout << "test stampa con operator<<" << std::endl;
  std::cout << db << std::endl;

}

/**
  funzione helper per i test dei metodi d'uso per un dbuffer<int> const
*/
void test_dbuffer_const_helper_int(const dbuffer<int> &db) {

  // Tutti i metodi di scrittura devono dare errore
  // db[1] = 10; // errore
  // db.value(1) = 10; // errore
  // db.set_value(1,10); // errore

  // I metodi di lettura devono funzionare
  int tmp1 = db[3];
  int tmp2 = db.value(3);
  int tmp3 = db.get_value(3);

  std::cout << "test stampa con operator<<" << std::endl;
  std::cout << db << std::endl;


  //dbuffer<int> db2;
  //db2.swap(db); // errore
} 

/**
  test dei metodi d'uso per un dbuffer<int> const
*/void test_dbuffer_const_int() {
  dbuffer<int> db(5,0);

  test_dbuffer_const_helper_int(db);  
}

/**
  Esempio di come si può serializzare un oggetto su file. 
*/
void serializzazione_dbuffer_int() {
  std::cout << std::endl << "*** PROVA DI SERIALIZZAZIONE ***" << std::endl;

  // dbuffer<int> di test
  dbuffer<int> src(5,9999);

  // Usando operator<< possiamo scrivere su file il contenuto del dbuffer<int>.
  // E' importante scrivere operator<< in modo tale da spedire SOLO i dati
  // e niente altro. Nel caso specifico vengono spediti nell'ordine la size 
  // e gli interi dell'array.  
  std::ofstream ofs;
  ofs.open("output.txt"); // il file è aperto come testo
  if (!ofs.is_open()) {
    std::cerr << "ERRORE NELLA'APERTURA DEL FILE" << std::endl;
    return;
  }
  ofs << src << std::endl;
  ofs.close();

  // Riapriamo il file e creiamo un secondo dbuffer<int> con i valori salvati 
  std::ifstream ifs;
  ifs.open("output.txt");
  if (!ifs.is_open()) {
    std::cerr << "ERRORE NELLA'APERTURA DEL FILE" << std::endl;
    return;
  }
  // Leggiamo la size
  unsigned int size;
  ifs >> size;

  // Creiamo un dbuffer<int> con la dimensione letta 
  dbuffer<int> dst(size);

  // Leggiamo i size interi dal file direttamente nelle celle dell'array 
  for(unsigned int i=0; i<size;++i)
    ifs >> dst[i];
  ifs.close();

  std::cout <<"Contenuto del dbuffer<int> src:" << std::endl;
  std::cout << src << std::endl;

  std::cout <<"Contenuto del dbuffer<int> dst:" << std::endl;
  std::cout << dst << std::endl;
}

void test_conversione_tipo_int() {
  dbuffer<int> d1(10);

  dbuffer<int> d2;
  
  d2 = d1; // op assegnamento
  
  d2 = dbuffer<int>(3); // conversione di tipo esplicita
  
  std::swap(d1,d2);
  
  dbuffer<int>::size_type s; // uso dei typedef
} 

void test_iteratori_int() {
  dbuffer<int> d(10);

  for (unsigned int j = 0; j < 10; ++j)
    d[j] = j;

  dbuffer<int>::iterator it; // iteratore di inizio lettura/scrittura
  dbuffer<int>::iterator ite;// iteratore di fine lettura/scrittura

  ite = d.end(); // chiediamo l'iteratore di fine

  for (it = d.begin(); it!=ite; ++it)
    std::cout << *it << std::endl;

  it = d.begin();
  *it = 100; // modifico il primo elemento della sequenza di dati

  assert(d[0] == 100);


  dbuffer<int>::const_iterator cit; // iteratore in lettura

  cit = d.begin();

  assert(*cit == 100); 

  // *cit = 0; // errore

  // NOTA1: gli iteratori sono come i puntatori. Possono 
  //        "puntare" a delle posizioni non valide se 
  //        iteriamo troppo.
  // 
  // NOTA2: non dereferenziare MAI l'iteratore di fine: 
  //        Non punta a dei dati utili.
}

struct utente {
  std::string nome;
  std::string cognome;

  utente(const std::string &n, const std::string &c) : 
    nome(n), cognome(c) {}
};

int main(int argc, char *argv[]) {
  
  // Test con dbuffer su interi: dbuffer<int> 
  test_fondamentali_int();
  
  test_dbuffer_int();
  
  test_dbuffer_const_int();
  
  serializzazione_dbuffer_int();

  test_conversione_tipo_int();
  
  test_iteratori_int();

  // Test con dbuffer su tipi custom: es. dbuffer<utente>

  // Rifare tutti i test come sopra
  //
  // test_dbuffer_utente();
  // ...
  return 0;
}