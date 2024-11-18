#ifndef ARRAY3D_H
#define ARRAY3D_H

#include <iostream> //std::cout
#include<ostream> //std::ostream
#include <cassert>
#include <algorithm>
#include <iterator>
#include <cstddef> 
#include <stdexcept>
/**
  @file array3d.h
  @brief dichiarazione della classe array3d
*/

/**
  @brief Classe array3d

  Classe che vuole rappresentare una Matrice 3d di oggetti di tipo T.
*/
template <typename T>
class array3d{
/*public:
	typedef unsigned int size_type;*/
public:

	typedef unsigned int size_type;

	/**
	 @brief Default constructor
	  rapresents a void 3d array
	 */
	array3d() :_DataPointer(nullptr), _rows(0), _col(0), _depth(0) {
		#ifndef NDEBUG
			std::cout << "array3d::array3d()" << std::endl;
		#endif
	}
	/**
	@brief secondary constructor

	Secondary constructor that creates a 3d array by the given dimension.
	Array's data is not initialized.

	@param r _rows
	@param c _columns
	@param d _depth

	@post _DataPointer != nullptr
	@post _rows = r
	@post _col = c
	@post _depth = d
  */
	explicit array3d(size_type r, size_type c, size_type d) : _DataPointer(nullptr), _rows(0), _col(0), _depth(0) {
		if (r >= 0 && c >= 0 && d >= 0) {
			_DataPointer = new T [r * c * d];
			_rows = r;
			_col = c;
			_depth = d;
			#ifndef NDEBUG
				std::cout << "array3d::array3d(size_type , size_type , size_type )" << std::endl;
			#endif
		}
		else throw std::invalid_argument("negative dimensions are not valid!");
	}
	/**
	@brief secondary constructor

	Secondary constructor that creates a 3d array by the given dimension.
	Array's data is initialized to the given value.

	@param r rows
	@param c columns
	@param d depth

	@post _DataPointer != nullptr
	@post rows = r
	@post col = c
	@post depth = d
	_DataPointer[i][j][k] = value
  */
	array3d(size_type r, size_type c, size_type d, T value) : _DataPointer(nullptr), _rows(0), _col(0), _depth(0) {
		if (r >= 0 && c >= 0 && d >= 0) {
			_DataPointer = new T [r * c * d];
					_rows = r;
					_col = c;
					_depth = d;
					try {
						for (size_type i = 0; i < r * c * d; i++)
							this->_DataPointer[i] = value;
					}
					catch (...) {
						delete[] _DataPointer;
						_DataPointer = nullptr;
						_rows = 0;
						_col = 0;
						_depth = 0;
						throw; // rilancio dell'eccezione !!
					}
					#ifndef NDEBUG
						std::cout << "array3d::array3d(size_type , size_type , size_type , T &)" << std::endl;
					#endif
		}
		else throw std::invalid_argument("negative dimensions are not valid!");
	}
	/**
	@brief Distructor

	the distructor deallocates the memory allocated on the head by the matrix.
  */
	~array3d() {
		delete[] _DataPointer;
		_DataPointer = nullptr;
		_rows = 0;
		_col = 0;
		_depth = 0;

		#ifndef NDEBUG
			std::cout << "array3d::~array3d()" << std::endl;
		#endif
	}
	/**
	@brief getters
	*/
	size_type getRows() const {
		return this->_rows;
	}

	size_type getCol() const {
		return this->_col;
	}

	size_type getDepth() const {
		return this->_depth;
	}

	T* getPointer() const {
		return this->_DataPointer;
	}

	size_type getSize() {
		return (this->_rows * this->_col * this->_depth);
	}

