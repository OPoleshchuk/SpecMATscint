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
  if (sciCryst->GetAlphaTrackerFlag()) {
    particleEnergy = "0";
    particleName = "3alpha";
  } else {
    if (source=="gamma") {
      particleEnergy = G4UIcommand::ConvertToString(gammaSource->GetGammaEnergy());
      particleName = source;
    } else if (source=="ion") {
      Z = gammaSource->GetZ();
      A = gammaSource->GetA();
      excitEnergy = gammaSource->GetExcitEnergy();
      particleEnergy = G4UIcommand::ConvertToString(gammaSource->GetIonEnergy()*MeV);
      particleName = G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy)->GetParticleName();
      std::replace(particleName.begin(), particleName.end(), '[', '_');
      particleName.erase(std::remove(particleName.begin(), particleName.end(), ']'), particleName.end());
    } else {
      particleEnergy = "unknown";
      particleName = "unknown";
    }
  }
  sourcePosition = G4UIcommand::ConvertToString(129+(gammaSource->GetPointSourceZposition()));
  sourceTypeRA =gammaSource->GetSourceType();
  if (sourceTypeRA=="point") {
    sourceTypeRA+="_sourcePosition_"+sourcePosition+"mm";
  }

  chamber = sciCryst->GetVacuumChamber();
  if (chamber == "yes") {
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
  sourceHolderRA = sciCryst->GetSourceHolder();
  if ((sourceHolderRA == "yes")) {
    sourceHolderRAName = "SourceHolder_";
    sourceHolderRAMatName = sciCryst->GetSourceHolderMat()->GetName();
    sourceHolderRAThick = G4UIcommand::ConvertToString(sciCryst->GetSourceHolderOuterRadius()-sciCryst->GetSourceHolderInnerRadius());
  } else {
    sourceHolderRAName = "";
    sourceHolderRAMatName = "";
  }

  sourceHousingHildeRA = sciCryst->GetSourceHousingHilde();
  if ((sourceHousingHildeRA == "yes")) {
    sourceHousingHildeRAName = "SourceHous_";
    sourceHousingHildeRAMatName = sciCryst->GetSourceHolderMat()->GetName();
    sourceHoldersourceHousingHildeRAThick = G4UIcommand::ConvertToString(sciCryst->GetSourceHousingHildeOuterRadius()-sciCryst->GetSourceHousingHildeInnerRadius());
  } else {
    sourceHousingHildeRAName = "";
    sourceHousingHildeRAMatName = "";
  }

  fieldCageEpoxy = sciCryst->GetFieldCageEpoxy();
  if ((fieldCageEpoxy == "yes")) {
    fieldCageEpoxyName = "FCEpoxy_";
    fieldCageEpoxyMatName = sciCryst->GetFieldCageEpoxyMat()->GetName();
    fieldCageEpoxyThick = G4UIcommand::ConvertToString(sciCryst->GetFieldCageEpoxyOuterRadius()-sciCryst->GetFieldCageEpoxyInnerRadius());
  } else {
    fieldCageEpoxyName = "";
    fieldCageEpoxyMatName = "";
  }
  /*
  if (sciCryst->GetAlphaTrackerFlag()) {
    gasMaterialName = sciCryst->GetGasVolumeMat()->GetName();
    gasMaterialDensity = sciCryst->GetGasVolumeMat()->GetDensity()/(g/cm3);
    gasVolumeRadius = sciCryst->GetGasVolumeOuterRadius();
  } else {
    gasMaterialName = "";
    gasMaterialDensity = 0;
    gasVolumeRadius = 0;
  }*/

  Gap = G4UIcommand::ConvertToString(sciCryst->GetGap());

  fileName = crystMatName+"_"+crystSizeX+"x"+crystSizeY+"x"+crystSizeZ+"mm_"+NbSegments+"x"+Rows+"x"+Columns+"_"+"R"+circleR+"mm_"+sourceTypeRA+"_"+particleName+"_Ek"+particleEnergy+"MeV_"+chamberName+vacuumChamberThicknessRA+"mm_"+sourceHolderRAName+sourceHolderRAMatName+sourceHousingHildeRAName+sourceHousingHildeRAMatName+fieldCageEpoxyName+"gap"+Gap+"mm_"+numberOfEvents+"evnt";
  fileNameRoot = fileName +".root";
  fileNAmeSettings = fileName +".txt";

  // Prints dimensions of the scintillation array
  G4cout <<""<< G4endl;

  myfile.open(fileNAmeSettings);
    //"CeBr3_48mmx48mmx48mm_15x1x3crystals_R131.5mm_gammaScan__point_sourcePosition_129mm_NOChamber_gap3mm.txt");
  myfile <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<< G4endl;
  myfile <<"$$$$"<< G4endl;
  myfile <<"$$$$"<< " Number of simulated events: "<< numberOfEvents;
  myfile <<"$$$$"<< G4endl;
  myfile <<"$$$$"<<" Crystal material: "<<crystMatName<< G4endl;
  myfile <<"$$$$"<<" Reflector material: "<<sciCryst->GetSciReflMat()->GetName()<< G4endl;
  myfile <<"$$$$"<<" Housing material: "<<sciCryst->GetSciHousMat()->GetName()<< G4endl;
  myfile <<"$$$$"<<" Optic window material: "<<sciCryst->GetSciWindMat()->GetName()<< G4endl;
  myfile <<"$$$$"<< G4endl;
  myfile <<"$$$$"<<" Single crystal dimensions: "<<crystSizeX<<"mmx"<<crystSizeY<<"mmx"<<crystSizeZ<<"mm "<< G4endl;
  myfile <<"$$$$"<<" Dimensions of the crystal housing: "<<sciCryst->GetSciHousSizeX()<<"mmx"<<sciCryst->GetSciHousSizeY()<<"mmx"<<sciCryst->GetSciHousSizeZ()<<"mm "<< G4endl;
  myfile <<"$$$$"<<" Housing wall thickness: "<<sciCryst->GetSciHousWallThickX()<<"mm "<< G4endl;
  myfile <<"$$$$"<<" Housing window thickness: "<<sciCryst->GetSciReflWindThick()<<"mm "<< G4endl;
  myfile <<"$$$$"<<" Reflecting material wall thickness: "<<sciCryst->GetSciReflWallThickX()<<"mm "<< G4endl;
  myfile <<"$$$$"<<" Reflecting material thickness in front of the window: "<<sciCryst->GetSciReflWindThick()<<"mm "<< G4endl;
  myfile <<"$$$$"<< G4endl;
  myfile <<"$$$$"<<" Number of segments in the array: "<<NbSegments<<" "<< G4endl;
  myfile <<"$$$$"<<" Number of crystals in the segment row: "<<Columns<<" "<< G4endl;
  myfile <<"$$$$"<<" Number of crystals in the segment column: "<<Rows<<" "<< G4endl;
  myfile <<"$$$$"<<" Number of crystals in the array: "<<sciCryst->GetNbSegments()*sciCryst->GetNbCrystInSegmentColumn()*sciCryst->GetNbCrystInSegmentRow()<<" "<< G4endl;
  myfile <<"$$$$"<< G4endl;
  myfile <<"$$$$"<<" Radius of a circle inscribed in the array: "<<circleR<<"mm "<< G4endl;
  myfile <<"$$$$"<< G4endl;
  if (chamber == "yes") {
    myfile <<"$$$$"<<" Vacuum Chamber Material: "<<sciCryst->GetVacuumChamberMat()->GetName()<< G4endl;
    myfile <<"$$$$"<<" Vacuum Chamber Wall Thickness: "<<sciCryst->GetVacuumChamberThickness()<<"mm "<< G4endl;
    myfile <<"$$$$"<<" Vacuum Chamber outer radius: "<<sciCryst->GetVacuumChamberOuterRadius()<<"mm "<< G4endl;
    myfile <<"$$$$"<<" Vacuum Chamber inner radius: "<<sciCryst->GetVacuumChamberInnerRadius()<<"mm "<< G4endl;
  }
  myfile <<"$$$$"<< G4endl;
  if (sourceHolderRA == "yes") {
    myfile <<"$$$$"<<" Source Holder Material: "<<sciCryst->GetSourceHolderMat()->GetName()<< G4endl;
    myfile <<"$$$$"<<" Source Holder Wall Thickness: "<<sciCryst->GetSourceHolderOuterRadius()-sciCryst->GetSourceHolderInnerRadius()<<"mm "<< G4endl;
    myfile <<"$$$$"<<" Source Holder outer radius: "<<sciCryst->GetSourceHolderOuterRadius()<<"mm "<< G4endl;
    myfile <<"$$$$"<<" Source Holder inner radius: "<<sciCryst->GetSourceHolderInnerRadius()<<"mm "<< G4endl;
  }
  myfile <<"$$$$"<< G4endl;
  if (sourceHousingHildeRA == "yes") {
    myfile <<"$$$$"<<" Source Housing Material: "<<sciCryst->GetSourceHousingHildeMat()->GetName()<< G4endl;
    myfile <<"$$$$"<<" Source Housing Wall Thickness: "<<sciCryst->GetSourceHousingHildeOuterRadius()-sciCryst->GetSourceHousingHildeInnerRadius()<<"mm "<< G4endl;
    myfile <<"$$$$"<<" Source Housing outer radius: "<<sciCryst->GetSourceHousingHildeOuterRadius()<<"mm "<< G4endl;
    myfile <<"$$$$"<<" Source Housing inner radius: "<<sciCryst->GetSourceHousingHildeInnerRadius()<<"mm "<< G4endl;
  }
  myfile <<"$$$$"<< G4endl;
  if (fieldCageEpoxy == "yes") {
    myfile <<"$$$$"<<" Field Cage material: "<<sciCryst->GetFieldCageEpoxyMat()->GetName()<< G4endl;
    myfile <<"$$$$"<<" Field Cage material: "<<sciCryst->GetFieldCageEpoxyMat()->GetDensity()<< G4endl;
    myfile <<"$$$$"<<" Field Cage thickness: "<<sciCryst->GetFieldCageEpoxyOuterRadius()-sciCryst->GetFieldCageEpoxyInnerRadius()<<"mm "<< G4endl;
    myfile <<"$$$$"<<" Field Cage outer radius: "<<sciCryst->GetFieldCageEpoxyOuterRadius()<<"mm "<< G4endl;
    myfile <<"$$$$"<<" Field Cage inner radius: "<<sciCryst->GetFieldCageEpoxyInnerRadius()<<"mm "<< G4endl;
  }
  /*
  myfile <<"$$$$"<< G4endl;
  if (sciCryst->GetAlphaTrackerFlag()) {
    myfile <<"$$$$"<<" Gas material: "<<sciCryst->GetGasVolumeMat()->GetName()<< G4endl;
    myfile <<"$$$$"<<" Gas material: "<<sciCryst->GetGasVolumeMat()->GetDensity()/(g/cm3)<<"g/cm3 "<< G4endl;
    myfile <<"$$$$"<<" Gas volume outer radius: "<<sciCryst->GetGasVolumeOuterRadius()<<"mm "<< G4endl;
  }*/
  myfile <<"$$$$"<< G4endl;
  myfile <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<< G4endl;
  myfile <<""<< G4endl;
  //myfile <<"Positions of the crystal centers in the world:"<< G4endl;
  //crystalPositionsArrayRA=sciCryst->GetCrystalPositionsArray();
  //for (i = 0; i < 45; i++) {
    //myfile << "CrystNb" << i+1 << ": " << crystalPositionsArrayRA[i] << G4endl;
  //}
  //myfile <<""<< G4endl;
  myfile.close();

  // Open the file
  //
  analysisManager->OpenFile(fileNameRoot);
  analysisManager->SetFirstHistoId(1);

  // Creating histograms
  //
  if (sciCryst->GetAlphaTrackerFlag()) {
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
    ComptSuppFlagTest = sciCryst->GetComptSuppFlag();
    if (ComptSuppFlagTest == "yes") {
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
    if (ComptSuppFlagTest == "yes") {
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
