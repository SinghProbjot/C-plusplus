#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <ostream> // std::ostream
#include <cassert> 
#include <iterator> // std::forward_iterator_tag
#include <cstddef>  // std::ptrdiff_t
#include <algorithm> //swap
#include <stdexcept>

/**
  @file stack.h
  @brief dichiarazione della classe stack
*/

/**
  @brief Classe stack

  Classe che vuole rappresentare uno stack di oggetti di tipo T.
*/

template <typename T>
class stack { 
public:

	typedef unsigned int size_type;

	/**
	 @brief Default constructor
	  rapresents a void stack
	 */
	stack() :_DataPointer(nullptr), _size(0), _top(-1) {
	#ifndef NDEBUG
		std::cout << "stack::stack()" << std::endl;
	#endif
	}

	/**
	@brief secondary constructor

	Secondary constructor that creates a stack by the given dimension.
	Stack's data is not initialized.

	@param size _size of the stack

	@post _DataPointer != nullptr
	@post _size = size
	@post _top = -1
  */
	explicit stack(size_type size) : _DataPointer(nullptr), _size(0), _top(-1) {
		assert(size >= 0);
		if (size >= 0) {
			try {
				_DataPointer = new T[size];
				_size = size;
				_top = -1;
			}
			catch (...) {
				_DataPointer = nullptr;
				_size = 0;
				_top = -1;
				throw;
			}
		#ifndef NDEBUG
			std::cout << "stack::stack(size_type)" << std::endl;
		#endif
		}
		else throw std::invalid_argument("negative dimensions are not valid!");
	}
	template<class I>
	stack(I start, I end) : _DataPointer(nullptr), _size(0), _top(-1) {
		_DataPointer = new T[end - start]; //distanza tra i due iteratori, operatore difference_type degli iteratori
		_size = end - start;
		_top = -1;
		try {
			for (; start != end; start++)
						push(*start);
		}
		catch (...) {
			_DataPointer = nullptr;
			_size = 0;
			_top = -1;
			throw;
		}
		
	#ifndef NDEBUG
		std::cout << "stack::stack(I, I)" << std::endl;
	#endif
	}

	/**
	@brief Distructor

	the distructor deallocates the memory allocated on the heap by the stack.
  */
	~stack() {
		delete[] _DataPointer;
		_DataPointer = nullptr;
		_size = 0;
		_top = -1;

	#ifndef NDEBUG
		std::cout << "stack::~stack()" << std::endl;
	#endif
	}

	/*
	@brief Public method that has the same functionality of the secondary constructor,
	that fills the stack using an iterator.
	@param start starting iterator
	@param end ending iterator
	*/
	template<class I>
	void fillUsingIterator(I start, I end) {
		if (!isEmpty())  //se stack contiene già dati, questi vengono rimossi.
			empty();
		for (; start != end; start++)
			push(*start);
	}


	/**
	@brief getters and utility functions for the stack class
	*/

	size_type getSize() const { //max size defined
		return this->_size;
	}

	int getActualSize() const { //actual size
		return this->_top + 1;
	}

	T* getPointer() const { //pointer to the stack
		return this->_DataPointer;
	}

	T getValue(size_type index) const { //getter to a value by index
		assert(index < _size && index <=_top && index>=0);
		if(index <= _top && index < _size && index>=0) 
			return _DataPointer[index];
		else throw std::invalid_argument("index greater than actual stack's dimension!");
	}

	/*
	* @brief function to know if the stack is empty.
	* @return true if is empty, so if top==-1
	* @return false otherwise
	*/
	bool isEmpty() {
		return _top == -1;
	}
	
	bool isFull() {
		return _top == _size - 1;
	}
	/**
	@brief Stack push function

	Insert an element of type T to the stack, if it isn't full.
	Throws overflow error if stack is full.

	@param value the item to push

	@post _top = top + 1 */
	void push(T value) {
		if (isFull()) {
			throw std::overflow_error("stack is full!");
			exit(EXIT_FAILURE);
		}
		else{
			std::cout << "INSERTING TO STACK: " << value << std::endl;
			_DataPointer[++_top] = value;
		}
	}

	/**
	@brief Stack pop function

	Extracts the top element of type T from the stack, if it isn't empty.
	Throws underflow error if stack is empty.

	@post _top = top - 1 */
	T pop() {
		assert(!isEmpty());
		if (isEmpty()) {
			throw std::underflow_error("stack is empty!");
			exit(EXIT_FAILURE);
		}
		else {
			std::cout << "REMOVING  " << _DataPointer[_top] << " FROM STACK" << std::endl;
		}

		return _DataPointer[_top--];
	}

	/**
	@brief Stack peek function

	Returns the top element of type T from the stack,without removing it, if it isn't empty.
	 */
	T peek()
	{
		assert(!isEmpty());
		if (!isEmpty()) {
			return _DataPointer[_top];
		}
		else {
			exit(EXIT_FAILURE);
		}
	}
	/**
	@brief Stack empty function

	Function to empy the stack.

	@post _top = -1
	 */

	void empty() {
		if (isEmpty())
			std::cout << "stack is already empty!" << std::endl;
		else
			_top = -1;
	}

	/**
  Funzione che ritorna true se lo stack soddisfa
  un predicato generico di tipo P, su un elemento di tipo T.

  @param pred predicato
  @param value value to test

*/
	template<typename P>
	bool checkif(T value, P pred) {
		if (pred(value) == true)
			return true;
		else return false;
	}

