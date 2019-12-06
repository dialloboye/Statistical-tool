#include "include/reBinHistogram.h"

using namespace std;

void getHistogramForFileNoDir(std::string& rootFileName, int firstBin, int lastBin)    
{
    //=======================================================
    /*
      Get access into each directory 
      1. create a list of histograms
      2. make new histograms for each bin
      3. remember to extract the bin errors
    */

    //Make a vector of new root file names with different mass points
    TFile *fileOriginal = new TFile(rootFileName.c_str(),"READ");
    string newFileName = rootFileName.substr(0,rootFileName.length()-5) + "reBin.root";

    TKey *keyHist;
    string classHist;
        
    TFile *fileNew = new TFile(newFileName.c_str(),"RECREATE");

    double threshold = 0.1;

    TIter nextHist(fileOriginal->GetListOfKeys());
    while( (keyHist = (TKey*) nextHist() ) )
	{
	    classHist = keyHist->GetClassName();
	    if(classHist != "TH1D" && classHist != "TH1F") continue;
	    
	    TH1D* hist = (TH1D*) keyHist->ReadObj();
	    const char* histName = hist->GetName();
	    int numBin = lastBin - firstBin + 1;
	    TH1D* newHist = new TH1D(histName, histName, numBin, firstBin-1, lastBin);
	    reBinHistogram(hist, newHist, firstBin, lastBin);
	    
	    fileNew->cd();
	    newHist->Write();
	    newHist->Delete();
	}
    
    fileNew->Close();
    fileNew->Delete();

    fileOriginal->Close();
    fileOriginal->Delete();
    
}



