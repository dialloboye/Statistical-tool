//====================Scripts to interpolate for different mass points=============================================

#include "include/reBinHistogram.h"

using namespace std;

void getNonZeroBinRange(TH1D* hist, double threshold, int &firstBin, int &lastBin){
    int tmpFirstBin, tmpLastBin;
    for(int i=0; i < hist->GetNbinsX(); ++i){
	if (hist->GetBinContent(i) > threshold){
	    tmpFirstBin = i;
	    break;
	}
    }

    for(int i = hist->GetNbinsX(); i > 0; --i){
	if (hist->GetBinContent(i) > threshold){
	    tmpLastBin = i;
	    break;
	}
    }

    //Loan: currently change this to adapt lowmass input being troublesome
    if (tmpFirstBin < firstBin)
	firstBin = tmpFirstBin;

    if(tmpLastBin > lastBin)
	lastBin = tmpLastBin;

}




