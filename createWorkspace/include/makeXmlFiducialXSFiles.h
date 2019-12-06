//===================Scripts to write xml files from each rootFile loan.truong@cern.ch======================
#ifndef MAKEXMLFIDUCIALXSFILES_H
#define MAKEXMLFIDUCIALXSFILES_H


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


void makeXmlFiducialXSFiles(string signalRootFile,
			    string pathInput,
			    string xSection,
			    string region,
			    string channel,
			    string particle,
			    string mass,
			    ofstream& fileRunWS,
			    ofstream& fileRunAsymptotics,
			    bool doNarrow);



#endif
