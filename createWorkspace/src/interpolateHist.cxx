//====================Scripts to interpolate for different mass points=============================================

#include "include/prepareSignal.h"

using namespace std;


void interpolateHist(TDirectoryFile* oldTDirectory,
		     TDirectoryFile* newTDirectory,
		     TFile* file,
		     double binFirst, /*=15 GeV*/
		     double binLast,  /*=60 GeV*/
		     double stepSize) /*=0.5 GeV*/
{//============================================
    /*
    This function is used to interpolate between MC generated mass points (in 5 GeV step)
    ==> to generate histograms for signal of the non-generated points in 0.5 GeV step.
    1. Get input: old TDirectory.
    2. Loop over histograms inside TDirectory:
        i. copy bin content-error at generated points.
        ii. make interpolation between bins in step (i.), extract error as it is.
        iii. Save information from (i.) and (ii.) into a new histogram
    3. Output: save new histograms into a new TDirectory, new TFile
    */
    
    int numBin = static_cast <int> ( (binLast - binFirst)/stepSize + 1 ); //Here numBin is actually the number of mass points so counting points
    string oldTDirectoryName = oldTDirectory->GetName();
    
    TIter next(oldTDirectory->GetListOfKeys());
    TKey* keyHist;

    string checkName[100];//for checking of no duplication in histograms
    checkName[0]="";
    int countHistName=1;
    
    //looping over all histograms in the TDirectory
    while( (keyHist = (TKey*) next() ) ){	

	string classHist = keyHist->GetClassName();
	if (classHist != "TH1D" && classHist != "TH1F")
	    continue;
	
	TH1D *h = (TH1D*) keyHist->ReadObj();
	const char *charHistName = keyHist->GetName();
	string strHistName = charHistName;

	//make sure not duplicating histograms
	checkName[countHistName]=strHistName;
	if ( checkName[countHistName] == checkName[countHistName-1] ) {
	    //	    cout << "Same Name" << endl;
	    continue;
	}
	++countHistName;
	//cout << strHistName << endl;
	
	//make a new histogram to store all the bin information, at both generated and interpolated
	TH1D *h_new = new TH1D(charHistName,
			       charHistName,
			       numBin,
			       binFirst - stepSize/2.,
			       binLast + stepSize/2.);
	//range from 14.75 to 60.25 with 91 bins for high mass, from 0.25 to 15.25 for low mass???

	bool doLowMass=binFirst<10.?true:false;
	
	if(doLowMass){
	    /*
	      Run 2 round 1 points:
	      double generatedPoints[] = {1., 2., 2.5, 5., 7., 15.};
	      double interPoints[] = {1.5, 5.5, 6., 6.5, 7.5, 12., 12.5, 13., 13.5, 14., 14.5};
	    
	      Run 2 round 2 points:
	      double generatedPoints[] = {1., 2., 2.5, 6., 8., 15.};
	      double interPoints[] = {1.5, 5., 5.5, 6., 6.5, 7., 7.5, 12., 12.5, 13., 13.5, 14., 14.5};
	      int numGenerated = sizeof(generatedPoints)/sizeof(*generatedPoints);
	      int numInter = sizeof(interPoints)/sizeof(*interPoints);
	    */
	    
   	    int binX, genBin;
	    //Because low mass histograms have 0-content bins so one has to extract them to a graph and do interpolation to not get interfered with 0 bins
	    TGraph gValue;
	    TGraph gError;
	    for(int i=1;i<=h->GetNbinsX();i++) {
		if(h->GetBinContent(i)){
		    gValue.SetPoint(gValue.GetN(),h->GetBinCenter(i),h->GetBinContent(i));
		    gError.SetPoint(gError.GetN(),h->GetBinCenter(i),h->GetBinError(i));
		}
	    }
	    string graphValName = "graph" + strHistName;
	    string graphErrName = "graphErr" + strHistName;
	    gValue.SetName(graphValName.c_str());
	    gError.SetName(graphErrName.c_str());
	    
	    //TH1D *h_new = new TH1D(hist_name,hist_name,nBin,firstBin,lastBin);
	    double mass, genPointX, interPointX;
	    
	    for(int bin=1; bin<=numBin; ++bin){
		mass = h_new->GetBinCenter(bin);

		h_new->SetBinContent(bin,gValue.Eval(mass));
		h_new->SetBinError(bin,gError.Eval(mass));

		/*
		if(mass==15.){
		    cout << "mass=" << mass << ", val=" << gValue.Eval(mass) << endl;
		}
		*/
	    }
	}
	else{//for high-mass case
	    for (int i=1; i<=numBin; ++i)
		{
		    //X-coordinate of precedented generated mass point. Step size is 0.5 GeV
		    double positionX = binFirst + (i-(i%10))*stepSize; //Modulo 10 because samples are generated at 5 GeV step, divided by interpolation step size of 0.5 GeV. All mass points from 15->19.5 must share the same precedented generated mass point which is 15 GeV.
		    
		    //bin number of the precedented generated mass point
		    int binX = h->GetXaxis()->FindBin(positionX);
		    
		    //set bin error as the same as the precedented generated mass point
		    h_new->SetBinError(i,h->GetBinError(binX));
		    //cout << "Bin " << i << " of histo " << charHistName << " has bin error " << h_new->GetBinError(1) << "\n";
		    
		    if( (i % 10) == 1 )
			{	//at generated mass points so just get values
			    h_new->SetBinContent(i,h->GetBinContent(binX));
			}
		    else
			{ //at interpolated mass points, must interpolate to get values
			    double interPoint = binFirst + (i-1)*stepSize;
			    h_new->SetBinContent(i,h->Interpolate(interPoint));
			}
		}
	}//end else !doLowMass
	h_new->SetDirectory(newTDirectory);
	newTDirectory->cd();
	h_new->Write();
	h_new->Delete();
	//	cout << endl;
	
	h->Delete();
	newTDirectory->cd("../");
    };
}

