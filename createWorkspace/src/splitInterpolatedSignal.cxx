#include "include/prepareSignal.h"
#include "include/common.h"

using namespace std;

void splitInterpolatedSignal(std::string& rootFileName,
			     std::ofstream& outFileSplitSignal,
			     std::string& channel,
			     std::string& particle,
			     double binFirst,    /*GeV*/
			     double binLast,     /*GeV*/
			     double stepSize)    /*GeV*/
    
{
    //=======================================================
    /*
      Get access into each directory 
      1. create a list of histograms
      2. make new histograms for each bin
      3. remember to extract the bin errors
    */
    int numBin = static_cast <int> ( (binLast - binFirst)/stepSize + 1 ); //Here numBin is actually the number of mass points so counting points
    
    //Make a vector of new root file names with different mass points
    vector<string> removeMass={"3", "3.5", "4", "9", "9.5", "10", "10.5", "11"};
    string newFileName[numBin];
    vector<string> massPoint; 
    for(double i=binFirst; i<=binLast; i+=stepSize){
	stringstream ss;
	ss << i;
	string strMassPoint = ss.str();
	massPoint.push_back(strMassPoint);
    }

    
    for(int i=0; i < massPoint.size(); ++i){
	newFileName[i] = rootFileName.substr(0, rootFileName.find_last_of("/") ) + "/signal_" + channel + "channel" + "_m" + particle + massPoint.at(i) + "GeV.root";
	/*
	cout << "path = " << rootFileName.substr(0, rootFileName.find_last_of("/") )<< "\n";
	cout << "channel " << channel << ", particle: " << particle << "\n";
	cout << "new File name is: " << newFileName[i] << endl;
	*/
	if ( particle=="a" && inVector(removeMass,massPoint.at(i) ) )
	    continue;
	outFileSplitSignal << newFileName[i] << "\n";
    }
    
    TFile *fileOriginal = new TFile(rootFileName.c_str(),"READ"); 
    TKey *keyDir,*keyHist;
    TDirectoryFile *dir;
    string classDir, classHist;
    
    for(int countBin=1; countBin<=massPoint.size(); ++countBin)
	{
	    if ( particle=="a" && inVector(removeMass,massPoint.at(countBin-1) ) )
		continue;

	    TIter next(fileOriginal->GetListOfKeys());

	    TFile *fileNew = new TFile(newFileName[countBin-1].c_str(),"RECREATE");
	    //	    cout << "making " << newFileName[countBin-1] << endl;
	    //loop over directories
	    while ( (keyDir = (TKey*) next() ) )
		{
		    classDir = keyDir->GetClassName();
		    if (classDir != "TDirectoryFile") continue;
		    dir = (TDirectoryFile*) keyDir->ReadObj();
		    const char *tdirectoryName = dir->GetName();
		    //		    cout << "Processing directory " << tdirectoryName << endl;
		    
		    TDirectoryFile *newTDirectory = new TDirectoryFile(tdirectoryName,tdirectoryName,"",0);
		    //cout << "Processing bin " << countBin << endl;
		    importOneBinHist(dir, newTDirectory, fileNew, countBin);
		    
		    newTDirectory->Save();
		    newTDirectory->Delete();
		}
	    
	    fileNew->Close();
	    fileNew->Delete();
	}
    
    fileOriginal->Close();
    fileOriginal->Delete();
    
}



