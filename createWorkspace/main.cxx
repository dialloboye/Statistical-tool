//===================Scripts to write xml files from each rootFile loan.truong@cern.ch======================

#include "include/makeXmlFiducialXSFiles.h"
#include "include/makeXmlTotalXSFiles.h"
#include "include/prepareSignal.h"
#include "include/scalingHist.h"
#include "include/common.h"
#include "include/reBinHistogram.h"

#include <vector>

#define CONFIG_SIZE (256)
#define F_ACCEPTANCE (1)
#define F_EFFICIENCY (1)
#define PATHINPUT (1)
#define REGION (1)
#define FIRSTBIN (1)
#define LASTBIN (1)
#define STEPSIZE (1)

#define FBIN (1)
#define LBIN (1)
#define BINWIDTH (1)

#define XSECTION (1)
#define PARTICLE (1)
#define LUMI (1)
#define CMS (1)
#define OBS (1)
#define NARROW (1) 

using namespace std;

typedef struct config
{
    unsigned set;
    char fEfficiency[CONFIG_SIZE];
    char fAcceptance[CONFIG_SIZE];
    char pathInput[CONFIG_SIZE];
    char region[CONFIG_SIZE];
    double firstBin;
    double lastBin;
    double stepSize;

    double fBin;
    double lBin;
    double binWidth;

    char xSection[CONFIG_SIZE];
    char particle[CONFIG_SIZE];
    double lumi;
    char cms[CONFIG_SIZE];
    int obs;
    int narrow;
    
} CONFIG;

void haddSignalFileForTotalXS();

string getWorkDirectoryPath();

void interpolateFile(string inFileSignalName, double firstBin, double lastBin, double stepSize, string fileType = "efficiency");
void splitFile(string particle, double firstBin, double lastBin, double stepSize, string fileType = "efficiency");//for continuous interpolation
void splitFile(string inFileName, string particle, string fileType = "efficiency");//for discrete mass points
void makeGaussianModelFile(string xSection, double fBin, double lBin, double binWidth, double lumi, string fileType = "efficiency");
void xmlFile(string pathInput, string particle, string xSection, string region, bool doFiducialXS = true, bool doNarrow = false);
void fiducialXS(string fileE, string pathInput, string xSection, string region, string particle, double lumi, string cms, double firstBin, double lastBin, double stepSize, double fBin, double lBin, double binWidth, int obs, int narrow);
void totalXS(string fileE, string fileA, string pathInput, string xSection, string region, string particle, double lumi, string cms, double firstBin, double lastBin, double stepSize, double fBin, double lBin, double binWidth, int obs, int narrow);
void muLimit(string fileE, string fileA, string pathInput, string xSection, string region, string particle, double lumi, string cms, double fBin, double lBin, double binWidth, int obs, int narrow);
void scalingSignalForTotalXS();
void narrowFile(double fBin, double lBin, double binWidth, string pathInput, string region, string particle);

