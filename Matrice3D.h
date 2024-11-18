/*********************************************************************
 * @file  Matrice3D.h
 * 
 * @brief Header of the class Matrice3D.
 *********************************************************************/

#ifndef MATRICE3D_H
#define MATRICE3D_H

#include <stdexcept>
#include <ostream>
#include <iterator>
#include <iostream>

template <typename T>
class Matrice3D {
private:
    unsigned int _cols; // number of elements on the x axis
    unsigned int _rows; // number of elements on the y axis
    unsigned int _depth; // number of elements on the z axis
    
    T* _data; // flattened 3d matrix of pointers, referencing the data

    /**
     @brief Check if matrix coordinates are within its bounds.
     
     @param x position on x axis
     @param y position on y axis
     @param z position on z axis
     
     @return true if it is within its bounds, false otherwise.

     @throw std::out_of_range Out of range
    */
    bool within_bounds(unsigned int x, unsigned int y, unsigned int z) const{
        return x < this->_cols
            && y < this->_rows
            && z < this->_depth;
    }
    
    unsigned int get_index(unsigned int x, unsigned int y, unsigned int z) const{
        return z*this->_cols*this->_rows+y*this->_rows+x;
    }

public:
    /**
     @brief Default constructor.

     @param cols number of elements on the x axis
     @param rows number of elements on the y axis
     @param depth number of elements on the z axis

     @throw std::invalid_argument One dimension is zero
    */
    Matrice3D(unsigned int cols, unsigned int rows, unsigned int depth):
        _cols(cols), _rows(rows), _depth(depth)
    {
        if (this->_cols > 0 && this->_rows > 0 && this->_depth > 0){
            unsigned int count = this->_cols * this->_rows * this->_depth;
            this->_data = new T[count];
            for(unsigned int i=0; i<count; i++){
                this->_data[i] = 0;
            }
        }else{
            throw std::invalid_argument("A dimension cannot be zero");
        }
    }

    /**
     @brief Destructor.
    */
    ~Matrice3D(){
        delete[] this->_data;
    }
    
    /**
     @brief Convert matrix elements from T type to U type.
    */
    template <typename U> operator U(){
        unsigned int count = this->_cols * this->_rows * this->_depth;
        for(unsigned int i=0; i<count; i++){
            this->_data[i] = 
                dynamic_cast<U>(this->data[i]);
        }
    }
    
    /**
     @brief Access a matrix value.
     
     @param x position on x axis
     @param y position on y axis
     @param z position on z axis
     
     @return the value at (x, y, z) position

     @throw std::out_of_range Out of range
    */
    T operator()(unsigned int x, unsigned int y, unsigned int z) const{
        if (!this->within_bounds(x, y, z))
            throw std::out_of_range("Out of range");

        return *this->_data[this->get_index(x,y,z)];
    }


    /**
     @brief Modify a matrix value.
     
     @param x position on x axis
     @param y position on y axis
     @param z position on z axis
     
     @return the value reference at (x, y, z) position

     @throw std::out_of_range Out of range
    */
    T& operator()(unsigned int x, unsigned int y, unsigned int z){
        if (!this->within_bounds(x, y, z))
            throw std::out_of_range("Out of range");

        return this->_data[this->get_index(x,y,z)];
    }

    /**
     @brief Check if two matrices are equals.
     
     @param other matrix on the right of the == operand

     @return a boolean indicating if the two matrices are equals.
    */
    bool operator==(Matrice3D<T> const& other) const{
        if (
            this->_cols != other.cols()
            || this->_rows != other.rows()
            || this->_depth != other.depth()
        ) return false;

        unsigned int count = this->_cols * this->_rows * this->_depth;
        for(unsigned int i=0; i<count; i++){
            if (this->_data[i] != other.data()[i])
                return false;
        }
        return true;
    }
    
