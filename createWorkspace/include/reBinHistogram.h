//===================Scripts to scale histograms on each rootFile loan.truong@cern.ch======================
#ifndef REBINHISTOGRAM_H
#define REBINHISTOGRAM_H

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




void getNonZeroBinRange(TH1D* hist, double threshold, int &firstBin, int &lastBin);

void reBinHistogram(TH1D *hist, TH1D *newHist, int firstBin, int lastBin);

void getRangeForFile(std::string& rootFileName, int &firstBin, int &lastBin);

void getHistogramForFile(std::string& rootFileName, int firstBin, int lastBin);

void getHistogramForFileNoDir(std::string& rootFileName, int firstBin, int lastBin);   

void remakeFile(std::string& rootFileName, double binFirst, double binLast, double binWidth, std::string& pathInput, std::string& region, std::string& particle);
//void remakeFile(std::string &rootFileName, double binFirst, double binLast, double binWidth);
    
#endif
