#pragma once

#include <cstddef> // size_t
#include <iterator> // std::bidirectional_iterator_tag
#include <type_traits> // std::is_same, std::enable_if

template <class T>
class List {
    private:
    struct Node {
        Node *next, *prev;
        T data;
        explicit Node(Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev} {}
        explicit Node(const T& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{data} {}
        explicit Node(T&& data, Node* prev = nullptr, Node* next = nullptr)
        : next{next}, prev{prev}, data{std::move(data)} {}
    };

    template <typename pointer_type, typename reference_type>
    class basic_iterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = pointer_type;
        using reference         = reference_type;
    private:
        friend class List<value_type>;
        using Node = typename List<value_type>::Node;

        Node* node;

        explicit basic_iterator(Node* ptr) noexcept : node{ptr} {}
        explicit basic_iterator(const Node* ptr) noexcept : node{const_cast<Node*>(ptr)} {}

    public:
        basic_iterator() {
            node = nullptr;
        };
        basic_iterator(const basic_iterator&) = default;
        basic_iterator(basic_iterator&&) = default;
        ~basic_iterator() = default;
        basic_iterator& operator=(const basic_iterator&) = default;
        basic_iterator& operator=(basic_iterator&&) = default;

        reference operator*() const {
            return node->data;
        }
        pointer operator->() const {

            return &(node->data);
        }

        // Prefix Increment: ++a
        basic_iterator& operator++() {
            // TODO
            node = node->next;
            return *this;
        }
        // Postfix Increment: a++ 
        basic_iterator operator++(int) {
            basic_iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        // Prefix Decrement: --a
        basic_iterator& operator--() {
            
            node = node->prev;
            return *this;
        }
        // Postfix Decrement: a--
        basic_iterator operator--(int) {
            
            basic_iterator tmp(*this);
            --(*this);
            return tmp;
        }

        bool operator==(const basic_iterator& other) const noexcept {
            
            if (node==other.node){
                return true;
            }
            return false;
        }
        bool operator!=(const basic_iterator& other) const noexcept {
            if (node!=other.node){
                return true;
            }
            return false;
        }
    };

public:
    using value_type      = T;
    using size_type       = size_t;
    using difference_type = ptrdiff_t;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;
    using iterator        = basic_iterator<pointer, reference>;
    using const_iterator  = basic_iterator<const_pointer, const_reference>;

private:
    Node head, tail;
    size_type _size;

public:
    List() {
        head.next = &tail;
        tail.prev = &head;
        _size = 0;
    }
    List( size_type count, const T& value ) {
        // TODO - Don't forget the list initialier
        head.next = &tail;
        tail.prev = &head;
        for (size_t i =0; i< count; i++){
            Node* newn = new Node();
            newn->data = value;
            head.next->prev = newn;
            newn->next = head.next;
            head.next = newn;
            newn->prev = &head;
        }
        _size = count;

    }
    explicit List( size_type count ) {
        // TODO - Don't forget the list initialier
        head.next = &tail;
        tail.prev = &head;
        for (size_t i = 0; i < count; ++i) {
            Node* newn = new Node(T());
            head.next->prev = newn;
            newn->next = head.next;
            head.next = newn;
            newn->prev = &head;
        }
        
        _size = count;
    }
    
    List( const List& other ) {
        // Constructs a linked list with a copy of the contents of other.
        
        _size = 0;
        head.next = &tail;
        tail.prev = &head;
        Node* tmp = other.head.next;
        
        for (size_t i =0; i< other._size; i++){
            push_back(tmp->data);
            tmp = tmp->next;    
        }
    }
    //move constructor
    List( List&& other ) {

        _size = other._size;
        head.next = other.head.next;
        other.head.next->prev = &head;
        tail.prev = other.tail.prev;
        other.tail.prev->next = &tail;
        other.head.next = &other.tail;
        other.tail.prev = &other.head;
        other._size = 0;
    }
    ~List() {
        Node* newn = head.next;
        Node* prev = head.next;
        while (newn!=&tail){
            prev = newn;
            newn= newn->next;
            //prev = nullptr;
            delete prev;
        }
        head.next=&tail;
        tail.prev=&head;
    }
    List& operator=( const List& other ) {
        if (this!=&other){
            clear();
            Node* tmp = other.head.next;
        
            for (size_t i =0; i< other._size; i++){
                push_back(tmp->data);
                tmp = tmp->next;    
            }
        }
        return *this;
    }
    //move assignment op
    List& operator=(List&& other) noexcept {
        if (this!=&other){
            clear();
            if (other._size ==0){
                other.head.next = &other.tail;
                other.tail.prev = &other.head;
                return *this;
            }
            _size = other._size;
            head.next = other.head.next;
            other.head.next->prev = &head;
            tail.prev = other.tail.prev;
            other.tail.prev->next = &tail;
            other.head.next = &other.tail;
            other.tail.prev = &other.head;
            other._size = 0;
        }
        return *this;
    }

