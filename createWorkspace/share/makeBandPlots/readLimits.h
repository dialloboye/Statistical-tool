#ifndef  __READLIMITS_H
#define __READLIMITS_H
/* Script to read out limit values from results limit root-files
   bin1 obs
   bin2 exp
   bin3 2sigma
   bin4 1sigma
   bin5 -1 sigma
   bin6 -2sigma
The results will be written in text files, each correspond to a workspace
lotruong@cern.ch or ngamrong@gmail.com
August 06th, 2019.
*/
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>
#include <string>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>

#include "TFile.h"
#include "TH1F.h"
#include "TH1D.h"

bool inVector(std::vector<std::string> &v, const std::string &item);

std::string getDirectory();

void readLimits(std::string storeFolder,
		std::string xSection,
		std::string region,
		std::vector<std::string> vecChannels,
		std::string particle,
		double firstMass,
		double lastMass,
		double stepSize,
		std::string nameTag);


#endif //   __READLIMITS_H
