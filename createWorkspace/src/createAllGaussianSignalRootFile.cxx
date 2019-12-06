#include "include/prepareSignal.h"

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

void createAllGaussianSignalRootFile(string &xSection,
				     std::ofstream &outFileGaussianModel,
				     double binFirst,//0.93 //0.
				     double binLast,//19.93 //64.
				     double binWidth,//0.1 // 1. GeV
				     double lumi,
				     std::string fileType) // default is "efficiency"
				    
{
    string inFileName = "listSplitSignalefficiency";
    ifstream listFile;
    listFile.open(inFileName.c_str());
    
    string rootFileName;
    
    if (listFile.is_open())
        while (getline(listFile, rootFileName)){
	    //cout << "Input for Gaussian model is: " << rootFileName << "\n";
	    createOneSignalRootFile(xSection,
				    outFileGaussianModel,
				    rootFileName,
				    binFirst,
				    binLast,
				    binWidth,
				    lumi,
				    fileType);
	}
    listFile.close();
}