	/**
	@brief Copy Constructor

	creates an object as a copy of another object, the objects need to be indipendent each other.

	@param other stack to copy

	@post _DataPointer != nullptr
	@post _size = other._size
	@post _top = other._top
  */
	stack(const stack& other) : _DataPointer(nullptr), _size(0), _top(-1) {
		_DataPointer = new T[other._size];
		_size = other._size;
		_top = other._top;
		try {
			for (size_type i = 0; i < other.getActualSize(); i++) //more computational
				this->_DataPointer[i] = other._DataPointer[i];
		}
		catch (...) {
			delete[] _DataPointer;
			_DataPointer = nullptr;
			_size = 0;
			_top = -1;
			throw; // rilancio dell'eccezione !!
		}
	#ifndef NDEBUG
		std::cout << "stack::stack(const stack &)" << std::endl;
	#endif
	}
/*
	@brief swap stack method
	@param other stack to swap
	*/
	void swap(stack& other) { 
		std::swap(this->_DataPointer, other._DataPointer);
		std::swap(this->_size, other._size);
		std::swap(this->_top, other._top);
	}

	/**
	@brief operator =

	The operator = copies the value of an object, to another object of the same
	type.

	@param other source stack to copy

	@return reference to current object

	@post _DataPointer != nullptr
	@post _size = other._size
	@post _top = other._top
  */
	stack& operator=(const stack& other) {
		if (this != &other) {

			stack tmp(other);

			this->swap(tmp);
		}

	#ifndef NDEBUG
		std::cout << "stack::operator=(const stack &)" << std::endl;
	#endif

		return *this;
	}

	/**
	@brief operator ==

	The operator == returns true if the object are equal, so if they have the same content.

	@param other stack to compare
	@return true if they are equal, false otherwise
	*/
	bool operator == (const stack& other) {
		if (this->_size != other.getSize() || this->_top != other._top)
			return false;
		for (size_type i = 0; i < getActualSize(); i++)
			if (this->_DataPointer[i] != other.getPointer()[i])
				return false;
	#ifndef NDEBUG
		std::cout << "stack::operator==(const stack &)" << std::endl;
	#endif
		return true;
	}

	/*@brief stream operator overload

		It helps to print a stack object.

		@param os output stream(left operator)
		@param m input stack to print(right operator)

		@return reference to output stream
		*/
		friend std::ostream & operator<<(std::ostream & os, const stack<T>&m) {
		os << "size: " << m.getSize() << std::endl;
		for (typename stack<T>::size_type i = 0; i < m.getActualSize(); i++) {
			os << m.getValue(i) << " ";
			}
		os << std::endl;
		return os;
	}

	


	class const_iterator {
		//  
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef T                        value_type;
		typedef ptrdiff_t                difference_type;
		typedef const T* pointer;
		typedef const T& reference;


		const_iterator() {
			ptr = nullptr;
		}

		const_iterator(const const_iterator& other) {
			ptr = other.ptr;
		}

		const_iterator& operator=(const const_iterator& other) {
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


		// Operatore di iterazione post-incremento
		const_iterator operator++(int) {
			const_iterator old(*this);
			--ptr;
			return old;
		}

		// Operatore di iterazione pre-incremento
		const_iterator& operator++() {
			--ptr;
			return *this;
		}

		// Operatore di iterazione post-decremento
		const_iterator operator--(int) {
			const_iterator old(*this);
			++ptr;
			return old;
		}

		// Operatore di iterazione pre-decremento
		const_iterator& operator--() {
			++ptr;
			return *this;
		}

		// Spostamentio in avanti della posizione
		const_iterator operator+(int offset) {
			return const_iterator(ptr + offset);
		}

		// Spostamentio all'indietro della posizione
		const_iterator operator-(int offset) {
			return const_iterator(ptr - offset);
		}

		// Spostamentio in avanti della posizione
		const_iterator& operator+=(int offset) {
			ptr -= offset;
			return *this;
		}

		// Spostamentio all'indietro della posizione
		const_iterator& operator-=(int offset) {
			ptr += offset;
			return *this;
		}

		// Numero di elementi tra due iteratori
		difference_type operator-(const const_iterator& other) {
			return ptr - other.ptr;
		}

		// Uguaglianza
		bool operator==(const const_iterator& other) const {
			return ptr == other.ptr;
		}

		// Diversita'
		bool operator!=(const const_iterator& other) const {
			return ptr >= other.ptr;
		}

		// Confronto
		bool operator>(const const_iterator& other) const {
			return ptr > other.ptr;
		}


		bool operator>=(const const_iterator& other) const {
			return ptr >= other.ptr;
		}

		// Confronto
		bool operator<(const const_iterator& other) const {
			return ptr < other.ptr;
		}


		// Confronto
		bool operator<=(const const_iterator& other) const {
			return ptr >= other.ptr;
		}
	private:
		const T* ptr;

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class stack;

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		const_iterator(const T* p) {
			ptr = p;
		}

		// !!! Eventuali altri metodi privati

  }; // classe const_iterator

  
  
  // Ritorna l'iteratore alla fine della sequenza dati
  const_iterator begin() const {
	  return const_iterator(this->_DataPointer + this->getActualSize() -1); // actual size
  }
  // Ritorna l'iteratore all'inizio della sequenza dati
  const_iterator end() const {
	  return const_iterator(this->_DataPointer);
  }

  



private:

	T* _DataPointer; //points to the head of the stack.
	size_type _size;  //dimesion of the stack.
	int _top;  // targets the top element, the one to push or pop.
};

#endif