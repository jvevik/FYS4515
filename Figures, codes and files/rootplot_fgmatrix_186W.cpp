//////////////////////////////////////////////////////
//	Script to plot MAMA files in ROOT				//
//	Here, the f.g. matrix of 186W from the  		//
//	186W(a,a'g)186W data 							//
//	Date: 16 Oct 2019								//
//	Cecilie											//
//////////////////////////////////////////////////////
{

	// Some root stuff
	gROOT->SetStyle("Plain");
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);
	gStyle->SetFillColor(1);


	// Input files
	ifstream  firstgen_file("compressedpartitionfg");

	// Various declarations
	string line;
	string cal_dummy;
	string dim_dummy;
	int dim;
	int dim_start;
	int dim_stop;
	int dim_size;
	int position;
	int file_length;
	line.resize(512);	// need long enough line to read MAMA headers
	double x_cal[3] = {0.,1.,0.};	// calibration coeffs. on x axis: a0, a1, a2
	double y_cal[3] = {0.,1.,0.};	// calibration coeffs. on y axis: a0, a1, a2
	int dx, dy;	// dimension on x and y axis
	int ix, iy;
	double value;
	double x,y;
	double number_of_counts = 0.;
	double new_y1, new_y2;


	// Read MAMA header (fixed format). The 10 first lines are info text
	if(!getline(firstgen_file,line) || line.substr(0,10) != "!FILE=Disk"){	// check correct format
		printf("\n This is not a MAMA file!!!\n ");
		exit(2);
	}
	getline(firstgen_file,line);	// skip !KIND=Spectrum
	getline(firstgen_file,line);	// skip !LABORATORY=Oslo Cyclotron Laboratory (OCL)
	getline(firstgen_file,line);	// skip !EXPERIMENT=mama
	getline(firstgen_file,line);	// skip !COMMENT=Sorted simulated data
	getline(firstgen_file,line);	// skip !TIME=DATE:    19/11/09 11:47:26
	getline(firstgen_file,line);	// get line with calibration
	cout << "\n Reading calibration coeffs.:" << endl;
	// calibration on x axis
	cal_dummy = line.substr(20,13);	// position 20, length 13 characters
	if(!(istringstream(cal_dummy) >> x_cal[0])) cout << "Could not convert string to number." << endl;
	else cout << " Calibration coeff. a0 on x axis is: " << x_cal[0] << " keV." << endl;
	cal_dummy = line.substr(34,13);
	if(!(istringstream(cal_dummy) >> x_cal[1])) cout << "Could not convert string to number." << endl;
	else cout << " Calibration coeff. a1 on x axis is: " << x_cal[1] << " keV/ch." << endl;
	cal_dummy = line.substr(48,13);
	if(!(istringstream(cal_dummy) >> x_cal[2])) cout << "Could not convert string to number." << endl;
	else cout << " Calibration coeff. a2 on x axis is: " << x_cal[2] << " (keV/ch)^2." << endl;
	// calibration on y axis
	cal_dummy = line.substr(62,13);
	if(!(istringstream(cal_dummy) >> y_cal[0])) cout << "Could not convert string to number." << endl;
	else cout << " Calibration coeff. a0 on y axis is: " << y_cal[0] << " keV." << endl;
	cal_dummy = line.substr(76,13);
	if(!(istringstream(cal_dummy) >> y_cal[1])) cout << "Could not convert string to number." << endl;
	else cout << " Calibration coeff. a1 on y axis is: " << y_cal[1] << " keV/ch." << endl;
	cal_dummy = line.substr(90,13);
	if(!(istringstream(cal_dummy) >> y_cal[2])) cout << "Could not convert string to number." << endl;
	else cout << " Calibration coeff. a2 on y axis is: " << y_cal[2] << " (keV/ch)^2.\n" << endl;
	getline(firstgen_file,line);	// skip !PRECISION=16
	getline(firstgen_file,line);	// get dimension
	// dimension of matrix
	dim_start = line.find_first_of("=") + 1;
	dim_dummy = line.substr(dim_start,1);
	if(!(istringstream(dim_dummy) >> dim)) cout << "Could not convert string to number." << endl;
	else cout << " Dimension of matrix is: " << dim << endl;
	getline(firstgen_file,line);	// get channels
	// dimension on x axis
	dim_start = line.find_first_of(":") + 1;
	dim_stop = line.find_last_of(",");
	dim_size = dim_stop - dim_start;
	dim_dummy = line.substr(dim_start,dim_size);
	if(!(istringstream(dim_dummy) >> dx)) cout << "Could not convert string to number." << endl;
	else cout << " Dimension on x axis is: " << dx+1 << " ch." << endl;
	dx = dx+1;
	// dimension on y axis
	dim_start = line.find_last_of(":");
	dim_stop = line.find_last_of(")");
	dim_size = dim_stop - dim_start;
	dim_dummy = line.substr(dim_start+1,dim_size-1);
	if(!(istringstream(dim_dummy) >> dy)) cout << "Could not convert string to number." << endl;
	else cout << " Dimension on y axis is: " << dy+1 << " ch." << endl;
	dy = dy+1;


	// Make histogram
	// MeV on the x,y axis
    TH2D *matrix = new TH2D("matrix"," ",dx,(x_cal[0]/1000.),(dx*x_cal[1]+x_cal[0])/1000.,dy,(y_cal[0]/1000.),(dy*y_cal[1]+y_cal[0])/1000.);
	matrix->SetOption("colz");
	// Fill matrix
	for(iy=0;iy<dy;iy++){
		for(ix=0;ix<dx;ix++){
			firstgen_file >> value;
			number_of_counts += value;
			matrix->SetBinContent(ix,iy,value); // Divide down by 100000. to get back to the normalized response matrix
		}
	}


	cout << " 186W f.g. matrix is now filled." << endl;
	cout << " Total number of counts in the matrix: " << number_of_counts << endl;
	cout << " Closing file."  << endl;
	// close file
	firstgen_file.close();

	// Create TCanvas
	TCanvas *c1 = new TCanvas("c1","c1: First-gen. matrix for 186W",800,650);

    c1->cd();
	gPad->SetLogz();
	gPad->SetLeftMargin(0.13);
    gPad->SetBottomMargin(0.14);
	//gPad->SetTopMargin(0.1);
	//gPad->SetRightMargin(0.1);
	//matrix->Rebin2D(2,2);
	
	matrix->GetXaxis()->SetTitle("E_{#gamma} (MeV)");
    //matrix->GetXaxis()->CenterTitle();
    matrix->GetXaxis()->SetTitleFont(42);
    matrix->GetXaxis()->SetTitleSize(0.05);
    matrix->GetXaxis()->SetLabelFont(42);
    matrix->GetXaxis()->SetLabelSize(0.045);
	matrix->GetXaxis()->SetTitleOffset(1.);
	matrix->GetYaxis()->SetTitle("E_{x} (MeV)");
	matrix->GetYaxis()->SetTitleOffset(1.);
    matrix->GetYaxis()->SetTitleSize(0.05);
    //matrix->GetYaxis()->CenterTitle();
    matrix->GetYaxis()->SetTitleFont(42);
    matrix->GetYaxis()->SetLabelFont(42);
    matrix->GetYaxis()->SetLabelSize(0.045);
	// Set user range in the matrix if you want to
	matrix->GetXaxis()->SetRangeUser(0.0,8.200);
	matrix->GetYaxis()->SetRangeUser(0.0,8.200);
    //matrix->GetZaxis()->SetRangeUser(0.,0.05);
    matrix->GetZaxis()->SetLabelFont(42);
    matrix->GetZaxis()->SetLabelSize(0.045);
    matrix->GetZaxis()->SetTitleFont(42);
    //matrix->GetZaxis()->SetTitle("Counts ");

	matrix->Draw("COLZ");

	TLine *line1 = new TLine(0.,0.,8.2,8.2); // (x1,y1,x2,y2)
	line1->Draw();

	TLine *line_Sn = new TLine(0.,4.786,4.786,4.786);
	line_Sn->SetLineColor(kAzure+3);
	line_Sn->SetLineStyle(2);
	line_Sn->SetLineWidth(4);
	line_Sn->Draw();

    c1->Update();

    TPaletteAxis *palette = (TPaletteAxis*)matrix->GetListOfFunctions()->FindObject("palette");
    if(!palette) cout << "nono." << endl;
    palette->SetX1NDC(0.90);
    palette->SetX2NDC(0.925);
    palette->Draw();

    TLegend *legend = new TLegend(0.4,0.2,0.8,0.3); // In normalized coordinates (0,0,1,1)= (x1,y1,x2,y2) is the size of the canvas.
   	legend->SetTextFont(42);
   	legend->SetTextSize(0.035);
   	legend->SetFillColor(0);
   	legend->SetBorderSize(0);	// Set to 1 if you want a line around the box
   	legend->AddEntry(line1," E_{x} = E_{#gamma}","L");
   	legend->AddEntry(line_Sn," Neutron separation energy S_{n}","L");
   	legend->Draw();

    c1->Update();
    c1->Print("compressedpartitionfg.png");

    // Write the matrix into a Root file
    TFile *rootfile = new TFile("compressedpartitionfg.root","recreate");
    matrix->Write();
    c1->Write();
    rootfile->Close();



}	// END of script
