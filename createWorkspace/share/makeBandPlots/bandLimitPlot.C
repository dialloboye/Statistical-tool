#include "bandLimitPlot.h"

using namespace std;

void bandLimitPlot(bool doBlind,
		   string &region,
		   string &file,
		   vector<string> &canvs,
		   string &xs,
		   string &particle,
		   string &lumi,
		   string &cms,
		   double firstBin,
		   double lastBin,
		   string nameTag)
{
    
    gStyle->SetOptStat(0);
    gROOT->SetBatch(true);
    SetAtlasStyle();
    

    //0. =====Get configurations limit files and canvas names ==============
    ifstream fileLimit(file);
    string lineL;
    vector<string> files;
    
    if (fileLimit.is_open())
	{
	    while( getline(fileLimit, lineL) ) {
		if (lineL.find("#") == string::npos)
		    files.push_back(lineL);
	    }
	}
    
    
    //Signal cross-sections, in case wanna incorporate XS onto signal strength limit, values at 8 TeV?
    Double_t ggFSigma[] = {22.66*1000., 20.86*1000., 19.27*1000., 17.85*1000., 13.55*1000., 7.801*1000., 3.594*1000.};
	
    
    //Looping over input files, making a plot for each case
    for(int i = 0; i < canvs.size(); ++i){
	TCanvas *can = new TCanvas( canvs.at(i).c_str(), canvs.at(i).c_str(), 66, 52, 800, 600);
	can->Range(0,0,1,1);
	can->SetFillColor(0);
	can->SetBorderMode(0);
	can->SetBorderSize(2);
	can->SetFrameBorderMode(0);
	
	TString tmps;
	ifstream inFile( files.at(i) );
	cout << "***** Making plot for the limit " << files.at(i) << " *****\n";
	
	//========1. Parsing input (columns of values) to vectors =============	
	std::vector<double> vec_massPoints;
	std::vector<double> vec_obsLimits;
	std::vector<double> vec_expLimits;
	std::vector<double> vec_posTwoSigma;
	std::vector<double> vec_posOneSigma;
	std::vector<double> vec_negOneSigma;
	std::vector<double> vec_negTwoSigma;
	
	std::vector<std::vector<double> > v2;
	
	v2.push_back(vec_massPoints);
	v2.push_back(vec_obsLimits);
	v2.push_back(vec_expLimits);
	v2.push_back(vec_posTwoSigma);
	v2.push_back(vec_posOneSigma);
	v2.push_back(vec_negOneSigma);
	v2.push_back(vec_negTwoSigma);
	
	//Read in all values for 7 columns: mass | obs | exp | 2sigma | 1sigma | -1sigma | -2sigma
	//by function
	cout << "\n Start parsing limit input \n";
	parseInput(inFile, v2);

	//Get back the vectors from the 2D vector v2
	cout << "\n Finish parsing input \n";
	for(int i = 0; i < v2[0].size(); ++i){
	    
	    vec_massPoints.push_back(v2[0][i]);                                                                                        
	    vec_obsLimits.push_back(v2[1][i]);                                                                                         
	    vec_expLimits.push_back(v2[2][i]);                                                                                         
	    vec_posTwoSigma.push_back(v2[3][i]);                                                                                       
	    vec_posOneSigma.push_back(v2[4][i]);                                                                                       
	    vec_negOneSigma.push_back(v2[5][i]);                                                                                       
	    vec_negTwoSigma.push_back(v2[6][i]);  
	    
	    //cout << vec_massPoints.at(i) << "\t" << vec_obsLimits.at(i) << "\t" << vec_expLimits.at(i) << "\t" << vec_posTwoSigma.at(i) <<"\t" << vec_posOneSigma.at(i) << "\t" << vec_negOneSigma.at(i) << "\t" << vec_negTwoSigma.at(i) << "\n";
	    
	}
	
	
	//=============2. Plotting========================
	//---------Define variables for ranging
	double minY = 0.;
	double maxY = *max_element(vec_posTwoSigma.begin(), vec_posTwoSigma.end())*2;
	//	cout << "************ maxY = " << maxY << "***************\n";
	
	//====2.1 Make graph of the observed limits   
	//Draw the empty histogram and then draw the graph using the existing axis from the histogram.
	TH1F *histoCLsObserved1 = new TH1F("histoCLsObserved1","",100,firstBin,lastBin);
	histoCLsObserved1->GetYaxis()->SetRangeUser(minY, maxY);
	histoCLsObserved1->SetDirectory(0);
	histoCLsObserved1->SetStats(0);
	Int_t ci;   // for color index setting
	ci = TColor::GetColor("#000099");
	histoCLsObserved1->SetLineColor(ci);
	histoCLsObserved1->GetXaxis()->SetTitle( ("M_{" + particle + "} [GeV]").c_str());
	histoCLsObserved1->GetXaxis()->SetLabelFont(42);
	histoCLsObserved1->GetXaxis()->SetLabelSize(0.035);
	histoCLsObserved1->GetXaxis()->SetTitleSize(0.035);
	histoCLsObserved1->GetXaxis()->SetTitleFont(42);
	histoCLsObserved1->GetYaxis()->SetTitle( ("#sigma_{" + xs + "}(H#rightarrow " + particle + particle + "#rightarrow 4l)").c_str() );
	histoCLsObserved1->GetYaxis()->SetLabelFont(42);
	histoCLsObserved1->GetYaxis()->SetLabelSize(0.035);
	histoCLsObserved1->GetYaxis()->SetTitleSize(0.035);
	histoCLsObserved1->GetYaxis()->SetTitleFont(42);
	histoCLsObserved1->Draw();
		
	//graph of obs limits vs mass points
	TGraph* graphObs = new TGraph(vec_massPoints.size(), &vec_massPoints[0], &vec_obsLimits[0]);
	graphObs->SetName("CLs_Observed");
	graphObs->SetTitle("Observed CLs");
	graphObs->SetLineWidth(2);
	cout << "graph obs 1 \n";

	graphObs->Draw("al");
	//Pay attention that GetMaximum() or GetMinimum() in TGraph will always return -1111.

	
	//====2.2 Make graphs for the expected limits and the +/- 1 and 2 sigma fluctuations
	TMultiGraph *multigraph = new TMultiGraph();
	multigraph->SetName("CLs_Expected");
	multigraph->SetTitle("Expected Limits");
	
	TGraphAsymmErrors *graphExp2Sigma = new TGraphAsymmErrors();
	graphExp2Sigma->SetName("sig2");
	graphExp2Sigma->SetTitle("Expected CLs #pm 2 #sigma");
	ci = TColor::GetColor("#ffff00");
	graphExp2Sigma->SetFillColor(ci);
	for(int i = 0; i < vec_massPoints.size(); ++i){
	    graphExp2Sigma->SetPoint(i,vec_massPoints.at(i),vec_expLimits.at(i)); 
	    graphExp2Sigma->SetPointError(i,0,0,vec_expLimits.at(i)-vec_negTwoSigma.at(i), vec_posTwoSigma.at(i) - vec_expLimits.at(i));
	}
	multigraph->Add(graphExp2Sigma,"3");
	cout << "Graph 2 sigma \n";
	
	TGraphAsymmErrors *graphExp1Sigma = new TGraphAsymmErrors();
	graphExp1Sigma->SetName("sig1");
	graphExp1Sigma->SetTitle("Expected CLs #pm 1 #sigma");
	ci = TColor::GetColor("#00ff00");
	graphExp1Sigma->SetFillColor(ci);
	for(int i = 0; i < vec_massPoints.size(); ++i){
	    graphExp1Sigma->SetPoint(i,vec_massPoints.at(i),vec_expLimits.at(i)); 
	    graphExp1Sigma->SetPointError(i,0,0,vec_expLimits.at(i)-vec_negOneSigma.at(i), vec_posOneSigma.at(i) - vec_expLimits.at(i));
	}
	multigraph->Add(graphExp1Sigma,"3");
	cout << "Graph 1 sigma \n";
	
	TGraph *graphExp = new TGraph(vec_massPoints.size(), &vec_massPoints[0], &vec_expLimits[0]);
	graphExp->SetName("Exp_Median");
	graphExp->SetTitle("Expected CLs - Median");
	graphExp->SetFillColor(1);
	graphExp->SetLineColor(1);
	graphExp->SetLineStyle(2);
	graphExp->SetLineWidth(2);
	multigraph->Add(graphExp,"L");
	cout << "Graph median \n";
	
	//For SM cross sections
	/*
	TGraph *graphSM = new TGraph(7);
	graphSM->SetName("SM_crosssections");
	graphSM->SetTitle("95%CL upper limit on #sigma_{tot}(H#rightarrow ZdZd #rightarrow 4l)");
	graphSM->SetFillColor(1);
	graphSM->SetLineColor(2);
	graphSM->SetLineStyle(2);
	graphSM->SetLineWidth(2);
	multigraph->Add(graphSM,"L");
	cout << "Graph SM \n";
	*/

	multigraph->Draw("");
	
	
	//==== 2.3 Re-draw the obs graph if unblinded since it's overdrawn, and to ensure the framing of canvas
	
	graphObs->SetHistogram(histoCLsObserved1);
	if(!doBlind)
	    graphObs->Draw("l");
	    
	

	//=======2.4 Making legends =================
	cout << "Making legends \n";
	TLegend *leg = new TLegend(0.65,0.65,0.9,0.9,NULL,"brNDC");
	leg->SetBorderSize(0);
	leg->SetLineColor(0);
	leg->SetLineStyle(0);
	leg->SetLineWidth(0);
	leg->SetFillColor(0);
	leg->SetFillStyle(0);

	TLegendEntry *entry=leg->AddEntry("CLs_Observed", "Observed","l");
	cout << "1st entry \n";
	entry->SetLineColor(1);
	entry->SetLineStyle(2);
	entry->SetLineWidth(2);
	entry->SetMarkerColor(1);
	entry->SetMarkerStyle(21);
	entry->SetMarkerSize(1);
	
	entry=leg->AddEntry("Exp_Median","Expected","l");
	cout << "2nd entry \n";
	entry->SetLineColor(1);
	entry->SetLineStyle(2);
	entry->SetLineWidth(2);
	entry->SetMarkerColor(1);
	entry->SetMarkerStyle(21);
	entry->SetMarkerSize(1);
	
	entry=leg->AddEntry("sig1","#pm 1#sigma","F");
	cout << "3rd entry \n";
	ci = TColor::GetColor("#00ff00");
	entry->SetFillColor(ci);
	entry->SetFillStyle(1001);
	entry->SetLineColor(1);
	entry->SetLineStyle(1);
	entry->SetLineWidth(1);
	entry->SetMarkerColor(3);
	entry->SetMarkerStyle(21);
	entry->SetMarkerSize(1);

	entry=leg->AddEntry("sig2","#pm 2#sigma","F");
	cout << "4th entry \n";
	ci = TColor::GetColor("#ffff00");
	entry->SetFillColor(2);
	entry->SetFillStyle(1001);
	entry->SetLineColor(1);
	entry->SetLineStyle(1);
	entry->SetLineWidth(1);
	entry->SetMarkerColor(1);
	entry->SetMarkerStyle(21);
	entry->SetMarkerSize(1);
	
	leg->Draw("same");
	cout << "Draw legend \n";


	//===Draw ATLAS label, CMS, luminosity, etc
	string strLabel;
	if(doBlind)
	    strLabel = " blinded";
	else
	    strLabel = " unblinded";
	ATLASLabel(0.21,0.85, "Work In Progress",1);
	TLatex l;
	l.SetTextFont(42);
	l.SetTextSize(0.05);
	l.DrawLatex(firstBin+3, maxY*0.75, (cms + " TeV, " + lumi + " fb^{-1}").c_str() );//The value for X and Y corresponding to x-y axis values
	l.DrawLatex(firstBin+3, maxY*0.65, (strLabel + " " + canvs.at(i) + " channel" ).c_str());
	cout << "draw latex \n";


	//===Redraw the axes covered by the multigraph
	gPad->RedrawAxis();

	//==draw shading for low-mass at Quarkonia veto
	Double_t mJpsi = 3.096;
	Double_t mPsi2S = 3.686;
	Double_t mUpsilon1S = 9.461;
	Double_t mUpsilon3S = 10.355;
	
	drawBox(can, mJpsi - 0.25, mPsi2S + 0.3, 0, maxY);
	drawBox(can, mUpsilon1S - 0.7, mUpsilon3S + 0.75, 0, maxY);
	
	
	//===Save Canvas
	std::string saveFold = "plots/" + region + "/" + xs + "/" + particle + "/" + nameTag;
	system( ("mkdir -vp " + saveFold).c_str() );
	std::string saveName = saveFold + "/" + canvs.at(i)+".root";
	cout << "string " << saveName << "\n";
	can->SaveAs(saveName.c_str());  
	cout << "save \n";
	
	//===Delete pointers
	can = NULL;
	delete can;
    }
}


