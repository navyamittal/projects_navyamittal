/*
 * TemperatureData.cpp
 *
 *  Created on: Jul 16, 2018
 *      Author: student
 */

#include "TemperatureData.h"
#include <string>
using namespace std;

TemperatureData::TemperatureData():id(""), year(0), month(0), temperature(0){} //: id(""), year(0), month(0), temperature(0) {} //initialize everything

TemperatureData::TemperatureData(std::string id, int year, int month, double temperature) : id(id), year(year), month(month), temperature(temperature){
} //initialize everything


TemperatureData::~TemperatureData() {} // You should not need to implement this

bool TemperatureData::operator<(const TemperatureData& b) {
	// Implement this
	bool out = false;
	if (this!=&b){
		// int loc = 0;
		// int thisid = 0; 
		// stringstream ss;
		// ss << b.id;
		// ss>> loc;
		// stringstream thisss;
		// thisss << this->id;
		// thisss >> thisid;
		if (b.id < this->id){ //this is not less than b.id
			out= false;
		}
		else if (b.id>this->id){
			out = true;
		}
		else if (b.id == this->id){
			if (b.year > this->year){
				out = true;
			}
			else if (b.year<this->year){
				out = false;
			}
			else if (b.year == this->year){
				if (b.month > this->month){
					out =  true;
				}
				else if (b.month < this->month){
					out = false;
				}

			}
		}
	}
	return out;
}

