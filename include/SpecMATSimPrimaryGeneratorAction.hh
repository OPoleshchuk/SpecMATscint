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
    SpecMATSimPrimaryGeneratorAction();    
    virtual ~SpecMATSimPrimaryGeneratorAction();

    virtual void GeneratePrimaries(G4Event*);         

    const G4ParticleGun* GetParticleGun() const { return fParticleGun; }
    
    void SetDistFromCrystSurfToSource(G4double val) { distFromCrystSurfToSource = val; }
    G4double GetDistFromCrystSurfToSource(void) { return distFromCrystSurfToSource;}
    
    void SetGammaEnergy(G4double val) { gammaEnergy = val; }
    G4double GetGammaEnergy(void) { return gammaEnergy;}
  private:
    SpecMATSimDetectorConstruction* sciCryst;
    
    G4ParticleGun*  fParticleGun; 
	
    G4double distFromCrystSurfToSource;
    G4double gammaEnergy;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif


