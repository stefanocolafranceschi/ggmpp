{
//g++ -g -w -c -O -fbounds-check -pthread -std=c++11 -m64 -I/usr/include/root -ggdb -fPIC -DLINUX -Wall -O0  -I. -c monitor.c -o monitor.o #-std=c++0x
//g++ -g -ggdb -Wall -s -O0 -o monitor monitor.o -l CAENVME -l curses -L/usr/lib64/root -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTDataFrame -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic -L/usr/lib64/root -lGui -lCore -lImt -lRIO -lNet -lHist -lGraf -lGraf3d -lGpad -lROOTDataFrame -lROOTVecOps -lTree -lTreePlayer -lRint -lPostscript -lMatrix -lPhysics -lMathCore -lThread -lMultiProc -pthread -lm -ldl -rdynamic
   // Setting plots range and style
   int temp_bin=200;
   int temp_min=15;
   int temp_max=25;

   int p_bin=200;
   int p_min=920;
   int p_max=980;

   int rh_bin=200;
   int rh_min=-5;
   int rh_max=65;

   int histo_bin=200;
   int histo_min=0;
   int histo_max=10000;

   int eff_bin=200;
   float eff_min=0;
   float eff_max=100;

   int charge_bin=200;
   float charge_min=0;
   float charge_max=500;

   int avalanche_bin=200;
   float avalanche_min=0;
   float avalanche_max=1000;

   int streamer_bin=200;
   float streamer_min=0;
   float streamer_max=1000;

   float label_font=0.025;
   float axis_offset=0.9;
   float time_bin=5;
   float label_size=0.03;
   float ref;

   //////////
   float wp_min=0;
   float wp_max=1.8;
   int wp_bin=200; 
   int std_ref=2;
   int unixtime_bin=100;
                
   time_t now;
   struct tm  ts;
   time(&now);
                
   ts = *localtime(&now);
   int finish = (int)static_cast<long int> (now);
   int start= (int)finish-604800;

   gStyle->SetMarkerSize(1);
   gStyle->SetMarkerStyle(20);
   gStyle->SetMarkerColor(0);
   gStyle->SetOptStat(0);

   // Text positioning
   Double_t x1=0.9,y1=0.6,x2=x1+0.09,y2=0.63;
/*
   TFile *f = new TFile("/ggmdata/DATA/dst.root");
   TTree *GGMDST = (TTree*)f->Get("GGMDST");
   TString Filter;
*/
   // Defining variable already present in the summary file
   Int_t unixtime;
   Float_t           Ravalanche2, Ravalanche3, Ravalanche4, Ravalanche5, Ravalanche6, Ravalanche7, Ravalanche8, Ravalanche9;
   Float_t           Rstreamer2, Rstreamer3, Rstreamer4, Rstreamer5, Rstreamer6, Rstreamer7, Rstreamer8, Rstreamer9;
   Float_t           Refficiency2, Refficiency3, Refficiency4, Refficiency5, Refficiency6, Refficiency7, Refficiency8, Refficiency9;
   Float_t           Rcharge2, Rcharge3, Rcharge4, Rcharge5, Rcharge6, Rcharge7, Rcharge8, Rcharge9;
   Float_t           Rchisquare2, Rchisquare3, Rchisquare4, Rchisquare5, Rchisquare6, Rchisquare7, Rchisquare8, Rchisquare9;

   // Opening the summary file
   std::ifstream InFile;
   InFile.open("/ggmdata/DATA/database.txt");

   TFile *hfile = 0;
   hfile = TFile::Open("/ggmdata/DATA/db.root","RECREATE");

   // Tree and Branch definitions
   TTree *GGMDST = new TTree("GGMDST","GGMDST");
   GGMDST->Branch("unixtime",&unixtime,"unixtime/F");
   GGMDST->Branch("avalanche2",&Ravalanche2,"Ravalanche2/F");
   GGMDST->Branch("avalanche3",&Ravalanche3,"Ravalanche3/F");
   GGMDST->Branch("avalanche4",&Ravalanche4,"Ravalanche4/F");
   GGMDST->Branch("avalanche5",&Ravalanche5,"Ravalanche5/F");
   GGMDST->Branch("avalanche6",&Ravalanche6,"Ravalanche6/F");
   GGMDST->Branch("avalanche7",&Ravalanche7,"Ravalanche7/F");
   GGMDST->Branch("avalanche8",&Ravalanche8,"Ravalanche8/F");
   GGMDST->Branch("avalanche9",&Ravalanche9,"Ravalanche9/F");
   GGMDST->Branch("streamer2",&Rstreamer2,"Rstreamer2/F");
   GGMDST->Branch("streamer3",&Rstreamer3,"Rstreamer3/F");
   GGMDST->Branch("streamer4",&Rstreamer4,"Rstreamer4/F");
   GGMDST->Branch("streamer5",&Rstreamer5,"Rstreamer5/F");
   GGMDST->Branch("streamer6",&Rstreamer6,"Rstreamer6/F");
   GGMDST->Branch("streamer7",&Rstreamer7,"Rstreamer7/F");
   GGMDST->Branch("streamer8",&Rstreamer8,"Rstreamer8/F");
   GGMDST->Branch("streamer9",&Rstreamer9,"Rstreamer9/F");
   GGMDST->Branch("charge2",&Rcharge2,"Rcharge2/F");
   GGMDST->Branch("charge3",&Rcharge3,"Rcharge3/F");
   GGMDST->Branch("charge4",&Rcharge4,"Rcharge4/F");
   GGMDST->Branch("charge5",&Rcharge5,"Rcharge5/F");
   GGMDST->Branch("charge6",&Rcharge6,"Rcharge6/F");
   GGMDST->Branch("charge7",&Rcharge7,"Rcharge7/F");
   GGMDST->Branch("charge8",&Rcharge8,"Rcharge8/F");
   GGMDST->Branch("charge9",&Rcharge9,"Rcharge9/F");
   GGMDST->Branch("efficiency2",&Refficiency2,"Refficiency2/F");
   GGMDST->Branch("efficiency3",&Refficiency3,"Refficiency3/F");
   GGMDST->Branch("efficiency4",&Refficiency4,"Refficiency4/F");
   GGMDST->Branch("efficiency5",&Refficiency5,"Refficiency5/F");
   GGMDST->Branch("efficiency6",&Refficiency6,"Refficiency6/F");
   GGMDST->Branch("efficiency7",&Refficiency7,"Refficiency7/F");
   GGMDST->Branch("efficiency8",&Refficiency8,"Refficiency8/F");
   GGMDST->Branch("efficiency9",&Refficiency9,"Refficiency9/F");
   GGMDST->Branch("chisquare2",&Rchisquare2,"Rchisquare2/F");
   GGMDST->Branch("chisquare3",&Rchisquare3,"Rchisquare3/F");
   GGMDST->Branch("chisquare4",&Rchisquare4,"Rchisquare4/F");
   GGMDST->Branch("chisquare5",&Rchisquare5,"Rchisquare5/F");
   GGMDST->Branch("chisquare6",&Rchisquare6,"Rchisquare6/F");
   GGMDST->Branch("chisquare7",&Rchisquare7,"Rchisquare7/F");
   GGMDST->Branch("chisquare8",&Rchisquare8,"Rchisquare8/F");
   GGMDST->Branch("chisquare9",&Rchisquare9,"Rchisquare9/F");

   // Scanning the dst file and filling the Tree
   std::cout << " -- Reading the file " << std::endl;
   while ( InFile >> unixtime >> Ravalanche2>> Ravalanche3>> Ravalanche4>> Ravalanche5>> Ravalanche6>> Ravalanche7>> Ravalanche8>> Ravalanche9>> Rstreamer2>> Rstreamer3>> Rstreamer4>> Rstreamer5>> Rstreamer6>> Rstreamer7>> Rstreamer8>> Rstreamer9>> Rcharge2>> Rcharge3>> Rcharge4>> Rcharge5>> Rcharge6>> Rcharge7>> Rcharge8>> Rcharge9>> Refficiency2>> Refficiency3>> Refficiency4>> Refficiency5>> Refficiency6>> Refficiency7>> Refficiency8>> Refficiency9>> Rchisquare2>> Rchisquare3>> Rchisquare4>> Rchisquare5>> Rchisquare6>> Rchisquare7>> Rchisquare8>> Rchisquare9) {
      std::cout << " unixtime=" << unixtime << " " <<Ravalanche2<< " " << Ravalanche3 << std::endl;
      GGMDST->Fill();
   }
   //cout << " " << endl;
   GGMDST->Write();


   ///////////////////////////////////
   ////// *Anodic Charge Plot*  //////
   ///////////////////////////////////
   
   TH2F *charge1 = new  TH2F("charge1","charge1",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);  
   charge1->SetMarkerSize(0.7);
   charge1->SetMarkerStyle(20);
   charge1->SetMarkerColor(801);//797
   
   TH2F *charge2 = new  TH2F("charge2","charge2",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);  
   charge2->SetMarkerSize(0.7);
   charge2->SetMarkerStyle(20);
   charge2->SetMarkerColor(600);//600
   
   TH2F *charge3 = new  TH2F("charge3","charge3",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);  
   charge3->SetMarkerSize(0.7);
   charge3->SetMarkerStyle(20);
   charge3->SetMarkerColor(416);//812
  
   TH2F *charge4 = new  TH2F("charge4","charge4",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);  
   charge4->SetMarkerSize(0.7);
   charge4->SetMarkerStyle(20);
   charge4->SetMarkerColor(922);//904
   
   TH2F *charge5 = new  TH2F("charge5","charge5",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);  
   charge5->SetMarkerSize(0.7);
   charge5->SetMarkerStyle(20);
   charge5->SetMarkerColor(435);//867
   
   TH2F *charge6 = new  TH2F("charge6","charge6",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);  
   charge6->SetMarkerSize(0.7);
   charge6->SetMarkerStyle(20);
   charge6->SetMarkerColor(632);//632
      
   TH2F *charge7 = new  TH2F("charge7","charge7",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);  
   charge7->SetMarkerSize(0.7);
   charge7->SetMarkerStyle(20);
   charge7->SetMarkerColor(901);//803
  
   TH2F *charge8 = new  TH2F("charge8","charge8",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);  
   charge8->SetMarkerSize(0.7);
   charge8->SetMarkerStyle(20);
   charge8->SetMarkerColor(1);
   

   TCanvas *Chambers_AQ = new TCanvas("Chambers_AQ","GGM Sys Chambers Anodic Charge",0,0,1500,900);
   TH2F *frame = new  TH2F("frame","frame",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);  
   frame->SetTitle("GGM System Anodic Charge");
   frame->GetXaxis()->SetTitleOffset(axis_offset);
   frame->GetXaxis()->SetTimeDisplay(1); 
   frame->GetXaxis()->SetTimeFormat("%Y %m %d %H:%M %F 1970-01-01 00:00:00"); 
   frame->GetXaxis()->SetNdivisions(time_bin); 
   frame->GetXaxis()->SetLabelSize(label_size);
   frame->GetXaxis()->SetTitle("time");
   frame->GetYaxis()->SetTitleOffset(axis_offset);
   frame->GetYaxis()->SetLabelSize(label_size);
   frame->GetYaxis()->SetTitle("Charge [pC]");
   gPad->SetGrid();

   GGMDST->Draw("charge2:unixtime>>charge1","","same");
   GGMDST->Draw("charge3:unixtime>>charge2","","same");
   GGMDST->Draw("charge4:unixtime>>charge3","","same");
   GGMDST->Draw("charge5:unixtime>>charge4","","same");
   GGMDST->Draw("charge6:unixtime>>charge5","","same");
   GGMDST->Draw("charge7:unixtime>>charge6","","same");
   GGMDST->Draw("charge8:unixtime>>charge7","","same");
   GGMDST->Draw("charge9:unixtime>>charge8","","same");

   frame->Draw();
   charge1->Draw("same");
   charge2->Draw("same");
   charge3->Draw("same");
   charge4->Draw("same");
   charge5->Draw("same");
   charge6->Draw("same");
   charge7->Draw("same");
   charge8->Draw("same");
/*
   x1=0.9,y1=0.4,x2=x1+0.07,y2=y1+0.03;
   TLegend *leg1=new TLegend(x1,y1,x2,y2);
   leg1->AddEntry(frame,"Q ch1","p");
   leg1->SetTextColor(1);
   leg1->SetFillColor(797);
   leg1->SetTextSize(label_font);
   leg1->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
 
   TLegend *leg2=new TLegend(x1,y1,x2,y2);
   leg2->AddEntry(frame,"Q ch2","p");
   leg2->SetTextColor(0);
   leg2->SetFillColor(600);
   leg2->SetTextSize(label_font);
   leg2->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   TLegend *leg3=new TLegend(x1,y1,x2,y2);
   leg3->AddEntry(frame,"Q ch3","p");
   leg3->SetTextColor(0);
   leg3->SetFillColor(812);
   leg3->SetTextSize(label_font);
   leg3->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   TLegend *leg4=new TLegend(x1,y1,x2,y2);
   leg4->AddEntry(frame,"Q ch4","p");
   leg4->SetTextColor(0);
   leg4->SetFillColor(904);
   leg4->SetTextSize(label_font);
   leg4->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   TLegend *leg5=new TLegend(x1,y1,x2,y2);
   leg5->AddEntry(frame,"Q ch5","p");
   leg5->SetTextColor(0);
   leg5->SetFillColor(867);
   leg5->SetTextSize(label_font);
   leg5->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   TLegend *leg6=new TLegend(x1,y1,x2,y2);
   leg6->AddEntry(frame,"Q ch6","p");
   leg6->SetTextColor(0);
   leg6->SetFillColor(632);
   leg6->SetTextSize(label_font);
   leg6->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   TLegend *leg7=new TLegend(x1,y1,x2,y2);
   leg7->AddEntry(frame,"Q ch7","p");
   leg7->SetTextColor(0);
   leg7->SetFillColor(803);
   leg7->SetTextSize(label_font);
   leg7->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   TLegend *leg8=new TLegend(x1,y1,x2,y2);
   leg8->AddEntry(frame,"Q ch8","p");
   leg8->SetTextColor(0);
   leg8->SetFillColor(1);
   leg8->SetTextSize(label_font);
   leg8->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
*/
   Chambers_AQ->Print("/ggmdata/DATA/charge.pdf");

   ///////////////////////////////////
   ////// *Anodic Charge Plot (Ratio)*  //////
   ///////////////////////////////////

   TH2F *rcharge1 = new  TH2F("rcharge1","rcharge1",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);
   rcharge1->SetMarkerSize(0.7);
   rcharge1->SetMarkerStyle(20);
   rcharge1->SetMarkerColor(801);//797

   TH2F *rcharge2 = new  TH2F("rcharge2","rcharge2",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);
   rcharge2->SetMarkerSize(0.7);
   rcharge2->SetMarkerStyle(20);
   rcharge2->SetMarkerColor(600);//600

   TH2F *rcharge3 = new  TH2F("rcharge3","rcharge3",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);
   rcharge3->SetMarkerSize(0.7);
   rcharge3->SetMarkerStyle(20);
   rcharge3->SetMarkerColor(416);//812

   TH2F *rcharge4 = new  TH2F("rcharge4","rcharge4",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);
   rcharge4->SetMarkerSize(0.7);
   rcharge4->SetMarkerStyle(20);
   rcharge4->SetMarkerColor(922);//904

   TH2F *rcharge5 = new  TH2F("rcharge5","rcharge5",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);
   rcharge5->SetMarkerSize(0.7);
   rcharge5->SetMarkerStyle(20);
   rcharge5->SetMarkerColor(435);//867
   
   TH2F *rcharge6 = new  TH2F("rcharge6","rcharge6",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);
   rcharge6->SetMarkerSize(0.7);
   rcharge6->SetMarkerStyle(20);
   rcharge6->SetMarkerColor(632);//632

   TH2F *rcharge7 = new  TH2F("rcharge7","rcharge7",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);
   rcharge7->SetMarkerSize(0.7);
   rcharge7->SetMarkerStyle(20);
   rcharge7->SetMarkerColor(901);//803
  
   TH2F *rcharge8 = new  TH2F("rcharge8","rcharge8",unixtime_bin,start,finish,charge_bin,charge_min,charge_max);
   rcharge8->SetMarkerSize(0.7);
   rcharge8->SetMarkerStyle(20);
   rcharge8->SetMarkerColor(1);
   

   TCanvas *Chambers_AQ2 = new TCanvas("Chambers_AQ2","GGM Sys Chambers Anodic Charge",0,0,1500,900);
   TH2F *rframe2 = new  TH2F("rframe2","rframe2",unixtime_bin,start,finish,100,0.5,1.5);
   rframe2->SetTitle("GGM System Anodic Charge");
   rframe2->GetXaxis()->SetTitleOffset(axis_offset);
   rframe2->GetXaxis()->SetTimeDisplay(1);
   rframe2->GetXaxis()->SetTimeFormat("%Y %m %d %H:%M %F 1970-01-01 00:00:00");
   rframe2->GetXaxis()->SetNdivisions(time_bin);
   rframe2->GetXaxis()->SetLabelSize(label_size);
   rframe2->GetXaxis()->SetTitle("time");
   rframe2->GetYaxis()->SetTitleOffset(axis_offset);
   rframe2->GetYaxis()->SetLabelSize(label_size);
   rframe2->GetYaxis()->SetTitle("Charge [pC]");
   gPad->SetGrid();

   //GGMDST->Draw("charge2:unixtime>>charge1","","same");
   //GGMDST->Draw("charge3:unixtime>>charge2","","same");
   GGMDST->Draw("(charge4:unixtime)/charge2>>rcharge4","","same");
   GGMDST->Draw("(charge5:unixtime)/charge2>>rcharge5","","same");
   GGMDST->Draw("(charge6:unixtime)/charge2>>rcharge6","","same");
   GGMDST->Draw("(charge7:unixtime)/charge2>>rcharge7","","same");
   GGMDST->Draw("(charge8:unixtime)/charge2>>rcharge8","","same");
   GGMDST->Draw("(charge9:unixtime)/charge2>>rcharge9","","same");

   frame2->Draw();
   //charge1->Draw("same");
   //charge2->Draw("same");
   //charge3->Draw("same");
   rcharge4->Draw("same");
   rcharge5->Draw("same");
   rcharge6->Draw("same");
   rcharge7->Draw("same");
   rcharge8->Draw("same");
   rcharge9->Draw("same");
   Chambers_AQ2->Print("/ggmdata/DATA/chargeRatio.pdf");



/*
   ////////////////////////////////
   ////// *Efficiency plot*  //////
   ////////////////////////////////
   
   TH2F *heff_ch1 = new  TH2F("heff_ch1","",unixtime_bin,start,finish,eff_bin,eff_min,eff_max);  
   heff_ch1->SetMarkerSize(0.7);
   heff_ch1->SetMarkerStyle(20);
   heff_ch1->SetMarkerColor(797);
   
   TH2F *heff_ch2 = new  TH2F("heff_ch2","",unixtime_bin,start,finish,eff_bin,eff_min,eff_max);  
   heff_ch2->SetMarkerSize(0.7);
   heff_ch2->SetMarkerStyle(20);
   heff_ch2->SetMarkerColor(600);
   
   
   TH2F *heff_ch3 = new  TH2F("heff_ch3","",unixtime_bin,start,finish,eff_bin,eff_min,eff_max);  
   heff_ch3->SetMarkerSize(0.7);
   heff_ch3->SetMarkerStyle(20);
   heff_ch3->SetMarkerColor(812);
   
   TH2F *heff_ch4 = new  TH2F("heff_ch4","",unixtime_bin,start,finish,eff_bin,eff_min,eff_max);  
   heff_ch4->SetMarkerSize(0.7);
   heff_ch4->SetMarkerStyle(20);
   heff_ch4->SetMarkerColor(904);
   
   TH2F *heff_ch5 = new  TH2F("heff_ch5","",unixtime_bin,start,finish,eff_bin,eff_min,eff_max);  
   heff_ch5->SetMarkerSize(0.7);
   heff_ch5->SetMarkerStyle(20);
   heff_ch5->SetMarkerColor(867);
   
   TH2F *heff_ch6 = new  TH2F("heff_ch6","",unixtime_bin,start,finish,eff_bin,eff_min,eff_max);  
   heff_ch6->SetMarkerSize(0.7);
   heff_ch6->SetMarkerStyle(20);
   heff_ch6->SetMarkerColor(632);
   
   TH2F *heff_ch7 = new  TH2F("heff_ch7","",unixtime_bin,start,finish,eff_bin,eff_min,eff_max);  
   heff_ch7->SetMarkerSize(0.7);
   heff_ch7->SetMarkerStyle(20);
   heff_ch7->SetMarkerColor(803);
 
   TH2F *heff_ch8 = new  TH2F("heff_ch8","",unixtime_bin,start,finish,eff_bin,eff_min,eff_max);  
   heff_ch8->SetMarkerSize(0.7);
   heff_ch8->SetMarkerStyle(20);
   heff_ch8->SetMarkerColor(1);

   Chambers_AQ = new TCanvas("Chamber1_eff","GGM timeline Chambers Eff",0,0,1500,900);
   TH2F *eff_frame = new  TH2F("eff_frame","eff_frame",unixtime_bin,start,finish,eff_bin,eff_min,eff_max);  
   eff_frame->SetTitle("GGM System Chambers Efficiency");
   eff_frame->GetXaxis()->SetTitleOffset(axis_offset);
   eff_frame->GetXaxis()->SetTimeDisplay(1); 
   eff_frame->GetXaxis()->SetTimeFormat("%Y %m %d %H:%M %F 1970-01-01 00:00:00"); 
   eff_frame->GetXaxis()->SetNdivisions(time_bin); 
   eff_frame->GetXaxis()->SetLabelSize(label_size);
   eff_frame->GetXaxis()->SetTitle("time");
   eff_frame->GetYaxis()->SetTitleOffset(0.65);
   eff_frame->GetYaxis()->SetLabelSize(label_size);
   eff_frame->GetYaxis()->SetTitle("#varepsilon");
   eff_frame->GetYaxis()->SetTitleSize(0.05);
   gPad->SetGrid();
     
   GGMDST->Draw("efficiency2:unixtime>>heff_ch1","","same");
   GGMDST->Draw("efficiency3:unixtime>>heff_ch2","","same");
   GGMDST->Draw("efficiency4:unixtime>>heff_ch3","","same");
   GGMDST->Draw("efficiency5:unixtime>>heff_ch4","","same");
   GGMDST->Draw("efficiency6:unixtime>>heff_ch5","","same"); 
   GGMDST->Draw("efficiency7:unixtime>>heff_ch6","","same");
   GGMDST->Draw("efficiency8:unixtime>>heff_ch7","","same");
   GGMDST->Draw("efficiency9:unixtime>>heff_ch8","","same");
   
   eff_frame->Draw();
   heff_ch1->Draw("same");
   heff_ch2->Draw("same");
   heff_ch3->Draw("same");
   heff_ch4->Draw("same");
   heff_ch5->Draw("same");
   heff_ch6->Draw("same");
   heff_ch7->Draw("same");
   heff_ch8->Draw("same");

   x1=0.9,y1=0.85,x2=x1+0.07,y2=y1+0.03;
   leg1=new TLegend(x1,y1,x2,y2);
   leg1->AddEntry(eff_frame,"#varepsilon ch1","p");
   leg1->SetTextColor(1);
   leg1->SetFillColor(797);
   leg1->SetTextSize(label_font);
   leg1->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg2=new TLegend(x1,y1,x2,y2);
   leg2->AddEntry(eff_frame,"#varepsilon ch2","p");
   leg2->SetTextColor(0);
   leg2->SetFillColor(600);
   leg2->SetTextSize(label_font);
   leg2->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   leg3=new TLegend(x1,y1,x2,y2);
   leg3->AddEntry(eff_frame,"#varepsilon ch3","p");
   leg3->SetTextColor(0);
   leg3->SetFillColor(812);
   leg3->SetTextSize(label_font);
   leg3->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   leg4=new TLegend(x1,y1,x2,y2);
   leg4->AddEntry(eff_frame,"#varepsilon ch4","p");
   leg4->SetTextColor(0);
   leg4->SetFillColor(904);
   leg4->SetTextSize(label_font);
   leg4->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   leg5=new TLegend(x1,y1,x2,y2);
   leg5->AddEntry(eff_frame,"#varepsilon ch5","p");
   leg5->SetTextColor(0);//era test bin
   leg5->SetFillColor(867);
   leg5->SetTextSize(label_font);
   leg5->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg6=new TLegend(x1,y1,x2,y2);
   leg6->AddEntry(eff_frame,"#varepsilon ch6","p");
   leg6->SetTextColor(0);
   leg6->SetFillColor(632);
   leg6->SetTextSize(label_font);
   leg6->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg7=new TLegend(x1,y1,x2,y2);
   leg7->AddEntry(eff_frame,"#varepsilon ch7","p");
   leg7->SetTextColor(0);
   leg7->SetFillColor(803);
   leg7->SetTextSize(label_font);
   leg7->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg8=new TLegend(x1,y1,x2,y2);
   leg8->AddEntry(eff_frame,"#varepsilon ch8","p");
   leg8->SetTextColor(0);
   leg8->SetFillColor(1);
   leg8->SetTextSize(label_font);
   leg8->Draw();
   y2=y2-0.04;
   y1=y1-0.04;   
   Chambers_AQ->Print("/ggmdata/DATA/efficiency.pdf");


   ///////////////////////////////////
   ////// * Avalanche avalanche Plot*  //////
   ///////////////////////////////////
   
   TH2F *avalanche1 = new  TH2F("avalanche1","avalanche1",unixtime_bin,start,finish,avalanche_bin,avalanche_min,avalanche_max);  
   avalanche1->SetMarkerSize(0.7);
   avalanche1->SetMarkerStyle(20);
   avalanche1->SetMarkerColor(801);//797
   
   TH2F *avalanche2 = new  TH2F("avalanche2","avalanche2",unixtime_bin,start,finish,avalanche_bin,avalanche_min,avalanche_max);  
   avalanche2->SetMarkerSize(0.7);
   avalanche2->SetMarkerStyle(20);
   avalanche2->SetMarkerColor(600);//600
   
   TH2F *avalanche3 = new  TH2F("avalanche3","avalanche3",unixtime_bin,start,finish,avalanche_bin,avalanche_min,avalanche_max);  
   avalanche3->SetMarkerSize(0.7);
   avalanche3->SetMarkerStyle(20);
   avalanche3->SetMarkerColor(416);//812
  
   TH2F *avalanche4 = new  TH2F("avalanche4","avalanche4",unixtime_bin,start,finish,avalanche_bin,avalanche_min,avalanche_max);  
   avalanche4->SetMarkerSize(0.7);
   avalanche4->SetMarkerStyle(20);
   avalanche4->SetMarkerColor(922);//904
   
   TH2F *avalanche5 = new  TH2F("avalanche5","avalanche5",unixtime_bin,start,finish,avalanche_bin,avalanche_min,avalanche_max);  
   avalanche5->SetMarkerSize(0.7);
   avalanche5->SetMarkerStyle(20);
   avalanche5->SetMarkerColor(435);//867
   
   TH2F *avalanche6 = new  TH2F("avalanche6","avalanche6",unixtime_bin,start,finish,avalanche_bin,avalanche_min,avalanche_max);  
   avalanche6->SetMarkerSize(0.7);
   avalanche6->SetMarkerStyle(20);
   avalanche6->SetMarkerColor(632);//632
      
   TH2F *avalanche7 = new  TH2F("avalanche7","avalanche7",unixtime_bin,start,finish,avalanche_bin,avalanche_min,avalanche_max);  
   avalanche7->SetMarkerSize(0.7);
   avalanche7->SetMarkerStyle(20);
   avalanche7->SetMarkerColor(901);//803
  
   TH2F *avalanche8 = new  TH2F("avalanche8","avalanche8",unixtime_bin,start,finish,avalanche_bin,avalanche_min,avalanche_max);  
   avalanche8->SetMarkerSize(0.7);
   avalanche8->SetMarkerStyle(20);
   avalanche8->SetMarkerColor(1);
   

   Chambers_AQ = new TCanvas("Chambers_AQ","GGM Sys Chambers Anodic avalanche",0,0,1500,900);
   frame = new  TH2F("frame","frame",unixtime_bin,start,finish,avalanche_bin,avalanche_min,avalanche_max);  
   frame->SetTitle("GGM System Anodic avalanche");
   frame->GetXaxis()->SetTitleOffset(axis_offset);
   frame->GetXaxis()->SetTimeDisplay(1); 
   frame->GetXaxis()->SetTimeFormat("%Y %m %d %H:%M %F 1970-01-01 00:00:00"); 
   frame->GetXaxis()->SetNdivisions(time_bin); 
   frame->GetXaxis()->SetLabelSize(label_size);
   frame->GetXaxis()->SetTitle("time");
   frame->GetYaxis()->SetTitleOffset(axis_offset);
   frame->GetYaxis()->SetLabelSize(label_size);
   frame->GetYaxis()->SetTitle("Avalanche");
   gPad->SetGrid();

   GGMDST->Draw("avalanche2:unixtime>>avalanche1","","same");
   GGMDST->Draw("avalanche3:unixtime>>avalanche2","","same");
   GGMDST->Draw("avalanche4:unixtime>>avalanche3","","same");
   GGMDST->Draw("avalanche5:unixtime>>avalanche4","","same");
   GGMDST->Draw("avalanche6:unixtime>>avalanche5","","same");
   GGMDST->Draw("avalanche7:unixtime>>avalanche6","","same");
   GGMDST->Draw("avalanche8:unixtime>>avalanche7","","same");
   GGMDST->Draw("avalanche9:unixtime>>avalanche8","","same");

   frame->Draw();
   avalanche1->Draw("same");
   avalanche2->Draw("same");
   avalanche3->Draw("same");
   avalanche4->Draw("same");
   avalanche5->Draw("same");
   avalanche6->Draw("same");
   avalanche7->Draw("same");
   avalanche8->Draw("same");

   x1=0.9,y1=0.4,x2=x1+0.07,y2=y1+0.03;
   leg1=new TLegend(x1,y1,x2,y2);
   leg1->AddEntry(frame,"Q ch1","p");
   leg1->SetTextColor(1);
   leg1->SetFillColor(797);
   leg1->SetTextSize(label_font);
   leg1->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
 
   leg2=new TLegend(x1,y1,x2,y2);
   leg2->AddEntry(frame,"Q ch2","p");
   leg2->SetTextColor(0);
   leg2->SetFillColor(600);
   leg2->SetTextSize(label_font);
   leg2->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   leg3=new TLegend(x1,y1,x2,y2);
   leg3->AddEntry(frame,"Q ch3","p");
   leg3->SetTextColor(0);
   leg3->SetFillColor(812);
   leg3->SetTextSize(label_font);
   leg3->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   leg4=new TLegend(x1,y1,x2,y2);
   leg4->AddEntry(frame,"Q ch4","p");
   leg4->SetTextColor(0);
   leg4->SetFillColor(904);
   leg4->SetTextSize(label_font);
   leg4->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   leg5=new TLegend(x1,y1,x2,y2);
   leg5->AddEntry(frame,"Q ch5","p");
   leg5->SetTextColor(0);
   leg5->SetFillColor(867);
   leg5->SetTextSize(label_font);
   leg5->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg6=new TLegend(x1,y1,x2,y2);
   leg6->AddEntry(frame,"Q ch6","p");
   leg6->SetTextColor(0);
   leg6->SetFillColor(632);
   leg6->SetTextSize(label_font);
   leg6->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg7=new TLegend(x1,y1,x2,y2);
   leg7->AddEntry(frame,"Q ch7","p");
   leg7->SetTextColor(0);
   leg7->SetFillColor(803);
   leg7->SetTextSize(label_font);
   leg7->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg8=new TLegend(x1,y1,x2,y2);
   leg8->AddEntry(frame,"Q ch8","p");
   leg8->SetTextColor(0);
   leg8->SetFillColor(1);
   leg8->SetTextSize(label_font);
   leg8->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   Chambers_AQ->Print("/ggmdata/DATA/avalanche.pdf");


   ///////////////////////////////////
   ////// * Streamer streamer Plot*  //////
   ///////////////////////////////////
   
   TH2F *streamer1 = new  TH2F("streamer1","streamer1",unixtime_bin,start,finish,streamer_bin,streamer_min,streamer_max);  
   streamer1->SetMarkerSize(0.7);
   streamer1->SetMarkerStyle(20);
   streamer1->SetMarkerColor(801);//797
   
   TH2F *streamer2 = new  TH2F("streamer2","streamer2",unixtime_bin,start,finish,streamer_bin,streamer_min,streamer_max);  
   streamer2->SetMarkerSize(0.7);
   streamer2->SetMarkerStyle(20);
   streamer2->SetMarkerColor(600);//600
   
   TH2F *streamer3 = new  TH2F("streamer3","streamer3",unixtime_bin,start,finish,streamer_bin,streamer_min,streamer_max);  
   streamer3->SetMarkerSize(0.7);
   streamer3->SetMarkerStyle(20);
   streamer3->SetMarkerColor(416);//812
  
   TH2F *streamer4 = new  TH2F("streamer4","streamer4",unixtime_bin,start,finish,streamer_bin,streamer_min,streamer_max);  
   streamer4->SetMarkerSize(0.7);
   streamer4->SetMarkerStyle(20);
   streamer4->SetMarkerColor(922);//904
   
   TH2F *streamer5 = new  TH2F("streamer5","streamer5",unixtime_bin,start,finish,streamer_bin,streamer_min,streamer_max);  
   streamer5->SetMarkerSize(0.7);
   streamer5->SetMarkerStyle(20);
   streamer5->SetMarkerColor(435);//867
   
   TH2F *streamer6 = new  TH2F("streamer6","streamer6",unixtime_bin,start,finish,streamer_bin,streamer_min,streamer_max);  
   streamer6->SetMarkerSize(0.7);
   streamer6->SetMarkerStyle(20);
   streamer6->SetMarkerColor(632);//632
      
   TH2F *streamer7 = new  TH2F("streamer7","streamer7",unixtime_bin,start,finish,streamer_bin,streamer_min,streamer_max);  
   streamer7->SetMarkerSize(0.7);
   streamer7->SetMarkerStyle(20);
   streamer7->SetMarkerColor(901);//803
  
   TH2F *streamer8 = new  TH2F("streamer8","streamer8",unixtime_bin,start,finish,streamer_bin,streamer_min,streamer_max);  
   streamer8->SetMarkerSize(0.7);
   streamer8->SetMarkerStyle(20);
   streamer8->SetMarkerColor(1);
   

   Chambers_AQ = new TCanvas("Chambers_AQ","GGM Sys Chambers Anodic streamer",0,0,1500,900);
   frame = new  TH2F("frame","frame",unixtime_bin,start,finish,streamer_bin,streamer_min,streamer_max);  
   frame->SetTitle("GGM System Anodic streamer");
   frame->GetXaxis()->SetTitleOffset(axis_offset);
   frame->GetXaxis()->SetTimeDisplay(1); 
   frame->GetXaxis()->SetTimeFormat("%Y %m %d %H:%M %F 1970-01-01 00:00:00"); 
   frame->GetXaxis()->SetNdivisions(time_bin); 
   frame->GetXaxis()->SetLabelSize(label_size);
   frame->GetXaxis()->SetTitle("time");
   frame->GetYaxis()->SetTitleOffset(axis_offset);
   frame->GetYaxis()->SetLabelSize(label_size);
   frame->GetYaxis()->SetTitle("streamer");
   gPad->SetGrid();

   GGMDST->Draw("streamer2:unixtime>>streamer1","","same");
   GGMDST->Draw("streamer3:unixtime>>streamer2","","same");
   GGMDST->Draw("streamer4:unixtime>>streamer3","","same");
   GGMDST->Draw("streamer5:unixtime>>streamer4","","same");
   GGMDST->Draw("streamer6:unixtime>>streamer5","","same");
   GGMDST->Draw("streamer7:unixtime>>streamer6","","same");
   GGMDST->Draw("streamer8:unixtime>>streamer7","","same");
   GGMDST->Draw("streamer9:unixtime>>streamer8","","same");

   frame->Draw();
   streamer1->Draw("same");
   streamer2->Draw("same");
   streamer3->Draw("same");
   streamer4->Draw("same");
   streamer5->Draw("same");
   streamer6->Draw("same");
   streamer7->Draw("same");
   streamer8->Draw("same");

   x1=0.9,y1=0.4,x2=x1+0.07,y2=y1+0.03;
   leg1=new TLegend(x1,y1,x2,y2);
   leg1->AddEntry(frame,"Q ch1","p");
   leg1->SetTextColor(1);
   leg1->SetFillColor(797);
   leg1->SetTextSize(label_font);
   leg1->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
 
   leg2=new TLegend(x1,y1,x2,y2);
   leg2->AddEntry(frame,"Q ch2","p");
   leg2->SetTextColor(0);
   leg2->SetFillColor(600);
   leg2->SetTextSize(label_font);
   leg2->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   leg3=new TLegend(x1,y1,x2,y2);
   leg3->AddEntry(frame,"Q ch3","p");
   leg3->SetTextColor(0);
   leg3->SetFillColor(812);
   leg3->SetTextSize(label_font);
   leg3->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   leg4=new TLegend(x1,y1,x2,y2);
   leg4->AddEntry(frame,"Q ch4","p");
   leg4->SetTextColor(0);
   leg4->SetFillColor(904);
   leg4->SetTextSize(label_font);
   leg4->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   leg5=new TLegend(x1,y1,x2,y2);
   leg5->AddEntry(frame,"Q ch5","p");
   leg5->SetTextColor(0);
   leg5->SetFillColor(867);
   leg5->SetTextSize(label_font);
   leg5->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg6=new TLegend(x1,y1,x2,y2);
   leg6->AddEntry(frame,"Q ch6","p");
   leg6->SetTextColor(0);
   leg6->SetFillColor(632);
   leg6->SetTextSize(label_font);
   leg6->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg7=new TLegend(x1,y1,x2,y2);
   leg7->AddEntry(frame,"Q ch7","p");
   leg7->SetTextColor(0);
   leg7->SetFillColor(803);
   leg7->SetTextSize(label_font);
   leg7->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   leg8=new TLegend(x1,y1,x2,y2);
   leg8->AddEntry(frame,"Q ch8","p");
   leg8->SetTextColor(0);
   leg8->SetFillColor(1);
   leg8->SetTextSize(label_font);
   leg8->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   Chambers_AQ->Print("/ggmdata/DATA/streamer.pdf");

/*
   //////////////////////////////////
   ////// *RELATIVE HUMIDITY*  //////
   //////////////////////////////////
   
   TH2F *hrh1 = new TH2F("hrh1","hrh1",unixtime_bin,start,finish,rh_bin,rh_min,rh_max);
   hrh1->SetMarkerColor(797);
   hrh1->SetLineColor(1);
   hrh1->SetFillColor(1);
   hrh1->SetMarkerStyle(20);
   hrh1->SetMarkerSize(0.7);

   TH2F *hrh2 = new TH2F("hrh2","hrh2",unixtime_bin,start,finish,rh_bin,rh_min,rh_max);
   hrh2->SetMarkerColor(600);
   hrh2->SetLineColor(2);
   hrh2->SetFillColor(2);
   hrh2->SetMarkerStyle(20);
   hrh2->SetMarkerSize(0.7);

   TH2F *hrh3 = new TH2F("hrh3","hrh3",unixtime_bin,start,finish,rh_bin,rh_min,rh_max);
   hrh3->SetMarkerColor(417);
   hrh3->SetLineColor(3);
   hrh3->SetFillColor(3);
   hrh3->SetMarkerStyle(20);
   hrh3->SetMarkerSize(0.7);

   TH2F *hrh4 = new TH2F("hrh4","hrh4",unixtime_bin,start,finish,rh_bin,rh_min,rh_max);
   hrh4->SetMarkerColor(617);
   hrh4->SetLineColor(4);
   hrh4->SetFillColor(4);
   hrh4->SetMarkerStyle(20);
   hrh4->SetMarkerSize(0.7);
  
   TH2F *hrh5 = new TH2F("hrh5","hrh5",unixtime_bin,start,finish,rh_bin,rh_min,rh_max);
   hrh5->SetMarkerColor(867);
   hrh5->SetLineColor(5);
   hrh5->SetFillColor(5);
   hrh5->SetMarkerStyle(20);
   hrh5->SetMarkerSize(0.7);

   TH2F *hrh6 = new TH2F("hrh6","hrh6",unixtime_bin,start,finish,rh_bin,rh_min,rh_max);
   hrh6->SetMarkerColor(632);
   hrh6->SetLineColor(6);
   hrh6->SetFillColor(6);
   hrh6->SetMarkerStyle(20);
   hrh6->SetMarkerSize(0.7);
 
   TH2F *environ2 = new TH2F("environ2","environ2",unixtime_bin,start,finish,rh_bin,rh_min,rh_max);
   environ2->SetMarkerColor(1);
   environ2->SetLineColor(7);
   environ2->SetFillColor(7);
   environ2->SetMarkerStyle(20);
   environ2->SetMarkerSize(0.7);
  
   Sensors_rh = new TCanvas("Sensors2","GGM timeline plots RH",0,0,1500,900);
   TH2F *sens2 = new  TH2F("sens2","sens2",unixtime_bin,start,finish,rh_bin,rh_min,rh_max);  
   sens2->SetTitle("GGM System Relative Humidity");
   sens2->GetXaxis()->SetTitleOffset(axis_offset);
   sens2->GetXaxis()->SetTimeDisplay(1); 
   sens2->GetXaxis()->SetTimeFormat("%Y %m %d %H:%M %F 1970-01-01 00:00:00"); 
   sens2->GetXaxis()->SetNdivisions(time_bin); 
   sens2->GetXaxis()->SetLabelSize(label_size);
   sens2->GetXaxis()->SetTitle("time");
   sens2->GetYaxis()->SetTitleOffset(axis_offset);
   sens2->GetYaxis()->SetLabelSize(label_size);
   sens2->GetYaxis()->SetTitle(" RH [%]");
   gPad->SetGrid();

   GGMDST->Draw("rh1:Unixtime>>hrh1","Channel==1","same");
   GGMDST->Draw("rh1:Unixtime>>hrh2","Channel==3","same");
   GGMDST->Draw("rh1:Unixtime>>hrh3","Channel==6","same");
   GGMDST->Draw("rh2:Unixtime>>hrh4","Channel==1","same");
   GGMDST->Draw("rh2:Unixtime>>hrh5","Channel==3","same");
   GGMDST->Draw("rh2:Unixtime>>hrh6","Channel==6","same");
   GGMDST->Draw("env_rh:Unixtime>>environ2","Channel==1","same");
   
   sens2->Draw();
   hrh1.Draw("same");
   hrh2.Draw("same");
   hrh3.Draw("same");
   hrh4.Draw("same");
   hrh5.Draw("same");
   hrh6.Draw("same");
   environ2.Draw("same");

   x1=0.85,y1=0.85,x2=x1+0.14,y2=y1+0.03;
   
   TLegend *leg1=new TLegend(x1,y1,x2,y2);
   leg1->AddEntry(sens2,"rh TO fresh","p");
   leg1->SetTextColor(1);
   leg1->SetFillColor(797);
   leg1->SetTextSize(label_font);
   leg1->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
 
   TLegend *leg2=new TLegend(x1,y1,x2,y2);
   leg2->AddEntry(sens2,"rh FROM fresh","p");
   leg2->SetTextColor(0);
   leg2->SetFillColor(600);
   leg2->SetTextSize(label_font);
   leg2->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   TLegend *leg3=new TLegend(x1,y1,x2,y2);
   leg3->AddEntry(sens2,"rh TO Bef.","p");
   leg3->SetTextColor(1);
   leg3->SetFillColor(417);
   leg3->SetTextSize(label_font);
   leg3->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   TLegend *leg4=new TLegend(x1,y1,x2,y2);
   leg4->AddEntry(sens2,"rh FROM Bef.","p");
   leg4->SetTextColor(1);
   leg4->SetFillColor(617);
   leg4->SetTextSize(label_font);
   leg4->Draw();
   y2=y2-0.04;
   y1=y1-0.04;
   
   TLegend *leg5=new TLegend(x1,y1,x2,y2);
   leg5->AddEntry(sens2,"rh TO Aft.","p");
   leg5->SetTextColor(1);
   leg5->SetFillColor(867);
   leg5->SetTextSize(label_font);
   leg5->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   TLegend *leg6=new TLegend(x1,y1,x2,y2);
   leg6->AddEntry(sens2,"rh FROM Aft.","p");
   leg6->SetTextColor(1);
   leg6->SetFillColor(632);
   leg6->SetTextSize(label_font);
   leg6->Draw();
   y2=y2-0.04;
   y1=y1-0.04;

   TLegend *leg7=new TLegend(x1,y1,x2,y2);
   leg7->AddEntry(sens2,"rh ENV","p");
   leg7->SetTextColor(0);
   leg7->SetFillColor(1);
   leg7->SetTextSize(label_font);
   leg7->Draw();

    //Environmental Temperature and Pressure
    TCanvas *FramexTandP = new TCanvas("FramexTandP","T & P",0,0,1500,900);//creating Canvas
    TPad *pad1 = new TPad("pad1","",0,0,1,1); //1st pad for T
    TPad *pad2 = new TPad("pad2","",0,0,1,1); //2nd pad for P
    pad2->SetFillStyle(4000); //will be transparent
    pad1->Draw();
    pad1->cd();

    TH2F *sens1 = new  TH2F("sens1","sens1",unixtime_bin,start,finish,temp_bin,temp_min,temp_max);  
    sens1->SetMarkerColor(1);
    sens1->SetMarkerStyle(20);
    sens1->SetMarkerSize(0.7);
	sens1->SetTitle("GGM System Environmental T and P");
    sens1->GetXaxis()->SetTitleOffset(axis_offset);
    sens1->GetXaxis()->SetTimeDisplay(1); 
    sens1->GetXaxis()->SetTimeFormat("%Y %m %d %H:%M %F 1970-01-01 00:00:00"); 
    sens1->GetXaxis()->SetNdivisions(time_bin); 
    sens1->GetXaxis()->SetLabelSize(label_size);
    sens1->GetXaxis()->SetTitle("time");
    sens1->GetYaxis()->SetTitleOffset(axis_offset);
    sens1->GetYaxis()->SetLabelSize(label_size);
    sens1->GetYaxis()->SetTitle("Temperature [°C]");
    sens1->GetYaxis()->SetTitleSize(0.03);
	gPad->SetGrid();
	
	TH2F *environ3 = new TH2F("environ3","environ3",unixtime_bin,start,finish,p_bin,p_min,p_max);
    environ3->SetMarkerColor(860);
    environ3->SetMarkerStyle(20);
    environ3->SetMarkerSize(0.7);
		
	GGMDST->Draw("env_t:Unixtime>>sens1","Channel==1");
    sens1->Draw("");
    
	pad1->Update();
	pad1->Modified();
    FramexTandP->cd();
	
	//compute the pad range with suitable margins
    Double_t ymin = p_min;
    Double_t ymax = p_max;
    Double_t dy = (ymax-ymin)/0.8; //% margins top and bottom
    Double_t xmin = start;
    Double_t xmax = finish;
    Double_t dx = (xmax-xmin)/0.8; //% margins left and right
    pad2->Range(xmin-0.1*dx,ymin-0.1*dy,xmax+0.1*dx,ymax+0.1*dy);
    pad2->Draw();
    pad2->cd();
	
	GGMDST->Draw("env_p:Unixtime>>environ3","Channel==1","same");
    environ3->Draw("][sames");
    pad2->Update();
   
    // draw axis on the right side of the pad
    TGaxis *axis = new TGaxis(xmax,ymin,xmax,ymax,ymin,ymax,50510,"+L");
    axis->SetLabelColor(860);
	axis->SetLineColor(860);
    axis->SetTitleOffset(axis_offset);
    axis->SetLabelSize(label_size);
	axis->SetLabelColor(860);
	axis->SetTitle("Pressure [mbar]");
	axis->SetTitleSize(0.03);
	axis->SetTitleColor(860);
	axis->Draw();
	
	x1=0.9,y1=0.17,x2=x1+0.095,y2=y1+0.035;
    TLegend *leg1=new TLegend(x1,y1,x2,y2);
    leg1->AddEntry(sens1,"T [°C]","p");
    leg1->SetTextColor(0);
    leg1->SetFillColor(1);
    leg1->SetTextSize(0.025);
    leg1->Draw();
    y2=y2-0.04;
    y1=y1-0.04;
 
    TLegend *leg2=new TLegend(x1,y1,x2,y2);
    leg2->AddEntry(sens1,"P [mbar]","p");
    leg2->SetTextColor(0);
    leg2->SetFillColor(860);
    leg2->SetTextSize(0.025);
    leg2->Draw();
    //y2=y2-0.04;
    //y1=y1-0.04;
   
    //Writing histo into ROOT file
    //TFile isto("plots.root","RECREATE");
    Chamber_eff->Write();
    Chambers_AQ->Write();
    Sensors_rh->Write();
    FrameXwp->Write();
    FramexTandP->Write();
    */
system("mail -a /ggmdata/DATA/chargeRatio.pdf -a /ggmdata/DATA/efficiency.pdf -a /ggmdata/DATA/charge.pdf -a /ggmdata/DATA/avalanche.pdf -a /ggmdata/DATA/streamer.pdf -s \"Automatic Monitoring\" stefano.colafranceschi@cern.ch < /dev/null");

}
