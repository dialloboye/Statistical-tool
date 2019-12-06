#include "include/prepareSignal.h"
#include "Math/ProbFuncMathCore.h" //necessary to use MathCore functions such as gaussian
#include "Math/QuantFuncMathCore.h" //necessary to use MathCore functions such as gaussian_cdf


using namespace std;

/*
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  1. Fiducial cross section is determined by: 
  -------------------
  Nreco(c) / Nfid(c) = XSreco(c) / XSfid(c)
  <=>  efficiency(c) = XSreco(c) / XSfid(c)
  Therefore in HistFactory frame work, replacing number of events at reco level:
  Nreco = XSreco x Lumi = efficiency x Lumi x XSfid
  Nexpected(c) = Nbkg_reco(c) + XSfid(c) x efficiency(c) x Lumi
  -------------------
  where:
  -efficiency = Nreco/Nfid, each term is determined for each channel c.
  -Nreco is the expected yield in the fiducial volume, computed using all reweighting and efficiency scale factors.
  -Nfid is the expected yield in the fiducial volume, computed using just generator-level event weights.
  This definition of efficiency therefore includes impurities: contribution to the numerator from events outside of the fiducial volume. For the selections and fiducial definition in this analysis, the level of impurity was checked and found to be negligible.
  
  ==>We just need to INPUT:
  -efficiency x Luminosity for signal, 
  -yield at reco level for bkg, 
  -then the parameter of interest will be XSfid
  
  =======================================
  
  
  ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
  2. Total cross section is determined by:
  --------------------
  acceptance(c,X) = Nfid(c,X) / Ntot(X) 
  Nexpected = Nbkg + XStot x  Lumi x Sum( α(c,X) x efficiency(c,X) ) .
  --------------------
  where:
  -acceptance is for a certain channel c, model of particle X.
  -Ntot is calculated for all the channels, model of particle X.
  
  ==>We just need to INPUT:
  -[sum of (efficiency x acceptance)] x Luminosity for signal
  -yield at reco level for bkg in all channels
  -then the parameter of interest will be XStot
  
  ======================================
  
*/

void makeGaussianHist(TH1D* hist,
		      double norm,
		      double errNorm,
		      double mean,
		      double sigma,
		      double lumi)
{
    for (int i=1 ; i<= hist->GetNbinsX() ; i++ )
	{
	    hist->SetBinContent(i, lumi*norm*(ROOT::Math::gaussian_cdf(hist->GetXaxis()->GetBinUpEdge(i), sigma, mean)-ROOT::Math::gaussian_cdf(hist->GetXaxis()->GetBinLowEdge(i), sigma, mean) ) );
	    hist->SetBinError(i, lumi*errNorm*(ROOT::Math::gaussian_cdf(hist->GetXaxis()->GetBinUpEdge(i), sigma, mean)-ROOT::Math::gaussian_cdf(hist->GetXaxis()->GetBinLowEdge(i), sigma, mean) ) );
	}// Will Buttinger's way when binning is not 1 GeV
    
     cout << "mean=" << mean << ", sigma=" << sigma << ", norm=" << norm  << ", errNorm = " << errNorm << endl;
     cout << "Integral=" <<	hist->Integral() << endl;
}
