#include <iostream>
#include <iterator>

int main() {
  std::cout << "Hello World!\n";
  std::string out;
  std:: string m(int n, int k, int l);
  out = m(6,2,3);
  //std::cout <<out;
  bool tripod(int n);
  //std::cout << tripod(121);
  int arrayseg(int arrs[]);
  int input[100]={5,2,2,3,4,4,4,4,1,1,1};
  std::cout << arrayseg(input);
}

std::string m(int n, int k, int l) {
  int i = 1;
  int j =0;
  
  std::string intstr="";
  while (true){
    i++;
    if (i % l == 0 || i % k ==0){
      //std::stringstream ss;
      //ss << i;
      //intstr = ss.str();
      intstr+=std::to_string(i);
      j++;
    }
    if (j==n){
      break;
    }
    
  }

  return intstr;
}


bool tripod(int n) {
  int i = 1;
  while (i<n/4) {
    if (i*(i+1)*(i+2)==n){
      return true;
    }
    else i++;
  }
  return false;
  
}


int arrayseg(int arrs[100]){
  int count=0;
  int i = 0;
  //std::cout<< std::size(arrs);
  for (i=0; i< std::size(arrs); i++) {
    if (arrs[i]!=arrs[i+1]){
      count+=1;
      std::cout << count;
    }
  }
  return count;
}