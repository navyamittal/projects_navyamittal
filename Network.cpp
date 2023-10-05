/*
 *
 * This code is copyrighted (c) 2021 by
 * Texas A&M Computer Science
 *
 *	There will be RIGOROUS cheat checking on your exams!!
 *	DON'T POST ANYWHERE!! such as CHEGG, public Github, etc
 *  You will be legally responsible.
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Network.h"

using std::string;
using std::vector;
using namespace std;

Network::Network() {
  // empty containers of vectors already created
  // no implementation is needed here
}

void Network::loadFromFile(string fileName) {
  // TODO(student): load user and post information from file
  ifstream fin(fileName);
  string p = "";
  if (!fin.is_open()){
    throw std::invalid_argument("");
  }
  while (getline(fin,p)){
    /*if (fin.fail()){
      throw std::runtime_error("");
    }*/
    stringstream sin(p);
    sin >> p;
    if (p != "Post" && p!="User"){
      cout << "1" <<endl;
      throw std::runtime_error("");
    }
    
    if (p=="User"){
      string uname = "";
      sin >> uname;
      if (sin.fail()){
        cout << "2"<<endl;
        throw std::runtime_error("");
      }
      string s = "";
      sin>> s;
      /*if (!sin.fail()){
        cout << "27816"<<endl;
        throw std::runtime_error("");

      }*/
      if (uname == ""){
        throw std::runtime_error("");
      }
      for (unsigned int i =0; i<uname.length();i++){
        if (!isalpha(uname.at(i))){
          throw std::runtime_error("");
        }
      }
      addUser(uname);
      
    } 
    else if (p == "Post"){
      unsigned int id = 0;
      sin >> id;
      if (sin.fail() || sin.eof()){
        cout << "3"<<endl;
        throw std::runtime_error("");
      }
      string uname = "";
      sin >> uname;
      if (sin.fail() || sin.eof()){
        cout << "4"<<endl;
        throw std::runtime_error("");
      }
      if (uname == ""){
        throw std::runtime_error("");
      }
      for (unsigned int i =0; i<uname.length();i++){
        if (!isalpha(uname.at(i))){
          throw std::runtime_error("");
        }
      }
      //uname -> getUserName(uname);
      string text = "";
      getline(sin,text);
      if (sin.fail()){
        cout << "5"<<endl;
        throw std::runtime_error("");
      }
      if (text == ""){
        throw std::runtime_error("");
      }
      text.erase(0,1);
      addPost(id,uname,text);
    }
  }
}

void Network::addUser(string userName) {
  // TODO(student): create user and add it to network
  for (unsigned int j=0; j<users.size();j++){
    if (users.at(j) -> getUserName () == userName){
      throw std::invalid_argument("");
    }
  }
  for (unsigned int i = 0; i<userName.length();i++){
    if (isalpha(userName[i])){
      userName[i] = tolower(userName[i]);
    }
  }
  bool present = false;
  for (unsigned int j = 0; j<users.size();j++){
    if (users.at(j) -> getUserName () == userName){
      present = true;
      throw std::invalid_argument("");
    }
  }
  if (!present){
    User* newuser = new User(userName);
    users.push_back(newuser);
    cout << "Added User " << userName <<endl;
  }
  
}

void Network::addPost(unsigned int postId, string userName, string postText) {
  // TODO(student): create post and add it to network
  for (unsigned int j=0; j<posts.size();j++){
    if (posts.at(j) -> getPostId () == postId){
      throw std::invalid_argument("");
    }
  }
  bool present = false;
  for (unsigned int j=0; j<users.size();j++){
    //bool present = false;
    if (users.at(j) -> getUserName () == userName){
      present = true;
    }
  }
  if (!present){
    throw std::invalid_argument("");
  }

  Post* newpost = new Post(postId,userName,postText);
  posts.push_back(newpost);
  for (unsigned int i =0; i<users.size();i++){
    if (users.at(i) -> getUserName () == userName){
      users.at(i) ->addUserPost(newpost);
    }
  }
  std::vector<std::string> temp = {};
  temp = (newpost->findTags()); //vector of tags within the post
  /*for (unsigned int i = 0; i<temp.size(); i++){
    cout << temp.at(i);
  }*/
  /*std::vector<Tag*> tagP;
  tagP = (newpost ->getTagPosts());*/
  //bool tagpresent = false;
  for (unsigned int j =0;j<temp.size();j++){
    bool tagpresent = false;
    for (unsigned int i =0; i<tags.size();i++){
      if (temp.at(j) == tags.at(i) -> getTagName() ){
        tagpresent = true;
        tags.at(i)->addTagPost(newpost);
        //cout << "Added Post " << postId << " by " << userName<<endl;
        break;
      }
    }
    if (!tagpresent){
      Tag* newtag;
        
    try{
      newtag = new Tag(temp.at(j));
      tags.push_back(newtag);
      tags.at(tags.size()-1) ->addTagPost(newpost);
      //cout << "Added Post " << postId << " by " << userName <<endl;
    }
    catch (std::invalid_argument){
    }
      
    
    }
    
 }
  cout << "Added Post " << postId << " by " << userName <<endl;
}

vector<Post*> Network::getPostsByUser(string userName) {
  // TODO(student): return posts created by the given user
  if (userName == ""){
    throw std::invalid_argument("");
  }
  std::vector<Post*> temp;
  for (unsigned int j=0; j<users.size();j++){
    if (users.at(j) -> getUserName () == userName){
      temp =  users.at(j) ->getUserPosts();
    }
  }
  return temp;
}

vector<Post*> Network::getPostsWithTag(string tagName) {
  // TODO(student): return posts containing the given tag
  std::vector<Post*> temp; 
  if (tagName == ""){
    throw std::invalid_argument("");
  }
  for (unsigned int j=0; j<tags.size();j++){
    if (tags.at(j) -> getTagName () == tagName){
      temp = (tags.at(j) ->getTagPosts());
    } 
  }
  return temp; 
}


vector<string> Network::getMostPopularHashtag() {
  // TODO(student): return the tag occurring in most posts
  unsigned int maxvectorsize = 0;
  for (unsigned int i =0; i<tags.size(); i++){
    if (maxvectorsize<tags.at(i) ->getTagPosts().size()){
        maxvectorsize = tags.at(i) ->getTagPosts().size();
    }
  }
  std::vector<string> temp = {};
  for (unsigned int i =0; i<tags.size(); i++){
    if (maxvectorsize == (tags.at(i) ->getTagPosts()).size()){
      temp.push_back(tags.at(i)->getTagName());
    }
  }
  return temp;
}

/**
  * Destructor
  * Do not change; already implemented.
  */
Network::~Network() {
  for (unsigned int i = 0; i < users.size(); ++i) {
    delete users.at(i);
  }
  for (unsigned int i = 0; i < tags.size(); ++i) {
    delete tags.at(i);
  }
  for (unsigned int i = 0; i < posts.size(); ++i) {
    delete posts.at(i);
  }
}
