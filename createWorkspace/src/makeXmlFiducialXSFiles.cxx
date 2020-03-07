//===================Scripts to write xml files from each rootFile loan.truong@cern.ch======================


#include "include/makeXmlFiducialXSFiles.h"
#include "include/writeChannelXml.h"
#include "include/writeDriverXml.h"
#include "include/writeSampleXml.h"
#include "include/common.h"
#include<unistd.h>

using namespace std;

void makeXmlFiducialXSFiles(string signalRootFile,
			    string pathInput,
			    string xSection,
			    string region,
			    string channel,
			    string particle,
			    string mass,
			    ofstream& fileRunWS,
			    ofstream& fileRunAsymptotics,
			    bool doNarrow){
    std::cout << "Begin making fiducial XMLs \n";
    char wd[256]; 
 string currentDir = getcwd(wd, 256);
 //string currentDir = get_current_dir_name();
    cout << "current directory will be used for path to input: " << currentDir << "\n";
	
    //Get bkg and data input corresponding to each channel

    map<string,string> mapRootFileName;
    string suffix = doNarrow ? "GeVreBin.root" : ".root";
    string suffixSig = doNarrow ? "reBin.root" : ".root";
    
    mapRootFileName["bkg"] = currentDir + "/" + pathInput + "/bkg/" + region + "/bkg_" + channel + "channel_m" + particle + mass + suffix; 
    mapRootFileName["data"] = currentDir + "/" + pathInput + "/data/" + region + "/data_" + channel + "channel_m" + particle + mass + suffix;
    mapRootFileName["sig"] = signalRootFile.substr(0,signalRootFile.length()-5) + suffixSig; 

    //make the directory to store the xml files
    string storePath = currentDir + "/xml/" + xSection + "/" + region + "/" + mass + "GeV/" + channel + "/";
    system(("mkdir -vp " + storePath).c_str()); 
    
    //====================== Write down xml files ===========================
    //xml channel write file
    
    string channelXmlFileName="channelHZdZd4l_2019.xml";
    string driverXmlFile="HZdZd4l_2019_driver.xml";
    
    ofstream channelWriteFile;
    channelWriteFile.open(channelXmlFileName.c_str());
    writeChannelXml(channelXmlFileName, channel, channelWriteFile, mapRootFileName );
    channelWriteFile.close();   
    
    //xml driver write file
    ofstream myDriverFile;
    myDriverFile.open(driverXmlFile.c_str());
    vector<string> vec_channelXmlFileName;
    vec_channelXmlFileName.push_back(channelXmlFileName);
    writeDriverXml(vec_channelXmlFileName, myDriverFile);
    myDriverFile.close(); 

    //move all xml files to their storing place
    system(("mv *.xml " + storePath).c_str());

    //write out the commands to make root files of workspaces to a file
    fileRunWS << "cd " << storePath << "\n" << "hist2workspace " << driverXmlFile << "\n" << "cd " << currentDir << " \n\n";

    //write out commands to run asymptotics
    fileRunAsymptotics << "root -b -q \'runAsymptoticsCLs.C+(\"" << storePath << "ATLAS_HZdZd4l_combined_allsys_model.root\",\"combined\",\"ModelConfig\",\"asimovData\",\"asimovData_0\",\"" << region << "/" << xSection << "/"<< channel << "_m" << particle << "_" << mass << "GeV" << "\",\"125\",0.95)\' > limit_" << region << "_" << xSection << "_" << channel << "_m" << particle << "_" << mass << "GeV \n\n";

}








