#ifndef  __BANDLIMITPLOT_H
#define __BANDLIMITPLOT_H

#include "AtlasStyle.h"
#include "AtlasUtils.h"
#include "AtlasLabels.h"
#include "TCanvas.h"
#include "TMultiGraph.h"
#include "TH1.h"
#include "TH1F.h"
#include "TLegend.h"
#include "TLatex.h"
#include "TLegendEntry.h"
#include "TLine.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <sstream>

void parseInput(std::ifstream &inFile, std::vector<std::vector<double> > &v2);

void bandLimitPlot(bool doBlind,
		   std::string &region,
		   std::string &file,
		   std::vector<std::string> &canvs,
 		   std::string &xs,
		   std::string &par,
		   std::string &lumi,
		   std::string &cms,
		   double firstBin,
		   double lastBin,
		   std::string nameTag);

void drawBox(TCanvas *c1, Double_t xl, Double_t xr, Double_t y1, Double_t y2);
#endif //   __BANDLIMITPLOT_H