// Parse the buffer for config info. Return an error code or 0 for no error.
int parse_config(char *buf, CONFIG *config)
{
    char dummy[CONFIG_SIZE];
    if (sscanf(buf, " %s", dummy) == EOF) return 0; // blank line
    if (sscanf(buf, " %[#]", dummy) == 1) return 0; // comment
    
    if (sscanf(buf, " fEfficiency = %s", config->fEfficiency) == 1) {
	if (config->set & F_EFFICIENCY) return F_EFFICIENCY; // error; fAcceptance already set
	config->set |= F_EFFICIENCY;
	return 0;
    }

    if (sscanf(buf, " fAcceptance = %s", config->fAcceptance) == 1) {   
	if (config->set & F_ACCEPTANCE) return F_ACCEPTANCE; // error; fEfficiency already set
	config->set |= F_ACCEPTANCE;
	return 0;
    }

    if (sscanf(buf, " pathInput = %s", config->pathInput) == 1) {
	if (config->set & PATHINPUT) return PATHINPUT; // error; pathInput already set
	config->set |= PATHINPUT;
	return 0;
    }
    
    if (sscanf(buf, " region = %s", config->region) == 1) {
	if (config->set & REGION) return REGION; // error; region already set
	config->set |= REGION;
	return 0;
    }
    
    if (sscanf(buf, " firstBin = %lf", &config->firstBin) == 1) {
	if (config->set & FIRSTBIN) return FIRSTBIN; // error; firstBin already set
	config->set |= FIRSTBIN;
	return 0;
    }
    
    if (sscanf(buf, " lastBin = %lf", &config->lastBin) == 1) {
	if (config->set & LASTBIN) return LASTBIN; // error; lastBin already set
	config->set |= LASTBIN;
	return 0;
    }

    if (sscanf(buf, " stepSize = %lf", &config->stepSize) == 1) {
	if (config->set & STEPSIZE) return STEPSIZE; // error; stepSize already set
	config->set |= STEPSIZE;
	return 0;
    }


        if (sscanf(buf, " fBin = %lf", &config->fBin) == 1) {
	if (config->set & FBIN) return FBIN; // error; fBin already set
	config->set |= FBIN;
	return 0;
    }
    
    if (sscanf(buf, " lBin = %lf", &config->lBin) == 1) {
	if (config->set & LBIN) return LBIN; // error; lBin already set
	config->set |= LBIN;
	return 0;
    }

    if (sscanf(buf, " binWidth = %lf", &config->binWidth) == 1) {
	if (config->set & BINWIDTH) return BINWIDTH; // error; binWidth already set
	config->set |= BINWIDTH;
	return 0;
    }

    
    if (sscanf(buf, " xSection = %s", config->xSection) == 1) {
	if (config->set & XSECTION) return XSECTION; // error; xSection already set
	config->set |= XSECTION;
	return 0;
    }
    
    if (sscanf(buf, " particle = %s", config->particle) == 1) {
	if (config->set & PARTICLE) return PARTICLE; // error; particle already set
	config->set |= PARTICLE;
	return 0;
    }

    if (sscanf(buf, " lumi = %lf", &config->lumi) == 1) {
	if (config->set & LUMI) return LUMI; // error; lumi already set
	config->set |= LUMI;
	return 0;
    }

    if (sscanf(buf, " cms = %s", config->cms) == 1) {
	if (config->set & CMS) return CMS; // error; cms already set
	config->set |= CMS;
	return 0;
    }

    if (sscanf(buf, " obs = %i", &config->obs) == 1) {
	if (config->set & OBS) return OBS; // error; obs already set
	config->set |= OBS;
	return 0;
    }

    if (sscanf(buf, " narrow = %i", &config->narrow) == 1) {
	if (config->set & NARROW) return NARROW; // error; narrow already set
	config->set |= NARROW;
	return 0;
    }
	
    return 3; // syntax error
}

void init_config(CONFIG *config)
{
    config->set = 0u;
}

void print_config(CONFIG *config)
{
    printf("[F_EFFICIENCY=%s, F_ACCEPTANCE=", config->set & F_EFFICIENCY? config->fEfficiency : "<unset>");
    if (config->set & F_ACCEPTANCE) printf("%s]\n", config->fAcceptance); else printf("<unset>]\n");

    printf("[PATHINPUT=%s]\n", config->set & PATHINPUT? config->pathInput : "<unset>");

    printf("[PARTICLE=%s, REGION=", config->set & PARTICLE ? config->particle : "<unset>");
    if (config->set & REGION) printf("%s]\n", config->region); else printf("<unset>]\n");
    printf("[XSECTION=%s]\n", config->set & XSECTION ? config->xSection : "<unset>");
    
    printf("[signal mass first bin =%lf, last bin =", config->set & FIRSTBIN ? config->firstBin : -1);
    if (config->set & LASTBIN) printf("%lf,", config->lastBin); else printf("<unset>");
    printf(" STEPSIZE=%lf ]\n", config->set & STEPSIZE ? config->stepSize : -1);

    printf("[<mll> first bin =%lf, last bin =", config->set & FBIN ? config->fBin : -1);
    if (config->set & LBIN) printf("%lf, ", config->lBin); else printf("<unset>]\n");
    printf(" bin width =%lf]\n", config->set & BINWIDTH ? config->binWidth : -1);

    printf("[LUMI=%lf, CMS=", config->set & LUMI ? config->lumi : -1 );
    if (config->set & CMS) printf("%s]\n", config->cms); else printf("<unset>]\n");

    printf("[OBS=%i]\n", config->set & OBS ? config->obs : 0 );

    printf("[NARROW=%i]\n", config->set & NARROW ? config->narrow : 0 );
	
}

