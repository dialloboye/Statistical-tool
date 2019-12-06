#include "include/reBinHistogram.h"

using namespace std;

void getHistogramForFile(std::string& rootFileName, int firstBin, int lastBin)    
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

    TKey *keyDir,*keyHist;
    TDirectoryFile *dir;
    string classDir, classHist;
    
    
    TIter next(fileOriginal->GetListOfKeys());
    TFile *fileNew = new TFile(newFileName.c_str(),"RECREATE");

    double threshold = 0.1;

    while ( (keyDir = (TKey*) next() ) )
	{
	    classDir = keyDir->GetClassName();
	    if (classDir != "TDirectoryFile") continue;
	    dir = (TDirectoryFile*) keyDir->ReadObj();
	    const char *tdirectoryName = dir->GetName();
	    //	    cout << "Processing directory " << tdirectoryName << endl;
	    
	    TDirectoryFile *newTDirectory = new TDirectoryFile(tdirectoryName,tdirectoryName,"",0);	    
	    TIter nextHist(dir->GetListOfKeys());
	    while( (keyHist = (TKey*) nextHist() ) )
		{
		    classHist = keyHist->GetClassName();
		    if(classHist != "TH1D" && classHist != "TH1F") continue;
		    
		    TH1D* hist = (TH1D*) keyHist->ReadObj();

		    //pay attention that ".ls" shows key's Names which can be different from histograms' names and titles
		    //cout << "KEY NAME " <<   keyHist->GetName() << endl;
	    
		    const char* histName = keyHist->GetName();
		    int numBin = lastBin - firstBin + 1;
		    TH1D* newHist = new TH1D(histName, histName, numBin, firstBin-1, lastBin);
		    reBinHistogram(hist, newHist, firstBin, lastBin);
		    
		    fileNew->cd();
		    newHist->SetDirectory(newTDirectory);
		    newTDirectory->cd();
		    newHist->Write();
		    newHist->Delete();
		    newTDirectory->cd("../");
		}
	    newTDirectory->Save();
	    newTDirectory->Delete();
	}	    
    fileNew->Close();
    fileNew->Delete();
    
    fileOriginal->Close();
    fileOriginal->Delete();
    
}



