#ifndef DBUFFER_H
#define DBUFFER_H

#include <ostream> // std::ostream
#include <cassert> 
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t

/**
  @file dbuffer.h
  @brief Dichiarazione della classe dbuffer
*/

/**
  @brief Classe dbuffer

  Classe che vuole rappresentare un array dinamico di interi.
*/
template <typename T>
class dbuffer {

public:
  
  typedef T value_type; ///< Tipo del dato dell'array
  typedef unsigned int size_type; ///< Tipo del dato size
 
  /**
    @brief Costruttore di default

    1° METODO FONDAMENTALE: COSTRUTTORE DI DEFAULT
    Il cosruttore di default serve a inizializzare i dati membro
    per rappresentare un dbuffer vuoto

    @post _buffer = nullptr
    @post _size = 0
  */
  dbuffer() : _buffer(nullptr), _size(0) { // Initialization list
  // I costruttori messi nella initialization list servono per
  // inizializzare (creare + impostare un valore) i dati membro.
  // Sono eseguiti prima di qualunque operazione interna al 
  // costruttore.
  
  #ifndef NDEBUG
  std::cout << "dbuffer::dbuffer()" << std::endl;
  #endif
}

  /**
    @brief Costruttore secondario

    Costruttore secondario che serve a costruire un dbuffer alla 
    dimensione data. Le celle dell'array non sono inizializzate.

    @param sz dimensione dell'array da creare

    @post _buffer != nullptr
    @post _size = sz 
  */
  explicit dbuffer(size_type sz) : _buffer(nullptr), _size(0) {
  _buffer = new value_type[sz];
  _size = sz;
  
  #ifndef NDEBUG
  std::cout << "dbuffer::dbuffer(size_type)" << std::endl;
  #endif
}

  /**
    @brief Costruttore secondario

    Costruttore secondario che serve a costruire un dbuffer alla 
    dimensione data e con le celle dell'array inizializzate al valore
    passato.

    @param sz dimensione dell'array da creare
    @param value valore da usare per inizializzare le celle dell'array

    @post _buffer != nullptr
    @post _size = sz 
  */
  dbuffer(size_type sz, const value_type &value) : _buffer(nullptr), _size(0) {

  _buffer = new value_type[sz];
  _size = sz;

  try {
    for(unsigned int i=0; i<_size; ++i)
      _buffer[i] = value;
  }
  catch(...) {
    delete[] _buffer;
    _buffer = nullptr;
    _size =0;

    throw; // rilancio dell'eccezione !!
  }

  #ifndef NDEBUG
  std::cout << "dbuffer::dbuffer(size_type, value_type)" << std::endl;
  #endif
}

  /**
    @brief Distruttore

    2° METODO FONDAMENTALE: DISTRUTTORE
    Distruttore della classe. Il distruttore deve rimuovere tutte 
    le risorse usate dalla classe. In questo caso l'array allocato 
    sullo heap deve essere deallocato.
  */
  ~dbuffer()  {
  delete[] _buffer;
  _buffer = nullptr;
  _size = 0;

  #ifndef NDEBUG
  std::cout << "dbuffer::~dbuffer()"<< std::endl;
  #endif
}

  /**
    @brief Copy Constructor

    3° METODO FONDAMENTALE: COPY CONSTRUCTOR
    Costruttore di copia. Serve a creare un oggetto come copia di un 
    altro oggetto. I due oggetti devono essere indipendenti.

    @param other dbuffer sorgente da copiare
    
    @post _buffer != nullptr
    @post _size = other._size
  */
  dbuffer(const dbuffer &other) : _buffer(nullptr), _size(0) {
  _buffer = new value_type[other._size];
  _size = other._size;
  try {
    for(size_type i=0; i<_size; ++i)
      _buffer[i] = other._buffer[i];
  }
  catch(...) {
    delete[] _buffer;
    _buffer = nullptr;
    _size =0;
    throw;
  }
  #ifndef NDEBUG
  std::cout << "dbuffer::dbuffer(const dbuffer&)"<< std::endl;
  #endif
}

