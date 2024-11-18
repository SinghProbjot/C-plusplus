#include <iostream>
#include <cassert>
#include "olist.h"

/**
  @brief Funtore di ordinamento tra tipi interi

  Ordina due interi in ordine crescente. 
*/
struct int_crescent {
  bool operator()(int a, int b) const {
    return a<b;
  }
};

/**
  @brief Funtore di ordinamento tra tipi interi

  Ordina due interi in ordine decrescente. 
*/
struct int_decrescent {
  bool operator()(int a, int b) const {
    return a>b;
  }
};

/**
  @brief Funtore predicato 

  Ritorna true se il valore intero passat è pari 
*/
struct is_even {
  bool operator()(int a) const {
    return (a % 2 ==0);
  }
};


// Typedef della classe lista su interi di comodo
typedef ordered_list<int, int_crescent> olint;


/**
  Test dei metodi fondamentali

  @brief Test dei metodi fondamentali
*/
void test_metodi_fondamentali() {
  std::cout<<"******** Test metodi fondamental della lista di interi ********"<<std::endl;

   olint ol;  // ctor default

   ol.add(5); // add 
   ol.add(8);
   ol.add(1);
   ol.add(4);

   std::cout << "Stampa di ol dopo inserimenti:" << std::endl;
   std::cout << ol << std::endl;  // operator<<

   olint ol2(ol);  // cctor

   std::cout << "Stampa di ol2 dopo copy constructor:" << std::endl;
   std::cout << ol2 << std::endl;  // operator<<

   olint ol3;

   ol3 = ol; // operator=

   std::cout << "Stampa di ol3 dopo assegnamento:" << std::endl;
   std::cout << ol3 << std::endl;  // operator<<

} // ~ordered_list()


/**
  Test d'uso della lista ordinata

  @brief Test d'uso della lista ordinata
*/
void test_uso() {
  std::cout<<"******** Test d'uso della lista di interi ********"<<std::endl;

  int a[5] = {20,10,5,100,80};

  olint ol(a,a+5); // ctor con i due iteratori

  std::cout << "Stampa di ol dopo costruttore secondario:" << std::endl;
  std::cout << ol << std::endl;  // operator<<

  std::cout << "Dimensione di ol:" << std::endl;
  std::cout << ol.size() << std::endl;
  assert(ol.size() == 5);

  bool ok1 = ol.exists(5);
  assert(ok1 == true);

  bool ok2 = ol.exists(99);
  assert(ok2 == false);
}

/**
  Test della funzione checkif

  @brief Test della funzione checkif
*/
void test_checkif() {
  int dati[5] = {5,8,4,9,2};

  ordered_list<int,int_crescent> ol3(dati, dati+5);

  std::cout<<"******** Test di checkif ********"<<std::endl;

  std::cout<<"Stampa con operator<<"<<std::endl;
  std::cout<< ol3 << std::endl;
  is_even ie;

  std::cout<<"Chiamata checkif is_even"<<std::endl;
  checkif(ol3,ie);
}


/**
  Test d'uso della lista ordinata non constante passata come parametro

  @brief Test d'uso della lista ordinata non constante

  @param ol lista da usare per i test
*/
void test_list_int(olint &ol) {
  std::cout<<"******** Test sulla lista di interi ********"<<std::endl;

  std::cout<<"Insertimento dei valori 56, 90, 60, 5, 60"<<std::endl;
  ol.add(56);
  ol.add(90);
  ol.add(60);
  ol.add(5);
  ol.add(60);

  std::cout<<"Stampa con operator<<"<<std::endl;
  std::cout<<ol<<std::endl;

  std::cout<<"Dimensione della lista: "<<ol.size()<<std::endl;

  std::cout<<"Stampa con iteratori"<<std::endl;

  olint::const_iterator i,ie;

  for(i=ol.begin(),ie=ol.end(); i!=ie; ++i)
    std::cout<<*i<<std::endl;

  ol.clear();
  std::cout<<"Dimensione della lista dopo clear(): "<<ol.size()<<std::endl;
}

