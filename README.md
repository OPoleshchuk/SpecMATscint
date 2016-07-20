# SpecMATscint

<img src="https://cloud.githubusercontent.com/assets/17125100/16989384/4f1894c0-4e94-11e6-8c2f-a0df139420df.png" align="right">

SpecMATscint is a simulation of a scintillator array for the SpecMAT detector in GEANT4.


### How to Build and Run

Build with CMake:
```
% cd path_to_SpecMATscint         # go to directory which contains the code
% mkdir SpecMATscint_build
% cd SpecMATscint_build
% cmake -DGeant4_DIR=path_to_Geant4_installation/lib[64]/Geant4-[Version]/ ../SpecMATscint
% make -jN                        # "N" is the number of processes
```
Run:
```
% ./SpecMATsim

Idle> /control/execute vis.mac    # visualization
Idle> run/beamOn 1                # beam on
```
