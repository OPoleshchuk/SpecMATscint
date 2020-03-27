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

class SpecMATSimDetectorConstruction;
class SpecMATSimPrimaryGeneratorAction;
class SpecMATSimEventAction;

/// Stepping action class.
///
/// In UserSteppingAction() there are collected the energy deposit and track
/// lengths of charged particles in Absober and Gap layers and
/// updated in SpecMATSimEventAction.

class SpecMATSimSteppingAction : public G4UserSteppingAction
{

private:
  //G4RunManager* theRunManager;
  G4AnalysisManager* analysisManager;
  SpecMATSimDetectorConstruction* sciCryst;

  G4ThreeVector momentumDirection;
  G4ThreeVector initialPoint;
  G4ThreeVector finalPoint;

  G4double initialPointX;
  G4double initialPointY;
  G4double initialPointZ;
  G4double finalPointX;
  G4double finalPointY;
  G4double finalPointZ;
  G4double stepLength;
  G4double edep;
  G4double timing;

  G4int eventNb;
  G4int stepNb;
  G4int crystNb;
  G4int volID;
  G4int materialID;
  G4int particleID;

  G4String materialName;
  G4String gasMaterialName;
  G4String particleName;


public:
  SpecMATSimSteppingAction();
  virtual ~SpecMATSimSteppingAction();

  virtual void UserSteppingAction(const G4Step* step);
  
};

// ###################################################################################

#endif
