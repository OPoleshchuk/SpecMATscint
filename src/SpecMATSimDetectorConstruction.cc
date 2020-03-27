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
/// \file SpecMATSimDetectorConstruction.cc
/// \brief Implementation of the SpecMATSimDetectorConstruction class

#include "SpecMATSimDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Trap.hh"
#include "G4Tubs.hh"
#include "G4Polyhedra.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4RotationMatrix.hh"
#include "G4Transform3D.hh"
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"
#include "G4PSDoseDeposit.hh"
#include "G4VisAttributes.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4VSensitiveDetector.hh"
#include "G4FieldManager.hh"
#include "G4UniformMagField.hh"
#include "G4TransportationManager.hh"
#include <G4AffineTransform.hh>

// ###################################################################################

SpecMATSimDetectorConstruction::SpecMATSimDetectorConstruction():
G4VUserDetectorConstruction(),
fCheckOverlaps(true)
{

  //****************************************************************************//
  //********************************* World ************************************//
  //****************************************************************************//
  // Dimensions of world
  // half-size
  worldSizeXY = 80*cm;
  worldSizeZ  = 80*cm;

  // Define world material manually

  N  = new G4Element("Nitrogen", "N", z=7., a=14.01*g/mole);
  O  = new G4Element("Oxygen", "O", z=8., a=16.00*g/mole);

  density = 1.225E-5*mg/cm3;
  //density = 1.225*mg/cm3;
  Air = new G4Material("Air", density, ncomponents=2);
  Air->AddElement(N, fractionmass=70*perCent);
  Air->AddElement(O, fractionmass=30*perCent);
  // or from the GEANT4 library

  nist = G4NistManager::Instance();
  //default_mat = nist->FindOrBuildMaterial("G4_AIR", false); // build-in Air

  default_mat = Air;
  solidWorld = new G4Box("World", worldSizeXY, worldSizeXY, worldSizeZ);
  logicWorld = new G4LogicalVolume(solidWorld, default_mat, "World");
  physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, fCheckOverlaps);
  //physWorld = new G4PVPlacement(no rotation, at (0,0,0), its logical volume, its name, its mother  volume, no boolean operation, copy number, checking overlaps);

  //World visual attributes
  worldVisAtt = new G4VisAttributes();					//Instantiation of visualization attributes with blue colour
  worldVisAtt->SetVisibility(false);						//Pass this object to Visualization Manager for visualization
  logicWorld->SetVisAttributes(worldVisAtt);

  //****************************************************************************//
  //************************ Detector Array Parameters *************************//
  //****************************************************************************//
  // Number of segments and rings in the array
  nbSegments = 15;                //# of detectors in one ring
  nbCrystInSegmentRow = 3;        //# of rings
  nbCrystInSegmentColumn = 1;     //# of detectors in a segment
  gap = 3*mm;                     //distance between detector rings

  //Optional parts of the TPC, to introduce real gamma ray attenuation
  vacuumChamberFlag = 1;          //1="yes"/0="no"
  vacuumChamberThickness = 3*mm;  // 3*mm is the real thickness of the tinnest wall in the vacuum chamber
  vacuumChamberShift = -29.25*mm; //to align with the scintillation detector array
  /*vacuumFlangeSizeX = 150*mm;
  vacuumFlangeSizeY = 29*mm;
  vacuumFlangeSizeZ = 3*mm;
  vacuumFlangeThickFrontOfScint = 3*mm;*/

  sourceHolderFlag = 0;           //1="yes"/0="no"
  sourceHousingHildeFlag = 0;     //1="yes"/0="no" if Hilde's radioactive sources from 00.87 is used
  pointSourcePositionZ=-129.25*mm;    //defines position of the point source in the PrimaryGeneratorAction as well as position of the sourceHousingHilde
  sourceHousingHildePositionZ = pointSourcePositionZ;

  fieldCageEpoxyFlag = 1;          //1="yes" 0="no"
  fieldCageEpoxyThickness = 16.25*mm;

  comptSuppFlag = 0;              //1="yes" 0="no"

  TPCFlag = 1;                    //1="yes" 0="no"
  if (TPCFlag) {
    comptSuppFlag = 0;
    sourceHolderFlag = 0;
    sourceHousingHildeFlag = 0;
  }

  dPhi = twopi/nbSegments;
  half_dPhi = 0.5*dPhi;
  tandPhi = std::tan(half_dPhi);

  BfieldFlag = 0;

  if (BfieldFlag) {
    BfieldZ = 2.45;
    G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(0., 0., BfieldZ*tesla));
    //G4UniformMagField* magField = new G4UniformMagField(G4ThreeVector(0., 0., 8*tesla));
    G4FieldManager* fieldMgr = G4TransportationManager::GetTransportationManager()->GetFieldManager();
    fieldMgr->SetDetectorField(magField);
    fieldMgr->CreateChordFinder(magField);
  }
  //****************************************************************************//
  //**************** CeBr3 cubic scintillator 1.5"x1.5"x1.5" *******************//
  //****************************************************************************//

  //--------------------------------------------------------//
  //***************** Scintillation crystal ****************//
  //--------------------------------------------------------//
  // Dimensions of the crystal
  sciCrystSizeX = 24.*mm;								//Size and position of all components depends on Crystal size and position.
  sciCrystSizeY = 24.*mm;
  sciCrystSizeZ = 24.*mm;

  // Define Scintillation material and its compounds
  // LaBr3 material
  La = new G4Element("Lanthanum", "La", z=57., a=138.9055*g/mole);
  Br = new G4Element("Bromine", "Br", z=35., a=79.904*g/mole);

  density = 5.1*g/cm3;
  LaBr3 = new G4Material("LaBr3", density, ncomponents=2);
  LaBr3->AddElement (La, natoms=1);
  LaBr3->AddElement (Br, natoms=3);

  // CeBr3 material
  Ce = new G4Element("Cerium", "Ce", z=58., a=140.116*g/mole);

  density = 5.1*g/cm3;
  CeBr3 = new G4Material("CeBr3", density, ncomponents=2);
  CeBr3->AddElement (Ce, natoms=1);
  CeBr3->AddElement (Br, natoms=3);

  sciCrystMat = CeBr3;

  // Position of the crystal
  sciCrystPosX = 0;									//Position of the Crystal along the X axis
  sciCrystPosY = 0;									//Position of the Crystal along the Y axis
  sciCrystPosZ = 0; 			 					//Position of the Crystal along the Z axis

  sciCrystPos = G4ThreeVector(sciCrystPosX, sciCrystPosY, sciCrystPosZ);
  sciCrystSolid = new G4Box("sciCrystSolid", sciCrystSizeX, sciCrystSizeY, sciCrystSizeZ);  // Define box for Crystal
  sciCrystLog = new G4LogicalVolume(sciCrystSolid, sciCrystMat, "crystal");                 // Define Logical Volume for Crystal

  // Visualization attributes for the Crystal logical volume
  sciCrystVisAtt = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));
  sciCrystVisAtt->SetVisibility(true);
  sciCrystVisAtt->SetForceSolid(true);
  //sciCrystVisAtt->SetForceWireframe(true);
  sciCrystLog->SetVisAttributes(sciCrystVisAtt);

  //--------------------------------------------------------//
  //*********************** Reflector **********************//
  //--------------------------------------------------------//
  // Thickness of reflector walls
  sciReflWallThickX = 0.5*mm;
  sciReflWallThickY = 0.5*mm;
  sciReflWindThick = 0.5*mm;

  // Outer dimensions of the reflector relative to the crystal size
  sciReflSizeX = sciCrystSizeX + sciReflWallThickX;
  sciReflSizeY = sciCrystSizeY + sciReflWallThickY;
  sciReflSizeZ = sciCrystSizeZ + sciReflWindThick/2;

  // Define Reflector (white powder TiO2) material and its compounds
  Ti = new G4Element("Titanium", "Ti", z=22., a=47.9*g/mole);
  O  = new G4Element("Oxygen", "O", z=8., a=16.00*g/mole);

  density = 4.23*g/cm3;
  TiO2 = new G4Material("TiO2", density, ncomponents=2);
  TiO2->AddElement (Ti, natoms=1);
  TiO2->AddElement (O, natoms=2);

  sciReflMat = TiO2;

  // Position of the reflector relative to the crystal position
  sciReflPosX = sciCrystPosX;
  sciReflPosY = sciCrystPosY;
  sciReflPosZ = sciCrystPosZ - sciReflWindThick/2;					//Position of the Reflector relative to the Al Housing along the Z axis

  sciReflPos = G4ThreeVector(sciReflPosX, sciReflPosY, sciReflPosZ);
  reflBoxSolid = new G4Box("reflBoxSolid", sciReflSizeX, sciReflSizeY, sciReflSizeZ); // Define box for Reflector
  sciReflSolid = new G4SubtractionSolid("sciReflSolid", reflBoxSolid, sciCrystSolid, 0, G4ThreeVector(sciCrystPosX, sciCrystPosY, sciReflWindThick/2)); // Subtracts Crystal box from Reflector box
  sciReflLog = new G4LogicalVolume(sciReflSolid, sciReflMat, "sciReflLog"); // Define Logical Volume for Reflector//

  // Visualization attributes for the Reflector logical volume
  sciReflVisAtt = new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));					//Instantiation of visualization attributes with yellow colour
  sciReflVisAtt->SetVisibility(true);							                      //Pass this object to Visualization Manager for visualization
  sciReflLog->SetVisAttributes(sciReflVisAtt);						              //Assignment of visualization attributes to the logical volume of the Reflector

  //--------------------------------------------------------//
  //******************** Aluminum Housing ******************//
  //--------------------------------------------------------//
  // Dimensions of Housing (half-side)
  sciHousWallThickX = 3.0*mm;
  sciHousWallThickY = 3.0*mm;
  sciHousWindThick = 1.0*mm;


  // Outer dimensions of the housing relative to the crystal size and to the thickness of the reflector
  sciHousSizeX = sciCrystSizeX + sciReflWallThickX + sciHousWallThickX;
  sciHousSizeY = sciCrystSizeY + sciReflWallThickY + sciHousWallThickY;
  sciHousSizeZ = sciCrystSizeZ + sciReflWindThick/2 + sciHousWindThick/2;

  // Define Housing material and its compounds
  Al = new G4Element("Aluminum", "Al", z=13.,	a=26.98*g/mole);

  density = 2.7*g/cm3;
  Al_Alloy = new G4Material("Aluminum_", density, ncomponents=1);
  Al_Alloy->AddElement (Al, natoms=1);

  sciHousMat = Al_Alloy;

  // Position of the housing relative to the crystal position
  sciHousPosX = sciCrystPosX;
  sciHousPosY = sciCrystPosY;
  sciHousPosZ = sciCrystPosZ - (sciReflWindThick/2 + sciHousWindThick/2);

  sciHousPos = G4ThreeVector(sciHousPosX, sciHousPosY, sciHousPosZ);


  housBoxASolid = new G4Box("housBoxASolid", sciHousSizeX, sciHousSizeY, sciHousSizeZ); // Define box for Housing
  sciHousSolid = new G4SubtractionSolid("housBoxBSolid", housBoxASolid, reflBoxSolid, 0, G4ThreeVector(sciReflPosX, sciReflPosY, sciHousWindThick/2)); // Subtracts Reflector box from Housing box
  sciHousLog = new G4LogicalVolume(sciHousSolid, sciHousMat, "sciCaseLog");	//Housing logic volume name

  // Visualization attributes for the Housing logical volume
  sciHousVisAtt =
  new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
  sciHousVisAtt->SetVisibility(true);
  sciHousLog->SetVisAttributes(sciHousVisAtt);

  //--------------------------------------------------------//
  //******************** Quartz window *********************//
  //--------------------------------------------------------//
  // Dimensions of the Window (half-side)
  sciWindSizeX = sciCrystSizeX + sciReflWallThickX + sciHousWallThickX;						//X half-size of the Window
  sciWindSizeY = sciCrystSizeY + sciReflWallThickY + sciHousWallThickY;						//Y half-size of the Window
  sciWindSizeZ = 1.*mm;									                                          //Z half-size of the Window

  // Define compound elements for Quartz material

  Si = new G4Element("Silicon", "Si", z=14.,	a=28.09*g/mole);

  // Define Quartz material
  density = 2.66*g/cm3;
  Quartz = new G4Material("Quartz", density, ncomponents=2);
  Quartz->AddElement (Si, natoms=1);
  Quartz->AddElement (O, natoms=2);

  sciWindMat = Quartz;

  // Position of the window relative to the crystal
  sciWindPosX = sciCrystPosX ;								                        //Position of the Window along the X axis
  sciWindPosY = sciCrystPosY ;							                         	//Position of the Window along the Y axis
  sciWindPosZ = sciCrystPosZ + sciCrystSizeZ + sciWindSizeZ;	 		  	//Position of the Window relative to the Al Housing along the Z axis

  sciWindPos = G4ThreeVector(sciWindPosX, sciWindPosY, sciWindPosZ);
  sciWindSolid = 	new G4Box("sciWindSolid",	sciWindSizeX, sciWindSizeY, sciWindSizeZ);  // Define solid for the Window
  sciWindLog = new G4LogicalVolume(sciWindSolid, sciWindMat, "sciWindLog");             // Define Logical Volume for Window


  // Visualization attributes for the Window
  sciWindVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));
  sciWindVisAtt->SetVisibility(true);
  sciWindVisAtt->SetForceWireframe(true);
  sciWindLog->SetVisAttributes(sciWindVisAtt);

  //--------------------------------------------------------//
  //******************* Flange material ********************//
  //--------------------------------------------------------//
  vacuumFlangeMat = nist->FindOrBuildMaterial("G4_Al", false);  //Al_Alloy;
  //vacuumChamberMat = nist->FindOrBuildMaterial("G4_Al", false);


  Mn = new G4Element("Manganese", "Mg", z=25.,	a=54.938044*g/mole);
  Fe = new G4Element("Iron",	"Fe",	z=26., a=55.845*g/mole);
  Cu = new G4Element("Copper",	"Cu",	z=29., a=63.546*g/mole);
  Mg = new G4Element("Magnesium",	"Mg", z=12., a=24.305*g/mole);
  Si = new G4Element("Silicon",	"Si",	z=14., a=28.0855*g/mole);
  Zn = new G4Element("Zinc",	"Zn",	z=30., a=65.38*g/mole);
  Cr = new G4Element("Chromium", "Cr",	z=24., a=51.9961*g/mole);
  Ti = new G4Element("Titanium", "Ti",	z=22., a=47.867*g/mole);
  Al = new G4Element("Aluminium", "Al",	z=13.,	a=26.9815384*g/mole);

  densityAluminum5083 = 2.65*g/cm3;

  Aluminum5083 = new G4Material("Aluminum5083", density, ncomponents=9);

  Aluminum5083->AddElement (Mn, fractionmassMn=1.0*perCent);
  Aluminum5083->AddElement (Fe, fractionmassFe=0.4*perCent);
  Aluminum5083->AddElement (Cu, fractionmassCu=0.1*perCent);
  Aluminum5083->AddElement (Mg, fractionmassMg=4.9*perCent);
  Aluminum5083->AddElement (Si, fractionmassSi=0.4*perCent);
  Aluminum5083->AddElement (Zn, fractionmassZn=0.25*perCent);
  Aluminum5083->AddElement (Cr, fractionmassCr=0.25*perCent);
  Aluminum5083->AddElement (Ti, fractionmassTi=0.15*perCent);
  Aluminum5083->AddElement (Al, fractionmassAl=(100-1-0.4-0.1-4.9-0.4-0.25-0.25-0.15)*perCent);

  vacuumChamberMat = Aluminum5083;

  Chlor = new G4Element("Chlor", "Chlor", z=17., a=35.453*g/mole);
  Carb = new G4Element("Carb",	"Carb", z=6., a=12.011*g/mole);
  H = new G4Element("Hidrogen",	"H", z=1., a=1.008*g/mole);
  //N = new G4Element("Nitrogen", "N",	z=7.,	a=14.007*g/mole);
  //density = 0.2E-5*mg/cm3;
  density = 1.45*g/cm3;
  PVC = new G4Material("PVC", density, ncomponents=3);
  PVC->AddElement (Chlor, natoms=1);
  PVC->AddElement (Carb, natoms=2);
  PVC->AddElement (H, natoms=3);
  /*
  PVC = new G4Material("PVC", densitySTM, ncomponents=2);
  //Quartz->AddElement (Chlor, natoms=1);
  PVC->AddElement (C, natoms=2);
  PVC->AddElement (H, natoms=3);
  */
  sourceHolderMat = PVC;
  sourceHousingHildeMat = PVC;

  densityEpoxy = 1.12*g/cm3;
  epoxy = new G4Material("epoxy", densityEpoxy, ncomponents=3); //based on the datasheet of 20-3001NC/20-3002NC epoxy from Epoxies,Etc (Triggerbond® T20-3002BK) components of up to 100% is 25068-38-6 Bisphenol A diglycidyl ether with ch formulae C21H24O4 thsi epoxy is used in the SpecMAT field cage v1
  epoxy->AddElement (O, natoms=4);
  epoxy->AddElement (Carb, natoms=21);
  epoxy->AddElement (H, natoms=24);
  /*
  C = new G4Element("Carbon",	"C", z=6., a=12.011*g/mole);
  Mg = new G4Element("Manganese", "Mg", z=25.,	a=54.938044*g/mole);
  Cr = new G4Element("Chromium",	"Cr",	z=24., a=51.9961*g/mole);
  Ni = new G4Element("Nickel",	"Ni",	z=28., a=58.6934*g/mole);
  Mo = new G4Element("Molybdenum",	"Mo",	z=42., a=95.95*g/mole);
  P = new G4Element("Phosphorus", "P",	z=15., a=30.973761998*g/mole);
  S = new G4Element("Sulfur", "S",	z=16., a=32.06*g/mole);
  N = new G4Element("Nitrogen", "N",	z=7.,	a=14.007*g/mole);
  Fe = new G4Element("Iron",	"Fe",	z=26., a=55.845*g/mole);

  density = 8.027*g/cm3;

  Steel_316L = new G4Material("Steel_316L", density, ncomponents=10);

  Steel_316L->AddElement (C, fractionmass=0.030*perCent);
  Steel_316L->AddElement (Mg, fractionmass=2*perCent);
  Steel_316L->AddElement (Si, fractionmass=0.75*perCent);
  Steel_316L->AddElement (Cr, fractionmass=18*perCent);
  Steel_316L->AddElement (Ni, fractionmass=14*perCent);
  Steel_316L->AddElement (Mo, fractionmass=3*perCent);
  Steel_316L->AddElement (P, fractionmass=0.045*perCent);
  Steel_316L->AddElement (S, fractionmass=0.030*perCent);
  Steel_316L->AddElement (N, fractionmass=0.1*perCent);
  Steel_316L->AddElement (Fe, fractionmass=62.045*perCent);
  vacuumFlangeMat = Steel_316L;
  */


  //--------------------------------------------------------//
  //****************** Insulator material ******************//
  //--------------------------------------------------------//
  // Define insulation tube material
  fieldCageEpoxyMat = nist->FindOrBuildMaterial("epoxy", false);
  /*
  H = new G4Element("Hidrogen",	"H", z=1., a=1.008*g/mole);

  density = 0.946*g/cm3;

  Polypropylen_C3H6 = new G4Material("Polypropylen_C3H6", density, ncomponents=2);
  Polypropylen_C3H6->AddElement (C, natoms=3);
  Polypropylen_C3H6->AddElement (H, natoms=6);

  fieldCageEpoxyMat = Polypropylen_C3H6;
  */



}

