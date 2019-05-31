///Author: Oleksii Poleshchuk
///
///KU Leuven 2019
///
///SpecMATscint is a GEANT4 code for simulation
///of gamma-rays detection efficiency with
///the SpecMAT scintillation array.
///
///Primarily, this code was written for identification of
///the best geometry of a scintillation array based
///on it's total detection efficiency.
///
/// \file SpecMATSimRunAction.cc
/// \brief Implementation of the SpecMATSimRunAction class

#include "SpecMATSimRunAction.hh"
#include "SpecMATSimPrimaryGeneratorAction.hh"
#include "SpecMATSimAnalysis.hh"
#include "SpecMATSimDetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4Material.hh"
#include "G4String.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

// ###################################################################################

SpecMATSimRunAction::SpecMATSimRunAction()
: G4UserRunAction(),
  fGoodEvents(0),
  sciCryst(0),
  gammaSource(0)
{
  sciCryst = new SpecMATSimDetectorConstruction();
  gammaSource = new SpecMATSimPrimaryGeneratorAction();
}

// ###################################################################################

SpecMATSimRunAction::~SpecMATSimRunAction()
{
  delete sciCryst;
  delete gammaSource;
}

// ###################################################################################

void SpecMATSimRunAction::BeginOfRunAction(const G4Run* run)
{
  G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

  fGoodEvents = 0;

  //inform the runManager to save random number seed
  G4RunManager::GetRunManager()->SetRandomNumberStore(false);

  // Creates analysis manager
  // The choice of analysis technology is done via selection of a namespace
  // in SpecMATSimAnalysis.hh
  analysisManager = G4AnalysisManager::Instance();
  G4cout << "Using " << analysisManager->GetType() << " analysis manager" << G4endl;

  // Creates directories
  //
  analysisManager->SetHistoDirectoryName("histograms");
  analysisManager->SetNtupleDirectoryName("ntuple");

  // Creates the file name
  //
  crystMat = sciCryst->GetSciCrystMat();
  crystMatName = crystMat->GetName();
  crystSizeX = G4UIcommand::ConvertToString(sciCryst->GetSciCrystSizeX()*2);
  crystSizeY = G4UIcommand::ConvertToString(sciCryst->GetSciCrystSizeY()*2);
  crystSizeZ = G4UIcommand::ConvertToString(sciCryst->GetSciCrystSizeZ()*2);

  G4String source =gammaSource->GetSource();
  if (source=="gamma") {
    particleEnergy = G4UIcommand::ConvertToString(gammaSource->GetGammaEnergy());
    particleName = source;
  } else if (source=="ion") {
    Z = gammaSource->GetZ();
    A = gammaSource->GetA();
    excitEnergy = gammaSource->GetExcitEnergy();
    particleEnergy = gammaSource->GetIonEnergy();
    particleName = G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy)->GetParticleName();
  } else {
    particleEnergy = "unknown";
    particleName = "unknown";
  }

  NbSegments = G4UIcommand::ConvertToString(sciCryst->GetNbSegments());
  Rows       = G4UIcommand::ConvertToString(sciCryst->GetNbCrystInSegmentColumn());
  Columns    = G4UIcommand::ConvertToString(sciCryst->GetNbCrystInSegmentRow());
  circleR    = G4UIcommand::ConvertToString(sciCryst->ComputeCircleR1());

  chamber = sciCryst->GetVacuumChamber();
  insulator = sciCryst->GetInsulationTube();

  if (chamber == "yes") {
    chamberName = "Al_Chamber_pos2";
    //flangeMatName = sciCryst->GetVacuumFlangeMat()->GetName();
    flangeMatName = "";
  } else {
    chamberName = "";
    flangeMatName = "";
  }

  if (/*(chamber == "yes") &&*/ (insulator == "yes")) {
    insulatorName = "Ins_";
    insulatorMatName = sciCryst->GetInsulationTubeMat()->GetName();
  } else {
    insulatorName = "";
    insulatorMatName = "";
  }

  Gap = G4UIcommand::ConvertToString(sciCryst->GetGap());
  fileName = crystMatName+"_"+crystSizeX+"mmx"+crystSizeY+"mmx"+crystSizeZ+"mm_"+NbSegments+"x"+Rows+"x"+Columns+"crystals_"+"R"+circleR+"mm_"+particleName+particleEnergy+"MeV_"+chamberName+flangeMatName+insulatorName+insulatorMatName+"_gap_"+Gap+".root";

  // Open the file
  //
  analysisManager->OpenFile(fileName);
  analysisManager->SetFirstHistoId(1);

  // Creating histograms
  //
  for(crystNb = 1; crystNb <= (sciCryst->GetNbCrystInSegmentRow())*(sciCryst->GetNbCrystInSegmentColumn())*(sciCryst->GetNbSegments()); crystNb++) {
    analysisManager->CreateH1(G4UIcommand::ConvertToString(crystNb),"Edep in crystal Nb" + G4UIcommand::ConvertToString(crystNb), 15500, 0., 15500*MeV);
  }
  analysisManager->CreateH1("Total", "Total Edep", 15500, 0., 15500*MeV);
  ComptSuppFlagTest = sciCryst->GetComptSuppFlag();
  if (ComptSuppFlagTest == "yes") {
    for(segmentNb = 1; segmentNb <= (sciCryst->GetNbSegments()); segmentNb++) {
      analysisManager->CreateH1(G4UIcommand::ConvertToString(100+segmentNb),"Edep in ComptSupp Nb" + G4UIcommand::ConvertToString(100+segmentNb), 15500, 0., 15500*MeV);
    }
  }
  // Creating ntuple
  //
  //From EventAction
  analysisManager->CreateNtuple("Total", "Total Edep");
  analysisManager->CreateNtupleDColumn("Event");
  analysisManager->CreateNtupleDColumn("CrystNb");
  analysisManager->CreateNtupleDColumn("EdepRes");
  analysisManager->CreateNtupleDColumn("EdepNoRes");
  analysisManager->CreateNtupleDColumn("EventCS");
  if (ComptSuppFlagTest == "yes") {
    analysisManager->CreateNtupleDColumn("ComptSuppNb");
    analysisManager->CreateNtupleDColumn("EdepComptSuppRes");
    analysisManager->CreateNtupleDColumn("EdepComptSuppNoRes");
  }
  //From SteppingAction
  /*
  analysisManager->CreateNtupleDColumn("EventSA");
  analysisManager->CreateNtupleDColumn("Step");
  analysisManager->CreateNtupleDColumn("CopyNb");
  analysisManager->CreateNtupleDColumn("materialID"); // 1 - scintillator; 0 - smth else
  analysisManager->CreateNtupleDColumn("particleID");
  analysisManager->CreateNtupleDColumn("InitialPointX");
  analysisManager->CreateNtupleDColumn("InitialPointY");
  analysisManager->CreateNtupleDColumn("InitialPointZ");
  analysisManager->CreateNtupleDColumn("FinalPointX");
  analysisManager->CreateNtupleDColumn("FinalPointY");
  analysisManager->CreateNtupleDColumn("FinalPointZ");
  analysisManager->CreateNtupleDColumn("EdepSA");
  analysisManager->CreateNtupleDColumn("Time");
  */
  analysisManager->FinishNtuple();
}

// ###################################################################################

void SpecMATSimRunAction::EndOfRunAction(const G4Run* aRun)
{
  NbOfEvents = aRun->GetNumberOfEvent();
  if (NbOfEvents == 0) return;

  //aRun conditions
  //
  const SpecMATSimPrimaryGeneratorAction* kinematic = static_cast<const SpecMATSimPrimaryGeneratorAction*>(G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  particle = kinematic->GetParticleGun()->GetParticleDefinition();
  partName = particle->GetParticleName();

  // save histograms
  //
  analysisManager = G4AnalysisManager::Instance();
  analysisManager->Write();
  analysisManager->CloseFile();

  // complete cleanup
  //
  delete G4AnalysisManager::Instance();

  //print
  //
  G4cout
  << "\n--------------------End of Run------------------------------\n"
  << " The Run was " << NbOfEvents << " "<< partName
  << "\n------------------------------------------------------------\n"
  << G4endl;
}

// ###################################################################################
