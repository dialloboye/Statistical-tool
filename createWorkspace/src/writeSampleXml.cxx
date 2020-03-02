//===================Scripts to write xml template for each sample from each rootFile loan.truong@cern.ch======================


#include "include/writeSampleXml.h"


using namespace std;


void writeSampleXml(string rootFile, ofstream& writeFile, string type){//function to write out HistFactory syntax for each sample
    //Getting systematics names from TDirectories of the root file
    vector<string> vectorTDirectory;
    string nameDown, nameUp;
    if(type == "bkg"){
	nameDown = "bkg_down";
	nameUp = "bkg_up";
    }
    else if (type == "sig"){
	nameDown = "sig_down";
	nameUp = "sig_up";
    }
    
    TFile *f = new TFile(rootFile.c_str(),"READ");
    TIter next(f->GetListOfKeys());
    TKey *key;
     
    while ((key = (TKey*) next())) {//each systematic type is stored in a TDirectory
	string className = key->GetClassName();
	if (className != "TDirectoryFile") continue;
	TDirectory *dir = (TDirectory*) key->ReadObj();
	string dirName = dir->GetName();
	
	
	//if(dirName != "Nominal" && dirName != "STAT"){ //Loan: origional
	if(dirName != "Nominal" && dirName != "STAT" && dirName.find("UncorrUncertainty") == string::npos && dirName.find("CorrUncertainty") ==string::npos ){//Loan: to remove simplified model uncertainties
	     vectorTDirectory.push_back(dirName);
	     //cout << "dirName=" << dirName << endl;
	}
    }//end while key for making systematics vector

    int numUncerts = vectorTDirectory.size();
    
    //======================
    //Write down xml file
    //write down sample declaration

    if (numUncerts != 0){
	writeFile << "<Sample Name=\"" << type << "\" \t InputFile=\"" << rootFile << "\" \t HistoName=\"" << type << "\" \t HistoPath=\"STAT/\" \t NormalizeByTheory=\"True\" > \n";
	writeFile << " <StatError Activate=\"True\" /> \n";
	if(type=="sig")
	    writeFile << "    <NormFactor Name=\"mu\" Val=\"0.\" \t Low=\"0.\" \t High=\"0.5\" \t Const=\"False\"/> \n";
	
	
	for(size_t uncert=0; uncert<numUncerts; ++uncert){    //write down systematics
	    writeFile << "   <HistoSys Name=\"" << vectorTDirectory.at(uncert) << "\" \t HistoPathLow=\"" << vectorTDirectory.at(uncert) << "/\" \t HistoNameLow=\"" << nameDown << "\" \t HistoPathHigh=\"" << vectorTDirectory.at(uncert) << "/\" \tHistoNameHigh=\"" << nameUp << "\"/> \n";
	}//end of for uncerts
	
	writeFile <<"  </Sample> \n\n";
    }
    else{//bkg file where there are no TDirectoryFiles
	//BB data-driven background
	cout << "numUncerts = " << numUncerts << " for sample " << type << "\n";
	writeFile << "<Sample Name=\"bkgBB\" \t InputFile=\"" << rootFile << "\" \t HistoName=\"h_Mmumu_shape_avg_scaled\" \t NormalizeByTheory=\"False\" > \n";
	writeFile << " <StatError Activate=\"False\" /> \n";
	writeFile << "  <HistoSys Name=\"ATLAS_bb_sys\" \t HistoNameLow=\"h_Mmumu_shape_avg_scaled_dw\" \t HistoNameHigh=\"h_Mmumu_shape_avg_scaled_up\"/> \n";
	writeFile <<"</Sample> \n\n";


	//MC based bkg
	
        writeFile << "<Sample Name=\"bkgMCbased\" \t InputFile=\"" << rootFile << "\" \t HistoName=\"MC_based_bkg\" \t NormalizeByTheory=\"True\" > \n";
	writeFile << " <StatError Activate=\"False\" />";
	writeFile << "  <HistoSys Name=\"ATLAS_MCbased_sys\" \t HistoNameLow=\"hMCprompt_down\" \t HistoNameHigh=\"hMCprompt_up\"/> \n\n";	
	writeFile <<"</Sample> \n\n";
	
    }

}