	/**
	@brief Copy Constructor

	creates an object as a copy of another object, the objects need to be indipendent each other.

	@param other matrix to copy

	@post _DataPointer != nullptr
	@post _rows = other._rows
	@post _col = other._col
	@post _depth = other._depth
  */
	array3d(const array3d & other) : _DataPointer(nullptr), _rows(0), _col(0), _depth(0) {
		_DataPointer = new T [other.getRows() * other.getCol() * other.getDepth()];
		_rows = other._rows;
		_col = other._col;
		_depth = other._depth;
		try {
			for (size_type i = 0; i < other.getRows() * other.getCol() * other.getDepth(); i++)
				this->_DataPointer[i] = other._DataPointer[i];
		}
		catch (...) {
			delete[] _DataPointer;
			_DataPointer = nullptr;
			_rows = 0;
			_col = 0;
			_depth = 0;
			throw; // rilancio dell'eccezione !!
		}
		#ifndef NDEBUG
			std::cout << "array3d::array3d(const array3d &)" << std::endl;
		#endif
	}
	void swap(array3d & other) { //swap matrix method 
		std::swap(this->_DataPointer, other._DataPointer);
		std::swap(this->_rows, other._rows);
		std::swap(this->_col, other._col);
		std::swap(this->_depth, other._depth);
	}
	/**
	@brief operator =

	The operator = copies the value of an object, to another object of the same
	type.

	@param other source matrix to copy

	@return reference to current object

	@post _DataPointer != nullptr
	@post _rows = other._rows
	@post _col = other._col
	@post _depth = other._depth
  */
	array3d & operator=(const array3d & other) {
		if (this != &other) {

			array3d tmp(other);

			this->swap(tmp);
		}

	#ifndef NDEBUG
		std::cout << "array3d::operator=(const array3d &)" << std::endl;
	#endif

		return *this;
	}
	/**
	@brief operator ()

	The operator () returns the value of the given parameter indexes.


	@return value of the index (r,c,d)
	*/

	T operator()(size_type r, size_type c, size_type d) const{
		assert(c < _rows);
		assert(r < _col);
		assert(d < _depth);
		#ifndef NDEBUG
			std::cout << "array3d::operator()(size_type, size_type, size_type)" << std::endl;
		#endif
		return *this->_DataPointer[getIndexByValues(r, c, d)];
	}


	T& operator()(size_type r, size_type c, size_type d) {
		assert(c < _rows);
		assert(r < _col);
		assert(d < _depth);
		#ifndef NDEBUG
			std::cout << "array3d::operator()(size_type, size_type, size_type)" << std::endl;
		#endif
		return this->_DataPointer[getIndexByValues(r,c,d)];
	}

	/**
	@brief operator ==

	The operator == returns true if the object are equal, so if they have the same content.

	@param other matrix to compare
	@return true if they are equal, false otherwise
	*/
	bool operator == (const array3d & other) {
		if (this->_rows != other.getRows() || this->_col != other.getCol() || this->_depth != other.getDepth())
			return false;
		for (size_type i = 0; i < getRows() * getCol() * getDepth(); i++)
			if (this->_DataPointer[i] != other.getPointer()[i])
				return false;
		#ifndef NDEBUG
			std::cout << "array3d::operator==(const array3d &)" << std::endl;
		#endif
		return true;
	}

	/**
	@brief Converts a matrix from type U to a type T.
   */
	template <typename U>
	operator U() {

		for (size_type i = 0; i < this->_cols * this->_rows * this->_depth; i++) {
			this->_DataPointer[i] = dynamic_cast<U>(this->_DataPointer[i]);
		}
	}

	/**
	@brief stream operator overload

	It helps to print a array3d object.

	@param os output stream  (left operator)
	@param m input array3d to print (right operator)

	@return reference to output stream
  */
	friend std::ostream& operator<<(std::ostream& os, const array3d<T>&m) {
		os << "rows: " << m.getRows() << std::endl;
		os << "columns: " << m.getCol() << std::endl;
		os << "depth: " << m.getDepth() << std::endl;
		for (typename array3d<T>::size_type i = 0; i < m.getDepth(); i++) {
			for (typename array3d<T>::size_type j = 0; j < m.getRows(); j++) {
				for (typename array3d<T>::size_type k = 0; k < m.getCol(); k++)
					os << m.getPointer()[m.getIndexByValues(k, j, i)] << ' ';
				os << std::endl;
			}
			os << std::endl << std::endl;
		}
		return os;
	}

	//class const_iterator; //forward declaration
	class iterator {
	public:
		typedef std::random_access_iterator_tag iterator_category;
		typedef T                        value_type;
		typedef ptrdiff_t                difference_type;
		typedef T* pointer;
		typedef T& reference;

		iterator() : ptr(nullptr) {}

		iterator(const iterator& other) : ptr(other.ptr) {}

		iterator& operator=(const iterator& other) {
			ptr = other.ptr;
			return *this;
		}

		~iterator() {}
		// Returns the pointed data (de-reference)
		reference operator*() const {
			return *ptr;
		}

