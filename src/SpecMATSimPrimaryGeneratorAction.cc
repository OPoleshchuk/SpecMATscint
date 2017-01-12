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
#include <stdlib.h>
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpecMATSimPrimaryGeneratorAction::SpecMATSimPrimaryGeneratorAction()
 : G4VUserPrimaryGeneratorAction(),
   sciCryst(0),
   fParticleGun(0)
{
  //source = "gamma";
  source = "ion";

  //################### Monoenergetic gamma source ############################//
  n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
  sciCryst = new SpecMATSimDetectorConstruction();
  gammaEnergy = 1000*keV;

  //################### Isotope source ################################//
  Z = 27;
  A = 60;
  ionCharge = 0.*eplus;
  excitEnergy = 0.*MeV;
  ionEnergy = 0.*MeV;
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

  if (source == "gamma") {
      //################### Monoenergetic gamma source ############################//
      //this function is called at the begining of event
      //
      //distribution uniform in solid angle
      //
      G4ParticleDefinition* particle
               = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
      fParticleGun->SetParticleDefinition(particle);
      fParticleGun->SetParticleEnergy(gammaEnergy);
      G4double cosTheta = 2*G4UniformRand() - 1., phi = twopi*G4UniformRand();
      G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
      G4double ux = sinTheta*std::cos(phi),
               uy = sinTheta*std::sin(phi),
               uz = cosTheta;
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
      fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,0.*mm));
      fParticleGun->GeneratePrimaryVertex(anEvent);
  } else {
      //################### Isotope source ################################//
      G4ParticleDefinition* ion
             = G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy);
      fParticleGun->SetParticleDefinition(ion);
      fParticleGun->SetParticleCharge(ionCharge);
      fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,0.*mm));
      fParticleGun->SetParticleEnergy(ionEnergy);
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.,0.,0.));
      fParticleGun->GeneratePrimaryVertex(anEvent);
  }

}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