/**
  Test d'uso della lista ordinata constante passata come parametro

  @brief Test d'uso della lista ordinata constante

  @param ol lista da usare per i test
*/
void test_const_list_int(const olint &ol) {
  std::cout<<"******** Test sulla lista costante di interi ********"<<std::endl;

  // add non e' chiamabile su un oggetto const

  std::cout<<"Stampa con operator<<"<<std::endl;
  std::cout<<ol<<std::endl;

  std::cout<<"Dimensione della lista: "<<ol.size()<<std::endl;

  std::cout<<"Stampa con iteratori"<<std::endl;

  olint::const_iterator i,ie;

  for(i=ol.begin(),ie=ol.end(); i!=ie; ++i)
    std::cout<<*i<<std::endl;

  // clear non e' chiamabile su un oggetto const
}

//--------------------------------------------------------------------

/**
  Funtore per il confronto tra stringhe. La valutazione e' fatta
  sulla lunghezza.  Ritorna true se la prima stringa e' piu' corta
  della seconda. 

  @brief Funtore per il confronto tra stringhe.
*/
struct compare_string {
  bool operator()(const std::string &a, const std::string &b) const {
    return (a.size()<=b.size());
  } 
};

/**
  Test della lista ordinata istanziata su stringhe

  @brief Test della lista ordinata istanziata su stringhe
*/
void test_lista_di_stringhe(void) {
  std::cout<<"******** Test sulla lista di stringhe ********"<<std::endl;

  ordered_list<std::string, compare_string> ols;

  std::cout<<"Insertimento dei valori 'pippo', 'pluto', 'paperino', 'cip'"<<std::endl;
  ols.add("pippo");
  ols.add("pluto"); 
  ols.add("paperino");
  ols.add("cip");

  std::cout<<"Stampa con operator<<"<<std::endl;
  std::cout<<ols<<std::endl;

  std::cout<<"Dimensione della lista: "<<ols.size()<<std::endl;

  std::cout<<"Ricerca di 'cip': "<<ols.exists("cip")<<std::endl;
  std::cout<<"Ricerca di 'cipp': "<<ols.exists("cipp")<<std::endl;

  //E' possibile usare le asserzioni per verificare i test
  //assert(ols.exists("cip")==true);
}


//--------------------------------------------------------------------

/**
  Struct point che implementa un punto 2D.

  @brief Struct point che implementa un punto 2D.
*/
struct point {
  int x; ///< coordinata x del punto
  int y; ///< coordinata y del punto

  point(int xx, int yy) : x(xx), y(yy) {}
};

/**
  Ridefinizione dell'operatore di stream << per un point.
  Necessario per l'operatore di stream della classe ordered_list.
*/
std::ostream &operator<<(std::ostream &os, const point &p) {
  std::cout<<"("<<p.x<<","<<p.y<<")";
  return os;
}

/**
  Funtore per il confronto di due punti. 
  Ritorna true se p1.x < p2.x.

  @brief Funtore per il confronto di due punti.
*/
struct compare_point {
  bool operator()(const point &p1, const point &p2) const {
    return (p1.x<p2.x);
  } 
};

/**
  Test della lista ordinata istanziata su oggetti point

  @brief Test della lista ordinata istanziata su oggetti point
*/
void test_lista_di_point(void) {

  ordered_list<point,compare_point> op;

  std::cout<<"******** Test sulla lista di point ********"<<std::endl;

  std::cout<<"Insertimento dei valori (1,1), (1,2), (2,7), (0,0), (5,4)"<<std::endl;
  op.add(point(1,1));
  op.add(point(1,2));
  op.add(point(2,7));
  op.add(point(0,0));
  op.add(point(5,4));

  std::cout<<"Stampa con operator<<"<<std::endl;
  std::cout<<op<<std::endl;

  std::cout<<"Dimensione della lista: "<<op.size()<<std::endl;

  std::cout<<"Stampa con iteratori"<<std::endl;

  ordered_list<point,compare_point>::const_iterator i,ie;

  for(i=op.begin(),ie=op.end(); i!=ie; ++i)
    std::cout<<*i<<std::endl;

  std::cout<<"Ricerca di '(1,1)': "<<op.exists(point(1,1))<<std::endl;
  std::cout<<"Ricerca di '(2,2)': "<<op.exists(point(2,2))<<std::endl;
}


int main() {
  test_metodi_fondamentali(); 

  test_uso();

  test_checkif();

  olint oli;

  test_list_int(oli); 

  test_const_list_int(oli); 

  test_lista_di_stringhe();

  test_lista_di_point();

}