#include "include/prepareSignal.h"

using namespace std;

void importHistFromArrayToTDirectory(TDirectoryFile* newTDirectory, TFile *fileNew, TObjArray *MyHistArray)
{ 
    newTDirectory->cd();
    //cout << "Import the new histograms" << endl;
    for (int i = 0; i < MyHistArray->GetEntriesFast(); i++)
	{
	    ( (TH1D*) MyHistArray->At(i) )->SetDirectory(newTDirectory);
	    MyHistArray->At(i)->Write();
	}
    newTDirectory->cd("../");	
}
