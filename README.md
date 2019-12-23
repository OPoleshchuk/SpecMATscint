# SpecMATscint
Author: Oleksii Poleshchuk

<img src="https://cloud.githubusercontent.com/assets/17125100/17013239/717b2170-4f1d-11e6-9bf0-72d171279fa0.png" align="right">

SpecMATscint is a GEANT4 simulation of a scintillator array for the SpecMAT detector. This simulation is used to estimate gamma ray detection efficiency of the setup for different detector shapes and for different dimensions and materials of the crystals. As an output, it generates a ROOT file which contains energy spectra obtained from each crystal in the array as well as a summed energy spectrum. To bring this model a bit closer to the real conditions crystals in the simulation have been wrapped in a reflecting material and stored in an aluminum housing.

## How to Build and Run

1. Clone the simulation code
 ```
 $ git clone https://github.com/OPoleshchuk/SpecMATscint.git
 ```

2. Build with CMake
 ```
 $ cd path_to_SpecMATscint         # go to directory which contains the code
 $ mkdir SpecMATscint_build
 $ cd SpecMATscint_build
 $ cmake -DGeant4_DIR=path_to_Geant4_installation/lib[64]/Geant4-[Version]/ ../SpecMATscint
 $ make -jN                        # "N" is the number of processes
 ```

3. Run through one of the following options
  - Execute in the interactive mode:

    ```
    $ ./SpecMATsim
    ```
    ```
    Idle> /control/execute vis.mac    # visualization
    Idle> run/beamOn 1                # beam on
    ```
  - Execute in batch mode from a macro file

    ```
    $ ./SpecMATsim SpecMATsim.in > SpecMATsim.out
    ```
  - or run the script to execute in batch mode and have a progress bar

    ```
    $ ./SpecMATsim.sh
    ```

## Requirements

- [GEANT4 9.6] (http://geant4.web.cern.ch/geant4/support/source_archive.shtml)
- [ROOT 5.34/34] (https://root.cern.ch/downloading-root)
- [CMAKE] (https://cmake.org/download/)
