#include "include/prepareSignal.h"

using namespace std;

void importOneBinHist(TDirectoryFile* oldTDirectory,
		      TDirectoryFile* newTDirectory,
		      TFile* newFile,
		      int atBin,
		      int numBin)
{ 
    string oldTDirectoryName = oldTDirectory->GetName();
    TIter next(oldTDirectory->GetListOfKeys());
    TKey* keyHist;
    
    
    string checkName[100];
    checkName[0]="";
    int countHistName=1;
    
    while( (keyHist = (TKey*) next() ) )
	{
	    string classHist = keyHist->GetClassName();
	    if(classHist != "TH1D" && classHist != "TH1F") continue;
	    
	    TH1D *h = (TH1D*) keyHist->ReadObj();
	    const char *charHistName = keyHist->GetName();
	    string strHistName = charHistName;
	    
	    checkName[countHistName]=strHistName;
	    
	    if (checkName[countHistName] == checkName[countHistName-1]) {
		//		cout << "Samename" << endl;
		continue;
	    }
	    ++countHistName;
	    //	    cout << strHistName << endl;
	    
	    TH1D *h_new = new TH1D(charHistName,charHistName,numBin,0,numBin);
	    
	    h_new->SetBinContent(1,h->GetBinContent(atBin));
	    //	    cout << "Finish setbincontent \n";
	    h_new->SetBinError(1,h->GetBinError(atBin));
	    //	    cout << "Finish setbinerror \n";

	    h_new->SetDirectory(newTDirectory);
	    //	    cout << "Finish setDirectory \n";
	    
	    newTDirectory->cd();
	    h_new->Write();
	    h_new->Delete();
	    //	    cout << endl;
	    h->Delete();
	    newTDirectory->cd("../");

	};
    //    cout << "finish import hist \n";
}

