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
/// \file SpecMATSimPhysicsList.cc
/// \brief Implementation of the SpecMATSimPhysicsList class

#include "SpecMATSimPhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4EmStandardPhysics.hh"

// ###################################################################################

SpecMATSimPhysicsList::SpecMATSimPhysicsList()
: G4VModularPhysicsList()
{
  SetVerboseLevel(1);

  // Default physics
  RegisterPhysics(new G4DecayPhysics());

  // Radioactive decay
  RegisterPhysics(new G4RadioactiveDecayPhysics());

  // EM physics
  RegisterPhysics(new G4EmStandardPhysics());
}

// ###################################################################################

SpecMATSimPhysicsList::~SpecMATSimPhysicsList()
{
}

// ###################################################################################

void SpecMATSimPhysicsList::SetCuts()
{
  G4VUserPhysicsList::SetCuts();
}

// ###################################################################################
