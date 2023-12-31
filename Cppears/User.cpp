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
#include "User.h"

using std::string;
using std::vector;

User::User(string userName) /* TODO(student): initialize */ :
  userName(userName),
  userPosts()
{
  // TODO(student): implement constructor checks
  if (userName == "" || !isalpha(userName[0]) && !islower(userName[0])){
    throw std::invalid_argument("");
  }
  for (int i =0; i<userName.length(); i++){
    if (!isalpha(userName[i])||!islower(userName[i])){
      throw std::invalid_argument("");
    }
  }
}


string User::getUserName() {
  // TODO(student): implement getter
  return userName;
}

vector<Post*>& User::getUserPosts() {
  // TODO(student): implement getter
  return userPosts;
}

void User::addUserPost(Post* post) {
  // TODO(student): add post to user posts
  if(post==nullptr){
    throw std::invalid_argument("");
  }
  userPosts.push_back(post);
}
