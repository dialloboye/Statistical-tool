#include "include/prepareSignal.h"
#include "include/common.h"

using namespace std;

void splitDiscreteSignal(std::string& rootFileName,
			 std::ofstream& outFileSplitSignal,
			 std::string& channel,
			 std::string& particle)
			 // double binFirst,    /*GeV*/
			 // double binLast,     /*GeV*/
			 // double stepSize)    /*GeV*/
    
{
    //=======================================================
    /*
      Get access into each directory 
      1. create a list of histograms
      2. make new histograms for each bin
      3. remember to extract the bin errors
    */
        /* Old pairing
    map <int, pair<int,int> > map_mHmZd_vs_Bin;
    
    //(mzd, ms ) = (20,175), (50,175), (50,400), (65,750), (80,325), (100, 400), (150,750), (250,800) (70, 250)
    pair <int,int> mHmZd;
    vector<int> mH {175, 175, 400, 750, 325, 400, 750, 800, 250};
    vector<int> mZd {20, 50, 50, 65, 80, 100, 150, 250, 70};
    */

    /* New Pairing, have to check which one is correct */
    map <int, pair<string,string> > map_mHmZd_vs_Bin;
    //(mzd, ms ) = (20,175), (50,175), (50,400), (65,750), (70, 250), (70, 250 NW), (80,325), (100, 400), (150,750), (250,800)
    pair <string,string> mHmZd;
    vector<string> mH {"175", "175", "400", "750", "250", "250NW", "325", "400", "750", "800"};
    vector<string> mZd {"20", "50", "50", "65", "70", "70", "80", "100", "150", "250"};
    for (int m=0; m< mH.size(); ++m){
	mHmZd = make_pair( mH.at(m), mZd.at(m) );
	map_mHmZd_vs_Bin[m] = mHmZd;
    }

    //mHmZd should replace massPoint vector
    int numBin = mH.size();
    
    //Make a vector of new root file names with different mass points
    /*
    vector<string> removeMass={"3", "3.5", "4", "9", "9.5", "10", "10.5", "11"};
    string newFileName[numBin];
    vector<string> massPoint; 
    for(double i=binFirst; i<=binLast; i+=stepSize){
	stringstream ss;
	ss << i;
	string strMassPoint = ss.str();
	massPoint.push_back(strMassPoint);
    }
    */

    //Pay attention that if you wanna remove some mass points then numBin must be reduced accordingly
    string newFileName[numBin];
    for(int i=0; i < numBin; ++i){
	mHmZd = map_mHmZd_vs_Bin[i];
	newFileName[i] = rootFileName.substr(0, rootFileName.find_last_of("/") ) + "/signal_" + channel + "channel" + "_m" + particle + mHmZd.second + "_mH" + mHmZd.first + "GeV.root";
	//	newFileName[i] = rootFileName.substr(0, rootFileName.find_last_of("/") ) + "/signal_" + channel + "channel" + "_m" + particle + to_string(mHmZd.second) + "_mH" + to_string(mHmZd.first) + "GeV.root";
	
	cout << "path = " << rootFileName.substr(0, rootFileName.find_last_of("/") )<< "\n";
	cout << "channel " << channel << ", particle: " << particle << "\n";
	cout << "new File name is: " << newFileName[i] << endl;
	
	// if ( particle=="a" && inVector(removeMass,mH.at(i) ) )
	//     continue;
	outFileSplitSignal << newFileName[i] << "\n";
    }
    
    TFile *fileOriginal = new TFile(rootFileName.c_str(),"READ"); 
    TKey *keyDir,*keyHist;
    TDirectoryFile *dir;
    string classDir, classHist;
    
    for(int countBin=1; countBin<=numBin; ++countBin)
	{
	    // if ( particle=="a" && inVector(removeMass,mH.at(countBin-1) ) )
	    // 	continue;
	    
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



