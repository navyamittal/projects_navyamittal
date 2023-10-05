#include <iostream>
#include "cstring.h"

unsigned int length(char str[]) {
  // returns the length of the string including zero (0)
  
  int i=0;

  while (str[i]) {
    i+=1;
  }
  return i;

}

unsigned int find(char str[], char character) {
  int i;
  int p=0;
  unsigned int sz;

  while (str[p]) {
    p+=1;
  }

  sz = p;
  //for (i=0; i)
  
  // returns 
  //  - the index of the first occurence of character in str
  //  - the size if the character is not found
  for (i=0; i<sz; i++) {
    if (str[i] == character) {
      return i;
    }
  }
  return sz;
  }
  


bool equalStr(char str1[], char str2[]) {
  // returns true if they are equal and false if they are not
  int i;
  int p=0;

  while (str1[p]) {
    p+=1;
  }
  for (i=0; i<p; i++) {
      if (str1[i] != str2[i]) {
        return false;
      }
  }
  return true;
}