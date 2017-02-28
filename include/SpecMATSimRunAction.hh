/// \file SpecMATSimRunAction.hh
/// \brief Definition of the SpecMATSimRunAction class

#ifndef SpecMATSimRunAction_h
#define SpecMATSimRunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4Material.hh"

class G4Run;
class SpecMATSimDetectorConstruction;
class SpecMATSimPrimaryGeneratorAction;
/// Run action class

class SpecMATSimRunAction : public G4UserRunAction
{
  public:
    SpecMATSimRunAction();
    virtual ~SpecMATSimRunAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void EndOfRunAction(const G4Run*);

    void CountEvents() { fGoodEvents++;};

    G4int fGoodEvents;

  private:
    SpecMATSimDetectorConstruction* sciCryst;
    SpecMATSimPrimaryGeneratorAction* gammaSource;
    G4Material* crystMat;

    G4String crystSizeX;
    G4String crystSizeY;
    G4String crystSizeZ;
    G4String crystMatName;

    G4String particleEnergy;
    G4String particleName;
    G4String crystSourceDist;

    G4String chamber;
    G4String insulator;
    G4String chamberName;
    G4String insulatorName;
    G4String flangeMatName;
    G4String insulatorMatName;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