  /**
    @brief Operatore di assegnamento

    4° METODO FONDAMENTALE: OPERATORE ASSEGNAMENTO
    L'operatore di assegnamento serve a copiare il contenuto di un oggetto
    in un altro oggetto dello stesso tipo. I due oggetti devono essere
    indipendenti.

    @param other dbuffer sorgenete da copiare

    @return un reference all'oggetto corrente

    @post _dbuffer != nullptr
    @post _size = other._size 
  */
  dbuffer& operator=(const dbuffer &other){
  if (this != &other) {

    dbuffer tmp(other);

    this->swap(tmp);
  }

  #ifndef NDEBUG
  std::cout << "dbuffer::operator=(const dbuffer &)" << std::endl;
  #endif

  return *this;
}


  /**
    @brief Accesso alla dimensione dell'array (stile C++)

    Metodo per ottenere la dimensione dell'array dinamico

    @return dimensione dell'array dinamico
  */
  size_type size(void) const{
    return _size;
  }

  /**
    @brief Accesso ai dati in lettura (stile Java)

    Metodo getter per leggere il valore index-esimo dell'array

    @param index indice della cella dell'array da leggere

    @return valore della cella index-esima

    @pre index < size()
  */
  value_type get_value(size_type index) const {
    assert(index<_size);

    return _buffer[index];
  }

  /**
    @brief Accesso ai dati in scrittura (stile Java)

    Metodo setter per scrivere un valore nella cella index-esima dell'array

    @param index indice della cella dell'array da scrivere
    @param value valore da scrivere nella cella

    @pre index < size()
  */  
  void set_value(unsigned int index, const value_type &value){
  assert(index<_size);

  _buffer[index] = value;
}

  /**
    @brief Getter/Setter della cella index-esima (stile C++)

    Metodo che permette di leggere e/o scrivere la cella
    index-esima dell'array

    @param index della cella da leggere/scrivere

    @return reference alla cella index-esima 

    @pre index < size()
  */
  value_type &value(size_type index) { 
  assert(index<_size);

  return _buffer[index];
}

  /**
    @brief Getter della cella index-esima (stile C++)

    Metodo che permette di leggere la cella
    index-esima dell'array. Il metodo si può usare
    solo su istanze costanti della classe.

    @param index della cella da leggere

    @return reference alla cella index-esima 

    @pre index < size()
  */
  const value_type &value(size_type index) const { 
  assert(index<_size);

  return _buffer[index];
}

  /**
    @brief Getter/Setter della cella index-esima (stile op[])

    Metodo che permette di leggere e/o scrivere la cella
    index-esima dell'array

    @param index della cella da leggere/scrivere

    @return reference alla cella index-esima 

    @pre index < size()
  */
  value_type &operator[](size_type index) { 
  assert(index<_size);

  return _buffer[index];
}

  /**
    @brief Getter della cella index-esima (stile op[])

    Metodo che permette di leggere la cella
    index-esima dell'array. Il metodo si può usare
    solo su istanze costanti della classe.

    @param index della cella da leggere

    @return reference alla cella index-esima 

    @pre index < size()
  */
  const value_type &operator[](size_type index) const { 
  assert(index<_size);

  return _buffer[index];
}


  /**
    @brief Metodo swap per la classe dbuffer

    Funzione che scambia il contenuto di due dbuffer

    @param other il dbuffer con cui scambiare il contenuto
  */
  void swap(dbuffer &other) {
    std::swap(_buffer, other._buffer);
    std::swap(_size, other._size); 
  }

  /**
    @brief ridefinizione operatore di stream

    Ridefinizione dell'operatore di stream per scrivere un
    dbuffer su uno stream di output

    @param os stream di output (operando di sinistra)
    @param db dbuffer da scrivere (operando di destra)

    @return reference allo stream di output
  */
  // Diamo accesso alla funzione globale, esterna alla classe, alle
  // parti private della classe
  friend std::ostream& operator<<(std::ostream &os, const dbuffer<T> &db) {
    os << db._size << ' ';
    for(typename dbuffer<T>::size_type i = 0; i < db.size() ; i++)
      os << db[i] << ' ';
  
    return os;
  }

/*
  // Versione "veloce" dell'iteratore di tipo random
  // Dato che noi abbiamo un puntatore ai dati di un array 
  // e un puntatore è anche un iteratore random, possiamo
  // usare typedef per mappare le classi iteratore ai rispettivi puntatori   
  typedef value_type *iterator;

  iterator begin() {
    return _buffer;
  }

  iterator end() {
    return _buffer + _size;
  }

  typedef const value_type *const_iterator;

  const_iterator begin() const {
    return _buffer;
  }

  const_iterator end() const {
    return _buffer + _size;
  }

*/

