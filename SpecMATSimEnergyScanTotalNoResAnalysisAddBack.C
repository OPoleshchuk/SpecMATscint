///Author: Oleksii Poleshchuk
///
///KU Leuven 2016-2019
///
///Part of SpecMATscint
///
/// \file SpecMATSimEnergyScanTotalNoResAnalysisAddBack.C
///This script is used for automatic sorting of the Histograms produced
///with SpecMATscint.
///

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

ofstream myfile;

void scanHisto(TString in_fname, float energyMeV, float energykeV, int nrebin) {

  TFile *f = new TFile(Form("%s.root",in_fname.Data()),"READ");
  if (f->IsZombie()) {
	  printf("ERROR opening FILE %s.root \n", in_fname.Data());
	  return -1;
  }

  TH1F *h1 = 0;
  TH1F *h2 = 0;
  TH1F *h3 = 0;

  f->GetObject("TotalHistABNoRes", h1);
  f->GetObject("TotalHistAB30CrystNoRes", h2);
  f->GetObject("TotalHistAB40CrystNoRes", h3);

  double max1 = h1->GetMaximum();
  double max2 = h2->GetMaximum();
  double max3 = h3->GetMaximum();

  double nbins1 = h1->GetNbinsX();
  double nbins2 = h2->GetNbinsX();
  double nbins3 = h3->GetNbinsX();

  double bincenter1 = 0;
  double bincenter2 = 0;
  double bincenter3 = 0;

  double bincontent1 = 0;
  double bincontent2 = 0;
  double bincontent3 = 0;
  double counter1=h1->GetBinContent(energykeV)+h1->GetBinContent(energykeV+1);
  double counter2=h2->GetBinContent(energykeV)+h2->GetBinContent(energykeV+1);
  double counter3=h3->GetBinContent(energykeV)+h3->GetBinContent(energykeV+1);

  myfile<<energyMeV<<" "<<counter1<<" "<<counter2<<" "<<counter3<<endl;
  cout<<energyMeV<<" "<<counter1<<" "<<counter2<<" "<<counter3<<endl;

}

void SpecMATSimEnergyScanTotalNoResAnalysisAddBack(){
  myfile.open ("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.5mm_gammaScan_Aluminum5083_Chamber_wallThick_linear_sourcePosition_325mm_3mm_SourceHolder_PVCSourceHous_PVC_gap3mm.txt");
  myfile<<"Energy "<<"PeakNoResCounts "<<"PeakNoRes30CrystCounts "<<"PeakNoRes40CrystCounts"<<endl;
  cout<<"Energy "<<"PeakNoResCounts "<<"PeakNoRes30CrystCounts "<<"PeakNoRes40CrystCounts"<<endl;

  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.02MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.02, 20,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.03MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.03, 30,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.04MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.04, 40,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.05MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.05, 50,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.06MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.06, 60,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.07MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.07, 70,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.08MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.08, 80,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.09MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.09, 90,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.1MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.1, 100,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.11MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.11, 110,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.12MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.12, 120,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.13MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.13, 130,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.14MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.14, 140,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.15MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.15, 150,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.16MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.16, 160,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.17MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.17, 170,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.18MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.18, 180,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.19MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.19, 190,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.2MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.2, 200,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.21MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.21, 210,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.22MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.22, 220,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.23MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.23, 230,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.24MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.24, 240,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.25MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.25, 250,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.26MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.26, 260,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.27MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.27, 270,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.28MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.28, 280,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.29MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.29, 290,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.3MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.3, 300,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.4MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.4, 400,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.5MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.5, 500,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.6MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.6, 600,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.7MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.7, 700,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.8MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.8, 800,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma0.9MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 0.9, 900,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1, 1000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.1MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1.1, 1100,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.2MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1.2, 1200,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.3MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1.3, 1300,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.4MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1.4, 1400,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.5MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1.5, 1500,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.6MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1.6, 1600,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.7MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1.7, 1700,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.8MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1.8, 1800,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma1.9MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 1.9, 1900,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma2MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 2, 2000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma3MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 3, 3000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma4MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 4, 4000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma5MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 5, 5000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma6MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 6, 6000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma7MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 7, 7000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma8MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 8, 8000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma9MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 9, 9000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma10MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 10, 10000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma11MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 11, 11000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma12MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 12, 12000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma13MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 13, 13000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma14MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 14, 14000,1);
  scanHisto("AddBack_CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.25mm_gamma15MeV_Aluminum5083_Chamber_wallThick_3mm_linear_sourcePosition_129mm_FCEpoxy_epoxy_gap3mm", 15, 15000,1);

  myfile.close();
  printf("The data was saved\n");
  printf("DONE\n");
}
