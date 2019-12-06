#include "include/prepareSignal.h"

using namespace std;

void interpolateSignal(string& rootFile,
		       ofstream& myFileInterpolatedSignal,
		       double binFirst, /*=15 GeV*/
		       double binLast,  /*=60 GeV*/
		       double stepSize) /*=0.5 GeV*/ /* = 91 */
		       
{
    //=========================================//
    /*
      1. Get access into each directory then create a list of histograms,
      2. Make new histograms for each bin 
      3. Extract the bin errors as well
    */

    
    TFile *fileOriginal = new TFile(rootFile.c_str(),"READ"); 
    TKey *keyDir,*keyHist;
    TDirectoryFile *dir;
    string classDir, classHist;
    string newName = rootFile.substr( 0, rootFile.find_last_of("/") + 1 ) + "interpolated_" + rootFile.substr( rootFile.find_last_of("/")+1, rootFile.length() - rootFile.find_last_of("/") );
    
    TIter next(fileOriginal->GetListOfKeys());
    TFile *fileNew = new TFile(newName.c_str(),"RECREATE");

    //loop over directories
    while ( (keyDir = (TKey*) next() ) )
	{
	    classDir = keyDir->GetClassName();
	    if (classDir != "TDirectoryFile") continue;
	    dir = (TDirectoryFile*) keyDir->ReadObj();
	    const char *dir_name = dir->GetName();
	    //	    cout << "Processing directory " << dir_name << endl;
	    
	    TDirectoryFile *newDir = new TDirectoryFile(dir_name,dir_name,"",0);
	    
	    interpolateHist(dir, newDir, fileNew, binFirst, binLast, stepSize);
	    
	    newDir->Save();
	    newDir->Delete();
	}
    
    fileNew->Close();
    fileNew->Delete();
    
    fileOriginal->Close();
    fileOriginal->Delete();
    
    myFileInterpolatedSignal << newName << "\n";
}

