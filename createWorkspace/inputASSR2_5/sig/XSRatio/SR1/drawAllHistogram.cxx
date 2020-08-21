void drawEachFile(string rootFileName);

void drawAllHistogram(){
    //=======================================================
    /*
      Get access into each directory 
      1. create a list of histograms
      2. make new histograms for each bin
      3. remember to extract the bin errors
    */

    //Make a vector of new root file names with different mass points

    string rootFileName;
    ifstream inFile;
    inFile.open("listFile");
    if(inFile.is_open() )
        {
            while( getline(inFile, rootFileName) )
                {
		    cout << rootFileName << endl;
		    drawEachFile(rootFileName);
		}
	    
        }

}

void drawEachFile(string rootFileName){
    TFile *fileOriginal = new TFile(rootFileName.c_str(),"READ");
    
    TKey *keyDir,*keyHist;
    TDirectoryFile *dir;
    string classDir, classHist;
    
    TIter next(fileOriginal->GetListOfKeys());
    TCanvas c;
    string saveName, tmp;
    while ( (keyDir = (TKey*) next() ) )
	{
	    classDir = keyDir->GetClassName();
	    if (classDir != "TDirectoryFile") continue;
	    dir = (TDirectoryFile*) keyDir->ReadObj();
	    string tdirectoryName = dir->GetName();
	    
	    // if(tdirectoryName != "Nominal") continue;//Loan: this is for current low mass due to problematic input
	    
	    TIter nextHist(dir->GetListOfKeys());
	    while( (keyHist = (TKey*) nextHist() ) )
		{
		    classHist = keyHist->GetClassName();
		    if(classHist != "TH1D" && classHist != "TH1F") continue;
		    
		    TH1D* hist = (TH1D*) keyHist->ReadObj();
		    const char* histName =  keyHist->GetName();// hist->GetName();

		    //cout << "Histo " << histName << "\n";
		    c.cd();
		    hist->Draw();
		    tmp = rootFileName.substr(rootFileName.find_last_of("/")+1, rootFileName.find_last_of(".")-rootFileName.find_last_of("/")-1);
		    //cout << tmp << endl;
		    saveName = "plots/" + tmp + "-" +  tdirectoryName + "-" + histName + ".png";
		    c.SaveAs(saveName.c_str());
		    hist->Delete();
		}
	}
    
    fileOriginal->Close();
    fileOriginal->Delete();
    
}



