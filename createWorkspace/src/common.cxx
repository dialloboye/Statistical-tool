#include "../include/common.h"

using namespace std;

void removeSubstrs(string& s, string& p) { //need to write more
    string::size_type n = p.length();
    for (string::size_type i = s.find(p); i != string::npos; i = s.find(p))
	s.erase(i, n);
}

//function to replace a subtring by another substring 
/* void replaceSubstrs(string& s, string& toRemove, string& Replace){//need to write more */
/*     string::size_type n = toRemove.length(); */
/*     for (string::size_type i = s.find(toRemove); i != string::npos; i = s.find(toRemove)){ */
/* 	s.replace(i,n,Replace); */
/*     } */
/* } */

void replace(string& str, const string& from, const string& to){
    size_t index;
    while (true) {
        index = str.find(from);
        if (index == string::npos) break;       /* Locate the substring to replace. */
        str.replace(index, from.length(), to);  /* Make the replacement. */
        index += from.length(); /* Advance index forward so the next iteration doesn't pick it up as well. */
    }
}


bool inVector(vector<string> &v, const string &item){
    vector<string>::iterator it = find(v.begin(), v.end(), item);
    if(it==v.end()){
        return false;
    }
    else return true;
}

//function to get a string between two strings
void getSubStrBetweenTwoStrings(const string& mainString, const string& string1, const string& string2, string& toReturn)
{ 
    string::size_type n1, n2, n, pos1, pos2;
    n1 = string1.length();
    n2 = string2.length();
    n = mainString.length();
    
    pos1 = (n1 > 1) ? mainString.find(string1):mainString.find_last_of(string1);
    
    pos2 = (n2 > 1) ? mainString.find(string2):mainString.find_last_of(string2);
    
    if(pos1 > pos2)
	{
	    string tmp = mainString.substr( mainString.find(string1), pos2 - mainString.find(string1) );
	    getSubStrBetweenTwoStrings(tmp, string1, string2, toReturn);
	    //cout << "tmp = " << tmp << "\n";
	    return;
	}
    toReturn = mainString.substr(pos1+n1, pos2-pos1-n1);
    // cout << "get substring " << toReturn << "\n";
    return;
}
