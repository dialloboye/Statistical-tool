#include "include/reBinHistogram.h"
#include "include/common.h"

using namespace std;

void remakeFile(std::string& rootFileName, double binFirst, double binLast, double binWidth, std::string& pathInput, std::string& region, std::string& particle) 
{
    //=======================================================
    /*
      1. checking each histograms where there are bins with yield > threshold.
      2. checking left edge by scanning from right to left
      3. checking right edge by scanning from left to right.
    */
    int firstBin(190), lastBin(1);
    int numBin = static_cast <int> ( (binLast - binFirst)/binWidth); //Here numBin is the # of intervals
    firstBin = numBin;
    lastBin = 1;
    
    
    //Cropping for data and bkg as well
    //gaussiansignal_4muchannel_ma2GeV.root

    string shortName = rootFileName.substr(rootFileName.find_last_of("/")+1, rootFileName.find(".root") - rootFileName.find_last_of("/")-1 );
    string channel, mass;
    getSubStrBetweenTwoStrings(shortName, "_", "channel", channel);
    cout << "CHANNEL " << channel << endl;
    
    getSubStrBetweenTwoStrings(shortName, particle , "GeV", mass);
    cout << "MASS " << mass << endl;

    string currentDir = get_current_dir_name();
    cout << "CURRENT DIR " << currentDir << endl;

    string mainBkg = currentDir + "/" + pathInput + "/bkg/" + region + "/bkg_" + channel + "channel_" + particle + ".root";
    string bkgFile = currentDir + "/" + pathInput + "/bkg/" + region + "/bkg_" + channel + "channel_m" + particle + mass + "GeV.root";
    system( ("cp " + mainBkg + " " + bkgFile).c_str());
  
    string mainData = currentDir + "/" + pathInput + "/data/" + region + "/data_" + channel + "channel_" + particle + ".root";
    string dataFile = currentDir + "/" + pathInput + "/data/" + region + "/data_" + channel + "channel_m" + particle + mass + "GeV.root";
    system( ("cp " + mainData + " " + dataFile).c_str());


    //Should respect the ordering below, DO NOT CHANGE THEM
    getRangeForFile(rootFileName, firstBin, lastBin);
    
    getHistogramForFile(rootFileName, firstBin, lastBin);

    cout << "REMAKE BKG FILE " << bkgFile << "\n";
    if( particle == "a") 
	getHistogramForFileNoDir(bkgFile, firstBin, lastBin);
    else
	getHistogramForFile(bkgFile, firstBin, lastBin);

    cout << "REMAKE DATA FILE " << dataFile << "\n";
    getHistogramForFileNoDir(dataFile, firstBin, lastBin);
	
    
}



