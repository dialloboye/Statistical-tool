//===================Scripts to write xml files from each rootFile loan.truong@cern.ch======================
#ifndef MAKEXMLTOTALXSFILES_H
#define MAKEXMLTOTALXSFILES_H


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


void makeXmlTotalXSFiles(string signalRootFile,
			 string channel,
			 string xSection,
			 string region,
			 string pathInput,
			 string particle,
			 string mass,
			 ofstream& fileRunWS,
			 ofstream& fileRunAsymptotics,
			 bool doNarrow);


#endif