    reference front() {
        return (head.next->data);
    }
    const_reference front() const {
        return (head.next->data);
    }
	
    reference back() {
        return (tail.prev->data);
    }
    const_reference back() const {
        return (tail.prev->data);
    }
	
    iterator begin() noexcept {
        
        return iterator(head.next);
    }
    const_iterator begin() const noexcept {
        
        return const_iterator(head.next);
    }
    const_iterator cbegin() const noexcept {
        
        return const_iterator(head.next);
    }

    iterator end() noexcept {
        
        return iterator(&tail);
    }
    const_iterator end() const noexcept {
        
        return const_iterator(&tail);
    }
    const_iterator cend() const noexcept {
        
        return const_iterator(&tail);
    }

    bool empty() const noexcept {
        return head.next == &tail;
    }

    size_type size() const noexcept {
        return _size;
    }

    void clear() noexcept {
        
        Node* newn = head.next;
        Node* prev = head.next;
        while (newn!=&tail){
            prev = newn;
            newn= newn->next;
            //prev = nullptr;
            delete prev;
        }
        _size =0;
        head.next=&tail;
        tail.prev=&head;
        
            
    }
    

    iterator insert( const_iterator pos, const T& value ) {
        
        Node* newn = new Node(value, pos.node->prev, pos.node);
        pos.node->prev->next=newn;
        pos.node->prev= newn;
        _size++;
        //newn->next= pos.node;
        return iterator(newn);

    }
    iterator insert( const_iterator pos, T&& value ) {
        
        Node* newn = new Node(std::move(value), pos.node->prev, pos.node);
        pos.node->prev->next=newn;
        pos.node->prev= newn;
        _size++;
        return iterator(newn);
    }

    iterator erase( const_iterator pos ) {
        
        Node* tmp = pos.node->next;
        pos.node->next->prev = pos.node->prev;
        pos.node->prev->next = pos.node->next;
        
        delete pos.node;
        
        _size--;
        return iterator(tmp);
    }

    void push_back( const T& value ) {
        // TODO
        Node* newn = new Node(value,tail.prev,&tail);
        //newn->data = value;
        tail.prev-> next = newn;
        //newn->prev = tail.prev-> prev;
        //newn->prev = tail.prev;
        //newn->next = &tail;
        tail.prev = newn;
        //newn->next=&tail;
        _size++;
    }
    void push_back( T&& value ) {
        // TODO
        Node* newn = new Node(std::move(value),tail.prev,&tail);
        tail.prev-> next = newn;
        tail.prev = newn;
        _size++;
    }

    void pop_back() {
        // TODO
        Node* newn = tail.prev;
        newn->prev->next = &tail;
        tail.prev=newn->prev;
        delete newn;
        _size--;
    }
	
    void push_front( const T& value ) {
        Node* newn = new Node(value,&head, head.next);
        head.next-> prev = newn;
        head.next = newn;
        _size++;
    }
	void push_front( T&& value ) {
        Node* newn = new Node(std::move(value),&head, head.next);
        head.next-> prev = newn;
        head.next = newn;
        _size++;
    }

    void pop_front() {
        Node* newn = head.next;
        newn->next->prev = &head;
        head.next=newn->next;
        delete newn;
        _size--;
    }

    /*
      You do not need to modify these methods!
      
      These method provide the non-const complement 
      for the const_iterator methods provided above.
    */
    iterator insert( iterator pos, const T & value) { 
        return insert((const_iterator &) (pos), value);
    }

    iterator insert( iterator pos, T && value ) {
        return insert((const_iterator &) (pos), std::move(value));
    }

    iterator erase( iterator pos ) {
        return erase((const_iterator&)(pos));
    }
};


/*
    You do not need to modify these methods!

    These method provide a overload to compare const and 
    non-const iterators safely.
*/
 
namespace {
    template<typename Iter, typename ConstIter, typename T>
    using enable_for_list_iters = typename std::enable_if<
        std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::iterator, 
            Iter
        >{} && std::is_same<
            typename List<typename std::iterator_traits<Iter>::value_type>::const_iterator,
            ConstIter
        >{}, T>::type;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) == rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator==(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) == lhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const Iterator & lhs, const ConstIter & rhs) {
    return (const ConstIter &)(lhs) != rhs;
}

template<typename Iterator, typename ConstIter>
enable_for_list_iters<Iterator, ConstIter, bool> operator!=(const ConstIter & lhs, const Iterator & rhs) {
    return (const ConstIter &)(rhs) != lhs;
}