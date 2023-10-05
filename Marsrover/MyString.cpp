// TODO: Implement this source file

#include <iostream>
#include <fstream>
#include <tuple>
#include <limits>
#include <stdexcept>
#include "MyString.h"
#include "Rover.h"
using std::ostream;
using namespace std;


MyString::MyString(const MyString &other): data_(new char[other.capacity_]), size_(other.size_), capacity_(other.capacity_) { //copy constructor
        // delete[] data_;
        // capacity_ = other.capacity_;
        //create a new array and then copy everything. don't delete anything 
        size_t i = 0;
        // data_ = new char[capacity_];
        // size_ = other.size_;
        for(i = 0; i<other.size_; i++){
            data_[i] = other.data_[i]; 
        }
        //cout << "fgaukdfyhkasjdf";
        //data_[other.size_] = '\0';
}
MyString::MyString (const char* s): data_(new char[1]), size_(0), capacity_(1) {
    //data_ = new char[capacity_];
    if (s==nullptr){
        return;
    }
    delete[] data_;
    size_t i =0;
        /*while(data_[i] != '\0'){
            data_[i] = s[i];
            i++;
        }*/
    while(s[i]!= '\0'){
        i++;
    }
    data_ = new char[i+1];
    for (size_t k = 0; k<i; k++){
        data_[k] = s[k];
    }
        data_[i] = '\0';
        size_= i;
        capacity_ = i+1;
}

MyString::~MyString()
    { 
        delete[] data_;
        data_ = nullptr;
    }

MyString& MyString::operator= (const MyString& other){
        if(this!= &other){
            //copy the data and then allocate memory   
            delete[] data_;
            
            capacity_ = other.capacity_;
            data_ = new char[capacity_];
            size_ = other.size_;
            for(size_t i = 0; i<other.size_; i++){
                data_[i] = other.data_[i];
            }
            //data_[other.size_] = '\0';
        }
        return *this;
    }

size_t MyString::size() const noexcept{
        /*size_t s = 0;
        //int i =0;
        while (data_[s]!= '\0'){
            s++;
        }
        s = size_;*/
        return size_;
    }

void MyString::resize (size_t n){
    if (n<=size_){
        char* newarr = new char[n+1];
        for (size_t i =0 ; i<n; i++){
            newarr[i] = data_[i];
        }
        newarr[n] = '\0';
        delete[] data_;
        data_ = newarr;
        size_ = n;
        capacity_ = n+1;
    }
    else if (n>size_){
        char* newarr = new char[n+1];
        for (size_t i =0 ; i<size_; i++){
            newarr[i] = data_[i];
        }
        for (size_t i = size_; i<n; i++){
            newarr[i] = '\0';
        }
        newarr[n] = '\0';
        delete[] data_;
        data_ = newarr;
        size_ = n;
        capacity_ = n+1;
    }
}


size_t MyString::length() {
    return size_;
    }

bool MyString::empty()const noexcept{
    if (size_ == 0){
        return true;
    }
    return false;
}

ostream& operator<< (std::ostream& os, const MyString& str){

        for (size_t i = 0; i<str.size_; i++){
            os << str.data_[i];
        }
        //os << '\0';
        return os;
    }

void MyString::clear() noexcept{
    if (data_==nullptr){
        return;
    }
    delete[] data_;
    data_ = new char[1];
    data_[0] = '\0';
    //data_= nullptr;
    size_ = 0;
    //capacity_ = 1;
}

MyString& MyString::operator+=(const MyString& str){
    size_t prev = size_;
    capacity_ = str.capacity_ + capacity_ -1;
    
    char* newarr = new char[capacity_];
    for (size_t i = 0; i< prev; i++){
        newarr[i] = data_[i];
    }
    for (size_t j = 0; j<str.size_; j++){
        newarr[j + prev] = str.data_[j];
    }
    /*
    if (str.size_+size_>=capacity_){
        resize(str.size_+size_);   
    }
    size_t i = 0;
    while (i!=str.size_){
        data_[size_ + i] = str.data_[i];
        i++;
    }*/
    size_ = str.size_+size_;
    
    newarr[size_] = '\0';
    delete[] data_;
    data_ = newarr;
    return *this;
}

const char* MyString::data() const noexcept{
    return data_;
}

const char& MyString::at (size_t pos) const{
        if (pos >= size_){
            throw out_of_range("");
        }
        return data_[pos];
    }

size_t MyString::capacity() const noexcept{
        return capacity_;
    }

const char& MyString::front() const{
    return data_[0];
}

size_t MyString::find (const MyString& str, size_t pos) const noexcept{
    size_t p = 0;
    bool found = false;
    size_t ans = 0;
    for (p = pos; p<size_; p++){
        if (data_[p] == str.data_[0]){
            ans = p;
            for (size_t i = 0; i<str.size_; i++){
                if (data_[p+i]== str.data_[i]){
                    found = true;
                    //cout << "this";
                }
                else{
                    found = false;
                    //ans = 0;
                    break;
                }
            }
        }
        if (found){ //break at first instance
            break;
        }
    }
    if (found){
        return ans;
    }
    return npos;
} 