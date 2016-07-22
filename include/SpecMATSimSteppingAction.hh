/// \file SpecMATSimSteppingAction.hh
/// \brief Definition of the SpecMATSimSteppingAction class

#ifndef SpecMATSimSteppingAction_h
#define SpecMATSimSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class SpecMATSimDetectorConstruction;
class SpecMATSimEventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track 
/// lengths of charged particles in Absober and Gap layers and
/// updated in SpecMATSimEventAction.

class SpecMATSimSteppingAction : public G4UserSteppingAction
{
    
private:
  const SpecMATSimDetectorConstruction* fDetConstruction;
  SpecMATSimEventAction*  fEventAction;
  
  /*
  G4ThreeVector initialPoint;
  G4double initialPointX;
  G4double initialPointY;
  G4double initialPointZ;
    
  G4ThreeVector finalPoint;
  G4double finalPointX;
  G4double finalPointY;
  G4double finalPointZ;
  */
    
public:
    SpecMATSimSteppingAction(const SpecMATSimDetectorConstruction* detectorConstruction,
                             SpecMATSimEventAction* eventAction);
    virtual ~SpecMATSimSteppingAction();
    
    virtual void UserSteppingAction(const G4Step* step);
    /*
    void SetInitialPointX(G4double val){initialPointX = val;}
    G4double GetInitialPointX(void){return initialPointX;}
    void SetInitialPointY(G4double val){initialPointY = val;}
    G4double GetInitialPointY(void){return initialPointY;}
    void SetInitialPointZ(G4double val){initialPointZ = val;}
    G4double GetInitialPoint(void){return initialPointZ;}
    
    void SetFinalPointX(G4double val){finalPointX = val;}
    G4double GetFinalPointX(void){return finalPointX;}
    void SetFinalPointY(G4double val){finalPointY = val;}
    G4double GetFinalPointY(void){return finalPointY;}
    void SetFinalPointZ(G4double val){finalPointZ = val;}
    G4double GetFinalPoint(void){return finalPointZ;}
    */
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