void getVar_config(CONFIG *config, string &fileE, string &fileA, string &pathInput, string &region, string &xSection, string&particle, double &lumi, string &cms, double &firstBin, double &lastBin, double &stepSize, double &fBin, double &lBin, double &binWidth, int &obs, int &narrow)
{
    fileE = config->fEfficiency;
    fileA = config->fAcceptance;
    pathInput = config->pathInput;
    region = config->region;
    xSection = config->xSection;
    particle = config->particle;
    firstBin = config->firstBin;
    lastBin = config->lastBin;
    stepSize = config->stepSize;

    fBin = config->fBin;
    lBin = config->lBin;
    binWidth = config->binWidth;

    lumi = config->lumi;
    cms = config->cms;
    narrow = config->narrow;
    obs = config->obs;
}


/*
  1. channel: 2e2mu, 4e, 4mu, 4tau
  2. region: highMass, lowMass, heavyScalar
  3. mass:
  4. particle: vector boson Zd, pseudoscalar a
*/

int main(int argc, char *argv[])
{
    if (argc != 2) {
	fprintf(stderr, "Usage: %s CONFIG_FILE\n", argv[0]);
	return 1;
    }

    system("./clean.sh");
    //========================Configuring part=================================
    FILE *f = fopen(argv[1], "r");
    char buf[CONFIG_SIZE];
    CONFIG config[1];
    init_config(config);
    int lineNumber = 0;
    while (fgets(buf, sizeof buf, f)) {
	++lineNumber;
	int err = parse_config(buf, config);
	//if (err) fprintf(stderr, "error line %lf: %lf\n", lineNumber, err);
    }

    cout << "\n Parsing config \n";
    print_config(config);
    
    string fileE, fileA, pathInput, region, xSection, particle, cms;
    double firstBin, lastBin, lumi, stepSize, fBin, lBin, binWidth;
    
    int obs, narrow;
    
    getVar_config(config, fileE, fileA, pathInput, region, xSection, particle, lumi, cms, firstBin, lastBin, stepSize, fBin, lBin, binWidth, obs, narrow);
    cout << "fileE, fileA, pathInput, region, xSection, particle, firstBin, lastBin, stepSize, fBin, lBin, binWidth, lumi, cms, unblinded = " << fileE << ", " << fileA << ", " << pathInput << ", " << region << ", " << xSection << ", " << particle << ", " <<  firstBin << ", " << lastBin << ", " << stepSize <<", " << fBin << ", " << lBin << ", " << binWidth  << ", " << lumi << ", " << cms << ", " << obs << ", " << narrow << "\n";
    bool doBlind = (obs==0) ? true : false;
    



    //===============================Running part====================================
    if (xSection == "fiducial"){
	fiducialXS(fileE, pathInput, xSection, region, particle, lumi, cms, firstBin, lastBin, stepSize, fBin, lBin, binWidth, obs, narrow);
	//narrowFile();
    }
    else if(xSection == "total")
	totalXS(fileE, fileA, pathInput, xSection, region, particle, lumi, cms, firstBin, lastBin, stepSize, fBin, lBin, binWidth, obs, narrow);
    else
	cout << "Invalid cross section type, please choose either fiducial or total in your run.cfg file\n";
    

    //cout << "Current path is " << getWorkDirectoryPath() << '\n';
    //cout << "Get current path: " << get_current_dir_name() << "\n";
    
    return 0;
}

