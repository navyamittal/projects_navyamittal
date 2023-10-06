#include <cstddef>    // size_t
#include <functional> // std::hash
#include <ios>
#include <utility>    // std::pair
#include <iostream>

#include "primes.h"



template <typename Key, typename T, typename Hash = std::hash<Key>, typename Pred = std::equal_to<Key>>
class UnorderedMap {
    public:

    using key_type = Key;
    using mapped_type = T;
    using const_mapped_type = const T;
    using hasher = Hash;
    using key_equal = Pred;
    using value_type = std::pair<const key_type, mapped_type>;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    private:

    struct HashNode {
        HashNode *next;
        value_type val;

        HashNode(HashNode *next = nullptr) : next{next} {}
        HashNode(const value_type & val, HashNode * next = nullptr) : next { next }, val { val } { }
        HashNode(value_type && val, HashNode * next = nullptr) : next { next }, val { std::move(val) } { }
    };

    size_type _bucket_count;
    HashNode **_buckets;

    HashNode * _head;
    size_type _size;

    Hash _hash;
    key_equal _equal;

    static size_type _range_hash(size_type hash_code, size_type bucket_count) {
        return hash_code % bucket_count;
    }

    public:

    template <typename pointer_type, typename reference_type, typename _value_type>
    class basic_iterator {
    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = _value_type;
        using difference_type = ptrdiff_t;
        using pointer = value_type *;
        using reference = value_type &;

    private:
        friend class UnorderedMap<Key, T, Hash, key_equal>;
        using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

        const UnorderedMap * _map;
        HashNode * _ptr;

        explicit basic_iterator(UnorderedMap const * map, HashNode *ptr) noexcept : _map(map), _ptr(ptr){}

    public:
        basic_iterator()  : _map(nullptr), _ptr(nullptr) {
        };

        basic_iterator(const basic_iterator &) = default;
        basic_iterator(basic_iterator &&) = default;
        ~basic_iterator() = default;
        basic_iterator &operator=(const basic_iterator &) = default;
        basic_iterator &operator=(basic_iterator &&) = default;
        reference operator*() const {
            return (_ptr->val);
        }
        pointer operator->() const {
            return &(_ptr->val);
        }
        basic_iterator &operator++() {
            if (_ptr->next!=nullptr){
                _ptr = _ptr->next;
            }
            else {
                size_t bucket = _map->_bucket(_ptr->val.first);
                for (bucket++; bucket < _map->_bucket_count; bucket++) {
                    if (_map->_buckets[bucket]) {
                        _ptr = _map->_buckets[bucket];
                        break;
                    }
                }
                // If no non-empty bucket was found, set _node to end node (nullptr)
                if (bucket == _map->_bucket_count) {
                    _ptr = nullptr;
                }
            }
            return *this;
        }

        basic_iterator operator++(int) {
            basic_iterator tmp(*this);
            ++(*this);
            return tmp;
        }
        bool operator==(const basic_iterator &other) const noexcept {
            return (_ptr==other._ptr);
        }
        bool operator!=(const basic_iterator &other) const noexcept {
            return (_ptr!=other._ptr);
        }
    };

    using iterator = basic_iterator<pointer, reference, value_type>;
    using const_iterator = basic_iterator<const_pointer, const_reference, const value_type>;

    class local_iterator {
        public:
            using iterator_category = std::forward_iterator_tag;
            using value_type = std::pair<const key_type, mapped_type>;
            using difference_type = ptrdiff_t;
            using pointer = value_type *;
            using reference = value_type &;

        private:
            friend class UnorderedMap<Key, T, Hash, key_equal>;
            using HashNode = typename UnorderedMap<Key, T, Hash, key_equal>::HashNode;

            HashNode * _node;

            explicit local_iterator( HashNode * node ) noexcept: _node{node}{}

        public:
            local_iterator() {
                _node = nullptr;
            }

            local_iterator(const local_iterator &) = default;
            local_iterator(local_iterator &&) = default;
            ~local_iterator() = default;
            local_iterator &operator=(const local_iterator &) = default;
            local_iterator &operator=(local_iterator &&) = default;
            reference operator*() const {
                return (_node->val);
            }
            pointer operator->() const {
                return &(_node->val);
            }
            local_iterator & operator++() {
                if(_node!=nullptr){
                    _node = _node->next;
                }
                
                return *this;
            }
            local_iterator operator++(int) {
                local_iterator tmp(*this);
                ++(*this);
                return tmp;
            }

            bool operator==(const local_iterator &other) const noexcept {
                if (_node==other._node){
                    return true;
                }
                return false;
            }
            bool operator!=(const local_iterator &other) const noexcept {
                if (_node== other._node){
                    return false;
                }
                return true;
            }
    };

private:

