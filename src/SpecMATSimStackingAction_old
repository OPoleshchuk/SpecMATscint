/// \file SpecMATSimStackingAction.cc
/// \brief Implementation of the SpecMATSimStackingAction class

#include "SpecMATSimStackingAction.hh"

#include "G4Track.hh"
#include "G4NeutrinoE.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpecMATSimStackingAction::SpecMATSimStackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

SpecMATSimStackingAction::~SpecMATSimStackingAction()
{ }

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ClassificationOfNewTrack
SpecMATSimStackingAction::ClassifyNewTrack(const G4Track* track)
{
  //keep primary particle
  if (track->GetParentID() == 0) return fUrgent;

  //kill secondary neutrino
  if (track->GetDefinition() == G4NeutrinoE::NeutrinoE()) return fKill;
  else return fUrgent;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