//Get the path of the run executable
string getWorkDirectoryPath()
{
    char arg1[20];
    int PATH_MAX = 100;
    char exepath[PATH_MAX + 1] = {0};
    
    sprintf( arg1, "/proc/%d/exe", getpid() );
    readlink( arg1, exepath, 1024 );
    return string( exepath );
}


//function to do interpolation for non-generated signal mass points
void interpolateFile(string inFileSignalName, double firstBin, double lastBin, double stepSize, string fileType)
{
    //==========For interpolating signal ===================
    cout << "Start Interpolation " << fileType << "\n";
    ifstream inFileSignal;
    inFileSignal.open(inFileSignalName.c_str());
    
    string signalInputFile;
   
    ofstream outFileInterpolatedSignal;
    string outFileName = "listInterpolatedSignal" + fileType;
    outFileInterpolatedSignal.open(outFileName.c_str());
    
    if(inFileSignal.is_open() )
	{
	    while( getline(inFileSignal, signalInputFile) )
		{
		    //		    cout << "signal acceptance file is: " << signalInputFile << "\n";
		    interpolateSignal(signalInputFile, outFileInterpolatedSignal, firstBin, lastBin, stepSize);
		}
	    outFileInterpolatedSignal.close();
	}
    cout << "Finish Interpolation " << fileType << "\n";
       
}


//function to split the interpolated histograms into 1-bin signal histograms for each mass point
//For continuous case (overloaded)
void splitFile(string particle, double firstBin, double lastBin, double stepSize, string fileType)
{
    //==========For splitting signal to different mass point hypothesis ===================
    string signalSplitFile, inFileName, outFileName;
    inFileName = "listInterpolatedSignal" + fileType;
    outFileName = "listSplitSignal" + fileType;
    
    ifstream inFileInterpolatedSignal;
    inFileInterpolatedSignal.open(inFileName.c_str());

    ofstream outFileSplitSignal;
    outFileSplitSignal.open(outFileName.c_str());
    system ( ("ls listInterpolatedSignal" + fileType).c_str() );
    if( inFileInterpolatedSignal.is_open() ) {
	string channel;
	while( getline(inFileInterpolatedSignal, signalSplitFile) )
	    {
		string shortName = signalSplitFile.substr(signalSplitFile.find_last_of("/")+1, signalSplitFile.find(".root") - signalSplitFile.find_last_of("/")-1 );
		//		cout << "Split file " << signalSplitFile << "\n";
		//		cout << "Short Name " << shortName << "\n";
		getSubStrBetweenTwoStrings(shortName, "_", "channel", channel);
		//getSubStrBetweenTwoStrings(signalSplitFile, "channel_", ".root", particle);
		cout << "Enter split \n";
		splitInterpolatedSignal(signalSplitFile,
					outFileSplitSignal,
					channel,
					particle,
					firstBin,
					lastBin,
					stepSize);
	    }
    }
    outFileSplitSignal.close(); 
    cout << "Finish Splitting \n";    
    //----------------------------------------------------------------------
}

//For discrete case
void splitFile(string inFileName, string particle, string fileType)
{
    //==========For splitting signal to different mass point hypotheses ===================
    //Get in the efficiency or acceptance root file
    string signalSplitFile, outFileName;
    outFileName = "listSplitSignal" + fileType;
    ofstream outFileSplitSignal;
    outFileSplitSignal.open(outFileName.c_str());

    ifstream inFile;
    inFile.open(inFileName.c_str());
    
    if( inFile.is_open() ) {
	string channel;
	while( getline(inFile, signalSplitFile) )
	    {
		string shortName = signalSplitFile.substr(signalSplitFile.find_last_of("/")+1, signalSplitFile.find(".root") - signalSplitFile.find_last_of("/")-1 );
		cout << "Split file " << signalSplitFile << "\n";
		cout << "Short Name " << shortName << "\n";
		getSubStrBetweenTwoStrings(shortName, "_", "channel", channel);
		//getSubStrBetweenTwoStrings(signalSplitFile, "channel_", ".root", particle);
		cout << "Enter split \n";
		
		splitDiscreteSignal(signalSplitFile,
				    outFileSplitSignal,
				    channel,
				    particle);
	    }
    }
    outFileSplitSignal.close(); 
    cout << "Finish Splitting \n";    
    //----------------------------------------------------------------------
}


