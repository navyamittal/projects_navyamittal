#include <iostream>
#include <string>
using std::cin, std::cout, std::endl, std::string;

int main() {
    string sentence;
    string word;
    cout <<  "Please enter the sentence: ";// << endl;
    getline(cin,sentence);
    cout << "Please enter the filter word: ";// << endl;
    cin >> word;
    int len = word.length();
    string output;
    for (int i=0; i<len;i++) {
        output += '#';
    }
    while (sentence.find(word) != string::npos){
        sentence.replace(sentence.find(word), len, output);
    }
    cout << "Filtered sentence: " << sentence << endl;

    return 0;
}
    /*stringstream s(sentence);
    string temp;
    bool found;
    int i = 0;
    while (s >> temp) {
 
        // Comparing the current word
        // with the word to be searched
        if (temp.compare(word) == 0) {
            
        }
    }
    
    return false; */

