#include <stdio.h>
#include <iostream>
#include "bandLimitPlot.h"
#include "readLimits.h"

#define CONFIG_SIZE (256)
#define FIRSTMASS (1)
#define STEPSIZE (1)
#define LASTMASS (1)
#define FILELIMIT (1)
#define REGION (1)
#define XSECTION (1)
#define PARTICLE (1)
#define LUMI (1)
#define CMS (1)
#define OBS (1)
#define FOLDLIMIT (1)
#define CHANNEL (256)
#define NAMETAG (1)

using namespace std;

typedef struct config
{
    unsigned set;
    char fileLimit[CONFIG_SIZE];
    char foldLimit[CONFIG_SIZE];
    double firstMass;
    double stepSize;
    double lastMass;
    char xSection[CONFIG_SIZE];
    char region[CONFIG_SIZE];
    char particle[CONFIG_SIZE];
    char lumi[CONFIG_SIZE];
    char cms[CONFIG_SIZE];
    int obs;
    vector<string> Channel;
    char nameTag[CONFIG_SIZE];
} CONFIG;


// Parse the buffer for config info. Return an error code or 0 for no error.
int parse_config(char *buf, CONFIG *config)
{
    char dummy[CONFIG_SIZE];

    if (sscanf(buf, " %s", dummy) == EOF) return 0; // blank line
    if (sscanf(buf, " %[#]", dummy) == 1) return 0; // comment

    
    if (sscanf(buf, " fileLimit = %s", config->fileLimit) == 1) {
	if (config->set & FILELIMIT) return FILELIMIT; // error; fileLimit already set
	config->set |= FILELIMIT;
	return 0;
    }
    
    if (sscanf(buf, " foldLimit = %s", config->foldLimit) == 1) {
	if (config->set & FOLDLIMIT) return FOLDLIMIT; // error; foldLimit already set
	config->set |= FOLDLIMIT;
	return 0;
    }

    if (sscanf(buf, " firstMass = %lf", &config->firstMass) == 1) {
	if (config->set & FIRSTMASS) return FIRSTMASS; // error; firstMass already set
	config->set |= FIRSTMASS;
	return 0;
    }

    if (sscanf(buf, " stepSize = %lf", &config->stepSize) == 1) {
	if (config->set & STEPSIZE) return STEPSIZE; // error; stepSize already set
	config->set |= STEPSIZE;
	return 0;
    }
    
    if (sscanf(buf, " lastMass = %lf", &config->lastMass) == 1) {
	if (config->set & LASTMASS) return LASTMASS; // error; lastMass already set
	config->set |= LASTMASS;
	return 0;
    }

    if (sscanf(buf, " xSection = %s", config->xSection) == 1) {
	if (config->set & XSECTION) return XSECTION; // error; xSection already set
	config->set |= XSECTION;
	return 0;
    }

    if (sscanf(buf, " region = %s", config->region) == 1) {
	if (config->set & REGION) return REGION; // error; region already set
	config->set |= REGION;
	return 0;
    }

    if (sscanf(buf, " particle = %s", config->particle) == 1) {
	if (config->set & PARTICLE) return PARTICLE; // error; particle already set
	config->set |= PARTICLE;
	return 0;
    }

    if (sscanf(buf, " lumi = %s", config->lumi) == 1) {
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

    if (sscanf(buf, "Channel = %s", dummy) == 1) {
    	cout << "tmp = " << dummy << "\n";
    	(config->Channel).push_back(dummy);
    }

    if (sscanf(buf, " nameTag = %s", config->nameTag) == 1) {
	if (config->set & NAMETAG) return NAMETAG; // error; nameTag already set
	config->set |= NAMETAG;
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
    printf("[FOLDLIMIT=%s]\n", config->set & FOLDLIMIT ? config->foldLimit : "<unset>]\n");

    printf("[FIRSTMASS=%lf, LASTMASS=", config->set & FIRSTMASS ? config->firstMass : -1);
    if (config->set & LASTMASS) printf("%lf, ", config->lastMass); else printf("<unset>\n");
    printf("STEPSIZE=%lf]\n", config->set & STEPSIZE ? config->stepSize : -99);

    printf("[XSECTION=%s, PARTICLE=", config->set & XSECTION ? config->xSection : "<unset>");
    if (config->set & PARTICLE) printf("%s, ", config->particle); else printf("<unset>, ");
    printf("REGION=%s]\n", config->set & REGION ? config->region : "<unset>]\n");

    printf("[FILELIMIT=%s  ]\n", config->set & FILELIMIT ? config->fileLimit : "<unset>]\n");
    
    printf("[LUMI=%s, CMS=", config->set & LUMI ? config->lumi : "<unset>" );
    if (config->set & CMS) printf("%s%s", config->cms, "]\n"); else printf("<unset>]\n");

    printf("[OBS=%i]\n", config->set & OBS ? config->obs : 99 );

    printf("[NAMETAG=%s]\n", config->set & NAMETAG ? config->nameTag : "<unset>" );
	
}

void getVar_config(CONFIG *config, vector<string> &Channel, string &fileLimit, string &xSection, string &region, string&particle, string &lumi, string &cms, double &firstMass, double &lastMass, double &stepSize, int &obs, string &foldLimit, string &nameTag)
{
    fileLimit = config->fileLimit;
    Channel = config->Channel;
    xSection = config->xSection;
    region = config->region;
    particle = config->particle;
    firstMass = config->firstMass;
    lastMass = config->lastMass;
    stepSize = config->stepSize;
    lumi = config->lumi;
    cms = config->cms;
    obs = config->obs;
    foldLimit = config->foldLimit;
    nameTag = config->nameTag;
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
	fprintf(stderr, "Usage: %s CONFIG_FILE\n", argv[0]);
	return 1;
    }
    FILE *f = fopen(argv[1], "r");
    char buf[CONFIG_SIZE];
    CONFIG config[1];
    init_config(config);
    int line_number = 0;
    while (fgets(buf, sizeof buf, f)) {
	++line_number;
	int err = parse_config(buf, config);
	//if (err) fprintf(stderr, "error line %d: %d\n", line_number, err);
    }

    cout << "\n\n\n*** PARSING CONFIG FILE " << argv[1] << " *** \n\n";
    print_config(config);
    
    string fileLimit, foldLimit, xSection, region, particle, lumi, cms, nameTag;
    double firstMass, lastMass, stepSize;
    int obs;
    vector<string> Channel;
    
    getVar_config(config, Channel, fileLimit, xSection, region, particle, lumi, cms, firstMass, lastMass, stepSize, obs, foldLimit, nameTag);
    
    //cout << "Channel, xSection, region, particle, firstMass, lastMass, stepSize, lumi, cms, unblinded = " << ", " << Channel << ", " << xSection << ", " << region << ", " << particle << ", " <<  firstMass << ", " << lastMass << ", " << stepSize << ", " << lumi << ", " << cms << ", " << obs << "\n";
    
    cout << "[Channel(s) = ";
    for(int i =0; i< Channel.size(); ++i)
    	cout << Channel.at(i) << " ";
    
    cout << "]\n\n";
    
    bool doBlind = (obs==0) ? true : false;


    //READ LIMIT FROM ROOT FILES. Can remove this step if you already got the list stored somewhere, remember to modify the fileLimit
    if(foldLimit != ""){
	readLimits(foldLimit,
		   xSection,
		   region,
		   Channel,
		   particle,
		   firstMass,
		   lastMass,
		   stepSize,
		   nameTag);
	
	if (fileLimit != ""){
	    cout << "!! list file limit is: " << fileLimit << " !!\n";
	    cout << "!! Limit file is set!! read limits from above step will be ignored. If not wanting this, please rerun without fileLimit set in the config file " << argv[1] << "!! \n\n";
	}
	else 		
	    fileLimit = "listReadOutFiles" + nameTag + ".txt";
    }
    else if (fileLimit == "")
	cout << "!! Please provide at least a path (set in foldLimit) to know where to read limit in, or a fileLimit where all read limits already written out. Check file " << argv[1] << " for more instructions !! \n\n";
    
    
    bandLimitPlot(doBlind,
    		  region,
    		  fileLimit,
    		  Channel,
    		  xSection,
    		  particle,
    		  lumi,
    		  cms,
    		  firstMass,
    		  lastMass,
		  nameTag);
    return 0;
}