    /**
     @brief Slice a matrix into a sub-matrix.
     
     @param x1 start of the sub-matrix on the x axis
     @param x2 end of the sub-matrix on the x axis
     @param y1 start of the sub-matrix on the y axis
     @param y2 end of the sub-matrix on the y axis
     @param z1 start of the sub-matrix on the z axis
     @param z2 end of the sub-matrix on the z axis
     
     @return A sliced sub-matrix.
    */
    Matrice3D<T> slice(
        unsigned int x1, unsigned int x2,
        unsigned int y1, unsigned int y2,
        unsigned int z1, unsigned int z2
    ) const{
        if (!this->within_bounds(x2, y2, z2))
            throw std::out_of_range("Out of range");
        
        if (
            x1 > x2
            || y1 > y2
            || z1 > z2
        ) throw std::invalid_argument("Invalid coordinates");

        Matrice3D<T> sub(x2-x1+1, y2-y1+1, z2-z1+1);
        
        unsigned int count = this->_cols * this->_rows * this->_depth;
        for(unsigned int i=0; i<count; i++){
            sub.data()[i] = T(this->_data[i]);
        }
        return sub;
    };

    /**
     @brief Get the matrix cols.
     
     @return The matrix cols.
    */
    unsigned int cols() const{
        return this->_cols;
    }

    /**
     @brief Get the matrix rows.
     
     @return The matrix rows.
    */
    unsigned int rows() const{
        return this->_rows;
    }

    /**
     @brief Get the matrix depth.
     
     @return The matrix depth.
    */
    unsigned int depth() const{
        return this->_depth;
    }

    /**
     @brief Get the matrix underlying data.
     
     @return The matrix depth.
    */
    T* data() const{
        return this->_data;
    }

    /**
     @brief Stream operator

     @param os output stream
     @param m matrix to send on the stream

     @return output stream
    */
    friend std::ostream&operator<<(std::ostream &os, 
        const Matrice3D &m) {
        os << "cols: " << m.cols() << std::endl;
        os << "rows: " << m.rows() << std::endl;
        os << "depth: " << m.depth() << std::endl;

        unsigned int count = m.cols() * m.rows() * m.depth();
        for(unsigned int i=0; i<count; i++){
            os << m.data()[i] << std::endl;
        }
        return os;
    }

    class iterator {
        public:
            typedef std::random_access_iterator_tag iterator_category;
            typedef T value_type;
            typedef ptrdiff_t difference_type;
            typedef T* pointer;
            typedef T& reference;
            
            iterator() : ptr(nullptr){}
            
            iterator(const iterator &other) : ptr(other.ptr) {}

            iterator& operator=(const iterator &other) {
                ptr = other.ptr;
                return *this;
            }

            ~iterator() {}

            // Ritorna il dato riferito dall'iteratore (dereferenziamento)
            reference operator*() const{
                return *ptr;
            }

            // Ritorna il puntatore al dato riferito dall'iteratore
            pointer operator->() const{
                return ptr;
            }

            // Ritorna il puntatore al dato riferito dall'indice
            pointer operator[](int i) const{
                return ptr + i;
            }

            // Operatore di iterazione post-decremento
            iterator operator--(int) {
                iterator tmp(ptr);
                ptr--;
                return tmp;
            }

            // Operatore di iterazione pre-decremento
            iterator& operator--() {
                --ptr;
                return *this;
            }

            // Operatore di iterazione post-incremento
            iterator operator++(int) {
                iterator tmp(ptr);
                ptr++;
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

        private:
            T* ptr;

            // La classe container deve essere messa friend dell'iteratore per poter
            // usare il costruttore di inizializzazione.
            friend class Matrice3D; 

            // Costruttore privato di inizializzazione usato dalla classe container
            // tipicamente nei metodi begin e end
            explicit iterator(T* p): ptr(p) {}
    };

    iterator begin() const {
        return iterator(this->_data);
    }
      
    // Ritorna l'iteratore alla fine della sequenza dati
    iterator end() const {
        return iterator(this->_data + (this->_cols * this->_rows * this->_depth));
    }

    /**
     @brief Fill the matrix with the given value.
    */
    template<class I>
    void fill(I first, I last, const T& value)
    {
        for (; first != last; ++first)
            *first = value;
    }
    
};

template<typename F, typename Q, typename T>
Matrice3D<Q> transform(Matrice3D<T> m){
    Matrice3D<Q> out(m.cols(), m.rows(), m.depth());
    auto m_iter = m.begin();
    auto out_iter = out.begin();
    F functor;

    while(m_iter != m.end()){
        *out_iter = functor(*m_iter);
        m_iter++;
        out_iter++;
    }

    return out;
}

#endif