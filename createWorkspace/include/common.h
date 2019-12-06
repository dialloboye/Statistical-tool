#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm> /* for vector method std::find to work. Used in function inVector**/

using namespace std;

//function to remove all substring from a string
void removeSubstrs(string& s, string& p);
//{ //need to write more
   /*  string::size_type n = p.length(); */
/*     for (string::size_type i = s.find(p); i != string::npos; i = s.find(p)) */
/* 	s.erase(i, n); */
/* 
//}

//function to replace a subtring by another substring 
/* void replaceSubstrs(string& s, string& toRemove, string& Replace){//need to write more */
/*     string::size_type n = toRemove.length(); */
/*     for (string::size_type i = s.find(toRemove); i != string::npos; i = s.find(toRemove)){ */
/* 	s.replace(i,n,Replace); */
/*     } */
/* } */

void replace(string& str, const string& from, const string& to);
//{
    /* size_t index; */
    /* while (true) { */
    /*     index = str.find(from); */
    /*     if (index == string::npos) break;       /\* Locate the substring to replace. *\/ */
    /*     str.replace(index, from.length(), to);  /\* Make the replacement. *\/ */
    /*     index += from.length(); /\* Advance index forward so the next iteration doesn't pick it up as well. *\/ */
    /* } */
//}

bool inVector(vector<string> &v, const string &item);


void getSubStrBetweenTwoStrings(const string& mainString,
				const string& string1,
				const string& string2,
				string& toReturn);

#endif
