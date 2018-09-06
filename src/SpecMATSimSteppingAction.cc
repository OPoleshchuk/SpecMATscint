/// \file SpecMATSimSteppingAction.cc
/// \brief Implementation of the SpecMATSimSteppingAction class

#include "SpecMATSimSteppingAction.hh"
#include "SpecMATSimEventAction.hh"
#include "SpecMATSimDetectorConstruction.hh"
#include "SpecMATSimAnalysis.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpecMATSimSteppingAction::SpecMATSimSteppingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpecMATSimSteppingAction::~SpecMATSimSteppingAction()
{

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SpecMATSimSteppingAction::UserSteppingAction(const G4Step* step)
{
  // Collects energy and track data step by step
  G4RunManager* theRunManager = G4RunManager::GetRunManager();

  //G4ThreeVector vertexPosition = theRunManager->GetCurrentEvent()->GetPrimaryVertex()->GetPosition();

  G4int eventNb = theRunManager->GetCurrentEvent()->GetEventID();

  G4int stepNb = step->GetTrack()->GetCurrentStepNumber();

  G4int crystNb = step->GetTrack()->GetVolume()->GetCopyNo();

  G4String materialName = step->GetTrack()->GetMaterial()->GetName();
  G4int materialID;
  if (materialName == "CeBr3" || materialName == "LaBr3") { // 1 - scintillator; 0 - smth else
      materialID = 1;
  }
  else {
      materialID = 0;
  }

  G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
  G4int particleID;
  if (particleName == "gamma") {
      particleID = 1;
  } else if (particleName == "e-") {
      particleID = 2;
  } else {
      particleID = 0;
  }

  G4ThreeVector momentumDirection = step->GetTrack()->GetMomentumDirection();

  initialPoint = step->GetPreStepPoint()->GetPosition();
  G4double initialPointX = initialPoint.getX();
  G4double initialPointY = initialPoint.getY();
  G4double initialPointZ = initialPoint.getZ();

  G4ThreeVector finalPoint = step->GetPostStepPoint()->GetPosition();
  G4double finalPointX = finalPoint.getX();
  G4double finalPointY = finalPoint.getY();
  G4double finalPointZ = finalPoint.getZ();

  //G4double stepLength = step->GetStepLength();

  G4double edep = step->GetTotalEnergyDeposit();

  G4double timeing = step->GetPostStepPoint()->GetGlobalTime();
  /*
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
  G4cout << "edep: " << edep/keV << "keV" << G4endl;
  G4cout << "time: "<< timeing << G4endl;
  
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

  analysisManager->FillNtupleDColumn(4, eventNb);
  analysisManager->FillNtupleDColumn(5, stepNb);
  analysisManager->FillNtupleDColumn(6, crystNb);
  analysisManager->FillNtupleDColumn(7, materialID); // 1 - scintillator; 0 - smth else
  analysisManager->FillNtupleDColumn(8, particleID); // 1 - gamma; 2 - e-; 0 - smth else
  analysisManager->FillNtupleDColumn(9, initialPointX);
  analysisManager->FillNtupleDColumn(10, initialPointY);
  analysisManager->FillNtupleDColumn(11, initialPointZ);
  analysisManager->FillNtupleDColumn(12, finalPointX);
  analysisManager->FillNtupleDColumn(13, finalPointY);
  analysisManager->FillNtupleDColumn(14, finalPointZ);
  analysisManager->FillNtupleDColumn(15, edep/keV);
  analysisManager->FillNtupleDColumn(16, timeing);
  analysisManager->AddNtupleRow();
  */
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
