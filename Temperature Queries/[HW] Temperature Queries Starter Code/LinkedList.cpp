#include <iostream>
#include <string>
#include "LinkedList.h"
#include "Node.h"
#include <sstream>
using namespace std;

LinkedList::LinkedList() {
	// Implement this function
	head = nullptr;
	tail = nullptr;

}

LinkedList::~LinkedList() {
	// Implement this function
	/*Node* curr = head;
	Node*prev = head;
	while (curr!=nullptr){
		prev = curr;
		curr = curr->next;
		delete prev;
	}
	delete tail;
	head = nullptr;
	tail = nullptr;
	curr = nullptr;
	prev = nullptr;*/

	clear();
}

LinkedList::LinkedList(const LinkedList& source): head(new Node), tail(new Node) {
	Node* temp = source.head;
	head = nullptr;
	tail = nullptr;
	while (temp!=nullptr){
		//temp->next = new Node*(temp->data);
		Node* curr = new Node(temp->data.id,temp->data.year,temp->data.month,temp->data.temperature);
		curr->next = nullptr;
		if (head ==nullptr){ //when first node is created, set it to null
			head = curr;
			tail = curr;
		}
		else {
			tail->next = curr;
			tail = curr;
		}
		temp = temp->next;
	}
}

LinkedList& LinkedList::operator=(const LinkedList& source) {
	// Implement this function
	if (this!= &source){
		clear();
		head = nullptr;
		tail = nullptr;
		Node* temp = source.head;
		//Node* prev;
		//head->next = source.head->next;
		while (temp!=nullptr){
			Node* curr = new Node(temp->data.id,temp->data.year,temp->data.month,temp->data.temperature);
			curr->next = nullptr;
			if (head ==nullptr){ 
				head = curr;
				tail = curr;
			}
			else {
				tail->next = curr;
				tail = curr;
			}
			temp = temp->next;
		}
	}
	return *this;
}

void LinkedList::insert(string location, int year, int month, double temperature) {
	// Implement this function
	Node* newnode = new Node(location, year, month, temperature);
	newnode->next = nullptr;
	if (head == nullptr){
		head = newnode;
		tail = newnode;
		return;
	}
	Node* prev = nullptr;
	Node* curr = head;
	while(curr!=nullptr && curr->data < newnode->data){
		prev = curr;
		curr = curr->next;
	}
	if (curr == head){
		newnode->next = head;
		head = newnode;
	}
	else if (prev == tail){
		prev->next = newnode;
		tail = newnode;
	}
	else{
		prev->next = newnode;
		newnode->next = curr;
	}
}

void LinkedList::clear() {
	// Implement this function
	Node* prev = nullptr;
	while (head!=nullptr){
		prev = head;
		head = head->next;
		delete prev;
	}
	head = nullptr;
	tail = nullptr;
	prev = nullptr;
}

Node* LinkedList::getHead() const {
	// Implement this function it will be used to help grade other functions
	return head;
}

string LinkedList::print() const {
	string outputString;
	Node* prev = head;
	while (prev!=nullptr){
		stringstream ssyear;
		stringstream ssmonth;
		stringstream sstemp;
		string year;
		string month;
		string temp;
		outputString += prev->data.id + " ";
		ssyear << prev->data.year;
		ssyear>>year;
		outputString+= year + " ";
		ssmonth << prev->data.month;
		ssmonth >> month;
		outputString += month + " ";
		sstemp << prev->data.temperature;
		sstemp >> temp;
		outputString += temp; //+ "\n";
		//cout << endl;
		prev = prev->next;
		outputString+= '\n';

		/*if (prev!=nullptr){
			outputString+= "\n";
		}*/
	}
	return outputString;
}

ostream& operator<<(ostream& os, const LinkedList& ll) {
	/* Do not modify this function */
	os << ll.print();
	return os;
}
