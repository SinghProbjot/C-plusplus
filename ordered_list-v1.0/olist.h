#ifndef OLIST_H
#define OLIST_H

#include <iostream> // cout ostream
#include <algorithm> // swap
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
  @brief classe lista ordinata

  La classe imlementa una generica lista ordinata di oggetti T.
  L'ordinamento è realizzato tramite il funtore Cmp che prende
  due valori a e b, e ritorna vero se i due valori sono nell'ordine
  voluto.
  La valutazione se due dati di tipo T sono uguali è realizzato
  tramite lo stesso funtore (vedere la funzione exists). Potevamo
  anche inserire un secondo funtore ad-hoc che avrebbe reso la 
  classe più flessibile. 

*/
template <typename T, typename Cmp>
class ordered_list {

  /**
    @brief struttira nodo

    Struttura dati nodo interna che viene usata per creare
    la lista.  
  */
  struct node {
    T value; ///< valore da memorizzare 
    node *next; ///< puntatore al prossimo nodo della lista

    /**
      Costruttore di default
      @post next == nullptr
    */
    node() : next(nullptr) {}

    /**
      Costruttore secondario

      @param v valore da colpiare
      @param n puntatore a next (con valore di default)

      @post next == n
      @post value == v
    */
    node(const T &v, node* n=nullptr) :value(v), next(n) {}

    /**
      Copy constructor. Copiamo i dati membro a membro.
      In questo caso essendo una struttura interna di suporto
      a ordered_list, va bene la condivisione dei dati.

      @param other nodo da copiare
    */
    node(const node &other) : value(other.value), next(other.next) {

    }

    /**
      Operatore di assegnamento. 

      @param other nodo da copiare
      @return reference al nodo this
    */
    node& operator=(const node &other) {
      // IL controllo dell'auto-assegnamento possiamo
      // ignorarlo in questo caso visti i dati coinvolti.
      value = other.value;
      next = other.next;
      return *this;
    }

    /**
      Distruttore. 
    */
    ~node() { }

  }; // struct node

  node *_head; ///< puntatore al primo nodo della lista
  unsigned int _size; ///< numero di elementi nella lista
  Cmp _compare; ///< funtore per il confronto tra dati T


public:

  /**
    Costruttore di default.

    @post _head == nullptr
    @post _size == 0
  */
  ordered_list() : _head(nullptr), _size(0) {}


  /**
    Copy constructor

    @param other lista da copiare

    @post _size = other._size
  */
  ordered_list(const ordered_list &other) : _head(nullptr), _size(0) {
    node *curr = other._head;

    // Cicliamo sui nodi di other e mano a mano
    // aggiungiamo un valore dentro this. La add
    // prevede una new e quindi può fallire. Per questo
    // dobbiamo gestire l'evenetuale eccezione che si 
    // genera con un blocco try/catch
    try {
      while(curr != nullptr) {
        add(curr->value);
        curr = curr->next;
      }
    }
    catch(...) {
      // Se c'e' un problema, svuotiamo la lista e rilanciamo
      // l'eccezione
      clear();
      throw;
    }
  }

  /**
    Operatore di assegnamento

    @param other lista da copiare

    @return reference alla lista this

    @post _size = other._size
  */
  ordered_list &operator=(const ordered_list &other) {
    if (this!=&other) {
      ordered_list tmp(other);
      this->swap(tmp);
    }
    return *this;
  }

  /**
    Distruttore 

    @post _head == nullptr
    @post _size == 0
  */
  ~ordered_list() {
    clear();
  }

  /**
    Costruttore che crea una ordered_list riempita con dati
    presi da una sequenza identificata da uniteratore di 
    inizio e uno di fine. Gli iteratori possono essere di
    qualunque tipo. 

    @param b iteratore di inizio sequenza
    @param e iteratore di fine sequenza

    @throw std::bad_alloc possibile eccezione di allocazione
  */
  template <typename Iter>
  ordered_list(Iter b, Iter e) : _head(nullptr), _size(0) {
    try {
      for(;b!=e; ++b)
        add(static_cast<T>(*b));
    }
    catch (...) {
      clear();
      throw;
    }
  }

  void swap(ordered_list &other) {
    std::swap(_head, other._head);
    std::swap(_size, other._size);
  }

  /**
    Svuota la lista

    @post _head == nullptr
    @post _size == 0
  */
  void clear() {
    node *curr = _head;
    while(curr!=nullptr) {
      node *n = curr->next;
      delete curr;
      //curr = nullptr;
      curr = n;
    }
    _head = nullptr;
    _size = 0;
  }

  /**
    Ritorna il numero di elementi nella lista

    @return il numero di elementi nella lista
  */
  unsigned int size() const {
    return _size;
  }