//function to make Gaussian distribution from mean-sigma-efficiency-acceptance of each splitted file at a signal mass point
void makeGaussianModelFile(string xSection, double fBin, double lBin, double binWidth, double lumi, string fileType)
{
    //==========For making Gaussian model for signal ===================
    cout <<"Start making Gaussian Model file \n";
    ofstream outFileGaussianModel;
    outFileGaussianModel.open("listGaussianSignal");
    
    createAllGaussianSignalRootFile(xSection, outFileGaussianModel, fBin, lBin, binWidth, lumi, fileType);
    outFileGaussianModel.close();
    cout << "Finish making Gaussian Model file \n";
}


//function to write out xml files from sig, bkg, data root files
void xmlFile(string pathInput, string particle, string xSection, string region, bool doFiducialXS, bool doNarrow)
{
    //===== For making xml files, making workspace, running limits======================
    cout << "Start making XML \n";    
    string signalRootFile, channel, mass;    
    ifstream inFileGaussianModel;
    inFileGaussianModel.open("listGaussianSignal");
    
    ofstream fileRunWS, fileRunAsymptotics;
    fileRunWS.open("runThis/runMakeAllWorkspace_" + region + "_" + particle + "_" + xSection + ".sh");
    fileRunAsymptotics.open("runThis/runAsymptoticsLimit_" + region + "_" + particle + "_" + xSection + ".sh");
    fileRunAsymptotics << "cd share \n\n";
    
    if(inFileGaussianModel.is_open() )
	{
	    while( getline(inFileGaussianModel, signalRootFile) )
		{
		    // cout << "signal file is: " << signalRootFile << "\n";
		    getSubStrBetweenTwoStrings(signalRootFile, "_", "channel", channel);
		    // cout << "channel is: " << channel << "\n";
		    
		    string shortName = signalRootFile.substr(signalRootFile.find_last_of("/")+1, signalRootFile.find(".root") - signalRootFile.find_last_of("/")-1 );
		    //cout << "Short Name " << shortName << "\n";
		    getSubStrBetweenTwoStrings(shortName, "_", "channel", channel);
		    getSubStrBetweenTwoStrings(shortName, particle, "GeV", mass);
		    
		    if(doFiducialXS)
			makeXmlFiducialXSFiles(signalRootFile, pathInput, xSection, region, channel, particle, mass, fileRunWS, fileRunAsymptotics, doNarrow);
		    else
			makeXmlTotalXSFiles(signalRootFile, pathInput, xSection, region, channel, particle, mass, fileRunWS, fileRunAsymptotics, doNarrow);
		    //		    cout << "done " << particle << ", at mass point " << mass << " GeV \n";
		}
	}
    fileRunAsymptotics << "cd " << get_current_dir_name() << "\n";    
    fileRunWS.close();
    fileRunAsymptotics.close();
    cout << "Finish making XML \n";    
}


void fiducialXS(string fileE, string pathInput, string xSection, string region, string particle, double lumi, string cms, double firstBin, double lastBin, double stepSize, double fBin, double lBin, double binWidth, int obs, int narrow){
    //If the preceded step was executed before, can comment that line out
    interpolateFile(fileE, firstBin, lastBin, stepSize);//eff
    splitFile(particle, firstBin, lastBin, stepSize);//eff

    makeGaussianModelFile(xSection, fBin, lBin, binWidth, lumi);
    bool doNarrow = (narrow==1) ? true : false;
    
    if(doNarrow)
	narrowFile(fBin, lBin, binWidth,  pathInput,  region, particle);
    //	narrowFile(fBin, lBin, binWidth);
    
    xmlFile(pathInput, particle, xSection, region, 1, doNarrow);
}


