//===================Scripts to write xml files from each rootFile loan.truong@cern.ch======================

#include "include/makeXmlTotalXSFiles.h"
#include "include/writeDriverXml.h"
#include "include/writeSampleXml.h"
#include "include/writeChannelXml.h"
#include "include/common.h"

using namespace std;

void makeXmlTotalXSFiles(string signalRootFile,
			 string pathInput,
			 string xSection,
			 string region,
			 string channel,
			 string particle,
			 string mass,
			 ofstream& fileRunWS,
			 ofstream& fileRunAsymptotics,
			 bool doNarrow){
    //    std::cout << "Begin making totalXS XMLs \n";
    vector<string> vecChannel;
    
    
    //For low-mass analysis, one just needs 4mu channel. For high-mass analysis, one need all 3 channels to be included in the driver xml file
    if (particle == "a")
	vecChannel.push_back("4mu");
    else
	//vecChannel.push_back("4l");
	{
	    vecChannel.push_back("4e");
	    vecChannel.push_back("2e2mu");
	    vecChannel.push_back("4mu");
	}
    int numChannels = vecChannel.size();
    
    string currentDir = get_current_dir_name();
    //    cout << "current directory will be used for path to input: " << currentDir << "\n";
	
    //Get bkg and data input corresponding to each channel
    map<string,string> mapRootFileName;
    string suffix = doNarrow ? "GeVreBin.root" : ".root";
    string suffixSig = doNarrow ? "reBin.root" : ".root";
   
    mapRootFileName["bkg"] = currentDir + "/" + pathInput + "/bkg/" + region + "/bkg_" + channel + "channel_m" + particle + mass + suffix; //"GeVreBin.root";
    mapRootFileName["data"] = currentDir + "/" + pathInput + "/data/" + region + "/data_" + channel + "channel_m" + particle + mass + suffix; //"GeVreBin.root";
    mapRootFileName["sig"] = signalRootFile.substr(0,signalRootFile.length()-5) + suffixSig; //"reBin.root";

    string inputNameTag = pathInput.find("/") == string::npos ? pathInput : pathInput.substr(pathInput.find_last_of("/"),pathInput.length() - pathInput.find_last_of("/"));
    //    cout << "inputNameTag = " << inputNameTag << "\n";
    //make the directory to store the xml files
    string storePath = currentDir + "/xml/" + xSection + "/" + region + "/" + inputNameTag + "/" + mass + "GeV/" + channel + "/";
    system(("mkdir -vp " + storePath).c_str());     

    //====================== Write down xml files ===========================
    //xml channel write file
    string channelXmlFileName= "channelHZdZd4l_2019.xml";
    
    ofstream channelWriteFile;
    channelWriteFile.open(channelXmlFileName.c_str());
    writeChannelXml(channelXmlFileName, channel, channelWriteFile, mapRootFileName);
    channelWriteFile.close();   
    
    //move all channel xml files to their storing place
    system(("mv *.xml " + storePath).c_str());
    
    //make xml driver file only when reaching to 4mu signal (since both low-mass and high mass have this channel)
    //xml driver write file
    string driverXmlFile="HZdZd4l_2019_driver.xml";
    ofstream myDriverFile;
    
    vector<string> vec_channelXmlFileName;

    if(channel == "4mu"){
	
	for(int chan=0; chan < numChannels; ++chan){
	    string channelFilePath =  currentDir + "/xml/" + xSection + "/" + region + "/" + inputNameTag + "/" + mass + "GeV/" + vecChannel.at(chan) + "/";
	    vec_channelXmlFileName.push_back(channelFilePath + channelXmlFileName);
	}
	
	myDriverFile.open(driverXmlFile.c_str());
	writeDriverXml(vec_channelXmlFileName, myDriverFile);
	myDriverFile.close(); 
	string driverFilePath = currentDir + "/xml/" + xSection + "/" + region + "/" + inputNameTag + "/" + mass + "GeV/";
	//move driver xml file to their storing place
	system(("mv *.xml " + driverFilePath).c_str());
	
	//write out the commands to make root files of workspaces to a file
	fileRunWS << "cd " << driverFilePath << "\n" << "hist2workspace " << driverXmlFile << "\n" << "cd " << currentDir << " \n\n";
	
	//write out commands to run asymptotics
	string foldLimitName = xSection + "/" + region + "/" + inputNameTag + "/" + channel + "_m" + particle + "_" + mass + "GeV";
	string logName = "limit_" + xSection + "_" + region + "_" + inputNameTag + "_" + channel + "_m" + particle + "_" + mass + "GeV";
	fileRunAsymptotics << "root -b -q \'runAsymptoticsCLs.C+(\"" << driverFilePath << "ATLAS_HZdZd4l_combined_allsys_model.root\",\"combined\",\"ModelConfig\",\"asimovData\",\"asimovData_0\",\"" << foldLimitName << "\",\"125\",0.95)\' >" << logName << "\n\n";
    }
}