  // Implementazione completa di ietartori di tipo random.
  // Come si vedrà, i metodi sono solo dei wrapper a delle
  // operazioni tra puntatori.

  class const_iterator; // forward declaration

  class iterator {
    //  
  public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef T                        value_type;
    typedef ptrdiff_t                difference_type;
    typedef T*                       pointer;
    typedef T&                       reference;

  
    iterator() {
      ptr = nullptr;
    }
    
    iterator(const iterator &other) {
      ptr = other.ptr;
    }

    iterator& operator=(const iterator &other) {
      ptr = other.ptr;
      return *this;
    }

    ~iterator() { }

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const {
      return *ptr;
    }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const {
      return ptr;
    }

    // Operatore di accesso random
    reference operator[](int index) {
      return ptr[index];
    }
    
    // Operatore di iterazione post-incremento
    iterator operator++(int) {
      iterator old(*this);
      ++ptr;
      return  old;
    }

    // Operatore di iterazione pre-incremento
    iterator &operator++() {
      ++ptr;
      return *this;
    }

    // Operatore di iterazione post-decremento
    iterator operator--(int) {
      iterator old(*this);
      --ptr;
      return old;
    }

    // Operatore di iterazione pre-decremento
    iterator &operator--() {
      --ptr;
      return *this;
    }

    // Spostamentio in avanti della posizione
    iterator operator+(int offset) {
      return iterator(ptr+offset);
    }

    // Spostamentio all'indietro della posizione
    iterator operator-(int offset) {
      return iterator(ptr-offset);
    }
    
    // Spostamentio in avanti della posizione
    iterator& operator+=(int offset) {
      ptr+=offset;
      return *this;
    }

    // Spostamentio all'indietro della posizione
    iterator& operator-=(int offset) {
      ptr-=offset;
      return *this;
    }

    // Numero di elementi tra due iteratori
    difference_type operator-(const iterator &other) {
      return ptr-other.ptr;
    }
  
    // Uguaglianza
    bool operator==(const iterator &other) const {
      return ptr == other.ptr;
    }

    // Diversita'
    bool operator!=(const iterator &other) const {
      return ptr != other.ptr;
    }

    // Confronto
    bool operator>(const iterator &other) const {
      return ptr > other.ptr;
    }
    

    bool operator>=(const iterator &other) const {
      return ptr >= other.ptr;
    }

    // Confronto
    bool operator<(const iterator &other) const {
      return ptr < other.ptr;
    }
    
    
    // Confronto
    bool operator<=(const iterator &other) const {
      return ptr <= other.ptr;
    }
    
      
    friend class const_iterator;

    // Uguaglianza
    bool operator==(const const_iterator &other) const {
      return ptr == other.ptr;
    }

    // Diversita'
    bool operator!=(const const_iterator &other) const {
      return ptr != other.ptr;
    }

    // Confronto
    bool operator>(const const_iterator &other) const {
      return ptr > other.ptr;
    }
    

    bool operator>=(const const_iterator &other) const {
      return ptr >= other.ptr;
    }

    // Confronto
    bool operator<(const const_iterator &other) const {
      return ptr < other.ptr;
    }
    
    
    // Confronto
    bool operator<=(const const_iterator &other) const {
      return ptr <= other.ptr;
    }
 
  private:
    T * ptr;

    // La classe container deve essere messa friend dell'iteratore per poter
    // usare il costruttore di inizializzazione.
    friend class dbuffer; 

    // Costruttore privato di inizializzazione usato dalla classe container
    // tipicamente nei metodi begin e end
    iterator(T *p) { 
      ptr = p;
    }
    
    // !!! Eventuali altri metodi privati
    
  }; // classe iterator
  
  // Ritorna l'iteratore all'inizio della sequenza dati
  iterator begin() {
    return iterator(_buffer);
  }
  
  // Ritorna l'iteratore alla fine della sequenza dati
  iterator end() {
    return iterator(_buffer+_size);
  }
  
  class const_iterator {
    //  
  public:
    typedef std::random_access_iterator_tag iterator_category;
    typedef T                        value_type;
    typedef ptrdiff_t                difference_type;
    typedef const T*                 pointer;
    typedef const T&                 reference;

  
    const_iterator() {
      ptr = nullptr;
    }
    
    const_iterator(const const_iterator &other) {
      ptr = other.ptr;
    }