// ###################################################################################

SpecMATSimDetectorConstruction::~SpecMATSimDetectorConstruction()
{
}

// ###################################################################################

void SpecMATSimDetectorConstruction::DefineMaterials()
{
}

// ###################################################################################

G4double SpecMATSimDetectorConstruction::ComputeCircleR1()
{
  if (nbSegments == 1) {
    circleR1 = 150;
  }
  else if (nbSegments == 2) {
    circleR1 = 100;
  }
  else {
    if (vacuumChamberFlag) {
      /*if (vacuumFlangeSizeY>sciHousSizeY*nbCrystInSegmentColumn) {
        circleR1 = vacuumFlangeSizeY/(tandPhi);
      }
      else {
        circleR1 = sciHousSizeY*nbCrystInSegmentColumn/(tandPhi);
      }*/
      circleR1 = sciHousSizeY*nbCrystInSegmentColumn/(tandPhi);
      //circleR1=132.56*mm; //Deviations from the design 131.25+1%
      circleR1=131.25*mm; //CAD size
    }
    else {
      circleR1 = sciHousSizeY*nbCrystInSegmentColumn/(tandPhi);
      circleR1=131.25*mm;
    }
  }
  return circleR1;
}

// ###################################################################################

G4VPhysicalVolume* SpecMATSimDetectorConstruction::Construct()
{
  //#####################################################################//
  //#### Positioning of scintillation crystals in the detector array ####//
  //#####################################################################//

  circleR1 = SpecMATSimDetectorConstruction::ComputeCircleR1();

  // Define segment which will conain crystals
  nist = G4NistManager::Instance();
  segment_mat = nist->FindOrBuildMaterial("G4_Galactic", false);
  segmentBox = new G4Box("segmentBox", sciHousSizeX*nbCrystInSegmentRow+gap*(nbCrystInSegmentRow-1)/2, sciHousSizeY*nbCrystInSegmentColumn, sciHousSizeZ+sciWindSizeZ);

  // Checking if the flange dimensions are not smaller than the segment dimensions
  if (vacuumFlangeSizeY<sciHousSizeY*nbCrystInSegmentColumn) {
    vacuumFlangeSizeY=sciHousSizeY*nbCrystInSegmentColumn;
  }

  //--------------------------------------------------------//
  //****************** Compton Suppressor ******************//
  //--------------------------------------------------------//
  if (comptSuppFlag) {
    Bi = new G4Element("Bismuth",	"Bi",	z=83., a=208.98*g/mole);
    Ge = new G4Element("Germanium",	"Ge",	z=32., a=72.63*g/mole);
    O = new G4Element("Oxygen",	"O", z=8., a=15.99*g/mole);

    density = 7.13*g/cm3;
    BGO = new G4Material("BGO", density, ncomponents=3);
    BGO->AddElement (Bi, natoms=4);
    BGO->AddElement (Ge, natoms=3);
    BGO->AddElement (O, natoms=12);

    ComptSuppMat = BGO;

    ComptSuppSizeX = 24.*mm;						//Size and position of all components depends on Crystal size and position.
    ComptSuppSizeY = 108.*mm;
    ComptSuppSizeZ = sciHousSizeX*3*mm + (gap/2)*2*mm;

    // Position of the Compton Suppressor
    ComptSuppPosX = 0;									//Position of the Compton Suppressor along the X axis
    ComptSuppPosY = 0;									//Position of the Compton Suppressor along the Y axis
    ComptSuppPosZ = -300; 			 				//Position of the Compton Suppressor along the Z axis

    ComptSuppPos = G4ThreeVector(ComptSuppPosX, ComptSuppPosY, ComptSuppPosZ);
    ComptSuppSolidBox = new G4Box("ComptSuppSolid", 117/2, 30, ComptSuppSizeZ);
    ComptSuppSolidBoxUp = new G4Box("ComptSuppSolidUp", 200, 30*std::cos(dPhi/2), 2*ComptSuppSizeZ);

    ComptSuppRotmBoxUp  = G4RotationMatrix();               //** rotation matrix for positioning ComptSupp
    ComptSuppRotmBoxUp.rotateZ(dPhi/2);                                      //** rotation matrix for positioning ComptSupp
    positionComptSuppBoxUp = G4ThreeVector(-117/2, 30, 0);
    transformComptSuppBoxUp = G4Transform3D(ComptSuppRotmBoxUp,positionComptSuppBoxUp);


    ComptSuppSolidBoxDown = new G4Box("ComptSuppSolidDown", 200, 30*std::cos(dPhi/2), 2*ComptSuppSizeZ);

    ComptSuppRotmBoxDown  = G4RotationMatrix();              //** rotation matrix for positioning ComptSupp
    ComptSuppRotmBoxDown.rotateZ(-dPhi/2);                                    //** rotation matrix for positioning ComptSupp
    positionComptSuppBoxDown = G4ThreeVector(-117/2, -30, 0);
    transformComptSuppBoxDown = G4Transform3D(ComptSuppRotmBoxDown,positionComptSuppBoxDown);


    ComptSuppSolidBoxWithoutUp = new G4SubtractionSolid("ComptSuppSolidBoxWithoutUp", ComptSuppSolidBox, ComptSuppSolidBoxUp, transformComptSuppBoxUp);
    ComptSuppSolidBoxWithoutDown = new G4SubtractionSolid("ComptSuppSolidBoxWithoutDown", ComptSuppSolidBoxWithoutUp, ComptSuppSolidBoxDown, transformComptSuppBoxDown);
    rotationAngle=dPhi/2;

    ComptSuppTrapLog = new G4LogicalVolume(ComptSuppSolidBoxWithoutDown, ComptSuppMat, "ComptSuppTrap");
    for (G4int i = 0; i < nbSegments; i++) {
      ComptSuppTrapRotm  = G4RotationMatrix();               //** rotation matrix for positioning ComptSupp
      ComptSuppTrapRotm.rotateZ(rotationAngle);                               //** rotation matrix for positioning ComptSupp
      positionComptSuppTrap = G4ThreeVector((117/2+circleR1/std::cos(dPhi/2))*std::cos(rotationAngle), (117/2+circleR1/std::cos(dPhi/2))*std::sin(rotationAngle), 0);
      transformComptSuppTrap = G4Transform3D(ComptSuppTrapRotm,positionComptSuppTrap);
      new G4PVPlacement(transformComptSuppTrap, ComptSuppTrapLog, "ComptSuppTrapPl", logicWorld, false, 100+i, fCheckOverlaps);
      rotationAngle += dPhi;
    }

    // Visualization attributes for the Compton Suppressor logical volume
    ComptSuppVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    ComptSuppVisAtt->SetVisibility(true);
    ComptSuppVisAtt->SetForceSolid(true);
    ComptSuppTrapLog->SetVisAttributes(ComptSuppVisAtt);

  }

  if (TPCFlag) {
    gasVolumeOuterRadius = 225.20/2-fieldCageEpoxyThickness;
    Fluorine  = new G4Element("Fluorine", "F", z=9., 19.00*g/mole);
    Argon = new G4Element("Argon", "Ar", z=18., 39.948*g/mole);
    density = 0.00069581*g/cm3; //400mbar
    //density = 0.00017375*g/cm3;
    ArCF4 = new G4Material("Ar_95_CF4_5", density, ncomponents=3);
    ArCF4->AddElement (Argon, 95*perCent);
    ArCF4->AddElement (Carb, 1*perCent);
    ArCF4->AddElement (Fluorine, 4*perCent);

    gasVolumeMat = ArCF4;

    gasVolumeSolid = new G4Tubs("gasVolumeSolid",	0, gasVolumeOuterRadius,	161.75*mm, 0*deg, 360*deg);
    gasVolumeLog = new G4LogicalVolume(gasVolumeSolid, gasVolumeMat, "gasVolumeLog");
    new G4PVPlacement(0, G4ThreeVector(0,0,32.5), gasVolumeLog, "gasVolumePhys", logicWorld, false, 200, fCheckOverlaps);
    gasVolumeVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
    gasVolumeVisAtt->SetVisibility(true);
    gasVolumeVisAtt->SetForceSolid(true);
    gasVolumeVisAtt->SetForceWireframe(true);
    gasVolumeLog->SetVisAttributes(gasVolumeVisAtt);
  }

  //Define the vacuum chamber flange
  if (vacuumChamberFlag) {
    /*vacuumFlangeBox = new G4Box("vacuumFlangeBox", vacuumFlangeSizeX,	vacuumFlangeSizeY, vacuumFlangeSizeZ);
    // Subtracts Reflector box from Housing box
    vacuumFlangeSolid = new G4SubtractionSolid("vacuumFlangeSolid", vacuumFlangeBox, segmentBox, 0, G4ThreeVector(0, 0, (sciHousSizeZ+sciWindSizeZ)+vacuumFlangeSizeZ-(2*vacuumFlangeSizeZ-vacuumFlangeThickFrontOfScint)));
    vacuumFlangeBoxLog = new G4LogicalVolume(vacuumFlangeSolid, vacuumFlangeMat, "vacuumFlangeBoxLog");

    vacuumSideFlangeMat = Al_Alloy;
    rotSideFlnge  = G4RotationMatrix();
    rotSideFlnge.rotateZ(dPhi/2);
    positionSideFlange1 = G4ThreeVector(0, 0, vacuumFlangeSizeX);
    transformSideFlange1 = G4Transform3D(rotSideFlnge, positionSideFlange1);
    positionSideFlange2 = G4ThreeVector(0, 0, -vacuumFlangeSizeX-2*vacuumFlangeSizeZ);
    transformSideFlange2 = G4Transform3D(rotSideFlnge, positionSideFlange2);
    G4double vacuumChamberSideFlangeThickness[] = {0, 2*vacuumFlangeSizeZ};
    G4double vacuumChamberSideFlangeInnerR[] = {0, 0};
    G4double vacuumChamberSideFlangeOuterR[] = {circleR1+2*vacuumFlangeSizeZ, circleR1+2*vacuumFlangeSizeZ};

    vacuumChamberSideFlange = new G4Polyhedra("vacuumChamberSideFlange", 0, 2*3.1415926535897932384626433, nbSegments, 2, vacuumChamberSideFlangeThickness, vacuumChamberSideFlangeInnerR, vacuumChamberSideFlangeOuterR);
    vacuumChamberSideFlangeLog = new G4LogicalVolume(vacuumChamberSideFlange, vacuumSideFlangeMat, "vacuumChamberSideFlangeLog");
    new G4PVPlacement(transformSideFlange1, vacuumChamberSideFlangeLog, "VacuumChamberSideFlangeLog", logicWorld, false, 1, fCheckOverlaps);
    new G4PVPlacement(transformSideFlange2, vacuumChamberSideFlangeLog, "VacuumChamberSideFlangeLog", logicWorld, false, 2, fCheckOverlaps);*/

    vacuumChamberInnerRadius = 131.25*mm-vacuumChamberThickness;
    vacuumChamberOuterRadius = 131.25*mm;
    vacuumChamberSolid = new G4Tubs("vacuumChamberSolid",	vacuumChamberInnerRadius, vacuumChamberOuterRadius,	102.25*mm, 0*deg, 360*deg);
    vacuumChamberLog = new G4LogicalVolume(vacuumChamberSolid, vacuumChamberMat, "vacuumChamberLog");
    new G4PVPlacement(0, G4ThreeVector(0,0,29.25*mm+vacuumChamberShift), vacuumChamberLog, "vacuumChamberPhys", logicWorld, false, 1, fCheckOverlaps);

    // Visualization attributes for the insulation tube
    vacuumChamberVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));
    vacuumChamberVisAtt->SetVisibility(true);
    vacuumChamberVisAtt->SetForceSolid(true);
    vacuumChamberLog->SetVisAttributes(vacuumChamberVisAtt);

    //Secondtube
    vacuumChamberInnerRadius2 = vacuumChamberInnerRadius;
    vacuumChamberOuterRadius2 = 226*mm;
    vacuumChamberSolid2 = new G4Tubs("vacuumChamberSolid2",	vacuumChamberInnerRadius2, vacuumChamberOuterRadius2,	5*mm, 0*deg, 360*deg);
    vacuumChamberLog2 = new G4LogicalVolume(vacuumChamberSolid2, vacuumChamberMat, "vacuumChamberLog2");
    new G4PVPlacement(0, G4ThreeVector(0,0,-78*mm+vacuumChamberShift), vacuumChamberLog2, "vacuumChamberPhys2", logicWorld, false, 1, fCheckOverlaps);

    // Visualization attributes for the insulation tube
    vacuumChamberLog2->SetVisAttributes(vacuumChamberVisAtt);

    //Thirdtube
    vacuumChamberInnerRadius3 = 150*mm;
    vacuumChamberOuterRadius3 = 255*mm;
    vacuumChamberSolid3 = new G4Tubs("vacuumChamberSolid3",	vacuumChamberInnerRadius3, vacuumChamberOuterRadius3,	5*mm, 0*deg, 360*deg);
    vacuumChamberLog3 = new G4LogicalVolume(vacuumChamberSolid3, vacuumChamberMat, "vacuumChamberLog3");
    new G4PVPlacement(0, G4ThreeVector(0,0,-88*mm+vacuumChamberShift), vacuumChamberLog3, "vacuumChamberPhys3", logicWorld, false, 1, fCheckOverlaps);

    // Visualization attributes for the insulation tube
    vacuumChamberLog3->SetVisAttributes(vacuumChamberVisAtt);

    //Fourthtube
    vacuumChamberInnerRadius4 = 200*mm;
    vacuumChamberOuterRadius4 = 255*mm;
    vacuumChamberSolid4 = new G4Tubs("vacuumChamberSolid4",	vacuumChamberInnerRadius4, vacuumChamberOuterRadius4,	15*mm, 0*deg, 360*deg);
    vacuumChamberLog4 = new G4LogicalVolume(vacuumChamberSolid4, vacuumChamberMat, "vacuumChamberLog4");
    new G4PVPlacement(0, G4ThreeVector(0,0,-108*mm+vacuumChamberShift), vacuumChamberLog4, "vacuumChamberPhys4", logicWorld, false, 1, fCheckOverlaps);

    // Visualization attributes for the insulation tube
    vacuumChamberLog4->SetVisAttributes(vacuumChamberVisAtt);

    //Fifthtube
    vacuumChamberInnerRadius5 = vacuumChamberInnerRadius*mm;
    vacuumChamberOuterRadius5 = 254*mm;
    vacuumChamberSolid5 = new G4Tubs("vacuumChamberSolid5",	vacuumChamberInnerRadius5, vacuumChamberOuterRadius5,	7.5*mm, 0*deg, 360*deg);
    vacuumChamberLog5 = new G4LogicalVolume(vacuumChamberSolid5, vacuumChamberMat, "vacuumChamberLog5");
    new G4PVPlacement(0, G4ThreeVector(0,0,139*mm+vacuumChamberShift), vacuumChamberLog5, "vacuumChamberPhys5", logicWorld, false, 1, fCheckOverlaps);

    // Visualization attributes for the insulation tube
    vacuumChamberLog5->SetVisAttributes(vacuumChamberVisAtt);

    //Sixthtube
    vacuumChamberInnerRadius6 = 239*mm;
    vacuumChamberOuterRadius6 = 254*mm;
    vacuumChamberSolid6 = new G4Tubs("vacuumChamberSolid6",	vacuumChamberInnerRadius6, vacuumChamberOuterRadius6,	37.5*mm, 0*deg, 360*deg);
    vacuumChamberLog6 = new G4LogicalVolume(vacuumChamberSolid6, vacuumChamberMat, "vacuumChamberLog6");
    new G4PVPlacement(0, G4ThreeVector(0,0,184*mm+vacuumChamberShift), vacuumChamberLog6, "vacuumChamberPhys6", logicWorld, false, 1, fCheckOverlaps);

    // Visualization attributes for the insulation tube
    vacuumChamberLog6->SetVisAttributes(vacuumChamberVisAtt);

    //Seventhtube
    vacuumChamberInnerRadius7 = 239*mm;
    vacuumChamberOuterRadius7 = 305*mm;
    vacuumChamberSolid7 = new G4Tubs("vacuumChamberSolid7",	vacuumChamberInnerRadius7, vacuumChamberOuterRadius7,	10*mm, 0*deg, 360*deg);
    vacuumChamberLog7 = new G4LogicalVolume(vacuumChamberSolid7, vacuumChamberMat, "vacuumChamberLog7");
    new G4PVPlacement(0, G4ThreeVector(0,0,231.5*mm+vacuumChamberShift), vacuumChamberLog7, "vacuumChamberPhys7", logicWorld, false, 1, fCheckOverlaps);

    // Visualization attributes for the insulation tube
    vacuumChamberLog7->SetVisAttributes(vacuumChamberVisAtt);

    if (sourceHolderFlag) {
      //PlasticSource Holder
      sourceHolderInnerRadius = 12.7*mm;
      sourceHolderOuterRadius = 15*mm;
      sourceHolderSolid = new G4Tubs("sourceHolderSolid",	sourceHolderInnerRadius, sourceHolderOuterRadius,	35*mm, 0*deg, 360*deg);
      sourceHolderLog = new G4LogicalVolume(sourceHolderSolid, sourceHolderMat, "sourceHolderLog");
      new G4PVPlacement(0, G4ThreeVector(0,0,sourceHousingHildePositionZ), sourceHolderLog, "sourceHolderPhys", logicWorld, false, 1, fCheckOverlaps);
      // Visualization attributes for the insulation tube
      sourceHolderVisAtt = new G4VisAttributes(G4Colour(0.97, 0.82, 0.09));
      sourceHolderVisAtt->SetVisibility(true);
      sourceHolderVisAtt->SetForceSolid(true);
      sourceHolderLog->SetVisAttributes(sourceHolderVisAtt);

      sourceHolderLeftInnerRadius = 0*mm;
      sourceHolderLeftOuterRadius = 12.7*mm;
      sourceHolderLeftSolid = new G4Tubs("sourceHolderLeftSolid",	sourceHolderLeftInnerRadius, sourceHolderLeftOuterRadius,	355/2*mm, 0*deg, 360*deg);
      sourceHolderLeftLog = new G4LogicalVolume(sourceHolderLeftSolid, sourceHolderMat, "sourceHolderLeftLog");
      new G4PVPlacement(0, G4ThreeVector(0,0,sourceHousingHildePositionZ-355/2-35+20), sourceHolderLeftLog, "sourceHolderLeftPhys", logicWorld, false, 1, fCheckOverlaps);
      // Visualization attributes for the insulation tube
      sourceHolderLeftVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
      sourceHolderLeftVisAtt->SetVisibility(true);
      sourceHolderLeftVisAtt->SetForceSolid(true);
      sourceHolderLeftLog->SetVisAttributes(sourceHolderLeftVisAtt);

      sourceHolderRightInnerRadius = 0*mm;
      sourceHolderRightOuterRadius = 12.7*mm;
      sourceHolderRightSolid = new G4Tubs("sourceHolderRightSolid",	sourceHolderRightInnerRadius, sourceHolderRightOuterRadius,	355/2*mm, 0*deg, 360*deg);
      sourceHolderRightLog = new G4LogicalVolume(sourceHolderRightSolid, sourceHolderMat, "sourceHolderRightLog");
      new G4PVPlacement(0, G4ThreeVector(0,0,sourceHousingHildePositionZ+355/2+35-20), sourceHolderRightLog, "sourceHolderRightPhys", logicWorld, false, 1, fCheckOverlaps);
      // Visualization attributes for the insulation tube
      sourceHolderRightVisAtt = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));
      sourceHolderRightVisAtt->SetVisibility(true);
      sourceHolderRightVisAtt->SetForceSolid(true);
      sourceHolderRightLog->SetVisAttributes(sourceHolderRightVisAtt);
    }

    if (sourceHousingHildeFlag) {
      //PlasticSourceHousing
      sourceHousingHildeInnerRadius = 6.7*mm;
      sourceHousingHildeOuterRadius = 12.7*mm;
      sourceHousingHildeSolid = new G4Tubs("sourceHousingHildeSolid",	sourceHousingHildeInnerRadius, sourceHousingHildeOuterRadius,	1.6*mm, 0*deg, 360*deg);
      sourceHousingHildeLog = new G4LogicalVolume(sourceHousingHildeSolid, sourceHousingHildeMat, "sourceHousingHildeLog");
      new G4PVPlacement(0, G4ThreeVector(0,0,sourceHousingHildePositionZ), sourceHousingHildeLog, "sourceHousingHildePhys", logicWorld, false, 1, fCheckOverlaps);
      // Visualization attributes for the insulation tube
      sourceHousingHildeVisAtt = new G4VisAttributes(G4Colour(0.0, 1.0, 0.0));
      sourceHousingHildeVisAtt->SetVisibility(true);
      sourceHousingHildeVisAtt->SetForceSolid(true);
      sourceHousingHildeLog->SetVisAttributes(sourceHousingHildeVisAtt);
    }
  }

  //Defines the field cage
  //
  if (fieldCageEpoxyFlag) {
    //Geometry of the insulation Tube
    fieldCageEpoxyInnerRadius = 225.20/2-fieldCageEpoxyThickness;
    fieldCageEpoxyOuterRadius = 225.20/2;
    fieldCageEpoxySolid = new G4Tubs("fieldCageEpoxySolid",	fieldCageEpoxyInnerRadius, fieldCageEpoxyOuterRadius,	161.75*mm, 0*deg, 360*deg);
    fieldCageEpoxyLog = new G4LogicalVolume(fieldCageEpoxySolid, fieldCageEpoxyMat, "fieldCageEpoxyLog");
    new G4PVPlacement(0, G4ThreeVector(0,0,32.5), fieldCageEpoxyLog, "fieldCageEpoxyPhys", logicWorld, false, 1, fCheckOverlaps);

    // Visualization attributes for the insulation tube
    fieldCageEpoxyVisAtt = new G4VisAttributes(G4Colour(0.96, 0.63, 0.15));
    fieldCageEpoxyVisAtt->SetVisibility(true);
    fieldCageEpoxyVisAtt->SetForceSolid(true);
    fieldCageEpoxyLog->SetVisAttributes(fieldCageEpoxyVisAtt);
  }

  //Positioning of segments and crystals in the segment

  //In TotalCrystNb array will be stored coordinates of the all crystals, which could be used for further Doppler correction
  TotalCrystNb = nbCrystInSegmentRow*nbCrystInSegmentColumn*nbSegments;   //Dimension of the dynamic the array
  crystalPositionsArray = new G4ThreeVector[TotalCrystNb];                //Dinamic mamory allocation for the array
  for (i=0; i<TotalCrystNb; i++) {
    crystalPositionsArray[i] = G4ThreeVector(0.,0.,0.);                   // Initialize all elements of the array to zero.
  }

  i = 0;          //counter for reconstruction of crystal positions
  crysNb = 1;     //crystal counter
  for (iseg = 0; iseg < nbSegments ; iseg++) {
    phi = iseg*dPhi+twopi/4;                         // +twopi/4  Rotates crystalls around the Z axes by 90deg to reproduce the same position as in the real setup
    rotm  = G4RotationMatrix();                      //** rotation matrix for positioning segments
    rotm.rotateY(90*deg);                            //** rotation matrix for positioning segments
    rotm.rotateZ(phi);                               //** rotation matrix for positioning segments

    rotm2  = G4RotationMatrix();                     //### rotation matrix for reconstruction of crystal positions
    rotm2.rotateX(360*deg - phi);                    //### rotation matrix for reconstruction of crystal positions
    rotm3  = G4RotationMatrix();                     //### rotation matrix for reconstruction of crystal positions
    rotm3.rotateY(90*deg);                           //### rotation matrix for reconstruction of crystal positions

    uz = G4ThreeVector(std::cos(phi), std::sin(phi), 0.); //cooficient which will be used for preliminary rotation of the segments and crystals
    segmentBoxLog = new G4LogicalVolume(segmentBox, segment_mat, "segmentBoxLog");
    positionInSegment = G4ThreeVector(-(nbCrystInSegmentRow*sciHousSizeX+gap*(nbCrystInSegmentRow-1)/2-sciHousSizeX), -(nbCrystInSegmentColumn*sciHousSizeY-sciHousSizeY), (sciHousSizeZ-sciCrystSizeZ-sciWindSizeZ));

    for (icrystRow = 0; icrystRow < nbCrystInSegmentColumn; icrystRow++) {
      for (icrystCol = 0; icrystCol < nbCrystInSegmentRow; icrystCol++) {
        rotm1  = G4RotationMatrix();

        positionCryst = (G4ThreeVector(0., 0., sciCrystPosZ) + positionInSegment);
        positionWind = (G4ThreeVector(0., 0., sciWindPosZ) + positionInSegment);
        positionRefl = (G4ThreeVector(0., 0., sciReflPosZ) + positionInSegment);
        positionHous = (G4ThreeVector(0., 0., sciHousPosZ) + positionInSegment);

        crystalPositionsArray[crysNb - 1] = positionCryst; //assigning initial crystal positions in a segment into array

        transformCryst = G4Transform3D(rotm1,positionCryst);
        transformWind = G4Transform3D(rotm1,positionWind);
        transformRefl = G4Transform3D(rotm1,positionRefl);
        transformHous = G4Transform3D(rotm1,positionHous);

        // Crystal position
        new G4PVPlacement(transformCryst,			//no rotation here rotm1 is empty, position
          sciCrystLog,                        //its logical volume
          "sciCrystPl",                       //its name
          segmentBoxLog,                      //its mother  volume
          false,                              //no boolean operation
          crysNb,                             //crystal unique number will
          fCheckOverlaps);                    // checking overlaps

        new G4PVPlacement(transformWind,
          sciWindLog,
          "sciWindPl",
          segmentBoxLog,
          false,
          crysNb,
          fCheckOverlaps);

        new G4PVPlacement(transformRefl,
          sciReflLog,
          "sciReflPl",
          segmentBoxLog,
          false,
          crysNb,
          fCheckOverlaps);

        new G4PVPlacement(transformHous,
          sciHousLog,
          "sciHousPl",
          segmentBoxLog,
          false,
          crysNb,
          fCheckOverlaps);

        crysNb += 1;
        positionInSegment += G4ThreeVector(sciHousSizeX*2+gap, 0., 0.);
      }
      positionInSegment -= G4ThreeVector(nbCrystInSegmentRow*sciHousSizeX*2+gap*(nbCrystInSegmentRow), 0., 0.);
      positionInSegment += G4ThreeVector(0., sciHousSizeY*2, 0.);
    }

    //segment and flange positioning
    /*if (vacuumChamberFlag) {
      //Flange positioning
      positionVacuumFlange = (circleR1+vacuumFlangeSizeZ)*uz;
      transformVacuumFlange = G4Transform3D(rotm, positionVacuumFlange);

      new G4PVPlacement(transformVacuumFlange,                            //position
        vacuumFlangeBoxLog,                                               //its logical volume
        "VacuumFlange",                                                   //its name
        logicWorld,                                                       //its mother  volume
        false,                                                            //no boolean operation
        iseg,                                                             //copy number
        fCheckOverlaps);                                                  // checking overlaps

      //Segment positioning
      positionSegment = (circleR1+2*vacuumFlangeSizeZ+(sciHousSizeZ+sciWindSizeZ)-(2*vacuumFlangeSizeZ-vacuumFlangeThickFrontOfScint))*uz;
      transformSegment = G4Transform3D(rotm, positionSegment);

      new G4PVPlacement(transformSegment,                                 //position
        segmentBoxLog,                                                    //its logical volume
        "Segment",                                                        //its name
        logicWorld,                                                       //its mother  volume
        false,                                                            //no boolean operation
        iseg,                                                             //copy number
        fCheckOverlaps);                                                  // checking overlaps

      //Saving crystal positions in the crystalPositionsArray array
      for (; i < crysNb-1; i++) {
        TransformCrystPos1.SetNetRotation(rotm2); //rotates the crystal centers (in one segment) by angle phi around X
        crystalPositionsArray[i] = TransformCrystPos1.TransformPoint(crystalPositionsArray[i]);

        TransformCrystPos.SetNetRotation(rotm3); //rotates the crystal centers (in one segment) by 90deg around Y
        TransformCrystPos.SetNetTranslation(positionSegment);
        crystalPositionsArray[i] = TransformCrystPos.TransformPoint(crystalPositionsArray[i]);
      }
    }*/
    //segment position in case vacuumChamberFlagis "no"
    //else {
      //Segment positioning
      positionSegment = (circleR1+(sciHousSizeZ+sciWindSizeZ))*uz;
      transformSegment = G4Transform3D(rotm, positionSegment);

      new G4PVPlacement(transformSegment,                                 //position
        segmentBoxLog,                                                    //its logical volume
        "Segment",                                                        //its name
        logicWorld,                                                       //its mother  volume
        false,                                                            //no boolean operation
        iseg,                                                             //copy number
        fCheckOverlaps);                                                  // checking overlaps

      //Saving crystal positions in the crystalPositionsArray array
      for (; i < crysNb-1; i++) {
        TransformCrystPos1.SetNetRotation(rotm2); //rotates the crystal centers (in one segment) by angle phi around X
        crystalPositionsArray[i] = TransformCrystPos1.TransformPoint(crystalPositionsArray[i]);

        TransformCrystPos.SetNetRotation(rotm3); //rotates the crystal centers (in one segment) by 90deg around Y
        TransformCrystPos.SetNetTranslation(positionSegment);
        crystalPositionsArray[i] = TransformCrystPos.TransformPoint(crystalPositionsArray[i]);
      }
    //}
  }
  //G4cout <<gammaSource->GetSource();<< G4endl;
  G4cout <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<< G4endl;
  G4cout <<"$$$$"<< G4endl;
  G4cout <<"$$$$"<<" Crystal material: "<<sciCrystMat->GetName()<< G4endl;
  G4cout <<"$$$$"<<" Reflector material: "<<sciReflMat->GetName()<< G4endl;
  G4cout <<"$$$$"<<" Housing material: "<<sciHousMat->GetName()<< G4endl;
  G4cout <<"$$$$"<<" Optic window material: "<<sciWindMat->GetName()<< G4endl;
  G4cout <<"$$$$"<< G4endl;
  G4cout <<"$$$$"<<" Single crystal dimensions: "<<sciCrystSizeX*2<<"mmx"<<sciCrystSizeY*2<<"mmx"<<sciCrystSizeZ*2<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Dimensions of the crystal housing: "<<sciHousSizeX*2<<"mmx"<<sciHousSizeY*2<<"mmx"<<sciHousSizeZ*2<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Housing wall thickness: "<<sciHousWallThickX<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Housing window thickness: "<<sciHousWindThick<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Reflecting material wall thickness: "<<sciReflWallThickX<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Reflecting material thickness in front of the window: "<<sciReflWindThick<<"mm "<< G4endl;
  G4cout <<"$$$$"<< G4endl;
  G4cout <<"$$$$"<<" Number of segments in the array: "<<nbSegments<<" "<< G4endl;
  G4cout <<"$$$$"<<" Number of crystals in the segment row: "<<nbCrystInSegmentRow<<" "<< G4endl;
  G4cout <<"$$$$"<<" Number of crystals in the segment column: "<<nbCrystInSegmentColumn<<" "<< G4endl;
  G4cout <<"$$$$"<<" Number of crystals in the array: "<<nbSegments*nbCrystInSegmentRow*nbCrystInSegmentColumn<<" "<< G4endl;
  G4cout <<"$$$$"<<" Segment width: "<<sciHousSizeY*nbCrystInSegmentColumn*2<<"mm "<< G4endl;
  G4cout <<"$$$$"<< G4endl;
  G4cout <<"$$$$"<<" Radius of a circle inscribed in the array: "<<circleR1<<"mm "<< G4endl;
  G4cout <<"$$$$"<< G4endl;
  if (vacuumChamberFlag) {
    G4cout <<"$$$$"<<" Vacuum Chamber Material: "<<vacuumChamberMat->GetName()<< G4endl;
    G4cout <<"$$$$"<<" Vacuum Chamber Wall Thickness: "<<vacuumChamberThickness<<"mm "<< G4endl;
    G4cout <<"$$$$"<<" Vacuum Chamber outer radius: "<<vacuumChamberInnerRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<<" Vacuum Chamber inner radius: "<<vacuumChamberOuterRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<< G4endl;
  }
  if (sourceHolderFlag) {
    G4cout <<"$$$$"<<" Source Holder Material: "<<sourceHolderMat->GetName()<< G4endl;
    G4cout <<"$$$$"<<" Source Holder Wall Thickness: "<<sourceHolderOuterRadius-sourceHolderInnerRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<<" Source Holder outer radius: "<<sourceHolderInnerRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<<" Source Holder inner radius: "<<sourceHolderOuterRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<< G4endl;
  }
  if (sourceHousingHildeFlag) {
    G4cout <<"$$$$"<<" Source Housing Material: "<<sourceHousingHildeMat->GetName()<< G4endl;
    G4cout <<"$$$$"<<" Source Housing Wall Thickness: "<<sourceHousingHildeOuterRadius-sourceHousingHildeInnerRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<<" Source Housing outer radius: "<<sourceHousingHildeInnerRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<<" Source Housing inner radius: "<<sourceHousingHildeOuterRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<< G4endl;
  }
  if (fieldCageEpoxyFlag) {
    G4cout <<"$$$$"<<" Field Cage material: "<<fieldCageEpoxyMat->GetName()<< G4endl;
    G4cout <<"$$$$"<<" Field Cage thickness: "<<fieldCageEpoxyThickness<<"mm "<< G4endl;
    G4cout <<"$$$$"<<" Field Cage outer radius: "<<fieldCageEpoxyOuterRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<<" Field Cage inner radius: "<<fieldCageEpoxyInnerRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<< G4endl;
  }
  if (TPCFlag) {
    G4cout <<"$$$$"<<" Gas material: "<<gasVolumeMat->GetName()<< G4endl;
    G4cout <<"$$$$"<<" Gas density: "<<gasVolumeMat->GetDensity()/(g/cm3)<<"g/cm3 "<< G4endl;
    G4cout <<"$$$$"<<" Gas volume outer radius: "<<gasVolumeOuterRadius<<"mm "<< G4endl;
    G4cout <<"$$$$"<< G4endl;
  }
  G4cout <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<< G4endl;
  G4cout <<""<< G4endl;
  G4cout <<"Position of the crystal centers in the G4 world:"<< G4endl;
  G4cout <<""<< G4endl;
  G4cout << "CrystID X Y Z" << G4endl;
  for (i = 0; i < TotalCrystNb; i++) {
    //fileParam << "CrystNb" << i+1 << ": " << crystalPositionsArray[i]::getX() << " " << crystalPositionsArray[i]::getY() << " " << crystalPositionsArray[i]::getZ() << G4endl;
    if (crystalPositionsArray[i].getX() > -0.001 && crystalPositionsArray[i].getX() < 0.001) {
      crystalPositionsArrayX = 0;
    } else {
      crystalPositionsArrayX = crystalPositionsArray[i].getX();
    }
    if (crystalPositionsArray[i].getY() > -0.001 && crystalPositionsArray[i].getY() < 0.001) {
      crystalPositionsArrayY = 0;
    } else {
      crystalPositionsArrayY = crystalPositionsArray[i].getY();
    }
    if (crystalPositionsArray[i].getZ() > -0.001 && crystalPositionsArray[i].getZ() < 0.001) {
      crystalPositionsArrayZ = 0;
    } else {
      crystalPositionsArrayZ = -1*crystalPositionsArray[i].getZ();
    }
    G4cout << "CrystNb" << i+1 << " " << crystalPositionsArrayX << " " << crystalPositionsArrayY << " " << crystalPositionsArrayZ << G4endl;
    //G4cout << "#/vis/scene/add/text" << " " << crystalPositionsArrayX << " " << crystalPositionsArrayY << " " << crystalPositionsArrayZ << " mm 18 0 0 " << i+1 << G4endl;
  }
  G4cout <<""<< G4endl;

  crystalPositionsArrayX = 0;
  crystalPositionsArrayY = 0;
  crystalPositionsArrayZ = 0;

  // Saveing parameters into a file
  fileParam.open("simParam.txt");
  fileParam <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<< G4endl;
  fileParam <<"$$$$"<< G4endl;
  fileParam <<"$$$$"<<" Crystal material: "<<sciCrystMat->GetName()<< G4endl;
  fileParam <<"$$$$"<<" Reflector material: "<<sciReflMat->GetName()<< G4endl;
  fileParam <<"$$$$"<<" Housing material: "<<sciHousMat->GetName()<< G4endl;
  fileParam <<"$$$$"<<" Optic window material: "<<sciWindMat->GetName()<< G4endl;
  fileParam <<"$$$$"<< G4endl;
  fileParam <<"$$$$"<<" Single crystal dimensions: "<<sciCrystSizeX*2<<"mmx"<<sciCrystSizeY*2<<"mmx"<<sciCrystSizeZ*2<<"mm "<< G4endl;
  fileParam <<"$$$$"<<" Dimensions of the crystal housing: "<<sciHousSizeX*2<<"mmx"<<sciHousSizeY*2<<"mmx"<<sciHousSizeZ*2<<"mm "<< G4endl;
  fileParam <<"$$$$"<<" Housing wall thickness: "<<sciHousWallThickX<<"mm "<< G4endl;
  fileParam <<"$$$$"<<" Housing window thickness: "<<sciHousWindThick<<"mm "<< G4endl;
  fileParam <<"$$$$"<<" Reflecting material wall thickness: "<<sciReflWallThickX<<"mm "<< G4endl;
  fileParam <<"$$$$"<<" Reflecting material thickness in front of the window: "<<sciReflWindThick<<"mm "<< G4endl;
  fileParam <<"$$$$"<< G4endl;
  fileParam <<"$$$$"<<" Number of segments in the array: "<<nbSegments<<" "<< G4endl;
  fileParam <<"$$$$"<<" Number of crystals in the segment row: "<<nbCrystInSegmentRow<<" "<< G4endl;
  fileParam <<"$$$$"<<" Number of crystals in the segment column: "<<nbCrystInSegmentColumn<<" "<< G4endl;
  fileParam <<"$$$$"<<" Number of crystals in the array: "<<nbSegments*nbCrystInSegmentRow*nbCrystInSegmentColumn<<" "<< G4endl;
  fileParam <<"$$$$"<<" Segment width: "<<sciHousSizeY*nbCrystInSegmentColumn*2<<"mm "<< G4endl;
  fileParam <<"$$$$"<< G4endl;
  fileParam <<"$$$$"<<" Radius of a circle inscribed in the array: "<<circleR1<<"mm "<< G4endl;
  fileParam <<"$$$$"<< G4endl;
  if (vacuumChamberFlag) {
    fileParam <<"$$$$"<<" Vacuum Chamber Material: "<<vacuumChamberMat->GetName()<< G4endl;
    fileParam <<"$$$$"<<" Vacuum Chamber Wall Thickness: "<<vacuumChamberThickness<<"mm "<< G4endl;
    fileParam <<"$$$$"<<" Vacuum Chamber outer radius: "<<vacuumChamberInnerRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<<" Vacuum Chamber inner radius: "<<vacuumChamberOuterRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<< G4endl;
  }
  if (sourceHolderFlag) {
    fileParam <<"$$$$"<<" Source Holder Material: "<<sourceHolderMat->GetName()<< G4endl;
    fileParam <<"$$$$"<<" Source Holder Wall Thickness: "<<sourceHolderOuterRadius-sourceHolderInnerRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<<" Source Holder outer radius: "<<sourceHolderInnerRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<<" Source Holder inner radius: "<<sourceHolderOuterRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<< G4endl;
  }
  if (sourceHousingHildeFlag) {
    fileParam <<"$$$$"<<" Source Housing Material: "<<sourceHousingHildeMat->GetName()<< G4endl;
    fileParam <<"$$$$"<<" Source Housing Wall Thickness: "<<sourceHousingHildeOuterRadius-sourceHousingHildeInnerRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<<" Source Housing outer radius: "<<sourceHousingHildeInnerRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<<" Source Housing inner radius: "<<sourceHousingHildeOuterRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<< G4endl;
  }
  if (fieldCageEpoxyFlag) {
    fileParam <<"$$$$"<<" Field Cage material: "<<fieldCageEpoxyMat->GetName()<< G4endl;
    fileParam <<"$$$$"<<" Field Cage thickness: "<<fieldCageEpoxyThickness<<"mm "<< G4endl;
    fileParam <<"$$$$"<<" Field Cage outer radius: "<<fieldCageEpoxyOuterRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<<" Field Cage inner radius: "<<fieldCageEpoxyInnerRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<< G4endl;
  }
  if (TPCFlag) {
    fileParam <<"$$$$"<<" Gas material: "<<gasVolumeMat->GetName()<< G4endl;
    fileParam <<"$$$$"<<" Gas density: "<<gasVolumeMat->GetDensity()/(g/cm3)<<"g/cm3 "<< G4endl;
    fileParam <<"$$$$"<<" Gas volume outer radius: "<<gasVolumeOuterRadius<<"mm "<< G4endl;
    fileParam <<"$$$$"<< G4endl;
  }
  fileParam <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<< G4endl;
  fileParam <<""<< G4endl;
  fileParam <<"Position of the crystal centers in the G4 world:"<< G4endl;
  fileParam <<""<< G4endl;
  fileParam << "CrystID X Y Z" << G4endl;
  for (i = 0; i < TotalCrystNb; i++) {
    //fileParam << "CrystNb" << i+1 << ": " << crystalPositionsArray[i]::getX() << " " << crystalPositionsArray[i]::getY() << " " << crystalPositionsArray[i]::getZ() << G4endl;
    if (crystalPositionsArray[i].getX() > -0.001 && crystalPositionsArray[i].getX() < 0.001) {
      crystalPositionsArrayX = 0;
    } else {
      crystalPositionsArrayX = crystalPositionsArray[i].getX();
    }
    if (crystalPositionsArray[i].getY() > -0.001 && crystalPositionsArray[i].getY() < 0.001) {
      crystalPositionsArrayY = 0;
    } else {
      crystalPositionsArrayY = crystalPositionsArray[i].getY();
    }
    if (crystalPositionsArray[i].getZ() > -0.001 && crystalPositionsArray[i].getZ() < 0.001) {
      crystalPositionsArrayZ = 0;
    } else {
      crystalPositionsArrayZ = -1*crystalPositionsArray[i].getZ();
    }
    fileParam << "CrystNb" << i+1 << " " << crystalPositionsArrayX << " " << crystalPositionsArrayY << " " << crystalPositionsArrayZ << G4endl;
  }
  fileParam <<""<< G4endl;
  fileParam.close();
  delete [] crystalPositionsArray; //Free memory allocated for the crystalPositionsArray array
  crystalPositionsArray = NULL;    //Clear a to prevent using invalid memory reference

  CreateScorers();

  return physWorld;
}

// ###################################################################################

void SpecMATSimDetectorConstruction::CreateScorers()
{

  SDman = G4SDManager::GetSDMpointer();
  SDman->SetVerboseLevel(1);

  // declare crystal as a MultiFunctionalDetector scorer
  //
  cryst = new G4MultiFunctionalDetector("crystal");
  primitiv = new G4PSEnergyDeposit("edep");
  cryst->RegisterPrimitive(primitiv);
  SDman->AddNewDetector(cryst);
  sciCrystLog->SetSensitiveDetector(cryst);

  if (comptSuppFlag) {
      ComptSupp = new G4MultiFunctionalDetector("ComptSupp");
      ComptSuppPrimitiv = new G4PSEnergyDeposit("edep");
      ComptSupp->RegisterPrimitive(ComptSuppPrimitiv);
      SDman->AddNewDetector(ComptSupp);
      ComptSuppTrapLog->SetSensitiveDetector(ComptSupp);
  }

  if (TPCFlag) {
      AlphaTracker = new G4MultiFunctionalDetector("AlphaTracker");
      AlphaTrackerPrimitiv = new G4PSEnergyDeposit("edep");
      AlphaTracker->RegisterPrimitive(AlphaTrackerPrimitiv);
      SDman->AddNewDetector(AlphaTracker);
      gasVolumeLog->SetSensitiveDetector(AlphaTracker);
  }
}

// ###################################################################################
