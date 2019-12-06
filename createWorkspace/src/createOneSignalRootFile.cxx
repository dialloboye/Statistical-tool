#include "include/prepareSignal.h"
#include "include/common.h"

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

void createOneSignalRootFile(string &xSection,
			     std::ofstream& outFileGaussianModel,
			     string& rootFileName,
			     double binFirst,// = 0. GeV,
			     double binLast,// = 64. GeV
			     double binWidth,// = 1. GeV
			     double lumi,
			     string fileType)  // = "efficiency" or "acceptance"
{
    /*  
	The binning here is for the <mll> distribution.
	It's different from the binning for the generated Zd mass points (used in interpolation and splitting).
	1. Loop over TDirectoryFiles
        2. Extract information of a Gaussian signal model (including normalization-which is efficiency, mean, standard deviations).
	3. Make new histograms of signal with Gaussian model for each directory. 
	4. Save new histograms to a new directory which has the same name to the old directory, then save to a new file
    */
    string acceptanceFile = rootFileName;
    replace(acceptanceFile, "efficiency", "acceptance");
    //    cout << "acceptance File is " << acceptanceFile << "\n";
    
    double acc = 1.;
    double errAcc = 0. ;
    
    if(fileType == "acceptance")
	{
	    TFile *fileAcc = new TFile(acceptanceFile.c_str(),"READ");
	    TH1D *hAcc = (TH1D*) fileAcc->Get("Nominal/acc");
	    if (!hAcc) cout << "Acceptance histogram NOT FOUND \n";
	    acc = hAcc->GetBinContent(1);
	    errAcc = hAcc->GetBinError(1);
	    fileAcc->Close();
	}
    //cout << "Outside acceptance loop \n";
	    
    string newFileName;
    int rootFileNameLength = rootFileName.length();
    int positionLastSlash = rootFileName.find_last_of("/");

    string fullPath = rootFileName.substr(0, positionLastSlash );
    replace(fullPath,"efficiency",xSection);
    system( ("mkdir -vp " + fullPath).c_str() );

    newFileName = fullPath + "/" + "gaussian" + rootFileName.substr(positionLastSlash+1, rootFileNameLength - positionLastSlash -1 );
    //newFileName = rootFileName.substr(0, positionLastSlash ) + "/" + "gaussian" + rootFileName.substr(positionLastSlash+1, rootFileNameLength - positionLastSlash -1 );
    
    //cout << "new File Name is " << newFileName <<"\n";
    outFileGaussianModel << newFileName << "\n";
    
    TFile *fileOriginal = new TFile(rootFileName.c_str(),"READ"); 
    TKey *keyDir1, *keyDir,*keyHist1, *keyHist;
    TDirectoryFile *dir;
    string classDir, classHist;
    string strTmpDirName;
    
    TIter nextDir1(fileOriginal->GetListOfKeys());
    TIter nextDir(fileOriginal->GetListOfKeys());
    
    TFile *fileNew = new TFile(newFileName.c_str(),"RECREATE");

    vector<string> vec_strBaseHistName = { "sig", "eff", "mean", "sigma"};
    vector<string> vec_strVariation = {"_up", "_down"};
    map<string, double> mapInput;
    bool isVariation = false;


    //Loan: Remove the below part when input is fixed. (the part between /*===TO BE REMOVED ONCE FIXED INPUT===*/)
    //Loan: temporarily loop over TDirectory twice, once get new input, loop over TDirectoryFiles only once
    //Get STAT to make sure mapInput will be filled for all cases of TDirectoryFiles

    /*===TO BE REMOVED ONCE FIXED INPUT= BEGIN ==*/
    while ( (keyDir1 = (TKey*) nextDir1() ) )
	{
	    classDir = keyDir1->GetClassName();
	    if (classDir != "TDirectoryFile") continue;
	    dir = (TDirectoryFile*) keyDir1->ReadObj();
	    const char *charDirName = dir->GetName();
	 
	    //Loan: temporarily find STAT first to have mean, sigma for all cases (have to modify input later to make sure all TDirectory Files have it)
	    if(string(charDirName).find("STAT") == string::npos)
	 	continue; //Loan check STAT firstly
	    
	    //cout << "Processing directory " << charDirName << endl;
	    
	    //the last one is to be created, remember to -1 for the size to loop over
	    TIter nextHist1(dir->GetListOfKeys());
	    //cout << "1 \n";
	    //loop over histograms inside the TDirectory
	    while ( (keyHist1 = (TKey*) nextHist1() ) )

		{
		    //cout << "2 \n";
		    classHist = keyHist1->GetClassName();
		    if (classHist != "TH1D" && classHist != "TH1F")
			continue;
		    //cout << "3 \n";
		    TH1D* hist = (TH1D*) keyHist1->ReadObj();
		    //cout << "4 \n";
		    const char *charHistName = hist->GetName();

		    if (string(charHistName).find("eff") != string::npos)
			{
			    //cout << "5 \n";
			    mapInput[string(charHistName)] = hist->GetBinContent(1) * acc;
			    mapInput[string(charHistName)+"_err"] = hist->GetBinContent(1) * errAcc + hist->GetBinError(1) * acc;
			    // eff*err_acc + err_eff*acc
			    //cout << "Error on eff is " << mapInput[string(charHistName)+"_err"] << "\n";
			}
		    else
			{
			    mapInput[string(charHistName)] = hist->GetBinContent(1);
			    mapInput[string(charHistName)+"_up"] = hist->GetBinContent(1);

			    mapInput[string(charHistName)+"_down"] = hist->GetBinContent(1);
			    // cout << "6 \n";
			}
		    //		    cout << "map at " << charHistName << " = " << mapInput[string(charHistName)] << "\n";
		    hist->Delete();
		    //cout << "7 \n";
		}
	    
	}//end 1st while over TDirectoryFiles but only STAT to fill all mapInput (so the values will be used in case there are no values in next loop
    
    /* ===TO BE REMOVED ONCE FIXED INPUT= END ==*/
  
    
    //loop over TDirectories
    while ( (keyDir = (TKey*) nextDir() ) )
	{
	    classDir = keyDir->GetClassName();
	    if (classDir != "TDirectoryFile") continue;
	    dir = (TDirectoryFile*) keyDir->ReadObj();
	    const char *charDirName = dir->GetName();


	    // if(string(charDirName).find("Nominal") == string::npos)
	    // 	continue; //Loan check Nominal firstly
	    
	    //cout << "Processing directory " << charDirName << endl;
	    
	    
	    //the last one is to be created, remember to -1 for the size to loop over
	    
	    TIter nextHist(dir->GetListOfKeys());
	    
	    //loop over histograms inside the TDirectory
	    while ( (keyHist = (TKey*) nextHist() ) )
		{
		    classHist = keyHist->GetClassName();
		    if (classHist != "TH1D" && classHist != "TH1F")
			continue;
		    TH1D* hist  = (TH1D*) keyHist->ReadObj();
		    const char *charHistName = hist->GetName();
		    
		    if (string(charHistName).find("_") != string::npos)
			isVariation = true;//For the case of not having mean, sigma, just use the ones available (from other TDirectory)
		    else
			isVariation = false;//If Nominal || STAT
		    
		    if (string(charHistName).find("eff") != string::npos)
			{
			    mapInput[string(charHistName)] = hist->GetBinContent(1) * acc;
			    mapInput[string(charHistName)+"_err"] = hist->GetBinContent(1) * errAcc + hist->GetBinError(1) * acc;
			    // eff*err_acc + err_eff*acc
			    //cout << "Error on eff is " <<  mapInput[string(charHistName)+"_err"] << "\n";
			}
		    else
			mapInput[string(charHistName)] = hist->GetBinContent(1);

		    //cout << "map at " << charHistName << " = " << mapInput[string(charHistName)] << "\n";
		    hist->Delete();
		}
	    
	    
	    
	    TDirectoryFile *newTDirectory = new TDirectoryFile(charDirName,charDirName,"",0);
	    if (isVariation)
		for(int i = 0; i < vec_strVariation.size(); ++i)
		    createOneSignalTDirectory(newTDirectory,
					      fileNew,
					      vec_strVariation.at(i),
					      mapInput,
					      vec_strBaseHistName, //= { "sig", "eff", "mean", "sigma"},
					      binFirst,
					      binLast,
					      binWidth,
					      lumi);
	    else
		{
		    std::string emptyStr("");
		    //cout << "emptyStr = " << emptyStr << "\n";
		    createOneSignalTDirectory(newTDirectory,
					      fileNew,
					      emptyStr,
					      mapInput,
					      vec_strBaseHistName, //= { "sig", "eff", "mean", "sigma"},
					      binFirst,
					      binLast,
					      binWidth,
					      lumi);
		    newTDirectory->Save();
		    newTDirectory->Delete();
		}
	}
    //    cout << "Finish directory " << strTmpDirName << endl << endl;
    
    fileNew->Close();
    fileNew->Delete();
    
    fileOriginal->Close();
    fileOriginal->Delete();
}