    const_iterator& operator=(const const_iterator &other) {
      ptr = other.ptr;
      return *this;
    }

    ~const_iterator() { }

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const {
      return *ptr;
    }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const {
      return ptr;
    }

    // Operatore di accesso random
    reference operator[](int index) {
      return ptr[index];
    }
    
    // Operatore di iterazione post-incremento
    const_iterator operator++(int) {
      const_iterator old(*this);
      ++ptr;
      return old;
    }

    // Operatore di iterazione pre-incremento
    const_iterator &operator++() {
      ++ptr;
      return *this;
    }

    // Operatore di iterazione post-decremento
    const_iterator operator--(int) {
      const_iterator old(*this);
      --ptr;
      return old;
    }

    // Operatore di iterazione pre-decremento
    const_iterator &operator--() {
      --ptr;
      return *this;
    }

    // Spostamentio in avanti della posizione
    const_iterator operator+(int offset) {
      return const_iterator(ptr+offset);
    }

    // Spostamentio all'indietro della posizione
    const_iterator operator-(int offset) {
      return const_iterator(ptr-offset);
    }
    
    // Spostamentio in avanti della posizione
    const_iterator& operator+=(int offset) {
      ptr+=offset;
      return *this;
    }

    // Spostamentio all'indietro della posizione
    const_iterator& operator-=(int offset) {
      ptr-=offset;
      return *this;
    }

    // Numero di elementi tra due iteratori
    difference_type operator-(const const_iterator &other) {
      return ptr - other.ptr;
    }
  
    // Uguaglianza
    bool operator==(const const_iterator &other) const {
      return ptr == other.ptr;
    }

    // Diversita'
    bool operator!=(const const_iterator &other) const {
      return ptr != other.ptr;
    }

    // Confronto
    bool operator>(const const_iterator &other) const {
      return ptr > other.ptr;
    }
    

    bool operator>=(const const_iterator &other) const {
      return ptr >= other.ptr;
    }

    // Confronto
    bool operator<(const const_iterator &other) const {
      return ptr < other.ptr;
    }
    
    
    // Confronto
    bool operator<=(const const_iterator &other) const {
      return ptr <= other.ptr;
    }
    
    
    // Solo se serve anche iterator aggiungere le seguenti definizioni
    
    friend class iterator;

    // Uguaglianza
    bool operator==(const iterator &other) const {
      return ptr == other.ptr;
    }

    // Diversita'
    bool operator!=(const iterator &other) const {
      return ptr != other.ptr;
    }

    // Confronto
    bool operator>(const iterator &other) const {
      return ptr > other.ptr;
    }
    

    bool operator>=(const iterator &other) const {
      return ptr >= other.ptr;
    }

    // Confronto
    bool operator<(const iterator &other) const {
      return ptr < other.ptr;
    }
    
    
    // Confronto
    bool operator<=(const iterator &other) const {
      return ptr <= other.ptr;
    }

    // Costruttore di conversione iterator -> const_iterator
    const_iterator(const iterator &other) {
      ptr = other.ptr;
    }

    // Assegnamento di un iterator ad un const_iterator
    const_iterator &operator=(const iterator &other) {
      ptr = other.ptr;
      return *this;
    }

    // Solo se serve anche iterator aggiungere le precedenti definizioni
  
  private:
    const T *ptr;

    // La classe container deve essere messa friend dell'iteratore per poter
    // usare il costruttore di inizializzazione.
    friend class dbuffer; 

    // Costruttore privato di inizializzazione usato dalla classe container
    // tipicamente nei metodi begin e end
    const_iterator(const T*p) { 
      ptr = p; 
    }
    
    // !!! Eventuali altri metodi privati
    
  }; // classe const_iterator
  
  // Ritorna l'iteratore all'inizio della sequenza dati
  const_iterator begin() const {
    return const_iterator(_buffer);
  }
  
