

void rawMassMerged(){
    
    TString eta_bins[]  = {"0.0", "0.5", "1.0", "1.5", "2.0", "2.5", "3.0", "3.2", "4.7"};
    TString jt[]        = {"40", "80", "140", "200", "260", "320", "400", "450"};
    
    gStyle->SetOptStat(0);
    Double_t w = 1200;
    Double_t h = 1000;
    
    TCanvas * c1 = new TCanvas("c1", "c1", w, h);
    
    //c1->cd();
    c1->SetLogy();
    c1->SetLogx();
    c1->SetTickx(1);
    c1->SetTicky(1);
    
    
    TLegend *leg = new TLegend(0.6994992,0.6619355,0.8731219,0.8722581);
    leg->SetBorderSize(0);
    leg->SetLineColor(0);
    leg->SetLineStyle(1);
    leg->SetLineWidth(0);
    leg->SetFillColor(0);
    leg->SetFillStyle(1001);
    
    TString inputFile1;
    inputFile1.Form("output-DATA-2a.root");
    
    TH1F *h_RAWdjmass[8];
    TH1F *h_djmassRef[8];
    TH1F *h_djmass[8]   ;
    TH1F *h_djmassCut[8];
    
    
    int bin_counter = (sizeof(eta_bins)/sizeof(*eta_bins));
    int counter = (sizeof(jt)/sizeof(*jt));
    
    //-----------------------------| Raw Dijet Mass merged plots |----------------------//
    bool RawMassPlots = false;
    if (RawMassPlots == true) {
        for (int i=0; i<5; i++) {
            c1->cd();
            for (int j=0; j< counter ; j++) {
                
                //open desired files and directories
                TFile* DATAFile = TFile::Open(inputFile1.Data(), "READ");
                DATAFile -> cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i+1] + "/jt" + jt[j] + "/");
                h_RAWdjmass[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
                
                
                
                //Draw raw dijetMasss of jt40 histogram
                if(j == 0){
                    h_RAWdjmass[0] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
                    h_RAWdjmass[0]->SetLineColor(1);
                    h_RAWdjmass[0]->GetXaxis()->SetTitle("M_{jj} (GeV)");
                    h_RAWdjmass[0]->GetYaxis()->SetTitle("A.U.");
                    //h_RAWdjmass[0]->GetXaxis()->SetRangeUser(0,5000.);
                    //h_RAWdjmass[0]->SetMaximum(2e5);
                    //h_RAWdjmass[0]->SetMinimum(1e-6);
                    h_RAWdjmass[0]->GetXaxis()->SetTitleOffset(1.2);
                    h_RAWdjmass[0]->Draw("hist");
                    //h_RAWdjmass[0]->SetTitle("#bf{CMS} Preliminary                              #sqrt{s} = 13 TeV 16.5 fb^{-1}");
                    //h_RAWdjmass[0]->GetTitleAlign(33);
                    //leg->AddEntry(h_RAWdjmass[0],"HLT_PFJet40","l");
                }
                //add other entries to TLegend
                h_RAWdjmass[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
                h_RAWdjmass[j]->SetLineColor(j+1);
                if (j == 2) h_RAWdjmass[j]->SetLineColor(kSpring+5);
                if (j == 4) h_RAWdjmass[j]->SetLineColor(kOrange);
                if (j == 6) h_RAWdjmass[j]->SetLineColor(kCyan+1);
                if (j == 7) h_RAWdjmass[j]->SetLineColor(kGreen-2);
                //h_RAWdjmass[j]->GetXaxis()->SetRangeUser(0, 4500.);
                
                //Draw raw dijetMass of jt>40 histograms on the same canvas with jt40
                if (j>0) h_RAWdjmass[j]->Draw("hist same");
                
                //TLegend Settings
                leg->AddEntry(h_RAWdjmass[j],"HLT_PFJet"+jt[j],"l");
                
            }//j end of jet loop
            
            leg->Draw();
            
            //TLatex
            TLatex ltitle;
            ltitle.SetTextSize(0.04);
            ltitle.DrawLatex(10.1288, 434749, "CMS #bf{Preliminary}");
            
            //TLatex ltitle2;
            //ltitle2.SetTextSize(0.035);
            //ltitle2.DrawLatex(487.923, 434749, "16.5 fb^{-1}  #sqrt{s} = 13 TeV");
            
            TLatex lt;
            lt.SetTextSize(0.025);
            lt.DrawLatex(13.0079,30896.4,"#bf{RunF_{Late}GH}");
            lt.DrawLatex(13.0079,7733.95,eta_bins[i]+" < #eta < "+eta_bins[i+1]);
            lt.DrawLatex(13.0079,1600,"#bf{#sqrt{s} = 13 TeV 16.5 fb^{-1}}");
            lt.SetTextAlign(12);
            
            c1->SaveAs("RawMassSpectrum_"+eta_bins[i]+"-"+eta_bins[i+1]+"_.pdf");
            c1->Update();
            leg->Clear(); //After listing one turn of eta clear legend otherwise all eta legends will be stacked together
            
        }//i end of eta loop for RAW dijet mass
    }// raw mass boolean
    
    
    
    //------------------------/ get Turn-On point values wrt corresponding HLT_PFJet /------------------------------//
    
    double turnOn[8][7];
    TString etaJet[8][7];
    float FitPar[8][7];
    bool printTurnON = true;

    TCanvas * c2 = new TCanvas("c2", "c2", w, h);

    
    for (int i=0; i< 5; i++) {
        
        
        TString XaxisName;
        
        for (int j=0; j<7; j++) {
            c2->cd();
            
            //open desired files and directories
            TFile* DATAFile = TFile::Open(inputFile1.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i+1] + "/jt" + jt[j] + "/");
            h_djmassRef[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
            
            DATAFile -> cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i+1] + "/jt" + jt[j+1] + "/");
            h_djmass[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
            
            TString xname = (TString)("jt"+jt[j+1]+"/jt"+jt[j]);
            
            h_djmass[j]->Divide(h_djmassRef[j]);
            
            
            //cout << i << endl;
            
            //fit parameter optimisation
            
            FitPar[i][j]=1.55;
            if ((i == 0) && j == 1) FitPar[i][j] = 1.7;     //jt140 0.0-0.5
            if ((i == 0) && j == 2) FitPar[i][j] = 1.7;     //jt200
            if ((i == 0) && j == 3) FitPar[i][j] = 1.7;     //jt260
            if ((i == 0) && j == 4) FitPar[i][j] = 1.85;    //jt320
            if ((i == 0) && j == 5) FitPar[i][j] = 1.85;    //jt400
            if ((i == 0) && j == 6) FitPar[i][j] = 1.85;    //jt450
            if (i == 1) FitPar[i][j] = 1.7;                 //      0.5-1.0
            if ((i == 1) && j > 2) FitPar[i][j] = 1.9;
            if (i == 2) FitPar[i][j] = 2.0;                 //      1.0-1.5
            if ((i == 2) && j == 2) FitPar[i][j] = 2.5;     //jt200
            if ((i ==2) && (j >= 3)) FitPar[i][j] = 2.8;
            if (i == 3) FitPar[i][j] = 3.0;                 //      1.5-2.0
            if ((i == 3) && (j == 1)) FitPar[i][j] = 3.4;   //jt140
            if ((i == 3) && (j == 2)) FitPar[i][j] = 3.8;   //jt200
            if ((i == 3) && (j == 3)) FitPar[i][j] = 4.1;   //jt260
            if ((i == 3) && (j == 4)) FitPar[i][j] = 4.4;   //jt320
            if ((i == 3) && (j == 5)) FitPar[i][j] = 4.3;   //jt400
            if ((i == 3) && (j == 6)) FitPar[i][j] = 5.0;   //jt450
            if (i == 4 ) FitPar[i][j] = 4.3;                //jt80 2.0-2.5
            if ((i == 4) && (j == 1) ) FitPar[i][j] = 5.9;  //jt140
            if ((i == 4) && (j == 2) ) FitPar[i][j] = 5.74; //jt200
            if ((i == 4) && (j == 3) ) FitPar[i][j] = 7.3;  //jt260
            if ((i == 4) && (j == 4) ) FitPar[i][j] = 5.9;  //jt320
            if ((i == 4) && (j ==5) ) FitPar[i][j] = 5.21;   //jt400
            if ((i == 4) && (j ==6) ) FitPar[i][j] = 5.5;   //jt450
            
            //Fit function
            TF1 *f1 = new TF1("f1","0.5*(1+TMath::Erf((x-[0])/[1]))",atoi(jt[j+1])*FitPar[i][j],8000.);
            
            f1->SetParameters(2*atoi(jt[j+1])*cosh(0.5),atoi(jt[j+1])*2*0.1);
            f1->SetLineWidth(1);
            h_djmass[j]->Fit(f1,"R");
            f1->GetX(0.99);
            
            turnOn[i][j] = f1->GetX(0.99);
            XaxisName = (TString) ("eta_"+eta_bins[i]+"-"+eta_bins[i+1]+"_jt"+jt[j+1]+"/jt"+jt[j]);
            etaJet[i][j] = XaxisName;
            //cout << foo[j-1]<< endl;
            
            
            if (printTurnON == true){
                
                
                
                h_djmass[j]->GetYaxis()->SetRangeUser(0., 1.3);
                h_djmass[j]->GetXaxis()->SetTitle("M_{"+xname+"} (GeV)");
                h_djmass[j]->GetYaxis()->SetTitle("Efficiency");
                h_djmass[j]->GetXaxis()->SetTitleOffset(1.2);
                h_djmass[j]->Draw();
                
                
                
                if (i < 4) {
                    h_djmass[j]->GetXaxis()->SetRangeUser(0., 4000.);
                    //Lines from point to each axis
                    TLine *line_h = new TLine(0.,1.,4000.,1.);
                    line_h->SetLineColor(kBlack);
                    line_h->SetLineStyle(2);
                    line_h->SetLineWidth(1);
                    line_h->Draw();
                } else {
                    h_djmass[j]->GetXaxis()->SetRangeUser(0., 8000.);
                    //Lines from point to each axis
                    TLine *line_h = new TLine(0.,1.,8000.,1.);
                    line_h->SetLineColor(kBlack);
                    line_h->SetLineStyle(2);
                    line_h->SetLineWidth(1);
                    line_h->Draw();
                }
                
                TLine *line_v = new TLine(f1->GetX(0.99), 0., f1->GetX(0.99), 1.);
                line_v->SetLineColor(kBlack);
                line_v->SetLineStyle(2);
                line_v->SetLineWidth(1);
                line_v->Draw();
                
                //Latex
                TLatex l;
                l.SetTextSize(0.025);
                //l.SetTextAngle(30.);
                l.DrawLatex(1320.114,0.296371,Form("TurnOn Point = %f",f1->GetX(0.99)));
                l.DrawLatex(1320.114,0.371371,Form("%s"+jt[j+1],"jt"));
                l.DrawLatex(1320.114,0.441533,Form(eta_bins[i] + "%s" + eta_bins[i+1]," < #||{#eta} < "));
                
                
                
                c2->SaveAs("eta_" + eta_bins[i] + "-" + eta_bins[i+1] + "_jt"+jt[j+1] + "_.pdf");
                c2->Update();
            }//print trun on
        }// j of turON loop
        
    }// i of TurnOn plots
    
    
    //--------------------------|| cut the right and left of massSpectrum wrt turnOn points ||--------------------------------//
    
    
        for (int i=0; i<5; i++) {
        TCanvas * c3 = new TCanvas("c3", "c3", w, h);
        c3->SetLogy();
        c3->SetLogx();
        c3->SetTickx(1);
        c3->SetTicky(1);
        
        for (int j=0; j<counter; j++) {
            
            //open desired files and directories
            TFile* DATAFile = TFile::Open(inputFile1.Data(), "READ");
            DATAFile -> cd("Standard/Eta_" + eta_bins[i] + "-" + eta_bins[i+1] + "/jt" + jt[j] + "/");
            h_djmassCut[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
            
            // cut the right and left
            int ncells = h_djmassCut[j]->GetSize(); //it adds undrfolw and overflow bins so I have to extract those
            int Data_Nbins = ncells-2;
            
            for (int k=1; k<=Data_Nbins; k++) {
                double check_bins = h_djmassCut[j]->GetBinContent(k);
                double check_err  = h_djmassCut[j]->GetBinError(k);
                
                if (check_bins != 0){
                    int binmax = h_djmassCut[0]->GetMaximumBin();
                    double x = h_djmassCut[0]->GetXaxis()->GetBinCenter(binmax);
                    double Xaxis_val = h_djmassCut[0]->GetBinCenter(k);
                    if ((j==0) && (Xaxis_val < x)){
                        h_djmassCut[0]->SetBinContent(k, 0.);
                        h_djmassCut[0]->SetBinError(k, 0.);
                    }// cut left side of jt40
                    
                    if ((k<h_djmassCut[j]->GetXaxis()->FindBin(turnOn[i][j-1])) && (j>0)){
                        h_djmassCut[j]->SetBinContent(k, 0.);
                        h_djmassCut[j]->SetBinError(k, 0.);
                    }//cut leftside >jt40
                    
                    if ((k>=h_djmassCut[j]->GetXaxis()->FindBin(turnOn[i][j])) && (j<7)){
                        h_djmassCut[j]->SetBinContent(k, 0.);
                        h_djmassCut[j]->SetBinError(k, 0.);
                    }//cut right side <jt450
                }//check bin
            }//k
            
            //Draw  dijetMasss of jt40 histogram after cut
            if(j == 0){
                h_djmassCut[0] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
                h_djmassCut[0]->SetLineColor(1);
                h_djmassCut[0]->GetXaxis()->SetTitle("M_{jj} (GeV)");
                h_djmassCut[0]->GetYaxis()->SetTitle("A.U.");
                h_djmassCut[0]->GetXaxis()->SetRangeUser(h_djmassCut[0]->GetMaximumBin(),10000);
                h_djmassCut[0]->SetMaximum(2e5);
                h_djmassCut[0]->SetMinimum(1e-6);
                h_djmassCut[0]->GetXaxis()->SetTitleOffset(1.2);
                h_djmassCut[0]->Draw();
            }
            
            h_djmassCut[j] = (TH1F*)gDirectory->FindObjectAny("hdjmass");
            h_djmassCut[j]->SetLineColor(j+1);
            h_djmassCut[j]->SetMarkerStyle(j+20);
            h_djmassCut[j]->SetMarkerColor(j+1);
            //h_djmassCut[j]->SetMarkerSize(2);
            if (j == 2) h_djmassCut[j]->SetLineColor(kSpring+5);
            if (j == 2) h_djmassCut[j]->SetMarkerColor(kSpring+5);
            if (j == 4) h_djmassCut[j]->SetLineColor(kOrange);
            if (j == 4) h_djmassCut[j]->SetMarkerColor(kOrange);
            if (j == 6) h_djmassCut[j]->SetLineColor(kCyan+1);
            if (j == 6) h_djmassCut[j]->SetMarkerColor(kCyan+1);
            if (j == 7) h_djmassCut[j]->SetLineColor(kGreen-2);
            if (j == 7) h_djmassCut[j]->SetMarkerColor(kGreen-2);
            //h_RAWdjmass[j]->GetXaxis()->SetRangeUser(0, 4500.);
            
            //Draw raw dijetMass of jt>40 histograms on the same canvas with jt40
            if (j>0) h_djmassCut[j]->Draw("same");
            
            //TLegend Settings
            leg->AddEntry(h_djmassCut[j],"HLT_PFJet"+jt[j],"p");
            
            
        }//j of cut
        leg->AddEntry("","","");
        leg->AddEntry("","RunBCDEFGH","");
        leg->AddEntry("",eta_bins[i]+" < y < "+eta_bins[i+1],"");
        leg->AddEntry("","#sqrt{s} = 13 TeV 16.5 fb^{-1}","");
        leg->Draw();
        
//        //TLatex
//        TLatex ltitle;
//        ltitle.SetTextSize(0.04);
//        ltitle.DrawLatex(10.1288, 434749, "CMS #bf{Preliminary}");
//        
//        TLatex lt;
//        lt.SetTextSize(0.025);
//        lt.DrawLatex(13.0079,30896.4,"#bf{RunF_{Late}GH}");
//        lt.DrawLatex(13.0079,7733.95,eta_bins[i]+" < #eta < "+eta_bins[i+1]);
//        lt.DrawLatex(13.0079,1600,"#bf{#sqrt{s} = 13 TeV 16.5 fb^{-1}}");
//        lt.SetTextAlign(12);
        
        c3->SaveAs("MassSpectrum_"+eta_bins[i]+"-"+eta_bins[i+1]+"_.pdf");
        c3->Update();
        leg->Clear(); //After listing one turn of eta clear legend otherwise all eta legends will be stacked together
    }//i of MassSpectrum after cutting right and left
    
    //cout turnOn value, fit parameter optimisation parameter, etc...information etaBYeta at the end together
    cout << "====================================================="<< endl;
    for (int n=0; n<5; n++) {
        
        for (int m=0; m<7; m++) {
            cout << etaJet[n][m] << " TurnOn point = " << turnOn[n][m]<< " Fit_Par =" << FitPar[n][m] << endl;
        }
        cout << "====================================================="<< endl;
    }
    
}


