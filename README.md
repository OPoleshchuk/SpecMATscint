# SpecMATscint
Simulation of a scintillator array for the SpecMAT detector in GEANT4

How to build and run:
Build:
With CMake
% cd path_to_exampleXYZ     # go to directory which contains your example
% mkdir exampleXYZ_build
% cd exampleXYZ_build
% cmake -DGeant4_DIR=path_to_Geant4_installation/lib[64]/Geant4-10.2.0/ ../exampleXYZ
% make -j N exampleXYZ      # "N" is the number of processes 
% make install              # this step is optional

Run:
% ./SpecMATsim
visualization:
Idle> /control/execute vis.mac
beam on:
Idle> run/beamOn 1    
