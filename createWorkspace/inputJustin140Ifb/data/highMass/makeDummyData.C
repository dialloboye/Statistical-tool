{
    string channels[3] = {"2e2mu", "4e", "4mu"};
    for (int i=0; i<3; ++i){
	string fName = "data_" + channels[i] + "channel_Zd.root";
	string newFName = "new/data_" + channels[i] + "channel_Zd.root";

	TFile *f = new TFile(fName.c_str(),"READ");
	TFile *fNew = new TFile(newFName.c_str(),"RECREATE");

	TH1D* h = (TH1D*) f->Get("Data");
	h->SetNameTitle("Data","Data");
	fNew->cd();
	h->Write();
	fNew->Close();
	h->Delete();
	f->Delete();
	fNew->Delete();
    }
}
