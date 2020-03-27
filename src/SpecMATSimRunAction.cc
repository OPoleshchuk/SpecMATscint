///Author: Oleksii Poleshchuk
///
///KU Leuven 2016-2019
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
#include <algorithm>

using namespace std;

// ###################################################################################

SpecMATSimRunAction::SpecMATSimRunAction():
G4UserRunAction(),
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

  NbSegments = G4UIcommand::ConvertToString(sciCryst->GetNbSegments());
  Rows       = G4UIcommand::ConvertToString(sciCryst->GetNbCrystInSegmentColumn());
  Columns    = G4UIcommand::ConvertToString(sciCryst->GetNbCrystInSegmentRow());
  circleR    = G4UIcommand::ConvertToString(sciCryst->ComputeCircleR1());

  numberOfEvents = G4UIcommand::ConvertToString(run->GetNumberOfEventToBeProcessed());

  source =gammaSource->GetSource();
  if (sciCryst->GetTPCFlag()) {
    particleEnergy = "_";
    particleName = "3alpha";
    if (sciCryst->GetBfieldFlag()) {
      strBfieldZ = G4UIcommand::ConvertToString(sciCryst->GetBfieldZ());
    } else {
      strBfieldZ = "0";
    }
  } else {
    if (source=="gamma") {
      particleEnergy = "_Ek"+G4UIcommand::ConvertToString(gammaSource->GetGammaEnergy())+"MeV_";
      particleName = source;
    } else if (source=="ion") {
      Z = gammaSource->GetZ();
      A = gammaSource->GetA();
      excitEnergy = gammaSource->GetExcitEnergy();
      particleEnergy = "_Ek"+G4UIcommand::ConvertToString(gammaSource->GetIonEnergy()*MeV)+"MeV_";
      particleName = G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy)->GetParticleName();
      std::replace(particleName.begin(), particleName.end(), '[', '_');
      particleName.erase(std::remove(particleName.begin(), particleName.end(), ']'), particleName.end());
    } else {
      particleEnergy = "_";
      particleName = "";
    }
  }
  sourcePosition = G4UIcommand::ConvertToString(129+(gammaSource->GetPointSourceZposition()));
  sourceTypeRA = gammaSource->GetSourceType();
  if (sourceTypeRA=="point") {
    sourceTypeRA+="SourcePosition_"+sourcePosition+"mm";
  }

  if (sciCryst->GetVacuumChamberFlag()) {
    vacuumChamberMatRA = sciCryst->GetVacuumChamberMat();
    vacuumChamberMatRAName = vacuumChamberMatRA->GetName();
    chamberName = vacuumChamberMatRAName + "_Chamber";
    vacuumChamberThicknessRA = G4UIcommand::ConvertToString(sciCryst->GetVacuumChamberThickness());
    //flangeMatName = sciCryst->GetVacuumFlangeMat()->GetName();
    //flangeMatName = "";
  } else {
    chamberName = "";
    vacuumChamberThicknessRA = "";
    //flangeMatName = "";
  }

  if (sciCryst->GetSourceHolderFlag()) {
    sourceHolderRAName = "SourceHolder_";
    sourceHolderRAMatName = sciCryst->GetSourceHolderMat()->GetName();
    sourceHolderRAThick = G4UIcommand::ConvertToString(sciCryst->GetSourceHolderOuterRadius()-sciCryst->GetSourceHolderInnerRadius());
  } else {
    sourceHolderRAName = "";
    sourceHolderRAMatName = "";
  }

  if (sciCryst->GetSourceHousingHildeFlag()) {
    sourceHousingHildeRAName = "SourceHous_";
    sourceHousingHildeRAMatName = sciCryst->GetSourceHolderMat()->GetName();
    sourceHoldersourceHousingHildeRAThick = G4UIcommand::ConvertToString(sciCryst->GetSourceHousingHildeOuterRadius()-sciCryst->GetSourceHousingHildeInnerRadius());
  } else {
    sourceHousingHildeRAName = "";
    sourceHousingHildeRAMatName = "";
  }

  if (sciCryst->GetFieldCageEpoxyFlag()) {
    fieldCageEpoxyName = "FCEpoxy_";
    fieldCageEpoxyMatName = sciCryst->GetFieldCageEpoxyMat()->GetName();
    fieldCageEpoxyThick = G4UIcommand::ConvertToString(sciCryst->GetFieldCageEpoxyOuterRadius()-sciCryst->GetFieldCageEpoxyInnerRadius());
  } else {
    fieldCageEpoxyName = "";
    fieldCageEpoxyMatName = "";
  }
  /*
  if (sciCryst->GetTPCFlag()) {
    gasMaterialName = sciCryst->GetGasVolumeMat()->GetName();
    gasMaterialDensity = sciCryst->GetGasVolumeMat()->GetDensity()/(g/cm3);
    gasVolumeRadius = sciCryst->GetGasVolumeOuterRadius();
  } else {
    gasMaterialName = "";
    gasMaterialDensity = 0;
    gasVolumeRadius = 0;
  }*/

  Gap = G4UIcommand::ConvertToString(sciCryst->GetGap());

  fileName = crystMatName+"_"+crystSizeX+"x"+crystSizeY+"x"+crystSizeZ+"mm_"+NbSegments+"x"+Rows+"x"+Columns+"_R"+circleR+"mm_"+sourceTypeRA+"_"+particleName+particleEnergy+chamberName+vacuumChamberThicknessRA+"mm_"+sourceHolderRAName+sourceHolderRAMatName+sourceHousingHildeRAName+sourceHousingHildeRAMatName+fieldCageEpoxyName+"gap"+Gap+"mm_"+numberOfEvents+"evnt_"+"B"+strBfieldZ+"T";
  fileNameRoot = fileName +".root";
  fileNameParamTXT = fileName +".txt";

  // Change name of the .txt file with simulation parameters created in SpecMATSimDetectorConstruction.cc
  rename("simParam.txt", fileNameParamTXT);

  // Open the root file
  //
  analysisManager->OpenFile(fileNameRoot);
  analysisManager->SetFirstHistoId(1);

  // Creating histograms
  //
  if (sciCryst->GetTPCFlag()) {
    analysisManager->CreateH1("AlphaTotal", "Total Edep in TPC", 16000, 0., 16000);
    analysisManager->CreateH1("AlphaTotalRes", "Total EdepRes in TPC", 16000, 0., 16000);


    analysisManager->CreateNtuple("Alpha", "Alpha Edep");
    analysisManager->CreateNtupleDColumn("Event");
    analysisManager->CreateNtupleDColumn("Step");
    analysisManager->CreateNtupleDColumn("InitialPointX");
    analysisManager->CreateNtupleDColumn("InitialPointY");
    analysisManager->CreateNtupleDColumn("InitialPointZ");
    analysisManager->CreateNtupleDColumn("FinalPointX");
    analysisManager->CreateNtupleDColumn("FinalPointY");
    analysisManager->CreateNtupleDColumn("FinalPointZ");
    analysisManager->CreateNtupleDColumn("AlphaEdep");
    analysisManager->CreateNtupleDColumn("particleID");
    analysisManager->CreateNtupleDColumn("PrimaryMomentumX");
    analysisManager->CreateNtupleDColumn("PrimaryMomentumY");
    analysisManager->CreateNtupleDColumn("PrimaryMomentumZ");
    analysisManager->CreateNtupleDColumn("PrimaryEnergy");
    analysisManager->FinishNtuple();
  } else {
    for(crystNb = 1; crystNb <= (sciCryst->GetNbCrystInSegmentRow())*(sciCryst->GetNbCrystInSegmentColumn())*(sciCryst->GetNbSegments()); crystNb++) {
      analysisManager->CreateH1(G4UIcommand::ConvertToString(crystNb),"Edep in crystal Nb" + G4UIcommand::ConvertToString(crystNb), 16000, 0., 16000);
    }
    analysisManager->CreateH1("Total", "Total Edep", 16000, 0., 16000);
    analysisManager->CreateH1("Total30Cryst", "Total 30Cryst Edep", 16000, 0., 16000);
    analysisManager->CreateH1("Total40Cryst", "Total 40Cryst Edep", 16000, 0., 16000);
    analysisManager->CreateH1("Total1stRow", "Total 1stRow Edep", 16000, 0., 16000);
    analysisManager->CreateH1("Total2ndRow", "Total 2ndRow Edep", 16000, 0., 16000);
    analysisManager->CreateH1("Total3rdRow", "Total 3rdRow Edep", 16000, 0., 16000);
    analysisManager->CreateH1("TotalNoRes", "Total EdepNoRes", 16000, 0., 16000);
    analysisManager->CreateH1("TotalNoRes30Cryst", "Total 30Cryst EdepNoRes", 16000, 0., 16000);
    analysisManager->CreateH1("TotalNoRes40Cryst", "Total 40Cryst EdepNoRes", 16000, 0., 16000);
    analysisManager->CreateH1("TotalNoRes1stRow", "Total 1stRow EdepNoRes", 16000, 0., 16000);
    analysisManager->CreateH1("TotalNoRes2ndRow", "Total 2ndRow EdepNoRes", 16000, 0., 16000);
    analysisManager->CreateH1("TotalNoRes3rdRow", "Total 3rdRow EdepNoRes", 16000, 0., 16000);
    if (sciCryst->GetComptSuppFlag()) {
      for(segmentNb = 1; segmentNb <= (sciCryst->GetNbSegments()); segmentNb++) {
        analysisManager->CreateH1(G4UIcommand::ConvertToString(100+segmentNb),"Edep in ComptSupp Nb" + G4UIcommand::ConvertToString(100+segmentNb), 16000, 0., 16000);
      }
    }
    // Creating ntuple
    //
    //From EventAction
    analysisManager->CreateNtuple("Total", "Total Edep");
    analysisManager->CreateNtupleDColumn("Event");
    analysisManager->CreateNtupleDColumn("CrystNb");
    analysisManager->CreateNtupleDColumn("EdepRes");
    analysisManager->CreateNtupleDColumn("EdepResCrystNb1");
    analysisManager->CreateNtupleDColumn("EdepResCrystNb2");
    analysisManager->CreateNtupleDColumn("EdepRes30Cryst");
    analysisManager->CreateNtupleDColumn("EdepRes40Cryst");
    analysisManager->CreateNtupleDColumn("EdepRes1stRow");
    analysisManager->CreateNtupleDColumn("EdepRes2ndRow");
    analysisManager->CreateNtupleDColumn("EdepRes3rdRow");
    analysisManager->CreateNtupleDColumn("EdepNoRes");
    analysisManager->CreateNtupleDColumn("EdepNoResCrystNb1");
    analysisManager->CreateNtupleDColumn("EdepNoResCrystNb2");
    analysisManager->CreateNtupleDColumn("EdepNoRes30Cryst");
    analysisManager->CreateNtupleDColumn("EdepNoRes40Cryst");
    analysisManager->CreateNtupleDColumn("EdepNoRes1stRow");
    analysisManager->CreateNtupleDColumn("EdepNoRes2ndRow");
    analysisManager->CreateNtupleDColumn("EdepNoRes3rdRow");
    analysisManager->CreateNtupleDColumn("PrimaryVertexX");
    analysisManager->CreateNtupleDColumn("PrimaryVertexY");
    analysisManager->CreateNtupleDColumn("PrimaryVertexZ");
    if (sciCryst->GetComptSuppFlag()) {
      analysisManager->CreateNtupleDColumn("EventCS");
      analysisManager->CreateNtupleDColumn("ComptSuppNb");
      analysisManager->CreateNtupleDColumn("EdepComptSuppRes");
      analysisManager->CreateNtupleDColumn("EdepComptSuppNoRes");
    }
    /*
    //From SteppingAction
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
    //From SteppingAction
    */
    analysisManager->FinishNtuple();

  }
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
