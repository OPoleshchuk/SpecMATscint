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
/// \file SpecMATSimRunAction.hh
/// \brief Definition of the SpecMATSimRunAction class

#ifndef SpecMATSimRunAction_h
#define SpecMATSimRunAction_h 1

#include "SpecMATSimAnalysis.hh"

#include "G4UserRunAction.hh"
#include "G4ParticleDefinition.hh"
#include "G4Material.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"

#include "globals.hh"

#include <iostream>
#include <fstream>
#include <string>
using namespace std;


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

  //void SetFileName (G4String val){fileName=val;}
  //G4String GetFileName(){return fileName;}


private:
  SpecMATSimDetectorConstruction* sciCryst;
  SpecMATSimPrimaryGeneratorAction* gammaSource;

  ofstream myfile;

  G4Material* crystMat;
  G4Material* vacuumChamberMatRA;

  G4String crystSizeX;
  G4String crystSizeY;
  G4String crystSizeZ;
  G4String crystMatName;
  G4String particleEnergy;
  G4String particleName;
  G4String crystSourceDist;
  G4String chamber;
  G4String fieldCageEpoxy;
  G4String chamberName;
  G4String fieldCageEpoxyName;
  G4String flangeMatName;
  G4String fieldCageEpoxyMatName;
  G4String NbSegments;
  G4String Rows;
  G4String Columns;
  G4String circleR;
  G4String Gap;
  G4String fileName;
  G4String fileNameRoot;
  G4String fileNAmeSettings;
  G4String ComptSuppFlagTest;
  G4String partName;
  G4String vacuumChamberThicknessRA;
  G4String source;
  G4String sourceTypeRA;
  G4String vacuumChamberMatRAName;
  G4String sourcePosition;
  G4String sourceHolderRA;
  G4String sourceHolderRAName;
  G4String sourceHolderRAMatName;
  G4String sourceHousingHildeRA;
  G4String sourceHousingHildeRAName;
  G4String sourceHousingHildeRAMatName;
  G4String sourceHolderRAThick;
  G4String sourceHoldersourceHousingHildeRAThick;
  G4String fieldCageEpoxyThick;
  G4String numberOfEvents;

  G4double Z;
  G4double A;
  G4double excitEnergy;

  G4int crystNb;
  G4int segmentNb;
  G4int NbOfEvents;
  G4int i;

  G4ParticleDefinition* particle;

  G4AnalysisManager* analysisManager;

  G4ThreeVector* crystalPositionsArrayRA;

};

// ###################################################################################

#endif
