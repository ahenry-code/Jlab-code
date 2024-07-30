#include "TImage.h"
#include "TCanvas.h"
#include "TArrayD.h"
#include "TROOT.h"
#include "TColor.h"
#include "TAttImage.h"
#include "TEnv.h"


void img2pad()
{
   TImage *img = TImage::Open("insert png file name here");
   TImage *img2 = TImage::Open("insert png file name here");
   TImage *img3 = TImage::Open("insert png file name here");
   TImage *img4 = TImage::Open("insert png file name here");
   TImage *img5 = TImage::Open("insert png file name here");
   TCanvas *c1 = new TCanvas("Plots", "Plots (2019-2022)", 500, 500);
   
   c1->Divide(3,2);
   c1->cd(1);
   img->Draw();
   c1->cd(2);
   img2->Draw();
   c1->cd(3);
   img3->Draw();
   c1->cd(4);
   img4->Draw();
   c1->cd(5);
   img5->Draw();
}
