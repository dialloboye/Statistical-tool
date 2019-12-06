//===================Scripts to write xml files from each rootFile loan.truong@cern.ch======================
#ifndef WRITESAMPLEXML_H
#define WRITESAMPLEXML_H


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


void writeSampleXml(string rootFile,
		    ofstream& writeFile,
		    string type);
#endif