  // Ritorna l'iteratore alla fine della sequenza dati
  const_iterator end() const {
    return const_iterator(_buffer + _size);
  }

/*
  
  // Implementazione di un forward iterator (a scopo didattico)
  // fatta a lezione

  class const_iterator; // forward declaration

  class iterator {
    //  
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T                         value_type;
    typedef ptrdiff_t                 difference_type;
    typedef T*                        pointer;
    typedef T&                        reference;

  
    iterator() {
      ptr = nullptr;
    }
    
    iterator(const iterator &other) {
      ptr = other.ptr;
    }

    iterator& operator=(const iterator &other) {
      ptr = other.ptr;
      return *this;
    }

    ~iterator() { }

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const {
      return *ptr;
    }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const {
      return ptr;
    }

    // Operatore di iterazione post-incremento
    iterator operator++(int) {
      iterator tmp(ptr);
      ++ptr;
      return tmp;
    }

    // Operatore di iterazione pre-incremento
    iterator& operator++() {
      ++ptr;
      return *this;
    }

    // Uguaglianza
    bool operator==(const iterator &other) const {
      return ptr == other.ptr;
    }

    // Diversita'
    bool operator!=(const iterator &other) const {
      return ptr != other.ptr;
    }
    
    // Solo se serve anche const_iterator aggiungere le seguenti definizioni
    friend class const_iterator;

    // Uguaglianza
    bool operator==(const const_iterator &other) const {
      return ptr == other.ptr;
    }

    // Diversita'
    bool operator!=(const const_iterator &other) const {
      return ptr != other.ptr;
    }

    // Solo se serve anche const_iterator aggiungere le precedenti definizioni

  private:
    value_type *ptr;

    // La classe container deve essere messa friend dell'iteratore per poter
    // usare il costruttore di inizializzazione.
    friend class dbuffer; // !!! Da cambiare il nome!

    // Costruttore privato di inizializzazione usato dalla classe container
    // tipicamente nei metodi begin e end
    explicit iterator(T* p) { 
       ptr = p;
    }
        
  }; // classe iterator
  
  // Ritorna l'iteratore all'inizio della sequenza dati
  iterator begin() {
    return iterator(_buffer);
  }
  
  // Ritorna l'iteratore alla fine della sequenza dati
  iterator end() {
    return iterator(_buffer + _size);
  }
  
  
  
  class const_iterator {
    //  
  public:
    typedef std::forward_iterator_tag iterator_category;
    typedef T                         value_type;
    typedef ptrdiff_t                 difference_type;
    typedef const T*                  pointer;
    typedef const T&                  reference;

  
    const_iterator() {
      ptr = nullptr;
    }
    
    const_iterator(const const_iterator &other) {
      ptr = other.ptr;
    }

    const_iterator& operator=(const const_iterator &other) {
      ptr = other.ptr;
    }

    ~const_iterator() {
      
    }

    // Ritorna il dato riferito dall'iteratore (dereferenziamento)
    reference operator*() const {
      return *ptr;
    }

    // Ritorna il puntatore al dato riferito dall'iteratore
    pointer operator->() const {
      return ptr;
    }
    
    // Operatore di iterazione post-incremento
    const_iterator operator++(int) {
      const_iterator tmp(ptr);
      ++ptr;
      return tmp;
    }

    // Operatore di iterazione pre-incremento
    const_iterator& operator++() {
      ++ptr;
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

    // Solo se serve anche iterator aggiungere le seguenti definizioni
    
    friend class iterator;

    // Uguaglianza
    bool operator==(const iterator &other) const {
      return ptr == other.ptr;
    }

    // Diversita'
    bool operator!=(const iterator &other) const {
      return ptr != other.ptr;
    }

    // Costruttore di conversione iterator -> const_iterator
    const_iterator(const iterator &other) {
      ptr = other.ptr;
    }

    // Assegnamento di un iterator ad un const_iterator
    const_iterator &operator=(const iterator &other) {
      ptr = other.ptr;
      return *this;
    }

    // Solo se serve anche iterator aggiungere le precedenti definizioni

  private:
    const T *ptr;

    // La classe container deve essere messa friend dell'iteratore per poter
    // usare il costruttore di inizializzazione.
    friend class dbuffer; // !!! Da cambiare il nome!

    // Costruttore privato di inizializzazione usato dalla classe container
    // tipicamente nei metodi begin e end
    const_iterator(const T *p) { 
      ptr = p; 
    }
    
    // !!! Eventuali altri metodi privati
    
  }; // classe const_iterator
  
  // Ritorna l'iteratore all'inizio della sequenza dati
  const_iterator begin() const {
    return const_iterator(_buffer);
  }
  
  // Ritorna l'iteratore alla fine della sequenza dati
  const_iterator end() const {
    return const_iterator(_buffer + _size);
  }
*/
  
private:

  value_type *_buffer; ///< Puntatore all'array di interi
  size_type _size; ///< Dimensione dell'array

};






#endif