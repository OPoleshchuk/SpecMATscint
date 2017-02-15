/// \file SpecMATSim.cc
/// \brief Main program of the SpecMATSim

#include "G4RunManager.hh"
#include "G4UImanager.hh"

#include "Randomize.hh"

#include "SpecMATSimDetectorConstruction.hh"
#include "SpecMATSimPhysicsList.hh"
#include "SpecMATSimPrimaryGeneratorAction.hh"
#include "SpecMATSimRunAction.hh"
#include "SpecMATSimEventAction.hh"
#include "SpecMATSimSteppingAction.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  // Choose the Random engine
  //
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);

  // Construct the default run manager
  //
  G4RunManager * runManager = new G4RunManager;

  // Set mandatory initialization classes
  //
  runManager->SetUserInitialization(new SpecMATSimDetectorConstruction);
  //
  runManager->SetUserInitialization(new SpecMATSimPhysicsList);

  // Set user action classes
  //
  runManager->SetUserAction(new SpecMATSimPrimaryGeneratorAction);
  //
  SpecMATSimRunAction* runAction = new SpecMATSimRunAction();
  runManager->SetUserAction(runAction);
  //
  runManager->SetUserAction(new SpecMATSimEventAction(runAction));
  //
  //runManager->SetUserAction(new SpecMATSimStackingAction);

  SpecMATSimSteppingAction* steppingAction
  = new SpecMATSimSteppingAction();
  runManager->SetUserAction(steppingAction);
  // Initialize G4 kernel
  //
  runManager->Initialize();

#ifdef G4VIS_USE
  // Initialize visualization
  G4VisManager* visManager = new G4VisExecutive;
  // G4VisExecutive can take a verbosity argument - see /vis/verbose guidance.
  // G4VisManager* visManager = new G4VisExecutive("Quiet");
  visManager->Initialize();
#endif

  // Get the pointer to the User Interface manager
  G4UImanager* UImanager = G4UImanager::GetUIpointer();

  if (argc!=1)   // batch mode
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }
  else
    {  // interactive mode : define UI session
#ifdef G4UI_USE
      G4UIExecutive* ui = new G4UIExecutive(argc, argv);
/*
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");
#endif
*/
      ui->SessionStart();
      delete ui;
#endif
    }

  // Job termination
  // Free the store: user actions, physics_list and detector_description are
  // owned and deleted by the run manager, so they should not be deleted
  // in the main() program !

#ifdef G4VIS_USE
  delete visManager;
#endif
  delete runManager;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo.....
