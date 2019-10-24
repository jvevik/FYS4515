{
	gROOT->Reset();
	gROOT->SetStyle("Plain");
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetFillColor(0);
   gStyle->SetPadBorderMode(0);
   m = (TH1F*)gROOT->FindObject("h");
   if (m) m->Delete();
   TCanvas *c1 = new TCanvas("c1","Spincut parameter",600,600);
   TH2F *h = new TH2F("h"," ",10,-0.750000,8.5,50,0.001,    8.5);
   ifstream spincutfile("spincut.cnt");
   float energy[173],spincut[173];
   int i = 0;
   float a0 =  -0.7500;
   float a1 =   0.1000;

	 float Sig1[1]={3.699};
	 float Sig1err[1]={0.3699};
	 float ex1[1]={0.1998};
	 float ex1err[1]={0.0};

	 float Sig2[1]={7.378};
	 float Sig2err[1]={0.7378};
	 float ex2[1]={4.786};
	 float ex2err[1]={0.0};


   float x,y,z;
   while(spincutfile){
   	spincutfile >> x;
	   spincut[i]=x;
	   energy[i]=a0+(a1*i);
	   i++;
   }
	 TGraphErrors *sig1 = new TGraphErrors(1,ex1,Sig1,ex1err,Sig1err);
	 TGraphErrors *sig2 = new TGraphErrors(1,ex2,Sig2,ex2err,Sig2err);
   TGraph *spincutgraph = new TGraph(172,energy,spincut);
   c1->SetLeftMargin(0.14);
   h->GetXaxis()->CenterTitle();
   h->GetXaxis()->SetTitle("Excitation energy E (MeV)");
   h->GetYaxis()->CenterTitle();
   h->GetYaxis()->SetTitleOffset(1.4);
   h->GetYaxis()->SetTitle("Spin cutoff #sigma");
   h->Draw();
   spincutgraph->Draw("L");

	 sig1->SetMarkerStyle(25);
	 sig1->SetMarkerSize(0.8);
	 sig1->Draw("P");
	 sig2->SetMarkerStyle(21);
	 sig2->SetMarkerSize(0.8);
	 sig2->Draw("P");

	 TLegend *leg = new TLegend(0.45,0.3,0.35,0.45);
	 leg->SetBorderSize(0);
	 leg->SetFillColor(0);
	 leg->AddEntry(sig1,"Spin-cutoff lower point #sigma_{1}","P");
	 leg->AddEntry(sig2,"Spin-cutoff higher point #sigma_{2}","P");
	 leg->SetTextSize(0.04);
	 leg->Draw();

   c1->Update();
   c1->Print("spincut.pdf");
   c1->Print("spincut.eps");
   c1->Print("spincut.ps");
}
