/// \file SpecMATSimEventAction.hh
/// \brief Definition of the SpecMATSimEventAction class

#ifndef SpecMATSimEventAction_h
#define SpecMATSimEventAction_h 1

#include "G4UserEventAction.hh"
#include "G4THitsMap.hh"
#include "G4Material.hh"
#include "globals.hh"

class SpecMATSimRunAction;
class G4GenericMessenger;
class SpecMATSimDetectorConstruction;

/// Event action class
///
/// In EndOfEventAction() there is collected information event per event
/// from Hits Collections, and accumulated statistic for
/// SpecMATSimRunAction::EndOfRunAction().

class SpecMATSimEventAction : public G4UserEventAction
{
  public:
    SpecMATSimEventAction(SpecMATSimRunAction* runAction);
    virtual ~SpecMATSimEventAction();

    virtual void  BeginOfEventAction(const G4Event* );
    virtual void    EndOfEventAction(const G4Event* );

    void SetPrintModulo(G4int value);

    G4double absoEdep;
    G4double edepComptSuppRes;



  private:
  // methods
    G4THitsMap<G4double>* GetHitsCollection(const G4String& hcName,
                                          const G4Event* event) const;
    G4double GetSum(G4THitsMap<G4double>* hitsMap) const;
    void PrintEventStatistics(G4double absoEdep) const;
    SpecMATSimDetectorConstruction* sciCryst;
    SpecMATSimRunAction*  fRunAct;

    G4int fCollID_cryst;
    G4int fCollID_ComptSupp;
	G4int fCollID_ring;

    G4Material* crystMat;
    G4int fPrintModulo;

    G4int copyNbComptSupp;
    G4double edepComptSupp;

    G4int copyNb;
    G4double edep;
};

// inline functions

inline void SpecMATSimEventAction::SetPrintModulo(G4int value) {
  fPrintModulo = value;
}
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
