//===================Scripts to write xml files from each rootFile loan.truong@cern.ch======================
#ifndef PREPARESIGNAL_H
#define PREPARESIGNAL_H

#include "TFile.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TH1F.h"
#include "TObjArray.h"
#include "TDirectory.h"
#include "TROOT.h"
#include "TGraph.h"
//#include "TMath.h"


#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <vector>

void interpolateHist(TDirectoryFile* oldTDirectory,
		     TDirectoryFile* newTDirectory,
		     TFile* file,
		     double binFirst,// = 15., /*GeV*/
		     double binLast,//  = 60., /*GeV*/
		     double stepSize);// = 0.5 /*GeV*/);


void interpolateSignal(std::string& rootFile,
		       std::ofstream& myFileInterpolatedSignal,
		       double binFirst,// = 15, /*GeV*/
		       double binLast,// = 60, /*GeV*/
		       double stepSize);// = 0.5 /*GeV*/);
		       

void importOneBinHist(TDirectoryFile* oldTDirectory,
		   TDirectoryFile* newTDirectory,
		   TFile* newFile,
		   int atBin,
		   int numBin = 1);

void splitInterpolatedSignal(std::string& rootFileName,
			     std::ofstream& outFileSplitSignal,
			     std::string& channel,
			     std::string& particle,
			     double binFirst,//    = 1., /*GeV*/
			     double binLast,//     = 15., /*GeV*/
			     double stepSize);//    = 0.5); /*GeV*/


void splitDiscreteSignal(std::string& rootFileName,
			 std::ofstream& outFileSplitSignal,
			 std::string& channel,
			 std::string& particle);
/* double binFirst,    /\*GeV*\/ */
/* double binLast,     /\*GeV*\/ */
/* double stepSize) */

 
void makeGaussianHist(TH1D* hist,
		      double norm,
		      double errNorm,
		      double mean,
		      double sigma,
		      double lumi);// = 139);//36.1);//36.1);/* fb-1 */
    
void importHistFromArrayToTDirectory(TDirectoryFile* newTDirectory,
				     TFile *fileNew,
				     TObjArray *MyHistArray);

void createOneSignalTDirectory(TDirectoryFile* newTDirectory,
			       TFile *fileNew,
			       std::string& suffix,
			       std::map<std::string,double> mapInput,
			       std::vector<std::string> vec_strBaseHistName, //= { "sig", "eff", "mean", "sigma"},
			       double binFirst,
			       double binLast,
			       double sSize,
			       double lumi);

void createOneSignalRootFile(std::string & xSection,
			     std::ofstream& outFileGaussianModel,
			     std::string &rootFileName,
			     double binFirst,// = 0.93,//0.,  /*GeV*/
			     double binLast,// = 19.93,//64., /*GeV*/
			     double sSize,// = 0.1, // 1. /*GeV*/
			     double lumi,
			     std::string fileType = "efficiency");

void createAllGaussianSignalRootFile(std::string & xSection,
				     std::ofstream& outFileGaussianModel,
				     /* double binFirst = 0., */
				     /* double binLast = 64., */
				     /* double sSize = 1., */
				     double binFirst,// = 0.93,
				     double binLast,// = 19.93,
				     double sSize,// = 0.1,
				     double lumi,
				     std::string fileType="efficiency"); /*GeV*/


#endif
