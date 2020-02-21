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
/// \file SpecMATSimPrimaryGeneratorAction.cc
/// \brief Implementation of the SpecMATSimPrimaryGeneratorAction class

#include "SpecMATSimPrimaryGeneratorAction.hh"
#include "SpecMATSimDetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4IonTable.hh"
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
  dopplerShiftSim = 0; //1=yes 0=no
  //sourceType = "linear";
  sourceType = "point";
  sciCryst = new SpecMATSimDetectorConstruction();
  pointSourceZposition= sciCryst->GetPointSourcePositionZ(); //PP=-129.25mm Cathode=194.25mm FCcentre=32.5mm for the 3rings configuration MiddleRingCentre=0mm
  //################### Monoenergetic gamma source ############################//
  n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);

  gammaEnergy=661.7*keV;

  //################### Isotope source ################################//
  Z = 94; //Cs-137
  A = 239; //Cs-137
  ionCharge = 0.*eplus;
  excitEnergy = 0.*keV;
  ionEnergy = 0.*MeV;
  if (dopplerShiftSim) {
    Z = 10; // non-existing Ne-15
    A = 15; // non-existing Ne-15
    ionCharge = 0.*eplus;
    //excitEnergy = 0.*keV;
    excitEnergy = 3000*keV; // for non-existing Ne-15 to define energy level and excitation energy

    //ionEnergy = 0.*MeV;
    ionEnergy = A*10.*MeV;
  }

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
    vacuumChamberShiftPGA = sciCryst->GetVacuumChamberShift();
    particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleEnergy(gammaEnergy);
    cosTheta = 2*G4UniformRand() - 1.;
    phi = twopi*G4UniformRand();
    sinTheta = std::sqrt(1. - cosTheta*cosTheta);
    ux = sinTheta*std::cos(phi);
    uy = sinTheta*std::sin(phi);
    uz = cosTheta;
    //G4int randomNum = G4UniformRand()*201 - 100;
    randomNum = 32.5 + G4UniformRand()*323.5 - 323.5/2; //32.5mm is the offest of the center of the field cage in respect to the center of the middle ring(out of the 3rings) also world center of the simulation

    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
    if (sourceType == "linear") {
      fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,randomNum*mm)); //randomNum*mm
    } else {
      fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,pointSourceZposition*mm));
    }
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
  else {
    //################### Isotope source ################################//
    if (sciCryst->GetAlphaTrackerFlag()) {
      /*
      int randNumb = rand()%100+1;

      if ((33 > randNumb) && (randNumb > 0)) {
        Z = 94; //Pu-239
        A = 239;
      } else if ((66 > randNumb) && (randNumb > 32)) {
        Z = 95; //Am-241
        A = 241;
      } else {
        Z = 96; //Cm-244
        A = 244;
      }*/

      Z = 2; //He-4
      A = 4;

      int randNumb = rand()%100+1;
      if ((33 > randNumb) && (randNumb > 0)) {
        int randNumbPu239 = rand()%10000+1;
        if (randNumbPu239>0 && randNumbPu239<=1194) {
          ionEnergy = 5.1055*MeV;
        } else if (randNumbPu239>1194 && randNumbPu239<=(1711+1194)) {
          ionEnergy = 5.1443*MeV;
        } else {
          ionEnergy = 5.1567*MeV;
        }
      } else if ((66 > randNumb) && (randNumb > 32)) {
        int randNumbAm241 = rand()%10000+1;
        if (randNumbAm241>0 && randNumbAm241<=166) {
          ionEnergy = 5.388*MeV;
        } else if (randNumbAm241>166 && randNumbAm241<=(166+1310)) {
          ionEnergy = 5.4428*MeV;
        } else {
          ionEnergy = 5.48556*MeV;
        }
      } else {
        int randNumbCm244 = rand()%10000+1;
        if (randNumbCm244>0 && randNumbCm244<=2310) {
          ionEnergy = 5.76264*MeV;
        } else {
          ionEnergy = 5.80488*MeV;
        }
      }

      cosTheta = G4UniformRand();
      phi = twopi*G4UniformRand();
      sinTheta = std::sqrt(1. - cosTheta*cosTheta);
      ux = sinTheta*std::cos(phi);
      uy = sinTheta*std::sin(phi);
      uz = cosTheta;

      ion = G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy);
      fParticleGun->SetParticleDefinition(ion);
      fParticleGun->SetParticleCharge(ionCharge);
      fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,pointSourceZposition*mm));
      fParticleGun->SetParticleEnergy(ionEnergy);
      //fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,0.));
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(ux,uy,uz));
      fParticleGun->GeneratePrimaryVertex(anEvent);

    } else {
      if (dopplerShiftSim) {
        ion = G4ParticleTable::GetParticleTable()->FindIon(Z,A,excitEnergy);
      }
      ion = G4ParticleTable::GetParticleTable()->GetIon(Z,A,excitEnergy);
      fParticleGun->SetParticleDefinition(ion);
      fParticleGun->SetParticleCharge(ionCharge);
      randomNum = 32.5 + G4UniformRand()*323.5 - 323.5/2;
      //G4cout << randomNum << G4endl;
      if (sourceType == "linear") {
        fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,randomNum*mm)); //randomNum*mm
      } else {
        fParticleGun->SetParticlePosition(G4ThreeVector(0.*mm,0.*mm,pointSourceZposition*mm));
      }
      fParticleGun->SetParticleEnergy(ionEnergy);
      fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,-1.));
      fParticleGun->GeneratePrimaryVertex(anEvent);
    }

  }
}

// ###################################################################################
