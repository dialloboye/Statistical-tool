//===================Scripts to write xml files from each rootFile loan.truong@cern.ch======================
#ifndef WRITECHANNELXML_H
#define WRITECHANNELXML_H

#include "include/writeSampleXml.h"

#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"

#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>


using namespace std;



/* void removeSubstrs(string& s, string& p);//function to remove all substring from a string */

/* void replaceSubstrs(string& s, string& toRemove, string& Replace);//function to replace a subtring by another substring  */
void writeChannelXml(string channelXmlFileName,
		     string channel,
		     ofstream&channelWriteFile,
		     map<string,string> mapRootFileName );
#endif
