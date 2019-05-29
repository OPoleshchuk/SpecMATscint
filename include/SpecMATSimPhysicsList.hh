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
/// \file SpecMATSimPhysicsList.hh
/// \brief Definition of the SpecMATSimPhysicsList class

#ifndef SpecMATSimPhysicsList_h
#define SpecMATSimPhysicsList_h 1

#include "G4VModularPhysicsList.hh"

/// Modular physics list
///
/// It includes the folowing physics builders
/// - G4DecayPhysics
/// - G4RadioactiveDecayPhysics
/// - G4EmStandardPhysics

class SpecMATSimPhysicsList: public G4VModularPhysicsList
{
public:
  SpecMATSimPhysicsList();
  virtual ~SpecMATSimPhysicsList();

  virtual void SetCuts();
};

// ###################################################################################

#endif
