//====================Scripts to interpolate for different mass points=============================================

#include "include/scalingHist.h"

using namespace std;

void scalingHist(TFile* oldFile, TFile* newFile, string channel)
    
{
    //============================================
    
    map<string,double> channelSF;
    channelSF["4mu"] = 0.25;
    channelSF["4e"] = 0.25;
    channelSF["2e2mu"] = 0.5;
    
    TKey* keyDir;
    TKey* keyHist;
    string classDir, classHist;
    
    TIter nextDir(oldFile->GetListOfKeys());
    while ( (keyDir = (TKey*) nextDir() ) )
    	{
    	    classDir = keyDir->GetClassName();
    	    if (classDir != "TDirectoryFile")
    		continue;
	    
	    TDirectoryFile *dir;
    	    dir = (TDirectoryFile*) keyDir->ReadObj();
    	    const char *tdirectoryName = dir->GetName();
	    //	    cout << "Processing directory " << tdirectoryName << endl;

	    TDirectoryFile *newTDirectory = new TDirectoryFile(tdirectoryName,tdirectoryName,"",0);
	    string checkName[100];//for checking of no duplication in histograms
	    checkName[0]="";
	    int countHistName=1;
	    
	    TIter nextHist( dir->GetListOfKeys() );
	    //looping over all histograms in the TDirectory
	    while( (keyHist = (TKey*) nextHist() ) ){	
		
		classHist = keyHist->GetClassName();
		if (classHist != "TH1D" && classHist != "TH1F")
		    continue;
		
		TH1D *h = (TH1D*) keyHist->ReadObj();
		const char *histName = keyHist->GetName();
		string hName = histName;
		
		//make sure not duplicating histograms
		checkName[countHistName]=hName;
		if ( checkName[countHistName] == checkName[countHistName-1] ) {
		    //		    cout << "Same Name" << endl;
		    continue;
		}
		++countHistName;
		//		cout << hName << endl;
		//cout << "orig intergal " << h->Integral() << "\n";
		
		//scaling histograms based on channel
		h->Scale(channelSF[channel]);
		//cout << "rescaled intergral " << h->Integral() << "\n";

		
		h->SetDirectory(newTDirectory);
		newFile->cd();
		newTDirectory->cd();
		h->Write();
		h->Delete();
		
	    };
	    newTDirectory->Save();
	    newTDirectory->cd("../");
	    dir->Delete();
	    newTDirectory->Delete();
	}
}



