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
/// \file SpecMATSimEventAction.hh
/// \brief Definition of the SpecMATSimEventAction class

#ifndef SpecMATSimEventAction_h
#define SpecMATSimEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4Material.hh"
#include "globals.hh"

#include "SpecMATSimEventAction.hh"
#include "SpecMATSimRunAction.hh"
#include "SpecMATSimAnalysis.hh"
#include "SpecMATSimDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4GenericMessenger.hh"
#include "G4THitsMap.hh"
#include "G4THitsCollection.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4THitsCollection.hh"

#include "Randomize.hh"
#include <iomanip>
#include <cmath>

class SpecMATSimRunAction;
class G4GenericMessenger;
class SpecMATSimDetectorConstruction;

/// Event action class
///
/// In EndOfEventAction() there is collected information event per event
/// from Hits Collections, and accumulated statistic for
/// SpecMATSimRunAction::EndOfRunAction().

class SpecMATSimEventAction : public G4UserEventAction
{
public:
  SpecMATSimEventAction(SpecMATSimRunAction* runAction);
  virtual ~SpecMATSimEventAction();

  virtual void  BeginOfEventAction(const G4Event* );
  virtual void  EndOfEventAction(const G4Event* );

  void SetPrintModulo(G4int value);

  G4double absoEdep;
  G4double edepComptSuppRes;

private:
  G4THitsMap<G4double>* GetHitsCollection(const G4String& hcName, const G4Event* event) const;
  G4double GetSum(G4THitsMap<G4double>* hitsMap) const;
  void PrintEventStatistics(G4double absoEdep) const;
  SpecMATSimDetectorConstruction* sciCryst;
  SpecMATSimRunAction*  fRunAct;
  SpecMATSimPrimaryGeneratorAction* gammaSource;

  G4int fCollID_cryst;
  G4int fCollID_ComptSupp;
  G4int fCollID_ring;
  G4int eventNb;
  G4int nbOfFired;
  G4int nbOfFiredComptSupp;
  G4int fPrintModulo;
  G4int copyNbComptSupp;
  G4int copyNb;

  G4String ComptSuppFlagTest;

  G4Material* crystMat;

  G4double edepComptSupp;
  G4double edep;

  G4SDManager* SDMan;

  G4AnalysisManager* analysisManager;

  G4HCofThisEvent* HCE;
};

// inline functions

inline void SpecMATSimEventAction::SetPrintModulo(G4int value) {
  fPrintModulo = value;
}
// ###################################################################################

#endif