void parseInput(ifstream &inFile, std::vector<std::vector<double> > &v2)
{
    cout << "Inside parsing \n";
    //Read in all values for 7 columns: mass | obs | exp | 2sigma | 1sigma | -1sigma | -2sigma
    int numVectors = v2.size();
    string line;
    TString tmps;

    while( getline(inFile, line) ) {
	//parse line with a stringstream
	cout << line << "\n";

	// if (line.find("nan") != string::npos) //cannot handle nan, or just assign it to 0?
	//     continue;
	if (line.find("#") != string::npos)
	    continue;
	
	string col;
	stringstream ss(line);
	
	for(int iCol = 0; iCol < numVectors; ++iCol){
	    ss >> col;
	    tmps = col;
	    Double_t val = tmps.IsFloat()?tmps.Atof():0;

	    //cout << "\t" << val;
	    //if( (iCol+1) % 7 == 0) 
	    //cout << "\n";
	    
	    v2[iCol].push_back(val);
	}//end for loop columns
    }//end while read file
    
    // for(int i = 0; i < v2[0].size(); ++i){
    // 	for(int iVec = 0; iVec < numVectors; ++iVec){
    // 	    cout << v2[iVec][i] << "\t";
    // 	}
    // 	cout << "\n";
    // }
}

void drawBox(TCanvas *c1, Double_t xl, Double_t xr, Double_t y1, Double_t y2)
{
    c1->SetGrid();
    
    TBox *bv = new TBox(xl, y1, xr, y2);
    bv->SetFillColor(1); bv->SetFillStyle(3352);
    bv->Draw();
    
    // TLine *lv1 = new TLine(xl, y1, xl, y2); lv1->Draw();
    // TLine *lv2 = new TLine(xr, y1, xr, y2); lv2->Draw();
}

