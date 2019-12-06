//===================Scripts to write xml template for each channel from each rootFile loan.truong@cern.ch======================


#include "include/writeChannelXml.h"


using namespace std;


//function to write out HistFactory syntax for a channel
void writeChannelXml(string channelXmlFileName, string channel, ofstream& channelWriteFile, map<string,string> mapRootFileName){

    //Write down the header
    channelWriteFile << "<!DOCTYPE Channel  SYSTEM '" << get_current_dir_name() << "/share/HistFactorySchema.dtd'> \n";
    channelWriteFile << "<Channel Name=\"SR" << channel << "_2019\" InputFile=\"" << mapRootFileName["data"] << "\" > \n \n";
    channelWriteFile << "<Data HistoName=\"Data\"/> \n";
    channelWriteFile <<" <StatErrorConfig RelErrorThreshold=\"0.0\" \t ConstraintType=\"Poisson\" /> \n";
    
    //Write down signal
    writeSampleXml(mapRootFileName["sig"],channelWriteFile,"sig");
    
    //Write down background    
    writeSampleXml(mapRootFileName["bkg"],channelWriteFile,"bkg");
    
    //Write end of the file and close
    channelWriteFile << "</Channel>" << endl;
}
