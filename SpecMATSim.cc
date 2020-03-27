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
/// \file SpecMATSim.cc
/// \brief Main program of the SpecMATSim

#include "SpecMATSimDetectorConstruction.hh"
#include "SpecMATSimPhysicsList.hh"
#include "SpecMATSimPrimaryGeneratorAction.hh"
#include "SpecMATSimRunAction.hh"
#include "SpecMATSimEventAction.hh"
#include "SpecMATSimSteppingAction.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UIsession.hh"

#include "Randomize.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif

// ###################################################################################

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
  SpecMATSimDetectorConstruction* detector = new SpecMATSimDetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  runManager->SetUserInitialization(new SpecMATSimPhysicsList);

  // Set user action classes
  //
  SpecMATSimPrimaryGeneratorAction* primary = new SpecMATSimPrimaryGeneratorAction;
  runManager->SetUserAction(primary);
  //
  SpecMATSimRunAction* runAction = new SpecMATSimRunAction();
  runManager->SetUserAction(runAction);
  //
  runManager->SetUserAction(new SpecMATSimEventAction(runAction));

  SpecMATSimSteppingAction* steppingAction = new SpecMATSimSteppingAction();
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
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);  //for interractive interface
    //G4UIsession* ui = 0;                                  //for idle> interface G4UIterminal – C-shell-like character terminaln runs on all Geant4-supported platforms
    /*
    #ifdef G4VIS_USE
    UImanager->ApplyCommand("/control/execute vis.mac");
    #endif
    */
    //ui = new G4UIterminal;                                //for idle> interface G4UIterminal – C-shell-like character terminaln runs on all Geant4-supported platforms
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

// ###################################################################################
