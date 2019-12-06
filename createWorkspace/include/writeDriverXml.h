//===================Scripts to write xml files from each rootFile loan.truong@cern.ch======================
#ifndef WRITEDRIVERXML_H
#define WRITEDRIVERXML_H

#include "TFile.h"
#include "TKey.h"
#include "TH1F.h"

#include <vector>
#include <string>
#include <sstream>
#include <map>
#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

void writeDriverXml(vector<string> vec_channelXmlFileName,
		    ofstream& myDriverFile);
#endif
