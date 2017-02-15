/// \file SpecMATSimStackingAction.hh
/// \brief Definition of the SpecMATSimStackingAction class

#ifndef SpecMATSimStackingAction_h
#define SpecMATSimStackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"

/// Stacking action class : manage the newly generated particles
///
/// One wishes do not track secondary neutrino.Therefore one kills it 
/// immediately, before created particles will  put in a stack.

class SpecMATSimStackingAction : public G4UserStackingAction
{
  public:
    SpecMATSimStackingAction();
    virtual ~SpecMATSimStackingAction();
     
    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);        
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

