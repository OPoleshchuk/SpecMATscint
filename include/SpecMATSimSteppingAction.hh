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

// ###################################################################################

#endif
