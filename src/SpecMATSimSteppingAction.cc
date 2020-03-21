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
/// \file SpecMATSimSteppingAction.cc
/// \brief Implementation of the SpecMATSimSteppingAction class

#include "SpecMATSimSteppingAction.hh"
#include "SpecMATSimEventAction.hh"
#include "SpecMATSimDetectorConstruction.hh"
#include "SpecMATSimAnalysis.hh"

#include "G4UserSteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

// ###################################################################################

SpecMATSimSteppingAction::SpecMATSimSteppingAction()
: sciCryst(0)
{
  sciCryst = new SpecMATSimDetectorConstruction();
}

// ###################################################################################

SpecMATSimSteppingAction::~SpecMATSimSteppingAction()
{

}

// ###################################################################################


void SpecMATSimSteppingAction::UserSteppingAction(const G4Step* step)
{
  /*
  // Collects energy and track data step by step
  G4RunManager* theRunManager = G4RunManager::GetRunManager();

  //G4ThreeVector vertexPosition = theRunManager->GetCurrentEvent()->GetPrimaryVertex()->GetPosition();

  eventNb = theRunManager->GetCurrentEvent()->GetEventID();

  stepNb = step->GetTrack()->GetCurrentStepNumber();

  crystNb = step->GetTrack()->GetVolume()->GetCopyNo();

  materialName = step->GetTrack()->GetMaterial()->GetName();

  if (materialName == "CeBr3" || materialName == "LaBr3") { // 1 - scintillator; 0 - smth else
    materialID = 1;
  }
  else {
    materialID = 0;
  }

  particleName = step->GetTrack()->GetDefinition()->GetParticleName();
  if (particleName == "gamma") {
    particleID = 1;
  } else if (particleName == "e-") {
    particleID = 2;
  } else {
    particleID = 0;
  }

  momentumDirection = step->GetTrack()->GetMomentumDirection();

  initialPoint = step->GetPreStepPoint()->GetPosition();
  initialPointX = initialPoint.getX();
  initialPointY = initialPoint.getY();
  initialPointZ = initialPoint.getZ();

  finalPoint = step->GetPostStepPoint()->GetPosition();
  finalPointX = finalPoint.getX();
  finalPointY = finalPoint.getY();
  finalPointZ = finalPoint.getZ();

  stepLength = step->GetStepLength();

  edep = step->GetTotalEnergyDeposit();

  timing = step->GetPostStepPoint()->GetGlobalTime();

  if (crystNb==0 && stepNb==1 && particleID==1) {
    //Uncomment this to print all this information after each step
    G4cout << "\nEVENT #" << eventNb << G4endl;
    G4cout << "STEP #" << stepNb << G4endl;
    G4cout << "crystNb: " << crystNb << G4endl;
    G4cout << "material: " << materialName << ", ID:" << materialID << G4endl;
    G4cout << "particle: " << particleName << ", ID:" << particleID << G4endl;
    G4cout << "processType: " << G4VProcess::GetProcessTypeName(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType()) << G4endl;
    G4cout << "process: " << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
    G4cout << "momentumDirection: " << momentumDirection << G4endl;
    G4cout << "initialPoint: " << initialPoint << G4endl;
    G4cout << "finalPoint: " << finalPoint << G4endl;
    G4cout << "length: " << stepLength/mm << "mm" << G4endl;
    G4cout << "edep: " << edep/keV << " keV" << G4endl;
    G4cout << "time: "<< timing << G4endl;

    analysisManager = G4AnalysisManager::Instance();

    analysisManager->FillNtupleDColumn(0, eventNb);
    analysisManager->FillNtupleDColumn(12, eventNb);
    analysisManager->FillNtupleDColumn(13, stepNb);
    analysisManager->FillNtupleDColumn(14, crystNb);
    analysisManager->FillNtupleDColumn(15, materialID); // 1 - scintillator; 0 - smth else
    analysisManager->FillNtupleDColumn(16, particleID); // 1 - gamma; 2 - e-; 0 - smth else
    analysisManager->FillNtupleDColumn(17, initialPointX);
    analysisManager->FillNtupleDColumn(18, initialPointY);
    analysisManager->FillNtupleDColumn(19, initialPointZ);
    analysisManager->FillNtupleDColumn(20, finalPointX);
    analysisManager->FillNtupleDColumn(21, finalPointY);
    analysisManager->FillNtupleDColumn(22, finalPointZ);
    analysisManager->FillNtupleDColumn(23, edep/keV);
    analysisManager->FillNtupleDColumn(24, timing);
    analysisManager->AddNtupleRow();
  }*/

  if (sciCryst->GetAlphaTrackerFlag()) {

    // Collects energy and track data step by step
    G4RunManager* theRunManager = G4RunManager::GetRunManager();

    //G4ThreeVector vertexPosition = theRunManager->GetCurrentEvent()->GetPrimaryVertex()->GetPosition();

    eventNb = theRunManager->GetCurrentEvent()->GetEventID();

    stepNb = step->GetTrack()->GetCurrentStepNumber();

    volID = step->GetTrack()->GetVolume()->GetCopyNo();

    materialName = step->GetTrack()->GetMaterial()->GetName();

    particleName = step->GetTrack()->GetDefinition()->GetParticleName();

    //G4cout << particleName << " " << crystNb << G4endl;
    if (particleName == "alpha") {
      particleID = 3;
    } else if (particleName == "e-") {
      particleID = 1;
    } else {
      particleID = 0;
    }

    momentumDirection = step->GetTrack()->GetMomentumDirection();

    initialPoint = step->GetPreStepPoint()->GetPosition();
    initialPointX = initialPoint.getX();
    initialPointY = initialPoint.getY();
    initialPointZ = initialPoint.getZ();

    finalPoint = step->GetPostStepPoint()->GetPosition();
    finalPointX = finalPoint.getX();
    finalPointY = finalPoint.getY();
    finalPointZ = finalPoint.getZ();

    stepLength = step->GetStepLength();

    edep = step->GetTotalEnergyDeposit();

    timing = step->GetPostStepPoint()->GetGlobalTime();

    if (materialName=="Ar_95_CF4_5") {

      //Uncomment this to print all this information after each step
      /*
      G4cout << "\nEVENT #" << eventNb << G4endl;
      G4cout << "STEP #" << stepNb << G4endl;
      G4cout << "volID: " << volID << G4endl;
      G4cout << "material: " << materialName << ", ID:" << materialID << G4endl;
      G4cout << "particle: " << particleName << ", ID:" << particleID << G4endl;
      G4cout << "processType: " << G4VProcess::GetProcessTypeName(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType()) << G4endl;
      G4cout << "process: " << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
      G4cout << "momentumDirection: " << momentumDirection << G4endl;
      G4cout << "initialPoint: " << initialPoint << G4endl;
      G4cout << "finalPoint: " << finalPoint << G4endl;
      G4cout << "length: " << stepLength/mm << "mm" << G4endl;
      G4cout << "edep: " << edep/keV << " keV" << G4endl;
      G4cout << "time: "<< timing << G4endl;
      */

      analysisManager = G4AnalysisManager::Instance();

      analysisManager->FillNtupleDColumn(0, eventNb);
      analysisManager->FillNtupleDColumn(1, stepNb);
      analysisManager->FillNtupleDColumn(2, initialPointX/mm);
      analysisManager->FillNtupleDColumn(3, initialPointY/mm);
      analysisManager->FillNtupleDColumn(4, initialPointZ/mm);
      analysisManager->FillNtupleDColumn(5, finalPointX/mm);
      analysisManager->FillNtupleDColumn(6, finalPointY/mm);
      analysisManager->FillNtupleDColumn(7, finalPointZ/mm);
      analysisManager->FillNtupleDColumn(8, edep/keV);
      analysisManager->FillNtupleDColumn(9, particleID);
      analysisManager->AddNtupleRow();

    }

    /*
    if (crystNb==0 && stepNb==1 && particleID==1) {
      //Uncomment this to print all this information after each step
      G4cout << "\nEVENT #" << eventNb << G4endl;
      G4cout << "STEP #" << stepNb << G4endl;
      G4cout << "crystNb: " << crystNb << G4endl;
      G4cout << "material: " << materialName << ", ID:" << materialID << G4endl;
      G4cout << "particle: " << particleName << ", ID:" << particleID << G4endl;
      G4cout << "processType: " << G4VProcess::GetProcessTypeName(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType()) << G4endl;
      G4cout << "process: " << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
      G4cout << "momentumDirection: " << momentumDirection << G4endl;
      G4cout << "initialPoint: " << initialPoint << G4endl;
      G4cout << "finalPoint: " << finalPoint << G4endl;
      G4cout << "length: " << stepLength/mm << "mm" << G4endl;
      G4cout << "edep: " << edep/keV << " keV" << G4endl;
      G4cout << "time: "<< timing << G4endl;

      analysisManager = G4AnalysisManager::Instance();

      analysisManager->FillNtupleDColumn(0, eventNb);
      analysisManager->FillNtupleDColumn(12, eventNb);
      analysisManager->FillNtupleDColumn(13, stepNb);
      analysisManager->FillNtupleDColumn(14, crystNb);
      analysisManager->FillNtupleDColumn(15, materialID); // 1 - scintillator; 0 - smth else
      analysisManager->FillNtupleDColumn(16, particleID); // 1 - gamma; 2 - e-; 0 - smth else
      analysisManager->FillNtupleDColumn(17, initialPointX);
      analysisManager->FillNtupleDColumn(18, initialPointY);
      analysisManager->FillNtupleDColumn(19, initialPointZ);
      analysisManager->FillNtupleDColumn(20, finalPointX);
      analysisManager->FillNtupleDColumn(21, finalPointY);
      analysisManager->FillNtupleDColumn(22, finalPointZ);
      analysisManager->FillNtupleDColumn(23, edep/keV);
      analysisManager->FillNtupleDColumn(24, timing);
      analysisManager->AddNtupleRow();
    }*/
  }

}

// ###################################################################################
