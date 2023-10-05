#include "TemperatureDatabase.h"
#include <iostream>
#include <fstream>
using namespace std;

// Default constructor/destructor. Modify them if you need to.
TemperatureDatabase::TemperatureDatabase() {}
TemperatureDatabase::~TemperatureDatabase() {}

void TemperatureDatabase::loadData(const string& filename) {
	// Implement this function for part 1
	int curryear = 2022;
	int oldyear = 1800;
	ifstream fin(filename);
    if (!fin.is_open()){
        cout << "Error: Unable to open " << filename;
    }
	string fileLine = "";
    while(!fin.eof()){
        getline(fin, fileLine);
        istringstream iss(fileLine);
        string id;
        fileLine >> id;
        int year;
        fileLine >> year;
        if (year>curryear || year<oldyear){
            cout << "Error: Invalid year " << year << endl;
            continue;
        }
        int month;
        fileLine >> month;
        if (fin.fail()){
            cout << "Error: Other invalid input " << endl;
            continue;
        }
        if (month<=0 || month > 12){
            cout << "Error: Invalid month " << month << endl;
            continue;
        }
        if(fin.eof()){
            cout << "Error: Other invalid input" << endl;
            continue;
        }
        double temp = 0;
        fileLine >> temp;
        if(temp=-99.99){
            continue;
        }
        if (temp>50 || temp < -50){
            cout << "Error: Invalid temperature " << temp << endl;
            continue;
        }
        records.insert(id, year, month, temp);
    }
}

// Do not modify
void TemperatureDatabase::outputData(const string& filename) {
	ofstream dataout("sorted." + filename);
	
	if (!dataout.is_open()) {
		cout << "Error: Unable to open " << filename << endl;
		exit(1);
	}
	dataout << records.print();
}
/*
void TemperatureDatabase::performQuery(const string& filename) {
	// Implement this function for part 2
	//  Leave it blank for part 1
}*/
