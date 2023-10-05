#include <iostream>
#include <string>
#include <limits>
#include "./string_calculator.h"

using std::cout, std::endl, std::cin;
using std::string;

int main() {
    cout << "String Calculator" << endl;
    cout << "\"q\" or \"quit\" or ctrl+d to exit" << endl;
    cout << ">> " << endl;
    string input;
    //int i =0;
    string lhs;
    string rhs;
    //int operator_ind;
    //getline(cin,input); 
    string addi;
    string prod;
    string opr;
    while (lhs+prod+rhs!= "q" || lhs+prod+rhs!= "quit") {
        cin >> lhs;
        if (lhs == "q" || lhs == "quit") {
            break;
        }
        cin>> opr;
        cin>> rhs;
        //int i = 0;
        /*while (input.at(i)!= '+' || input.at(i) != '*'){
            lhs+=input[i];
            i++;
        }
        operator_ind = i;
        i+=1; 
        while (input[i]){
            rhs+=input[i];
        }*/
        if (opr=="+") {
            addi = add(lhs,rhs);
            cout << "ans =" << endl << endl;
            cout << "    " << addi << endl<<endl;
            cout << ">> " << endl;
            
        }
        else if (opr=="*"){
            prod = multiply(lhs,rhs);
            cout << "ans =" << endl << endl;
            //cout << endl;
            cout << "    " << prod << endl << endl;
            cout << ">> " << endl;;
            
        }
        
   }

   cout<< "farvel!"<<endl<<endl;
   return 0;
}

