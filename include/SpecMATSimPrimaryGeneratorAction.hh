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
/// \file SpecMATSimPrimaryGeneratorAction.hh
/// \brief Definition of the SpecMATSimPrimaryGeneratorAction class

#ifndef SpecMATSimPrimaryGeneratorAction_h
#define SpecMATSimPrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "globals.hh"

class G4ParticleGun;
class G4Event;
class SpecMATSimDetectorConstruction;

/// The primary generator action class with particle gum.
///
/// It defines an ion (Co60), at rest). Ion can be changed
/// with the G4ParticleGun commands (see run2.mac).

class SpecMATSimPrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
public:
  G4int Z;
  G4int A;
  G4int n_particle;
  G4int dopplerShiftSimFlag;
  G4int tripleAlphaSourceFlag;

  G4double ionCharge;
  G4double excitEnergy;
  G4double ionEnergy;
  G4double gammaEnergy;
  G4double cosTheta;
  G4double sinTheta;
  G4double phi;
  G4double theta;
  G4double ux, uy, uz;
  G4double pointSourceZposition;
  G4double circleSourcePhi;
  G4double circleSourceR;
  G4double circleSourcePositionX;
  G4double circleSourcePositionY;

  G4String source;
  G4String sourceType;

  SpecMATSimPrimaryGeneratorAction();
  virtual ~SpecMATSimPrimaryGeneratorAction();

  virtual void GeneratePrimaries(G4Event*);

  const G4ParticleGun* GetParticleGun() const { return fParticleGun; }

  void SetDistFromCrystSurfToSource(G4double val) { distFromCrystSurfToSource = val; }
  G4double GetDistFromCrystSurfToSource(void) { return distFromCrystSurfToSource; }

  void SetGammaEnergy(G4double val) { gammaEnergy = val; }
  G4double GetGammaEnergy(void) { return gammaEnergy; }

  void SetZ(G4double val) { Z = val; }
  G4double GetZ(void) { return Z; }

  void SetA(G4double val) { A = val; }
  G4double GetA(void) { return A; }

  void SetIonCharge(G4double val) { ionCharge = val; }
  G4double GetIonCharge(void) { return ionCharge; }

  void SetExcitEnergy(G4double val) { excitEnergy = val; }
  G4double GetExcitEnergy(void) { return excitEnergy; }

  void SetIonEnergy(G4double val) { ionEnergy = val; }
  G4double GetIonEnergy(void) { return ionEnergy; }

  void SetSource(G4String val) { source = val; }
  G4String GetSource(void) { return source; }

  void SetSourceType(G4String val) { sourceType = val; }
  G4String GetSourceType(void) { return sourceType; }

  void SetPointSourceZposition(G4double val) { pointSourceZposition = val; }
  G4double GetPointSourceZposition(void) { return pointSourceZposition; }

  void SetPointSourceRandomNum(G4double val) { randomNum = val; }
  G4double GetPointSourceRandomNum(void) { return randomNum; }

private:
  SpecMATSimDetectorConstruction* sciCryst;
  G4ParticleDefinition* particle;
  G4ParticleDefinition* ion;

  G4ParticleGun* fParticleGun;

  G4double distFromCrystSurfToSource;
  G4double vacuumChamberShiftPGA;
  G4double randomNum;
};

// ###################################################################################

#endif
