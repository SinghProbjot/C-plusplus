#ifndef GRAPH_H
#define GRAPH_H

#include <iostream> //std::cout
#include<ostream> //std::ostream
#include <cassert> //assert
#include <algorithm> //swap
#include <iterator> // std::forward_iterator_tag
#include <cstddef> // std::ptrdiff_t
#include <stdexcept>
/**
  @file graph.h
  @brief declaration of the graph class
*/

/**
  @brief Class graph

  Class to implement a directed graph using adjency matrix.
*/
template <typename T>
class graph {
public:
	typedef unsigned int size_type;
	/**
	 @brief Default constructor
	  rapresents a void graph
	 */
	graph() : _idArray(nullptr), _adjMatrix(nullptr), _aSize(0) {
		#ifndef NDEBUG
			std::cout << "graph::graph()" << std::endl;
		#endif
	}

	/**
	@brief Copy Constructor

	creates an object as a copy of another object, the objects need to be indipendent each other.

	@param other graph to copy

	@post _idArray != nullptr
	@post _adjMatrix != nullptr
	@post _aSize = other._aSize
  */
	graph(const graph& other) : _idArray(nullptr), _adjMatrix(nullptr), _aSize(0) {
		_idArray = new T[other._aSize];
		_aSize = other._aSize;
		_adjMatrix = new bool*[other._aSize];
		for (int i = 0; i < other._aSize; i++)
			_adjMatrix[i] = new bool[other._aSize];
		try {
			//fill the parallel array
			for (size_type i = 0; i < other._aSize; i++) 
				this->_idArray[i] = other._idArray[i];
			//fill the matrix
			for (size_type i = 0; i < other._aSize; i++)
				for (size_type j = 0; j < other._aSize; j++)
				this->_adjMatrix[i][j] = other._adjMatrix[i][j];
		}
		catch (...) {
			delete[] _idArray;
			delete[] _adjMatrix;
			_idArray = nullptr;
			_adjMatrix = nullptr;
			_aSize = 0;
			throw; // rilancio dell'eccezione !!
		}
		#ifndef NDEBUG
			std::cout << "graph::graph(const graph &)" << std::endl;
		#endif
	}

	/**
	@brief Distructor

	the distructor deallocates the memory allocated on the heap by the graph.
  */
	~graph() {
		delete[] _idArray;
		delete[] _adjMatrix;
		_idArray = nullptr;
		_adjMatrix = nullptr;
		_aSize = 0;

		#ifndef NDEBUG
			std::cout << "graph::~graph()" << std::endl;
		#endif
	}

	/*
	@brief swap stack method
	@param other stack to swap
	*/
	void swap(graph& other) {
		std::swap(this->_idArray, other._idArray);
		std::swap(this->_adjMatrix, other._adjMatrix);
		std::swap(this->_aSize, other._aSize);
	}

	/**
	@brief operator =

	The operator = copies the value of an object, to another object of the same
	type.

	@param other source graph to copy

	@return reference to current object

	@post _idArray != nullptr
	@post _adjMatrix != nullptr
	@post _aSize = other._aSize
  */
	graph& operator=(const graph& other) {
		if (this != &other) {

			graph tmp(other);

			this->swap(tmp);
		}

		#ifndef NDEBUG
			std::cout << "graph::operator=(const graph &)" << std::endl;
		#endif

		return *this;
	}

	/**
	@brief operator ==

	The operator == returns true if the object are equal, so if they have the same content.

	@param other graph to compare
	@return true if they are equal, false otherwise
	*/
	bool operator == (const graph& other) {
		if (this->_aSize != other._aSize)
			return false;
		for (size_type i = 0; i < _aSize; i++)
			if (this->_idArray[i] != other._idArray[i])
				return false;
		for (size_type i = 0; i < other._aSize; i++)
			for (size_type j = 0; j < other._aSize; j++)
				if (this->_adjMatrix[i][j] != other._adjMatrix[i][j])
					return false;
		#ifndef NDEBUG
			std::cout << "graph::operator==(const graph &)" << std::endl;
		#endif
		return true;
	}


	/**
	@brief method that returns the number of nodes.
	*/
	size_type getNodes() const{
		return _aSize;
	}

	/*@brief stream operator overload

		It helps to print a graph object.

		@param os output stream(left operator)
		@param m input graph to print(right operator)

		@return reference to output stream
		*/
	friend std::ostream& operator<<(std::ostream& os, const graph<T>& m) {
		for (typename graph<T>::size_type i = 0; i < m.getNodes(); i++) {
			os << m._idArray[i] << ": ";
			for (typename graph<T>::size_type j = 0; j < m.getNodes(); j++) {
				if (m._adjMatrix[i][j] == true)
					os << m._idArray[j] << ", ";
				if (j == m.getNodes() - 1 && i != m.getNodes() - 1)
					os << std::endl;
			}
		}
		return os;
	}


	/**
	@brief method that returns the number of arcs.
	*/

	size_type getArcs() const{
		size_type cont = 0;
		for (size_type i = 0; i < _aSize; i++)
			for (size_type j = 0; j < _aSize; j++)
				if (_adjMatrix[i][j] == true)
					cont++;
		return cont;
	}
   
	/**
	@brief method that returns true if the passed node exists in the graph.
	*/

	bool exists(const T &node) const{
		bool ok = false;
		for (size_type i = 0; i < _aSize; i++)
			if (_idArray[i] == node)
				ok = true;
		return ok;
	}

	/**
	@brief method that returns true if the passed nodes are connected by an arc.
	*/

	bool connected(const T &node1,const T &node2) const{
		if(exists(node1) && exists(node2))
			return _adjMatrix[indexOf(node1)][indexOf(node2)];
		else throw std::invalid_argument("an input node does not exist!");
	}