    size_type _bucket(size_t code) const {
        return _range_hash(code, bucket_count());
    }
    size_type _bucket(const Key & key) const {
        size_type hash_code = _hash(key);
        return _bucket(hash_code);
    }
    size_type _bucket(const value_type & val) const {
        const Key& key = val.first;
        return _bucket(key);
    }

    HashNode*& _find(size_type code, size_type bucket, const Key & key) {

        HashNode ** current = &(_buckets[bucket]);
        while (*current) {
            if (_equal((*current)->val.first, key)) {
                return *current;
            }
            current = &((*current)->next);
        }
        return *current;
    }

    HashNode*& _find(const Key & key) {
        size_type hash_code = _hash(key);
        return _find(hash_code, _bucket(key), key);
    }

    HashNode * _insert_into_bucket(size_type bucket, value_type && value) {
        HashNode* new_node = new HashNode[value];
        HashNode* curr = _buckets[bucket];
        _head = new_node;
        _head->next = curr;
    }

    void _move_content(UnorderedMap & src, UnorderedMap & dst) {
        dst._size = src._size;
        dst._bucket_count = src._bucket_count;
        dst._hash = std::move(src._hash);
        dst._equal = std::move(src._equal);
        dst._head = src._head;
        //dst.begin() = src.begin();
        dst._buckets = src._buckets;

        src._buckets = new HashNode*[src._bucket_count]();
        src._size = 0;
        //src._bucket_count = 0;
        //src._hash = nullptr;
        //src._equal = nullptr;
        src._head = nullptr;
    }

public:
    explicit UnorderedMap(size_type bucket_count, const Hash & hash = Hash { },
                const key_equal & equal = key_equal { }) : _bucket_count(next_greater_prime(bucket_count)), _hash(hash), _equal(equal), _head(nullptr), _size(0) {
                
        _buckets = new HashNode*[_bucket_count]();
        }

    ~UnorderedMap() {
        clear();
        delete _buckets;
    }

    UnorderedMap(const UnorderedMap & other) {
        _size = 0;
        _bucket_count = other._bucket_count;
        _hash = other._hash;
        _equal = other._equal;
        _head = nullptr;
        _buckets = new HashNode*[other._bucket_count]();
        // for (size_type i =0; i<_bucket_count; i++){
        //     if (other._buckets[i]!=nullptr){
        //         _buckets[i] = new HashNode(*other._buckets[i]);
        //     }
        // }    
        // while (i<_size){
        //     insert(other.);
        //     i++
        // }
        const_iterator i = other.cbegin();
        //basic_iterator i(other._map, other._head);
        while (i!=cend()){
            insert(*i);
            i++;
        }
    }
    

    UnorderedMap(UnorderedMap && other) {
        _move_content(other, *this);
    }

    UnorderedMap & operator=(const UnorderedMap & other) {
        if (this!=&other){
            clear();
            delete _buckets;
            _size = 0;
            _bucket_count = other._bucket_count;
            _hash = other._hash;
            _equal = other._equal;
            _head = nullptr;
            _buckets = new HashNode*[other._bucket_count]();
            const_iterator i = other.cbegin();
            //basic_iterator i(other._map, other._head);
            while (i!=cend()){
                insert(*i);
                i++;
            }
        }
        return *this;
    }

    UnorderedMap & operator=(UnorderedMap && other) {
        if (this!=&other){
            clear();
            delete _buckets;
            _move_content(other, *this);

        }
        return *this;
    }

    void clear() noexcept {
        for (size_type i = 0; i < _bucket_count; i++) {
            HashNode *node = _buckets[i];
            while (node != nullptr) {
                HashNode *tmp = node;
                node = node->next;
                delete tmp;
            }
            _buckets[i] = nullptr;
        }
        _size = 0;
        _head = nullptr;
    }

    size_type size() const noexcept {
        return _size;
    }

    bool empty() const noexcept {
        return (_size==0);
    }

    size_type bucket_count() const noexcept {
        return _bucket_count;
    }

    iterator begin() {
        if (_bucket_count==0){
            return end();
        }
        return iterator(this, _head);
    }
    iterator end() {
        return iterator(this, nullptr);
    }

    const_iterator cbegin() const {
        if (_bucket_count==0){
            return cend();
        }
        return const_iterator(this, _head);
    };
    const_iterator cend() const {
        return const_iterator(this, nullptr);
    };

    local_iterator begin(size_type n) { //N is a bucket
        
        return local_iterator(_buckets[n]); //_bucktes[n] takes us to the nth bucket in the array and you return a pointer to that which wil be the head of the linked list
    } 
    local_iterator end(size_type n) { 
        return local_iterator(nullptr);
    }

