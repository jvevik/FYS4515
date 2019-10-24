{
   gROOT->Reset();
   gROOT->SetStyle("Plain");
   gStyle->SetOptTitle(0);
   gStyle->SetOptStat(0);
   gStyle->SetFillColor(0);
   gStyle->SetPadBorderMode(0);
   m = (TH1F*)gROOT->FindObject("h");
   if (m) m->Delete();
   TCanvas *c1 = new TCanvas("c1","Normalization of gamma-transmission coefficient",600,600);
   TH2F *h = new TH2F("h"," ",10,-0.750000,   5.950,50,2.104e+01,7.781e+07);
   ifstream sigfile("sigpaw.cnt");
   float sig[59],sigerr[59];
   float energy[201],energyerr[201];
   float extL[202],extH[202];
   int i;
   float a0 = -0.7500;
   float a1 =  0.1000;
   for(i = 0; i < 61; i++){
   	energy[i] = a0 + (a1*i);
   	energyerr[i] = 0.0;
   	extL[i] = 0.0;
   	extH[i] = 0.0;
   }
   float x, y;
   i = 0;
   while(sigfile){
   	sigfile >> x;
   	if(i<58){
   		sig[i]=x;
   	}
   	else{sigerr[i-58]=x;}
   	i++;
   }
   ifstream extendfile("extendLH.cnt");
   i = 0;
   while(extendfile){
   	extendfile >> x >> y ;
   	extL[i]=x;
   	extH[i]=y;
   	i++;
   }
   TGraph *extLgraph = new TGraph(61,energy,extL);
   TGraph *extHgraph = new TGraph(61,energy,extH);
   TGraphErrors *sigexp = new TGraphErrors(58,energy,sig,energyerr,sigerr);
   c1->SetLogy();
   c1->SetLeftMargin(0.14);
   h->GetXaxis()->CenterTitle();
   h->GetXaxis()->SetTitle("#gamma-ray energy E_{#gamma} (MeV)");
   h->GetYaxis()->CenterTitle();
   h->GetYaxis()->SetTitleOffset(1.4);
   h->GetYaxis()->SetTitle("Transmission coeff. (arb. units)");
   h->Draw();
   sigexp->SetMarkerStyle(21);
   sigexp->SetMarkerSize(0.8);
   sigexp->Draw("P");
   extLgraph->SetLineStyle(1);
   extLgraph->DrawGraph(25,&extLgraph->GetX()[0],&extLgraph->GetY()[0],"L");
   extHgraph->SetLineStyle(1);
   extHgraph->DrawGraph(14,&extHgraph->GetX()[47],&extHgraph->GetY()[47],"L");
   TArrow *arrow1 = new TArrow(1.150e+00,1.029e+04,1.150e+00,1.844e+03,0.02,">");
   arrow1->Draw();
   TArrow *arrow2 = new TArrow(1.650e+00,7.874e+04,1.650e+00,1.411e+04,0.02,">");
   arrow2->Draw();
   TArrow *arrow3 = new TArrow(3.950e+00,5.094e+06,3.950e+00,9.126e+05,0.02,">");
   arrow3->Draw();
   TArrow *arrow4 = new TArrow(4.650e+00,2.869e+07,4.650e+00,5.140e+06,0.02,">");
   arrow4->Draw();
   c1->Update();
   c1->Print("sigext.pdf");
   c1->Print("sigext.eps");
   c1->Print("sigext.ps");
}
