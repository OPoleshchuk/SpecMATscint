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
#include <iostream>

// ###################################################################################

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
  gammaEnergy=1000*keV;

  //################### Isotope source ################################//
  Z = 27;
  A = 60;
  ionCharge = 0.*eplus;
  excitEnergy = 0.*MeV;
  ionEnergy = 0.*MeV;
}

// ###################################################################################

SpecMATSimPrimaryGeneratorAction::~SpecMATSimPrimaryGeneratorAction()
{
  delete fParticleGun;
  delete sciCryst;
}

// ###################################################################################

void SpecMATSimPrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  if (source == "gamma") {
    //################### Monoenergetic gamma source ############################//
    //this function is called at the begining of event
    //
    //distribution uniform in solid angle
    //
    G4ParticleDefinition* particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(gammaEnergy);
    G4double cosTheta = 2*G4UniformRand() - 1., phi = twopi*G4UniformRand();
    G4double sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    G4double  ux = sinTheta*std::cos(phi),
              uy = sinTheta*std::sin(phi),
              uz = cosTheta;
    //G4int randomNum = G4UniformRand()*201 - 100;
    G4int randomNum = G4UniformRand()*131 - 65;
    G4cout << randomNum << G4endl;
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
    fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,randomNum*mm)); //randomNum*mm
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  else {
    //################### Isotope source ################################//
    G4ParticleDefinition* ion= G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy);
    fParticleGun->SetParticleDefinition(ion);
    fParticleGun->SetParticleCharge(ionCharge);
    G4int randomNum = G4UniformRand()*131 - 65;
    G4cout << randomNum << G4endl;
    fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,randomNum*mm));
    fParticleGun->SetParticleEnergy(ionEnergy);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,29.25));
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
}


// ###################################################################################
