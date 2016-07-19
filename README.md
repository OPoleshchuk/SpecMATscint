# SpecMATscint
Simulation of a scintillator array for the SpecMAT detector in GEANT4

How to build and run:
Build with CMake:
```
% cd path_to_SpecMATscint     # go to directory which contains the code
% mkdir SpecMATscint_build
% cd SpecMATscint_build
% cmake -DGeant4_DIR=path_to_Geant4_installation/lib[64]/Geant4-[Version]/ ../SpecMATscint
% make -jN      # "N" is the number of processes 
```
Run:
```
% ./SpecMATsim

Idle> /control/execute vis.mac    # visualization
Idle> run/beamOn 1                # beam on
```
