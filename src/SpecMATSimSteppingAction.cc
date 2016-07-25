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
  // Collect energy and track data step by step
  G4RunManager* theRunManager = G4RunManager::GetRunManager();
  G4int eventNb = theRunManager->GetCurrentEvent()->GetEventID();
  G4cout << "\nEVENT #" << eventNb << G4endl;
  G4int stepNb = step->GetTrack()->GetCurrentStepNumber();
  G4cout << "STEP #" << stepNb << G4endl; 
  
  G4String partName = step->GetTrack()->GetDefinition()->GetParticleName();
  G4String materialName = step->GetTrack()->GetMaterial()->GetName();
  G4int crystNb = step->GetTrack()->GetVolume()->GetCopyNo();
  G4ThreeVector momentumDirection = step->GetTrack()->GetMomentumDirection();
  G4ThreeVector initialPoint = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector finalPoint = step->GetPostStepPoint()->GetPosition();
  G4double stepLength = step->GetStepLength();
  G4double edep = step->GetTotalEnergyDeposit();
  G4double time = step->GetPostStepPoint()->GetGlobalTime();

  G4double initialPointX = initialPoint.getX();
  G4double initialPointY = initialPoint.getY();
  G4double initialPointZ = initialPoint.getZ(); 
  G4double finalPointX = finalPoint.getX();
  G4double finalPointY = finalPoint.getY();
  G4double finalPointZ = finalPoint.getZ();
    
  G4cout << "particle: " << partName << G4endl;
  G4cout << "processType: " << G4VProcess::GetProcessTypeName(step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessType()) << G4endl;
  G4cout << "process: " << step->GetPostStepPoint()->GetProcessDefinedStep()->GetProcessName() << G4endl;
  G4cout << "material: " << materialName << G4endl;
  G4cout << "crystNb: " << crystNb << G4endl;
  G4cout << "momentumDirection: " << momentumDirection << G4endl;
  G4cout << "initialPoint: " << initialPoint << G4endl;
  G4cout << "finalPoint: " << finalPoint << G4endl;
  G4cout << "length: " << stepLength/mm << "mm" << G4endl;
  G4cout << "edep: " << edep/keV << "keV" << G4endl;
  G4cout << "time: "<< time << G4endl;
    
  G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  if (materialName == "CeBr3" || materialName == "LaBr3") {
  	analysisManager->FillNtupleDColumn(3, eventNb);
    	analysisManager->FillNtupleDColumn(4, stepNb);
    	//analysisManager->FillNtupleSColumn(5, partName); 
    	//analysisManager->FillNtupleSColumn(6, materialName); 
    	analysisManager->FillNtupleDColumn(5, crystNb); 
    	analysisManager->FillNtupleDColumn(6, initialPointX); 
    	analysisManager->FillNtupleDColumn(7, initialPointY); 
    	analysisManager->FillNtupleDColumn(8, initialPointZ); 
    	analysisManager->FillNtupleDColumn(9, finalPointX); 
    	analysisManager->FillNtupleDColumn(10, finalPointY); 
    	analysisManager->FillNtupleDColumn(11, finalPointZ); 
    	analysisManager->FillNtupleDColumn(12, edep/keV); 
	analysisManager->FillNtupleDColumn(13, time);
	analysisManager->AddNtupleRow();
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
