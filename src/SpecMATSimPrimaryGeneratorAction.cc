/// \file SpecMATSimPrimaryGeneratorAction.cc
/// \brief Implementation of the SpecMATSimPrimaryGeneratorAction class

#include "SpecMATSimPrimaryGeneratorAction.hh"
#include "SpecMATSimDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include <cmath>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpecMATSimPrimaryGeneratorAction::SpecMATSimPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   sciCryst(0),
   fParticleGun(0)
{
  /*
  //################### Monoenergetic gamma source ############################//
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  sciCryst = new SpecMATSimDetectorConstruction();
  
  gammaEnergy = 15000*keV;
  // default particle kinematic
  //

  G4ParticleDefinition* particle
           = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
  fParticleGun->SetParticleDefinition(particle); 
  //################### Monoenergetic gamma source ############################//
  */

  //#########################  Isotope  #######################################//
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  sciCryst = new SpecMATSimDetectorConstruction();
  
  // default particle kinematic
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle
                    = particleTable->FindParticle("geantino");//geantino
  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,-500.*mm));
  fParticleGun->SetParticleEnergy(10*MeV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  //#########################  Isotope  #######################################//
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpecMATSimPrimaryGeneratorAction::~SpecMATSimPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete sciCryst;

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void SpecMATSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  /*
  //################### Monoenergetic gamma source ############################//

  //this function is called at the begining of event
  //
  //distribution uniform in solid angle
  //
  
  //Old not used resolution correction of generated gamma energy, new correction done in EventAction after gamma energy has been deposited in the crystal volume. 
  //fParticleGun->SetParticleEnergy(G4RandGauss::shoot(gammaEnergy, (40/2.355)*keV));
  
  fParticleGun->SetParticleEnergy(gammaEnergy);

  G4double cosTheta = 2*G4UniformRand() - 1., phi = twopi*G4UniformRand();
  G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
  G4double ux = sinTheta*std::cos(phi),
           uy = sinTheta*std::sin(phi),
           uz = cosTheta;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
  
  
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,0.*mm));
  fParticleGun->GeneratePrimaryVertex(anEvent);
  //################### Monoenergetic gamma source ############################//
  */
  
   
  //#########################  Isotope  #######################################//
  G4ParticleDefinition* particle = fParticleGun->GetParticleDefinition();
  if (particle == G4Geantino::Geantino()) {
    G4int Z = 28, A = 60;
    
    G4double ionCharge   = 0.*eplus;
    G4double excitEnergy = 2.*MeV;
    
    G4ParticleDefinition* ion
       = G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(ionCharge); 
  }
  
  fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,0.*mm));
  fParticleGun->SetParticleEnergy(190*MeV);
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));           
  fParticleGun->GeneratePrimaryVertex(anEvent);
  //#########################  Isotope  #######################################// 
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

