//===================Scripts to scale histograms on each rootFile loan.truong@cern.ch======================
#ifndef SCALINGHIST_H
#define SCALINGHIST_H

#include "TFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TH1F.h"
#include "TObjArray.h"
#include "TDirectory.h"
#include "TROOT.h"

#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <fstream>


void scalingHist(TFile* oldFile, TFile* newFile, std::string channel);
    		 
#endif
