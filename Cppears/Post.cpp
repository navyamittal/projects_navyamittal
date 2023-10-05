/*
 *
 * This code is copyrighted (c) 2021 by
 * Texas A&M Computer Science
 *
 *	There will be RIGOROUS cheat checking on your exams!!
 *	DON'T POST ANYWHERE!! such as CHEGG, public Github, etc
 *  You will be legally responsible.
 */

//#include istringstream
#include <iostream>
#include <string>
#include <stdexcept>
#include "Post.h"
#include <sstream>

 

using std::string;
using std::vector;

Post::Post(unsigned int postId, string userName, string postText)
  : postId(postId), userName(userName), postText(postText) {
  if (postId == 0 || userName == "" || postText == "") {
    throw std::invalid_argument("post constructor: invalid parameter values");
  }
}

unsigned int Post::getPostId() {
  return postId;
}

string Post::getPostUser() {
  return userName;
}

string Post::getPostText() {
  return postText;
}

vector<string> Post::findTags() {
  // TODO(student): extracts candidate tags based on occurrences of # in the post
  std::vector<string> output = {};
  std::stringstream ss(postText);
  string temp = "";  
  while (ss >> temp){
    if (temp.length()>0){  
      if (temp[0] == '#' /*&& temp.length()>1*/){
        while (temp.at(temp.length()-1) == '!'||temp.at(temp.length()-1) == '.' || temp.at(temp.length()-1) == ','|| temp.at(temp.length()-1)=='?'){//check if punct is at end of string 
          //temp.pop_back();
          temp.erase(temp.length()-1,1);
        }
        /*if (temp=="#"){
           break;
        }*/
        int newlength = temp.length(); 
        for (unsigned int i =1; i<newlength;i++){
          if (isalpha(temp[i])){
            temp.at(i) = tolower(temp.at(i));
          }          
        }
      
        bool present = false;
        for (unsigned int i=0; i<output.size(); i++){
          if (output.at(i) == temp){
            present = true;
            break;
          }
        }
        if (!present){
          output.push_back(temp);
        }        
      }
      
    }
  }
  return output;
}
