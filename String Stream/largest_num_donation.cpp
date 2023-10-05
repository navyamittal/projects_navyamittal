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
  
  // read data
  int uin=0;
  /*ifs >> uin;
  maxTotalCollector = uin;*/
  //int don = 0;
  int numlines = 0;
  string one;
 /* while(getline(ifs,one)){
    ifs >> uin;
    numlines+=1;
    istringstream is(one);
    while (is>>don){
      maxTotalDonation+=don;
    }
  }*/
  string s;
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

  if (maxTotalDonation==0){
    cout << "No donations.";
  }
  
  else{
  cout << "Highest donation total: " << maxTotalDonation << endl;
  cout << "-- collected by id: " << maxTotalCollector << endl;
  }
}
