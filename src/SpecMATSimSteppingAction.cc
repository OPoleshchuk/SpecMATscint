/// \file SpecMATSimSteppingAction.cc
/// \brief Implementation of the SpecMATSimSteppingAction class

#include "SpecMATSimSteppingAction.hh"
#include "SpecMATSimEventAction.hh"
#include "SpecMATSimDetectorConstruction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

#include "G4Step.hh"
#include "G4RunManager.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpecMATSimSteppingAction::SpecMATSimSteppingAction(
                      const SpecMATSimDetectorConstruction* detectorConstruction,
                      SpecMATSimEventAction* eventAction)
  : G4UserSteppingAction(),
    fDetConstruction(detectorConstruction),
    fEventAction(eventAction)
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpecMATSimSteppingAction::~SpecMATSimSteppingAction()
{
    
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SpecMATSimSteppingAction::UserSteppingAction(const G4Step* step)
{
// Collect energy and track length step by step

  // get volume of the current step
  G4VPhysicalVolume* volume 
    = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume();
  
    // energy deposit
    G4double edep = step->GetTotalEnergyDeposit();
    G4cout << "\nTest edep: " << edep/keV << "keV" << G4endl;
    
  // step length
  
  G4double stepLength = 0.;
  
  //if ( step->GetTrack()->GetDefinition()->GetPDGCharge() != 0. ) {
    stepLength = step->GetStepLength();
    G4ThreeVector initialPoint = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector finalPoint = step->GetPostStepPoint()->GetPosition();
    
    G4double initialPointX = initialPoint.getX();
    G4double initialPointY = initialPoint.getY();
    G4double initialPointZ = initialPoint.getZ();
    
    G4double finalPointX = finalPoint.getX();
    G4double finalPointY = finalPoint.getY();
    G4double finalPointZ = finalPoint.getZ();
    
    G4cout << "TestInit: " << initialPoint << G4endl;
    G4cout << "TestFinal: " << finalPoint << G4endl;
    G4cout << "Length: " << stepLength/mm << "mm" << G4endl;
  //}
  
  /*
  if ( volume == fDetConstruction->GetAbsorberPV() ) {
    fEventAction->AddAbs(edep,stepLength);
  }
  
  if ( volume == fDetConstruction->GetGapPV() ) {
    fEventAction->AddGap(edep,stepLength);
  }
  */

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
