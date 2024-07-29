#include "TCanvas.h"
#include "DFCALGeometry.h"
#include "TFile.h"
#include "TTree.h"
#include "TVector2.h"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <math.h>

using namespace std;

void led_mod() {
/* Areas of Investigation
  --------------------------
  27-31 X,Y
  
Close,
X:29 Y:27 LED:2 
X:27 Y:29 LED:2
X:31 Y:29 LED:2
X:29 Y:31 LED:2
X:31 Y:30 LED:5 202208
Far,
X:29 Y:58 LED:2
X:0 Y:29 LED:2
X:58 Y:29 LED:2
X:29 Y:0 LED:2
*/
// These will not work if you dont have the files accessble
    //TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2019-01-ver01.root");
    TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2021-08-ver01.root");
    //TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022-05-ver01.root");
    //TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022-08-ver00.root");
    //TFile * file = new TFile("/work/halld/home/ijaegle/FCAL-LED/FCAL-LED-RunPeriod-2022.root");

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
    
    Tree->GetEntry(5);
    cout << m_peak[30][4] << endl; 
    
    Long64_t nentries = Tree->GetEntries();
    
    
    
    DFCALGeometry *geo = new DFCALGeometry();
    
    int X,LED;
    double Y;
    TString Fit1 = "gaus";
    cout << endl << "LED: ";
    cin >> LED;
    cout << "Fit: ";
    cin >> Fit1;
    cout << endl;
    Y = 27;
    X = 27;
    int pos = 1;
    TCanvas *c1 = new TCanvas("c1","PeakVRun(Corners)",1000,1000);
    c1->Divide(2,2);

 for (int yy = 1; yy < 5; yy ++) {
 cout << yy << endl;
   if (yy == 1) {
    X = 27;
    Y = 31;
    pos = 1;
}
   if (yy == 2) {
    X = 31;
    Y = 31;
    pos = 2;
}
   if (yy == 3) {
    X = 27;
    Y = 27;
    pos = 3;
}
   if (yy == 4) {
    X = 31;
    Y = 27;
    pos = 4;
}


    c1->cd(pos); 
    cout << pos << endl;  
        cout << X << endl;
        cout << Y << endl; 
 
    //cout << Fit1 << endl;
    int C = geo->channel(Y,X);
    std::vector<double> m_peak_entries;
    std::vector<double> m_rnb_entries;
    int counter = 0;
    double sum = 0;
    double mean1 = 0;
    std::sort (m_peak_entries.begin(), m_peak_entries.end());
    
    
    for (Long64_t ii = 0; ii < nentries; ii ++) {
   
      Tree->GetEntry(ii);
      if (m_rnb != 0) {
      if (m_peak[C][LED-1] > 1) {
      	m_peak_entries.push_back(m_peak[C][LED-1]);
      	m_rnb_entries.push_back(m_rnb);
      	counter = counter + 1;
      	//sum = sum + m_peak_entries[0]; 
      	//cout << sum << endl;
      	}
      }
    }
    //mean1 = sum / counter;
    //cout << mean1 << endl;
    //cout << counter << endl;
    double stddev = 0;
    double median = 0;
    for (Long64_t ii = 0; ii < counter; ii ++) {
 //   FOR MEDIAN
 // ------------------  
if (m_peak_entries.size()%2==0) {
	median = (m_peak_entries[m_peak_entries.size()/2]+m_peak_entries[(m_peak_entries.size()/2)-1]/2);
	//cout << median << endl;	
}
else {
	median = (m_peak_entries[trunc(m_peak_entries.size()/2)-1]);
	//cout << median << endl;	
}
//    FOR MEAN
// ----------------

      /*
        m_peak_entries[0] = m_peak_entries[0] + m_peak_entries[0];
        m_peak_entries[0] - mean1;
        m_peak_entries[0] = m_peak_entries[0] * m_peak_entries[0];
      */
        stddev = stddev + pow(m_peak_entries[ii]-median,2);
        //cout << stddev << endl;
    }
    //cout << median << endl;
    //cout << stddev << endl;
    stddev = sqrt(stddev/counter);
    //cout << stddev << endl;


    for (Long64_t ii = 0; ii < counter; ii ++) {


     if ( ( m_peak_entries[ii] > median + 80) || ( m_peak_entries[ii] < median - 80 ) ) {
      m_peak_entries.erase(m_peak_entries.begin()+ii);
      counter = counter - 1;
      }
    }
    
    //m_peak_entries[0] = m_peak_entries[0] / counter;
    //stddev = sqrt(m_peak_entries[0]);
    
   // cout << counter << endl;
    TString CanvasTitle = "Channel_";
    CanvasTitle = CanvasTitle + X;
    CanvasTitle = CanvasTitle + ",";
    CanvasTitle = CanvasTitle + Y;
    
    
    TGraphErrors *channelpeakVrun = new TGraphErrors(counter,&m_rnb_entries[0],&m_peak_entries[0],nullptr,nullptr);
    
    
    
    channelpeakVrun->SetTitle(CanvasTitle);
    channelpeakVrun->GetXaxis()->SetNoExponent(true);
    channelpeakVrun->GetYaxis()->SetNoExponent(true);
    channelpeakVrun->Draw("A*");
    channelpeakVrun->Fit(Fit1);
    channelpeakVrun->SetMarkerSize(0.5);
    c1->Print();
  } 
    int SC;
    cout << "Save?: ";
    cin >> SC;
    cout << endl;
    
    
    if (SC == 1) {
    	//CanvasTitle = CanvasTitle + ".png";
    	c1->SaveAs("test");
    }
}