    size_type bucket_size(size_type n) {
        if (n >= _bucket_count) {
            return 0;  // bucket index out of range
        }
        size_type count = 0;
        HashNode* node = _buckets[n];
        //local_iterator tmp = begin(n);
        while (node != nullptr) {
            count++;
            node = node->next;
        }
        return count;
    }

    float load_factor() const {
        return float(_size)/float(_bucket_count);
    }

    size_type bucket(const Key & key) const {
        if (bucket_count()!=0){
            return _range_hash(_hash(key), bucket_count());
        }
    }

    std::pair<iterator, bool> insert(value_type && value) {
        size_type code = _hash(value.first);
        size_type bucket = _bucket(code);
        HashNode * node = _buckets[bucket];
        while (node != nullptr) {
            if (_equal(node->val.first, value.first)) {
                return {iterator(this, node), false};
            }
            node = node->next;
        }
        HashNode * new_node = new HashNode(std::move(value), _buckets[bucket]);
        _buckets[bucket] = new_node;
        _size++;

        if (_head==nullptr || bucket <= _bucket(_head->val)) //bucket that head is in
        {
            _head = new_node;
        }
        return {iterator(this, new_node), true};
    }

    std::pair<iterator, bool> insert(const value_type & value) {
        size_type code = _hash(value.first);
        size_type bucket = _bucket(code);
        HashNode * node = _buckets[bucket];
        while (node != nullptr) {
            if (_equal(node->val.first, value.first)) {
                return {iterator(this, node), false};
            }
            node = node->next;
        }
        HashNode * new_node = new HashNode(value, _buckets[bucket]);
        _buckets[bucket] = new_node;
        _size++;


        if (_head==nullptr || bucket <= _bucket(_head->val)) //bucket that head is in
        {
            _head = new_node;
        }
        return {iterator(this, new_node), true};
    }

    iterator find(const Key & key) {
        // iterator tmp= begin();
        // while (tmp!=end()){ 
        //     if (_equal((*tmp)->val.first, key)){
        //         return tmp;
        //     }
        //     tmp++;
        // }
        // return tmp;
        return iterator(this, _find(_hash(key), _bucket(key), key));
    }

    T& operator[](const Key & key) {
        //iterator iter = find(key);
        //if (iter!=)
        value_type* v = &(find(key)._ptr->val);
        if (find(key)!=end()){
            return v->second;
        }
        else {
            std::pair<iterator, bool> hi = insert({key, T{}});
            return hi.first->second;
        }
    }
    

    iterator erase(iterator pos) {
        if (pos == end()) {
            return end();
        }
        // HashNode* node = pos._ptr;
        // HashNode* prev = nullptr;
        // auto index = _bucket(node->val);
        // HashNode* current = _buckets[index];
        // while (current!=nullptr){
        //     if (_equal(current->val.first, node->val.first)) {
        //         if (prev == nullptr) {
        //             _buckets[index] = current->next;
        //         } else {
        //             prev->next = current->next;
        //         }
        //         delete current;
        //         _size--;
        //         // Return the iterator following the erased element
        //         return iterator(this, node->next);
        //     }
        //     prev = current;
        //     current = current->next;
        // }
        // return end(); 

        HashNode *& node = _find(pos->first); //sth to do w operator overload pass by reference
        if (node!=nullptr){
            iterator tmp = ++iterator(this,node);
            HashNode* curr = node;
            //HashNode* prev = nullptr;

            //curr = curr.
            node = node->next;
            //curr = node;
            delete curr;

            if (_head==curr){
                _head = tmp._ptr;
            }
            _size--;
            return tmp;

        }

        return end();


    }

    size_type erase(const Key & key) {
        size_type count = 0;
        auto index = _bucket(key);

        HashNode* prev = nullptr;
        HashNode* curr = _buckets[index];

        while (curr != nullptr) {
            if (_equal(curr->val.first, key)) {
                if (prev == nullptr) {
                    _buckets[index] = curr->next;
                } else {
                    prev->next = curr->next;
                }

                delete curr;
                _size--;
                count++;
                break;
            }

            prev = curr;
            curr = curr->next;
        }
        return count;
    }

    template<typename KK, typename VV>
    friend void print_map(const UnorderedMap<KK, VV> & map, std::ostream & os);
};

template<typename K, typename V>
void print_map(const UnorderedMap<K, V> & map, std::ostream & os = std::cout) {
    using size_type = typename UnorderedMap<K, V>::size_type;
    using HashNode = typename UnorderedMap<K, V>::HashNode;

    for(size_type bucket = 0; bucket < map.bucket_count(); bucket++) {
        os << bucket << ": ";

        HashNode const * node = map._buckets[bucket];

        while(node) {
            os << "(" << node->val.first << ", " << node->val.second << ") ";
            node = node->next;
        }

        os << std::endl;
    }
}
