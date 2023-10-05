// TODO: Implement this header file
#ifndef MYSTRING_H
#define MYSTRING_H
#include <iostream>
using std::ostream;
using namespace std;
class MyString
{
private:
    char *data_;
    size_t size_;
    size_t capacity_;
    
public:
    static const size_t npos = -1;
    MyString(): data_(new char[1]), size_(0), capacity_(1) { data_[0] = '\0';}
    ~MyString();
    MyString(const MyString &other);
    MyString (const char* s);
    void resize (size_t n);
    size_t size() const noexcept;
    MyString& operator= (const MyString& other);
    MyString& operator+=(const MyString& str);
    size_t capacity() const noexcept;
    size_t length();
    bool empty() const noexcept;
    const char& at(size_t pos=0) const;
    const char& front() const;
    const char* data() const noexcept;
    friend std::ostream& operator<< (std::ostream& os, const MyString& str); 
    void clear() noexcept;
    //ostream& operator<< (ostream& os, const MyString& str);
    size_t find (const MyString& str, size_t pos = 0) const noexcept; //return npos if you can't find 
};

#endif