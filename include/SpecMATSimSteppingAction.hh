/// \file SpecMATSimSteppingAction.hh
/// \brief Definition of the SpecMATSimSteppingAction class

#ifndef SpecMATSimSteppingAction_h
#define SpecMATSimSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"

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

public:
    SpecMATSimSteppingAction();
    virtual ~SpecMATSimSteppingAction();

    virtual void UserSteppingAction(const G4Step* step);

    G4ThreeVector initialPoint;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
