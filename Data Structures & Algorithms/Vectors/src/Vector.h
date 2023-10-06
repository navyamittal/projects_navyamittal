#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::out_of_range
#include <type_traits> // std::is_same

template <class T>
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow() {
        //if (_size >= _capacity){
            T* newar;
            if (_capacity == 0){
                newar = new T[1];
                _capacity= 1;
            }
            else {
                newar = new T[2*_capacity];
                _capacity*=2;
            }
            for (size_t i = 0; i<_size; i++){
                newar[i] = std::move(array[i]);
            }

            //newar = std::move(array);
            delete array;
            
            array = newar;
       // }
    }

public:
    Vector() noexcept { 
        array = nullptr;
        _capacity = 0;
        _size = 0;
    }
    Vector(size_t count, const T& value) {
        //update size and capacity based on count. for loop to 
        //increase the pointer array space to (using count) and apply
        //value to each cell of the array
        array = new T[count];
        _size = count;
        _capacity = count;

        for (int i = 0; i<count; i++){
            array[i] = value;
        } 

    }
    explicit Vector(size_t count) { 
        array = new T[count]();
        _size = count;
        _capacity = count;
    }

    Vector(const Vector& other) {
        _size = other._size;
        _capacity = other._capacity;
        array = new T[other._capacity];
        for(size_t i = 0; i<other._size; i++){
            array[i] = other.array[i]; 
        }
    }
    //move contructor
    Vector(Vector&& other) noexcept {
        array = std::move(other.array);
        _size = other._size;
        _capacity = other._capacity;
        other._size = 0;
        other._capacity = 0;
        other.array = nullptr;
    }

    ~Vector() {
        delete[] array;
        array=nullptr;
        _capacity=0;
        clear();       
    }

    Vector& operator=(const Vector& other) {
        if (this!=&other){
            delete array;
            _size = other._size;
            _capacity = other._capacity;
            array = new int[other._capacity];
            for(size_t i = 0; i<other._size; i++){
                array[i] = other.array[i]; 
            }
        }
        return *this;
    }

    //move assignment operator
    Vector& operator=(Vector&& other) noexcept {
        if (this!=&other){
            delete array;
            array = std::move(other.array);
            _size = other._size;
            _capacity = other._capacity;
            other._size = 0;
            other._capacity = 0;
            other.array = nullptr;
        }
        return *this;
    }

    iterator begin() noexcept {
        iterator n(array);
        return n;
    }
    iterator end() noexcept {
        iterator n(array+_size);
        return n;
        //return iter;
    }

    [[nodiscard]] bool empty() const noexcept {
        if (_size ==0){
            return true;
        }
        return false;
    }

    size_t size() const noexcept {
        return _size;
    }
    size_t capacity() const noexcept {
        return _capacity;
    }

    T& at(size_t pos) {
        if (pos >= _size){
            throw std::out_of_range("");
        }
        else{
            return array[pos];
        }
    }
    const T& at(size_t pos) const {
        if (pos >= _size){
            throw std::out_of_range("");
        }
        else{
            return array[pos];
        }
    }
    T& operator[](size_t pos) {
        return array[pos];
    }
    const T& operator[](size_t pos) const {
        return array[pos];
    }
    T& front() {
        return array[0];
    }
    const T& front() const {
        return array[0];
    }
    T& back() {
        return array[_size-1];
    }
    const T& back() const {
        return array[_size-1];
    }

    void push_back(const T& value) {
        if (_size >= _capacity){
            grow();
        }
        array[_size] = value;
        _size+=1;
    }
    void push_back(T&& value) {
        if (_size >= _capacity){
            grow();
        }
        array[_size] = std::move(value);
        _size+=1;
    }
    void pop_back() {
        _size-=1;
    }

    iterator insert(iterator pos, const T& value) {
        size_t diff = pos - begin();
        if (_size>=_capacity){
            grow();
        }
        // if (pos == end()){
        //     push_back(value);
        //     return end();
        // }
        for (size_t i = _size; i > diff; i--){
            //*(i) = std::move(*(i-1));
            array[i] = std::move(array[i-1]);
        }
       array[diff] = value;
        _size+=1;
        return iterator(array+diff);
    }
    iterator insert(iterator pos, T&& value) {
        // if (pos == end()){
        //     push_back(std::move(value));
        //     return end();
        // }
        size_t diff = pos - begin();
        if (_size>=_capacity){
            grow();
        }

        for (size_t i = _size; i > diff; i--){
            //*(i) = std::move(*(i-1));
            array[i] = std::move(array[i-1]);
        }
        array[diff] = std::move(value);
        _size+=1;
        return iterator(array+diff);
    }
    iterator insert(iterator pos, size_t count, const T& value) {
        if (count==0){
            return pos;
        }
        ptrdiff_t diff = pos - begin();
        while( _size+count>_capacity){
            grow();
        }
        for (ptrdiff_t i = _size+count-1; i > diff+count-1; i--){
            //*(i) = std::move(*(i-1));
            array[i] = std::move(array[i-count]);
        }
        for (size_t i = diff; i<diff+count; i++){
            array[i] = value;
        }
        _size+=count;
        return begin() + diff;
    }
    iterator erase(iterator pos) {
        if (pos == end()-1){
            pop_back();
            return end();
        }
        for (iterator i = pos; i < end()-1; i++){
            *i = *(i+1);
        }
        _size-=1;
        return pos;
    }
    iterator erase(iterator first, iterator last) {
        ptrdiff_t shift = last - first;
        
        if (last == end()){
            _size -= shift;
            return end();
        }
        
        for (size_t i = first - begin(); i <_size - shift; i++){
            array[i] = array[shift+i];
        }
        
        // for (iterator i = begin(); i<(last-first); i++){
        //     *(i+shift) = *(i+last);
        // }

        _size -= shift;
        return first;
    }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*;
        using reference         = T&;
    private:
        // Add your own data members here
        T* point;
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
    public:
        iterator() {
            point = nullptr;
        }
        // Add any constructors that you may need
        iterator(T* p){
            point = p;
        }
        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept {
            return *point;
        }
        [[nodiscard]] pointer operator->() const noexcept { 
            return point;
        }

        // Prefix Increment: ++a
        iterator& operator++() noexcept {
            point++;
            return *(this);
        }
        // Postfix Increment: a++
        iterator operator++(int) noexcept {
            iterator tmp(*this);
            point++;
            return tmp;
        }
        // Prefix Decrement: --a
        iterator& operator--() noexcept {
            point--;
            return *(this);
        }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept {
            iterator tmp(*this);
            point--;
            return tmp;
        }

        iterator& operator+=(difference_type offset) noexcept {
            point += offset;
            return *this;
        }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept {
            iterator tmp(*this);
            tmp.point+=offset;
            return tmp;
        }
        
        iterator& operator-=(difference_type offset) noexcept {
            point -= offset;
            return *this;
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept {
            iterator tmp(*this);
            tmp.point -=offset;
            return tmp;
        }
        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept { 
            return point - rhs.point;
        }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept {
            return *(*this + offset);
        }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept {
            if (point==rhs.point){
                return true;
            }
            return false;
            //return point==rhs.point
        }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept {
            if (point!=rhs.point){
                return true;
            }
            return false;
        }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept {
            if (point<rhs.point){
                return true;
            }
            return false;
        }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept {
            if (point>rhs.point){
                return true;
            }
            return false;
        }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept {
            if (point<=rhs.point){
                return true;
            }
            return false;
        }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept {
            if (point>=rhs.point){
                return true;
            }
            return false;
        }
    };


    void clear() noexcept {
        // for (size_t i =0; i<_size; i++){
        //     delete ;
        // }
        
        _size = 0;

    }
};

// This ensures at compile time that the deduced argument _Iterator is a Vector<T>::iterator
// There is no way we know of to back-substitute template <typename T> for external functions
// because it leads to a non-deduced context
namespace {
    template <typename _Iterator>
    using is_vector_iterator = std::is_same<typename Vector<typename _Iterator::value_type>::iterator, _Iterator>;
}

template <typename _Iterator, bool _enable = is_vector_iterator<_Iterator>::value>
[[nodiscard]] _Iterator operator+(typename _Iterator::difference_type offset, _Iterator const& iterator) noexcept {
    return (iterator+offset);
}

#endif
