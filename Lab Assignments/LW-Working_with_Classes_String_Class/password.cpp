#include <iostream>
#include <string>
using std::cin, std::cout, std::endl, std::string;

int main() {
    string pwd;
    
    cout<< "Please enter your text input: ";
    cin >> pwd;
    cout<< "input: " << pwd << endl;
    string output;
    /*
    pwd.replace(pwd.find("a"),1,"@");
    pwd.replace(pwd.find("e"),1,"3");
    pwd.replace(pwd.find("i"),1,"!");
    pwd.replace(pwd.find("o"),1,"0");
    pwd.replace(pwd.find("u"),1,"^");
    
    cout << pwd << endl;;*/
    



    while (pwd.find("a") != string::npos){
        pwd.replace(pwd.find("a"), 1, "@");
    }
    while (pwd.find("e") != string::npos){
        pwd.replace(pwd.find("e"), 1, "3");
    }
    while (pwd.find("i") != string::npos){
        pwd.replace(pwd.find("i"), 1, "!");
    }
    while (pwd.find("o") != string::npos){
        pwd.replace(pwd.find("o"), 1, "0");
    }
    while (pwd.find("u") != string::npos){
        pwd.replace(pwd.find("u"), 1, "^");
    }

    output=pwd;
    for (int i = pwd.length()-1;i>=0;i--) {
        output+=pwd[i];
    }

    cout << "output: " << output << endl;
    return 0;
}
