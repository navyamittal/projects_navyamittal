#include "hash_functions.h"

size_t polynomial_rolling_hash::operator() (std::string const & str) const {
    size_t hash = 0;
    size_t p = 1;
    size_t b = 19;
    size_t m = 3298534883309ul;

    for (char const & c : str) {
        hash += c *p;
        //hash = (hash + (size_t)c * p) % m;
        p = (p * b) % m;
    }

    return hash;
}

size_t fnv1a_hash::operator() (std::string const & str) const {
    const size_t prime = 0x00000100000001B3;
    size_t hash = 0xCBF29CE484222325;
    for (char c : str) {
        hash ^= static_cast<size_t>(c);
        hash *= prime;
    }
    return hash;
}
