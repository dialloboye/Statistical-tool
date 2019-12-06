//===================Scripts to write the Driver xml files from each rootFile loan.truong@cern.ch======================


#include "include/writeDriverXml.h"

using namespace std;



void writeDriverXml(vector<string> vec_channelXmlFileName, ofstream& myDriverFile){
    //function to write out HistFactory syntax for the driver xml file

    //==================Write down the driver xml file================
    myDriverFile << "<!--Driver xml or top xml, we need only one of this kind. It combines all channel xml files -->\n" ; 
    myDriverFile << "<!DOCTYPE Combination  SYSTEM '" << get_current_dir_name() << "/share/HistFactorySchema.dtd'> \n"; 
    myDriverFile << "<Combination OutputFilePrefix=\"ATLAS_HZdZd4l\"> \n";

    for (int i = 0; i < vec_channelXmlFileName.size(); ++i){
	myDriverFile << "<Input>" << vec_channelXmlFileName.at(i) << "</Input> \n";
    };
    
    myDriverFile << "<Measurement Name=\"allsys\" Lumi=\"1\" LumiRelErr=\"0.022\" BinLow=\"0\" BinHigh=\"300\" Mode=\"comb\" ExportOnly=\"True\"> \n";
    myDriverFile << "    <POI>mu</POI> \n";
    myDriverFile << "    <ParamSetting Const=\"True\">Lumi</ParamSetting> \n";
    myDriverFile << "</Measurement> \n";
    myDriverFile << "</Combination> \n";
}
