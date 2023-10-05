/*
 *
 * This code is copyrighted (c) 2021 by
 * Texas A&M Computer Science
 *
 *	There will be RIGOROUS cheat checking on your exams!!
 *	DON'T POST ANYWHERE!! such as CHEGG, public Github, etc
 *  You will be legally responsible.
 */

#include <string>
#include <stdexcept>
#include "Tag.h"

using std::string;
using std::vector;

Tag::Tag(string tagName): /* TODO(student): initialize */ 
  // TODO(student): implement constructor checks
  tagName(tagName), tagPosts()
  {
  if (tagName.length() < 2 || tagName[0] != '#' || !isalpha(tagName[1])){
    throw std::invalid_argument("1");
  }
  for (int i =0; i<tagName.length(); i++){
    if (isupper(tagName[i])){ //is an upper case alphabet
      throw std::invalid_argument("2");
    }
  }
  if (tagName[tagName.length()-1] == '!' || tagName[tagName.length()-1] == '.' || tagName[tagName.length()-1] == ',' || tagName[tagName.length()-1] == '?'){
    //if (tagName[tagName.length()-2-i] == '!' || tagName[tagName.length()-2-i] == '.' || tagName[tagName.length()-2-i] == ',' || tagName[tagName.length()-2-i] == '?'){
    throw std::invalid_argument("3");
    }
   
}

string Tag::getTagName() {
  // TODO(student): implement getter
  return tagName;
}

vector<Post*>& Tag::getTagPosts() {
  // TODO(student): implement getter
  return tagPosts;
}

void Tag::addTagPost(Post* post) {
  // TODO(student): add post to tag posts
  if (post == nullptr){
    throw std::invalid_argument("4");
  }
  tagPosts.push_back(post);
}
