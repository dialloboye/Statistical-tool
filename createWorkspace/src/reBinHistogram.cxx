//====================Scripts to interpolate for different mass points=============================================

#include "include/reBinHistogram.h"

using namespace std;

void reBinHistogram(TH1D* hist, TH1D* newHist, int firstBin, int lastBin){

    for(int i = 0; i < lastBin-firstBin+1; ++i){
	newHist->SetBinContent(i+1,hist->GetBinContent(i+firstBin));
	//cout << newHist->GetBinContent(i+1) << endl;
	newHist->SetBinError(i+1,hist->GetBinError(i+firstBin));
    }
    cout << "Finish re binning histogram " << newHist->GetName() << "\n";
}
