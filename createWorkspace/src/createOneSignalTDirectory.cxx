#include "include/prepareSignal.h"

using namespace std;

void createOneSignalTDirectory(TDirectoryFile* newTDirectory,
			       TFile *fileNew,
			       string& suffix,
			       map<string,double> mapInput,
			       vector<string> vec_strBaseHistName, //= { "sig", "eff", "mean", "sigma"},
			       double binFirst,
			       double binLast,
			       double binWidth,
			       double lumi)
{
    int numBin = static_cast <int> ( (binLast - binFirst)/binWidth); //Here numBin is the # of intervals
    TH1D *sig = new TH1D( (vec_strBaseHistName.at(0) + suffix).c_str(), (vec_strBaseHistName.at(0) + suffix).c_str(), numBin, binFirst, binLast);
    makeGaussianHist(sig, mapInput[vec_strBaseHistName.at(1) + suffix], mapInput[vec_strBaseHistName.at(1) + suffix + "_err"], mapInput[vec_strBaseHistName.at(2) + suffix], mapInput[vec_strBaseHistName.at(3) + suffix], lumi );
    TObjArray *histArray = new TObjArray(0);
    histArray->AddLast(sig);
    importHistFromArrayToTDirectory(newTDirectory, fileNew, histArray);
    sig->Delete();
}    