		// Returns the pointer to the value
		pointer operator->() const {
			return ptr;
			}

		// Returns the pointer to the value by the index
		pointer operator[](int i) const {
			return ptr + i;
		}

		// post-decrease
		iterator operator--(int) {
			iterator tmp(ptr);
			ptr--;
			return tmp;
		}

		// pre-decrease
		iterator& operator--() {
			--ptr;
			return *this;
		}

		// post-increase
		iterator operator++(int) {
			iterator tmp(ptr);
			ptr++;
			return tmp;
		}

		// pre-increase
		iterator& operator++() {
			++ptr;
			return *this;
		}

		// equals
		bool operator==(const iterator& other) const {
			return ptr == other.ptr;
		}

		// not equal
		bool operator!=(const iterator& other) const {
			return ptr != other.ptr;
		}
		// Spostamentio in avanti della posizione
		iterator operator+(int offset) {
			return iterator(ptr + offset);
		}

		// Move back the index
		iterator operator-(int offset) {
			return iterator(ptr - offset);
		}

		// Move on the index
		iterator& operator+=(int offset) {
			ptr += offset;
			return *this;
		}

		// Move back the index
		iterator& operator-=(int offset) {
			ptr -= offset;
			return *this;
		}

		// Move on the index
		difference_type operator-(const iterator& other) {
			return ptr - other.ptr;
		}

		// greater than
		bool operator>(const iterator& other) const {
			return ptr > other.ptr;
		}

		//great or equal
		bool operator>=(const iterator& other) const {
			return ptr >= other.ptr;
		}

		// less than
		bool operator<(const iterator& other) const {
			return ptr < other.ptr;
		}


		// less or qual
		bool operator<=(const iterator& other) const {
			return ptr <= other.ptr;
		}

	private:
		T* ptr;

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class array3d;

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		explicit iterator(T* p) : ptr(p) {}
	}; //end class iterator

	iterator begin() const {
		return iterator(this->_DataPointer);
	}

	// Ritorna l'iteratore alla fine della sequenza dati
	iterator end() const {
		return iterator(this->_DataPointer + (this->_rows * this->_col * this->_depth));
	}

	/**
	@brief Method to fill the array3d using an iterator.
	@param I iterator 
	@param 
   */
	template<class I>
	void fill(I start, I end)
	{
		for ( int i=0; start!=end; start++,i++)
			this->_DataPointer[i] = *start;
	}
	/**
	@brief Method to slice a matrix, and return a sub matrix.
	@param x1 start of x size
	@param x2 end of x size
	@param y1 start of y size
	@param y2 end of y size
	@param z1 start of z size
	@param z2 end of z size
   */
	array3d<T> slice(size_type x1, size_type x2, size_type y1, size_type y2, size_type z1, size_type z2) const {
		assert(x2 < this->_rows); // <= not considered
		assert(y2 < this->_col);
		assert(z2 < this->_depth);
		assert(x1 < x2);
		assert(y1 < y2);
		assert(z1 < z2);
		array3d<T> result(x2 - x1 + 1, y2 - y1 + 1, z2 - z1 + 1);
		for(int depth = z1; depth<= z2; depth++){
			for (int rows = y1; rows <= y2; rows++){
				for (int col = x1; col <= x2; col++){
					result._DataPointer[result.getIndexByValues(col - x1, rows - y1, depth - z1)] = T(this->_DataPointer[this->getIndexByValues(col, rows, depth)]);
				}
			}
		}
		return result;
	}



private:
	T* _DataPointer; //points to the start of the matrix
	size_type _rows;
	size_type _col;
	size_type _depth;
	/**
	* @brief method that returns the index of the data, by rows, clomuns and depth, using pointers logic
	* @param r rows
	* @param c col
	* @param d depth
	*/
	size_type getIndexByValues(size_type c, size_type r, size_type d) const{
		return d * this->_col * this->_rows + c * this->_rows + r;
	}

	
}; //END CLASS array3d

template< typename F,typename Q, typename T >
array3d<Q> transform (array3d<T> &m) {
	array3d<Q> result(m.getCol(), m.getRows(), m.getDepth());
	auto m_iter = m.begin();
	auto result_iter = result.begin();
	F functor;

	while (m_iter != m.end()) {
		*result_iter = functor(*m_iter);
		++m_iter;
		++result_iter;
	}

	return result;
}

#endif // !ARRAY3D_H
