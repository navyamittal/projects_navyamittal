#include <iostream>
#include <sstream>
#include <fstream>
using std::cin, std::cout, std::endl, std::string, std::getline,
      std::ifstream, std::istringstream;

int main() {
  string filename;
  cout << "Enter donation filename: ";
  cin >> filename;
  ifstream ifs(filename);
  if (!ifs.is_open()) {
    cout << "Failed to open file " << filename << "." << endl;
    return 1;
  }
  
  double maxTotalDonation = 0;
  int maxTotalCollector = 0;
  int uin = 0;
  double tempmax=0;
  double don = 0;
  int count = 0;
  string one;
  while(getline(ifs,one)){
    istringstream is(one);
    is >> uin;     
    tempmax = 0;
    count = 0;    
    while (is>>don){
      count+=1;
      tempmax +=don;     
    }
    if (tempmax> maxTotalDonation && count > 1){
      maxTotalDonation = tempmax;
      maxTotalCollector = uin;
    }
    
  }

  if (maxTotalDonation==0){
    cout << "No donations.";
  }
  
  else{
    cout << "Highest donation total: " << maxTotalDonation << endl;
    cout << "-- collected by id: " << maxTotalCollector << endl;
  }
}



/*  string s;
  double sum[50], donation;
  int j = 0;
  for(int i=0; i<numlines && getline(ifs,s); i++){
    istringstream sin(s);
    sum[i] = 0;

    while (sin>>donation){
      if (j==0){
        uin = donation;
      }
      else {
        sum[i] +=donation;
      }
      j++;
    }
    if(sum[i] > maxTotalDonation && j>2){
      maxTotalDonation = sum[i];
      maxTotalCollector = uin;
    }
    
  }
*/