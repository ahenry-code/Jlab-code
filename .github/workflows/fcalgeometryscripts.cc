#include "TCanvas.h"
#include "DFCALGeometry.h"
#include "TFile.h"
#include "TTree.h"
#include <stdio.h>
#include <iostream>

using namespace std;

void led_mod() {

    TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022-05-ver01.root");
	

    int m_led[2800][5], m_rnb = 0;
    double m_mean[2800][5], m_rms[2800][5], m_integral[2800][5], m_integral_err[2800][5];
    double m_peak[2800][5], m_width[2800][5], m_yield[2800][5], m_chi2[2800][5];
    double m_peak_err[2800][5], m_width_err[2800][5], m_yield_err[2800][5];
    double peak, width, mean;
    double peak_err;
    double max, xmin, xmax;
    int bxmin, bxmax;
    double ratio_peak = 0, ratio_mean = 0;
    double ratio_peak_err = 0;
    
    TTree *Tree = (TTree*)file->Get("LED");

    Tree->SetBranchAddress("m_rnb", &m_rnb);
    Tree->SetBranchAddress("m_led", &m_led);
    Tree->SetBranchAddress("m_mean", &m_mean);
    Tree->SetBranchAddress("m_rms", &m_rms);
    Tree->SetBranchAddress("m_integral", &m_integral);
    Tree->SetBranchAddress("m_integral_err", &m_integral_err);
    Tree->SetBranchAddress("m_peak", &m_peak);
    Tree->SetBranchAddress("m_width", &m_width);
    Tree->SetBranchAddress("m_yield", &m_yield);
    Tree->SetBranchAddress("m_peak_err", &m_peak_err);
    Tree->SetBranchAddress("m_width_err", &m_width_err);
    Tree->SetBranchAddress("m_yield_err", &m_yield_err);
    Tree->SetBranchAddress("m_chi2", &m_chi2);
  
    Long64_t nentries = Tree->GetEntries();
    
    TCanvas *c1 = new TCanvas("c1","posVpeak",650,500);
    
    TH2F *posVpeak = new TH2F("Position Vs Peak","Position Vs Peak",58,0,58,58,0,58);

    
    DFCALGeometry *geo = new DFCALGeometry();
    
    for (Long64_t ii = 0; ii < nentries; ii ++) {
      
      Tree->GetEntry(ii);


      int X,Y;
      for (int jj = 0; jj < 2800; jj++) {
      
      X = geo->column(jj);   
      Y = geo->row(jj);
      
      posVpeak->Fill(X,Y,m_peak[jj][4]);
	if (m_peak[jj][2] < 0) {
	//cout << m_peak[jj][2] << " @" << X << "," << Y << " [" << jj << "]" << endl;
	} 
      }
      
    }
    //gStyle->SetPalette(kAvocado,0);
    posVpeak->Draw("colz");

}