void totalXS(string fileE, string fileA, string pathInput, string xSection, string region, string particle, double lumi, string cms, double firstBin, double lastBin, double stepSize, double fBin, double lBin, double binWidth, int obs, int narrow)
{
    //If the preceded step was executed before, can comment that line out
    interpolateFile(fileE, firstBin, lastBin, stepSize);//eff 
    splitFile(particle, firstBin, lastBin, stepSize);//eff
    
    interpolateFile(fileA, firstBin, lastBin, stepSize,"acceptance");//acc
    splitFile(particle, firstBin, lastBin, stepSize, "acceptance"); //acc
    
    makeGaussianModelFile(xSection, fBin, lBin, binWidth, lumi, "acceptance");
    
    //this part is to make workspace with hadding all channels of signal/background/data root files. 
    if(particle == "Zd" && region != "lowMass"){
    	scalingSignalForTotalXS();
    	//haddSignalFileForTotalXS();// can do combination without hadding signal files. If wanna hadd, have to change the list file name (Loan: to check)
    }
    bool doNarrow = (narrow==1) ? true : false;
    if(doNarrow)
	narrowFile(fBin, lBin, binWidth,  pathInput,  region,  particle);
    //	narrowFile(fBin, lBin, binWidth);
    xmlFile(pathInput, particle, xSection, region, 0, doNarrow);
}


void muLimit(string fileE, string fileA, string pathInput, string xSection, string region, string particle, double lumi, string cms, double fBin, double lBin, double binWidth, int obs, int narrow)
{
    double ggFXS =  6.02392; //fb, from https://gitlab.cern.ch/atlas-phys/exot/ueh/EXOT-2016-22/ZdZd13TeV/blob/master/ZdZdAnalysis/share/crossSections.txt
    splitFile(fileE, particle);//eff
    splitFile(fileA, particle, "acceptance"); //acc
    
    //here input the Higgs XS as well so that the parameter of interest is sigma(S->ZdZd->4l)/sigma_SM(H->ZZ*->4l) 
    makeGaussianModelFile(xSection, fBin, lBin, binWidth, lumi*ggFXS, "acceptance");
    
    //this part is to make workspace with scaling channels 
    if(particle == "Zd" && region != "lowMass"){
    	scalingSignalForTotalXS();
    	//haddSignalFileForTotalXS();// can do combination without hadding signal files. If wanna hadd, have to change the list file name (Loan: to check)
    }

    
    double doNarrow = (narrow == 1) ? true : false;
    xmlFile(pathInput, particle, xSection, region, 0, doNarrow);
}


void scalingSignalForTotalXS()
{
    cout << "Start doing scaling for totalXS \n";    
    ofstream outFileGaussianForTotalXS;
    
    outFileGaussianForTotalXS.open("listRescaledGaussianSignal");
    
    ifstream inFileGaussianModel;
    system("mv listGaussianSignal listGaussianSignalUnscaled");
    string inFileName = "listGaussianSignalUnscaled";
    inFileGaussianModel.open(inFileName.c_str());
    string signalFileName;
    
    //scaling all the files with ratio: 0.25:0.5:025 for 4e:2e2mu:4mu
    if(inFileGaussianModel.is_open() ){
	string channel;
	while( getline(inFileGaussianModel, signalFileName) )
	    {
		string shortName = signalFileName.substr(signalFileName.find_last_of("/")+1, signalFileName.find(".root") - signalFileName.find_last_of("/")-1 );
		//cout << "gaussian file " << signalFileName << "\n";
		//		cout << "Short Name " << shortName << "\n";
		getSubStrBetweenTwoStrings(shortName, "_", "channel", channel);
		
		TFile *fileOriginal = new TFile(signalFileName.c_str(),"READ");
		int posLastSlash =  signalFileName.find_last_of("/") ;
		string newFileName;
		
		newFileName = signalFileName.substr(0, posLastSlash) + "/BRscaled_" + signalFileName.substr(posLastSlash + 1,  signalFileName.length() - posLastSlash - 1);
		//		cout << "new File name is: " << newFileName << endl;
		

		TFile *fileNew = new TFile(newFileName.c_str(),"RECREATE");
		scalingHist(fileOriginal, fileNew, channel);
		fileNew->Close();
		fileNew->Delete();
    
		fileOriginal->Close();
		fileOriginal->Delete();
  
		outFileGaussianForTotalXS << newFileName << "\n";
	    }
    }
    outFileGaussianForTotalXS.close();
    system ( "cp listRescaledGaussianSignal listGaussianSignal" );
    cout << "Finish doing scaling \n";    
}