  /**
    Aggiunge un elemento nella lista mantenendo l'ordinamento
    indotto dal funtore di tipo Comp.

    @param v valore da inserire nella lista

    @post _size = _size+1

    @throw std::bad_alloc possibile eccezione di allocazione
  */
  void add(const T&v) {
    node *n = new node(v);

    ++_size;

    // Caso lista vuota
    if (_head == nullptr) {
      _head = n;
      return;
    }

    node *curr = _head;
    node *prev = _head;

    // Dobbiamo ciclare sui nodi fino a trovare la posizione
    // giusta di inserimento o fino alla fine della lista
    while (curr!=nullptr && _compare(curr->value,v)) {
      prev = curr;
      curr = curr->next;
    }

    // Caso inserimento in testa
    if (curr == _head) {
      _head = n;
      n->next = curr;
      return;
    }

    prev->next = n;
    n->next = curr;
  }

  /**
    Cerca se un valore passato è presente nella lista. Usa il
    funtore di confronto.

    @param v valore da cercare nella lista

    @return true se il valore è stato trovato
  */ 
  bool exists(const T &v) const {
    node *curr = _head;
    while (curr != nullptr) {
      bool c1 = _compare(curr->value,v);
      bool c2 = _compare(v,curr->value);
      if (c1 == c2) return true;

      curr = curr->next;
    }
    return false;
  }

  /**
    Funzione GLOBALE che implementa l'operatore di stream.
    La funzione è templata sull'ordered list ed è messa
    friend perchè accediamo ai dati privati di ordered_list.

    @param os stream di output
    @param ol ordered_list da spedire sullo stream

    @return lo stream di output
  */
  friend std::ostream&operator<<(std::ostream &os, 
    const ordered_list &ol) {
    os << "size: " << ol._size << std::endl;

    node *curr = ol._head;
    while(curr!=nullptr) {
      os << curr->value <<std::endl;
      curr = curr->next;
    }
    return os;
  }

  /**
    Gli iteratori devono iterare sui dati inseriti nella classe
    principale. Siccome nella ordered_list mettiamo dei dati di 
    tipo T, l'iteratore deve iterare sui dati di tipo T. NON sui
    nodi della lista che sono un dettaglio implementativo interno.
  */
  class const_iterator {
    //  
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T                        value_type;
    typedef ptrdiff_t                 difference_type;
    typedef const T*                  pointer;
    typedef const T&                  reference;

  
    const_iterator() : ptr(nullptr){
      //!!!
    }
    
    const_iterator(const const_iterator &other) : ptr(other.ptr) {
      //!!!
    }

    const_iterator& operator=(const const_iterator &other) {
      ptr = other.ptr;
      return *this;
    }

    ~const_iterator() {
      //!!!
    }

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const {
      return ptr->value;
    }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const {
      return &(ptr->value);
    }
    
    // Operatore di iterazione post-incremento
    const_iterator operator++(int) {
      const_iterator tmp(ptr);
      ptr = ptr->next;
      return tmp;
    }

    // Operatore di iterazione pre-incremento
    const_iterator& operator++() {
      ptr = ptr->next;
      return *this;
    }

    // Uguaglianza
    bool operator==(const const_iterator &other) const {
      return ptr == other.ptr;
    }
    
    // Diversita'
    bool operator!=(const const_iterator &other) const {
      return ptr != other.ptr;
    }

  private:
    const node *ptr;

    // La classe container deve essere messa friend dell'iteratore per poter
    // usare il costruttore di inizializzazione.
    friend class ordered_list; 

    // Costruttore privato di inizializzazione usato dalla classe container
    // tipicamente nei metodi begin e end
    const_iterator(const node *p) : ptr(p) { 
    }
    
    // !!! Eventuali altri metodi privati
    
  }; // classe const_iterator
  
  // Ritorna l'iteratore all'inizio della sequenza dati
  const_iterator begin() const {
    return const_iterator(_head);
  }
  
  // Ritorna l'iteratore alla fine della sequenza dati
  const_iterator end() const {
    return const_iterator(nullptr);
  }
  
};

/**
  Funzione GLOBALE che stampa a schermo i soli valori 
  contenuti in una generica ordered_list che soddisfano 
  un predicato generico di tipo P.

  @param ol ordered_list sorgente
  @param pred predicato

*/
template <typename T, typename C, typename P> 
void checkif(const ordered_list<T,C> &ol, P pred) {
  typename ordered_list<T,C>::const_iterator i, ie;
  
  for (i = ol.begin(), ie=ol.end(); i!=ie; ++i)
    if (pred(*i))
      std::cout << *i << std::endl; 
}

#endif
