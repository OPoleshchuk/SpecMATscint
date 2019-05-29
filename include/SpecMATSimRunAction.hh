///Author: Oleksii Poleshchuk
///
///KU Leuven 2019
///
///SpecMATscint is a GEANT4 code for simulation
///of gamma-rays detection efficiency with
///the SpecMAT scintillation array.
///
///Primarily, this code was written for identification of
///the best geometry of a scintillation array based
///on it's total detection efficiency.
///
/// \file SpecMATSimRunAction.hh
/// \brief Definition of the SpecMATSimRunAction class

#ifndef SpecMATSimRunAction_h
#define SpecMATSimRunAction_h 1

#include "SpecMATSimAnalysis.hh"

#include "G4UserRunAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4Material.hh"

#include "globals.hh"


class G4Run;
class SpecMATSimDetectorConstruction;
class SpecMATSimPrimaryGeneratorAction;
/// Run action class

class SpecMATSimRunAction : public G4UserRunAction
{
public:
  SpecMATSimRunAction();
  virtual ~SpecMATSimRunAction();

  virtual void BeginOfRunAction(const G4Run*);
  virtual void EndOfRunAction(const G4Run*);

  void CountEvents() { fGoodEvents++;};

  G4int fGoodEvents;

private:
  SpecMATSimDetectorConstruction* sciCryst;
  SpecMATSimPrimaryGeneratorAction* gammaSource;

  G4Material* crystMat;

  G4String crystSizeX;
  G4String crystSizeY;
  G4String crystSizeZ;
  G4String crystMatName;
  G4String particleEnergy;
  G4String particleName;
  G4String crystSourceDist;
  G4String chamber;
  G4String insulator;
  G4String chamberName;
  G4String insulatorName;
  G4String flangeMatName;
  G4String insulatorMatName;
  G4String NbSegments;
  G4String Rows;
  G4String Columns;
  G4String circleR;
  G4String Gap;
  G4String fileName;
  G4String ComptSuppFlagTest;
  G4String partName;

  G4double Z;
  G4double A;
  G4double excitEnergy;

  G4int crystNb;
  G4int segmentNb;
  G4int NbOfEvents;

  G4ParticleDefinition* particle;

  G4AnalysisManager* analysisManager;

};

// ###################################################################################

#endif
