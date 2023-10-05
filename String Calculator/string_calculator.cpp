#include <iostream>
#include <string>
#include "./string_calculator.h"
#include <sstream>

using std::cout, std::endl;
using std::string;
using std::swap;

unsigned int digit_to_decimal(char digit) {
    std::stringstream ss;
    ss << digit;
    string str = ss.str();
    //int c = str[0]; '1'
    /*if (str[1]) {
        throw std::invalid_argument("");
    }*/
    if (str.find_first_not_of("01234567890") != std::string::npos)
{
    //std::cerr << "Error\n";
    throw std::invalid_argument("ABCS");
}

    return digit - '0';
}

char decimal_to_digit(unsigned int decimal) {
    //cout << decimal << '\n';
    std::stringstream ss;
    ss << decimal;
    string str = ss.str();
    char c = str[0];
    if (str[1]) {
        throw std::invalid_argument("yrgfukwegyfukj");
    }
    return c;
}

string trim_leading_zeros(string num) {
    int i = 0;
    bool neg=false;
    string strnum = "";
    if (num[0]=='-') {
        num.erase(0,1);
        neg = true;
    }
    else {
        neg = false;
    }
    int k=num.length();
    for (i=0;i<k; i++) {
        
        if (num[i]!='0') {
            strnum+=num[i];
        }
        
        if (num[i]=='0' && strnum.length()>0){
            strnum+=num[i];
        }        
    }
    if (strnum == "") {
        return "0";
    }
    if (neg){
        return "-"+strnum;
    }
    
    return strnum;
}

string reverse(string str)
{
    string rev="";
    for (int i = str.length() - 1; i >= 0; i--)
        rev.push_back(str[i]);
    return rev;
}
string add(string lhs, string rhs) {
    // TODO(student): implement
    string output="";
    int i;
    int j;
    const int SIZE = 1000;
    int diglhs[SIZE];
    int digrhs[SIZE];
    for (int l=0; l<SIZE-1;l++){
        diglhs[l] = 0;
    }
    for (int l=0; l<SIZE-1;l++){
        digrhs[l] = 0;
    }
    int carry=0;
    int sum;
    bool neg=false;
    if( (lhs.find('-') != string::npos && rhs.find('-') != string::npos) ||  // both are negative
		(rhs.find('-') != string::npos && lhs.find('0') == 0) ||  // right is negative, left is 0
		(lhs.find('-') != string::npos && rhs.find('0') == 0) // right is 0, left is negative
	  ) 
	{ 
		lhs.erase(0,1);  // deletes negative sign 
		rhs.erase(0,1);
		neg = true;
    }
    lhs= trim_leading_zeros(lhs);
    rhs = trim_leading_zeros(rhs);
    lhs=reverse(lhs);
    rhs=reverse(rhs);
    for (i=lhs.length()-1; i>=0; i--) {
        diglhs[i] = digit_to_decimal(lhs[i]);

    }
    for (j=rhs.length()-1; j>=0; j--){
        digrhs[j] = digit_to_decimal(rhs[j]);
    }
    for (int k=0; k<SIZE-1;k++){
        sum = diglhs[k]+digrhs[k] + carry;
        carry = 0; //reset
        if (sum / 10 != 0)
        {
            carry = sum/10;
            output+=decimal_to_digit(sum%10);
        }
        else {
            carry = 0;
            output+=decimal_to_digit(sum%10);
        }

        
    }
    if (neg) {
        output.append("-");
    }
    output=reverse(output);
    output = trim_leading_zeros(output);
    return output;
}

string multiply(string lhs, string rhs) {

    string output="";
    string sum = "0";
    bool neg = false;
    int carry=0;
    int multi;
    int counter = 1;
    if (lhs=="0" || rhs=="0"){
        return "0";
    }
    //both are negative
    if (lhs[0]=='-' && rhs[0]=='-') { 
        neg = false;
        rhs.erase(0,1);
        lhs.erase(0,1);
    }
    //right is negative, lhs is +
    else if (rhs[0]=='-' && lhs[0]!= '-') {
        rhs.erase(0,1);
        neg = true;
    }
        
	else if (lhs[0]=='-' && rhs[0]!='-') {
        lhs.erase(0,1);
        neg = true;
    }

    else if (lhs[0]!='-' && rhs[0]!='-'){
        neg = false;
    }
    if (rhs.length() < lhs.length())  // swap them so the bigger number is on top
	{
		swap(lhs,rhs);
	}
    lhs= trim_leading_zeros(lhs);
    rhs = trim_leading_zeros(rhs);

    for (int i=lhs.length()-1; i>=0; i--){
        for (int j=rhs.length()-1; j>=0; j--){
            multi = (digit_to_decimal(lhs[i]) * digit_to_decimal(rhs[j])) + carry;
            carry = 0;
            if (multi>=10){
				if (j == 0) 
				{  
					int loopProduct = multi;
					while (loopProduct != 0) 
					{
						output += decimal_to_digit(loopProduct % 10);  // append last digit then the next digit
						loopProduct /= 10;
					}
				}
				else 
				{
					carry = multi/10;
                    output+=decimal_to_digit(multi%10);
				}
                

            }
            else {
                output+=decimal_to_digit(multi % 10);
                
            }
        }

        /*if (carry > 0) {
            output+=decimal_to_digit(carry);
        }*/

        //output = reverse(output);
        int j = output.length()-1;
        string newResult = "";
        while (j >= 0)
        {
			newResult += output.at(j);
			j--;
		}
        sum = add(sum,newResult);
        //cout << sum << " " << newResult << '\n';
        output = "";
		for (int k = 0; k < counter; k++){  // number of zeroes to add is equal to the outer loop (add one zero after first iteration (first row))
			//cout << "K: " << '\n';
            output += "0";
		    
        }
        counter++;
    }
    if (neg){
        return "-"+sum;
    }
    else{

	    return sum;
    }
}