	/**
	@brief method that adds a node to the graph, if the node does not exist yet.
	*/

	void add_node(const T &node) {
		if(exists(node))
			throw std::invalid_argument("node already exists!");
		else {
			T* tmpArray = new T[_aSize + 1]; //allocate new array
			bool** tmpMatrix = new bool*[_aSize + 1];//allocate new matrix
			for (int i = 0; i < _aSize + 1; i++) {

				// Declare a memory block
				// of size _aSize
				tmpMatrix[i] = new bool[_aSize + 1];
			}

			//filling the new idArray
			for (size_type i = 0; i < _aSize; i++)
				tmpArray[i] = _idArray[i];

			tmpArray[_aSize] = node; //new node
			
			//filling the new adjency matrix
			for (size_type i = 0; i < _aSize; i++)
				for (size_type j = 0; j < _aSize; j++)
					tmpMatrix[i][j] = _adjMatrix[i][j];

			for (size_type i = 0; i < _aSize + 1; i++){
				tmpMatrix[_aSize][i] = false;  //new isolated node
				tmpMatrix[i][_aSize] = false;
			}

			//make the tmps the new objects
			std::swap(_idArray, tmpArray);
			std::swap(_adjMatrix, tmpMatrix);
			this->_aSize++;
			//remove from heap the old allocated memory
			delete[] tmpArray;
			delete[] tmpMatrix;
			
			#ifndef NDEBUG
				std::cout << "ADDING NEW NODE : " << node << std::endl;
			#endif	
		}
	}
	/**
	@brief method that adds an arc to the passed nodes, if the arc does not exist yet.
	*/
	void add_arc(const T &node1,const T &node2) {
		if (connected(node1, node2)) //connected also control if they exist
			throw std::invalid_argument("nodes already connected!");
		else
			_adjMatrix[indexOf(node1)][indexOf(node2)] = true;
	}

	/**
	@brief method that removes a node from the graph, if the node exists.
	*/
	void remove_node(const T &node) {
		if(!exists(node))
			throw std::invalid_argument("node does not exist!");
		else {
			T* tmpArray = new T[_aSize - 1];
			bool** tmpMatrix = new bool* [_aSize - 1];
			for (int i = 0; i < _aSize - 1; i++) {

				// Declare a memory block
				// of size _aSize
				tmpMatrix[i] = new bool[_aSize - 1];
			}

			//filling the new idArray
			for (size_type i = 0, j = 0; i < _aSize; i++)
				if (i != indexOf(node)) {
					tmpArray[j] = _idArray[i];
					j++;
				}
			
			//filling the new adjency matrix
			for (size_type i = 0, k = 0; i < _aSize; i++){
				if (i != indexOf(node)) {
					for (size_type j = 0, l = 0; j < _aSize; j++)
										if (j != indexOf(node)) {
											tmpMatrix[k][l] = _adjMatrix[i][j];
											l++;
										}
					k++;
				}
			}
			//make the tmps the new objects
			std::swap(tmpArray, _idArray);
			std::swap(tmpMatrix, _adjMatrix);
			this->_aSize--;
			//remove from heap the old allocated memory
			delete[] tmpArray;
			delete[] tmpMatrix;

			#ifndef NDEBUG
				std::cout << "REMOVING THE NODE : " << node << std::endl;
			#endif		
					
		}
	}

	/**
	@brief method that removes an arc by the passed nodes, if the arc exists.
	*/
	void remove_arc(const T &node1,const T &node2) {
		if (!connected(node1, node2)) //connected also control if they exist
			throw std::invalid_argument("arc does not exist!");
		else
			_adjMatrix[indexOf(node1)][indexOf(node2)] = false;
	}

	/**
	@brief A const-iterator that iterates on the nodes' IDs.
	*/
	class const_iterator { 
	public:
		typedef std::forward_iterator_tag iterator_category;
		typedef T                         value_type;
		typedef ptrdiff_t                 difference_type;
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
		bool operator==(const const_iterator& other) const {
			return ptr == other.ptr;
		}

		// Diversita'
		bool operator!=(const const_iterator& other) const {
			return ptr != other.ptr;
		}

	private:
		const T* ptr;

		// La classe container deve essere messa friend dell'iteratore per poter
		// usare il costruttore di inizializzazione.
		friend class graph;

		// Costruttore privato di inizializzazione usato dalla classe container
		// tipicamente nei metodi begin e end
		const_iterator(const T* p) {
			ptr = p;
		}

	}; // classe const_iterator

	// Ritorna l'iteratore all'inizio della sequenza dati
	const_iterator begin() const {
		return const_iterator(_idArray);
	}

	// Ritorna l'iteratore alla fine della sequenza dati
	const_iterator end() const {
		return const_iterator(_idArray + _aSize);
	}
	template<class I>
	void add_nodes(I start, I end) {
		for (; start != end; start++) {
			try
			{
			 add_node(*start);
			}
			catch (const std::exception&){
				std::cout << "FAILED!" << std::endl;
			}
			
		}
		#ifndef NDEBUG
			std::cout << "graph::add_nodes(const_interator, const_interator)" << std::endl;
		#endif		
	}

private:
	T* _idArray; //pointer to the support array for the identificators
	bool** _adjMatrix; //pointer to the adjency matrix
	size_type _aSize; //size of the array
	int indexOf(const T &value)const{ //returns the index of the parameter ID in the array
		int index = -1;
		for (size_type i = 0; i < _aSize; i++)
			if (_idArray[i] == value)
				index = i;
		return index;
	}
}; //END CLASS GRAPH

#endif