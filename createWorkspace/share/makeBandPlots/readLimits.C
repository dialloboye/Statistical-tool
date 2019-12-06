
/* Script to read out limit values from results limit root-files
   bin1 obs
   bin2 exp
   bin3 2sigma
   bin4 1sigma
   bin5 -1 sigma
   bin6 -2sigma
The results will be written in text files, each correspond to a workspace
lotruong@cern.ch or ngamrong@gmail.com
August 06th, 2019.
*/
#include "readLimits.h"

using namespace std;

bool inVector(vector<string> &v, const string &item){
    vector<string>::iterator it = find(v.begin(), v.end(), item);
    if(it==v.end()){
        return false;
    }
    else return true;
}


string getDirectory()
{
    char cwd[1024];
    chdir("/path/to/change/directory/to");
    getcwd(cwd, sizeof(cwd));
    return cwd;
}


void readLimits(string storeFolder,
		string xSection,
		string region,
		vector<string> vecChannels,
		string particle,
		double firstMass, double lastMass, double stepSize,
		string nameTag){
    
    //example folder name: 2e2mu_mZd_16GeV
    string runDir = getDirectory();
    
    //Low mass declaration:
    vector<string> removeMass={"3", "3.5", "4", "9", "9.5", "10", "10.5", "11"};//Loan: remove mass points for lowmass
    
    //Common declaration:
    vector<string> mass;
    string extraString = "GeV";
    
    cout << "*** PROCESSING INPUT FROM " << storeFolder << " ***\n\n";
    
    //File to store all names of read out limit files
    ofstream fileList; fileList.open(("listReadOutFiles" + nameTag + ".txt").c_str());
    
    //Make mass vector
    for (double i=firstMass; i<=lastMass; i+=stepSize)
	{
	    stringstream sstr;
	    sstr << i;
	    string tmp = sstr.str();
	    
	    if ( firstMass < 15. && inVector(removeMass,tmp) )
		continue;
	    
	    mass.push_back(tmp);
	    //cout << tmp << "\n";
	}
    
    int numMassPoints = mass.size();
    cout << "numMassPoints = " << numMassPoints << "\n";
    
    for(Int_t chan = 0; chan < vecChannels.size(); ++chan){ // For limit on BR or on BR x XS Note starting from zero i-1
	//File to store the read out limits
	string name = "listLimit/MassVsLimit_" + xSection + "_" + particle + "_" + vecChannels.at(chan) + "_" + region +  ".txt";
	//cout << "name " << name << "\n";
	ofstream file; file.open(name);
	fileList << name << "\n";
	std::streamsize oldPres = file.precision(4);
	file.setf(ios::fixed, ios::floatfield);
	
	for(Int_t m = 0; m < numMassPoints; ++m){
	    string foldID = vecChannels.at(chan) + "_m" + particle + "_" + mass.at(m);
	    string limitRootFile= storeFolder + "/" + foldID + extraString + "/125.root";
	    
	    //cout << limitRootFile << endl;
	    
	    TFile *f = new TFile(limitRootFile.c_str(),"READ");
	    if(f->IsZombie())
		continue;
	    
	    TH1D* h;
	    h = (TH1D*)f->Get("limit");
	    //cout << h->GetBinContent(1) << endl;
	    
		cout.setf(ios_base::fixed, ios_base::floatfield);// cout.setf(ios::fixed);
		cout.precision(2);
		file << mass.at(m) << "\t" << h->GetBinContent(1) << "\t" << h->GetBinContent(2) << "\t" << h->GetBinContent(3) << "\t" << h->GetBinContent(4) << "\t"  << h->GetBinContent(5) << "\t"  << h->GetBinContent(6) << "\t"  << endl;
		
		h=NULL;
		delete h;
		f=NULL;
		delete f;
	}
	cout << "\n\n*** FINISH PROCESSING INPUT FROM FOLDER " << storeFolder << ", LIMITS ARE WRITTEN INTO FILE " << name << " ***\n\n\n";	    
	file.precision(oldPres);
	file.unsetf(std::ios::fixed);
	file.close();
    }
    
}//end the main process