void haddSignalFileForTotalXS()
{
      cout << "Start hadding files for totalXS \n";    
    //============================================
    ofstream outFileToHadd;
    string outFileName = "listToHadd";
    outFileToHadd.open(outFileName);

    ofstream outFileFinalGaussian;
    string outFileFinalGaussianName = "listGaussianForTotalXS";
    outFileFinalGaussian.open(outFileFinalGaussianName);

    ifstream inFileGaussianForTotalXS;
    string inFileName = "listRescaledGaussianSignal";
    inFileGaussianForTotalXS.open(inFileName.c_str());
    string signalFileName, channel;
    
    vector<string> vec_channels;

    vec_channels.push_back("4mu");
    vec_channels.push_back("4e");
    
    while( getline(inFileGaussianForTotalXS, signalFileName) ){
	
	    int posLastSlash =  signalFileName.find_last_of("/") ;
	    string shortName = signalFileName.substr(posLastSlash + 1, signalFileName.find(".root") - posLastSlash - 1 );
	    string pathFile =  signalFileName.substr(0, posLastSlash+1);
	    
	    //cout << "gaussian file " << signalFileName << "\n";
	    //cout << "Short Name " << shortName << "\n";
	    getSubStrBetweenTwoStrings(shortName, "_", "channel", channel);

	    string mergedRootFileName = signalFileName;
	    //cout << "Finish getting string \n";	    
	    replace(mergedRootFileName, "/BRscaled_", "/");
	    replace(mergedRootFileName, "2e2mu", "4l");

	    if(channel != "2e2mu")
		continue;
	    
	    string tmp = "hadd " + mergedRootFileName + " -f " + signalFileName;
	    outFileFinalGaussian << mergedRootFileName << "\n";
	    //cout << tmp << "\n";	    
	    //cout << vec_channels.at(0)  << "\n";
	    //cout << vec_channels.size() << "\n";
	    for(int i = 0; i < vec_channels.size(); ++i){
		string tmpFileName = signalFileName;
		replace(tmpFileName, "2e2mu", vec_channels.at(i));
		tmp = tmp + " " + tmpFileName;
	    }
	    tmp = tmp + "\n";
	    //cout << tmp << "\n";
	    outFileToHadd << tmp;
	}
    
    outFileToHadd.close();
    outFileFinalGaussian.close();
    system( ("chmod +x " + outFileName).c_str() );
    system( ("./" + outFileName).c_str() );
    system ( "cp listGaussianForTotalXS listGaussianSignal" );
    cout << "Finish hadding for total XS \n";    
}


void narrowFile(double fBin, double lBin, double binWidth, string pathInput, string region, string particle){
    cout << "Start narrowing fitting range of <mll> \n";
    string inFileName = "listGaussianSignal";
    ifstream inFile;
    inFile.open(inFileName.c_str());
    string rootFileName;
    
    if(inFile.is_open() )
	{
	    while( getline(inFile, rootFileName) )
		{
		    //cout << "full gaussian signal file is: " << rootFileName << "\n";
		    remakeFile(rootFileName, fBin, lBin, binWidth, pathInput, region, particle);
		}
	}
    cout << "Finish narrowing fitting range of <mll> \n";
}

