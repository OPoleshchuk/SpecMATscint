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

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

