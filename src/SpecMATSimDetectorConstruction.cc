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
#include <G4AffineTransform.hh>

// ###################################################################################

SpecMATSimDetectorConstruction::SpecMATSimDetectorConstruction():G4VUserDetectorConstruction(),fCheckOverlaps(true)
{
  //****************************************************************************//
  //********************************* World ************************************//
  //****************************************************************************//
  // Dimensions of world
  // half-size
  worldSizeXY = 40*cm;
  worldSizeZ  = 40*cm;

  G4double z1, a1, fractionmass1, density1;
  G4String name1, symbol1;
  G4int ncomponents1;

  a1 = 14.01*g/mole;
	G4Element* elN  = new G4Element(name1="Nitrogen",symbol1="N" , z1= 7., a1);

	a1 = 16.00*g/mole;
	G4Element* elO  = new G4Element(name1="Oxygen"  ,symbol1="O" , z1= 8., a1);

	density1 = 0.2E-5*mg/cm3;
	Air = new G4Material(name1="Air",density1,ncomponents1=2);
  Air->AddElement(elN, fractionmass1=70*perCent);
	Air->AddElement(elO, fractionmass1=30*perCent);

  // Define world material
  nist = G4NistManager::Instance();
  default_mat = nist->FindOrBuildMaterial("G4_AIR", false);

  solidWorld = new G4Box("World", worldSizeXY, worldSizeXY, worldSizeZ);  //Worls size

  logicWorld = new G4LogicalVolume(solidWorld, Air, "World");             //World material and id name

  physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, fCheckOverlaps);
  //physWorld = new G4PVPlacement(no rotation, at (0,0,0), its logical volume, its name, its mother  volume, no boolean operation, copy number, checking overlaps);

  worldVisAtt = new G4VisAttributes();					//Instantiation of visualization attributes with blue colour
  worldVisAtt->SetVisibility(false);						//Pass this object to Visualization Manager for visualization

  //World visual attributes
  //sciCrystVisAtt->SetForceWireframe(true);
  logicWorld->SetVisAttributes(worldVisAtt);

  //****************************************************************************//
  //******************************* Detector Array *****************************//
  //****************************************************************************//
  // Number of segments and rings in the array
  nbSegments = 15;                //# of detectors in one ring
  nbCrystInSegmentRow = 2;        //# of rings
  nbCrystInSegmentColumn = 1;     //# of detectors in a segment
  gap = 3*mm;                     //distance between rings

  //Optional parts of the TPC, to introduce additional gamma ray attenuation
  //in the materials in between the beam and the detectors
  vacuumChamber = "no";           //"yes"/"no"
  vacuumFlangeSizeX = 150*mm;
  vacuumFlangeSizeY = 29*mm;
  vacuumFlangeSizeZ = 3*mm;
  vacuumFlangeThickFrontOfScint = 3*mm;

  insulationTube = "no";          //"yes"/"no"
  insulationTubeThickness = 3*mm;

  ComptSupp = "no";               //"yes"/"no"

  dPhi = twopi/nbSegments;
  half_dPhi = 0.5*dPhi;
  tandPhi = std::tan(half_dPhi);
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

  /*
  // LaBr3 material
  La =
      new G4Element("Lanthanum",
            "La",
            z=57.,
            a=138.9055*g/mole);

  Br =
      new G4Element("Bromine",
            "Br",
            z=35.,
            a=79.904*g/mole);

  density = 5.1*g/cm3;
  LaBr3 =
      new G4Material("LaBr3",
             density,
                 ncomponents=2);
  LaBr3->AddElement (La, natoms=1);
  LaBr3->AddElement (Br, natoms=3);

  sciCrystMat = LaBr3;
  */

  // CeBr3 material
  Ce =
	  new G4Element("Cerium",
		  	"Ce",
			z=58.,
			a=140.116*g/mole);
  Br =
	  new G4Element("Bromine",
		  	"Br",
			z=35.,
			a=79.904*g/mole);

  density = 5.1*g/cm3;
  CeBr3 =
	  new G4Material("CeBr3",
		  	 density,
		         ncomponents=2);
  CeBr3->AddElement (Ce, natoms=1);
  CeBr3->AddElement (Br, natoms=3);

  sciCrystMat = CeBr3;

  // Position of the crystal
  sciCrystPosX = 0;									//Position of the Crystal along the X axis
  sciCrystPosY = 0;									//Position of the Crystal along the Y axis
  sciCrystPosZ = 0; 			 						//Position of the Crystal along the Z axis

  sciCrystPos = G4ThreeVector(sciCrystPosX,
		  	      sciCrystPosY,
			      sciCrystPosZ);
  // Define box for Crystal
  sciCrystSolid =
	  new G4Box("sciCrystSolid",
		    sciCrystSizeX,
		    sciCrystSizeY,
		    sciCrystSizeZ);

  // Define Logical Volume for Crystal
  sciCrystLog =
	  new G4LogicalVolume(sciCrystSolid,
			      sciCrystMat,
			      "crystal");


  // Visualization attributes for the Crystal logical volume
  sciCrystVisAtt =
	  new G4VisAttributes(G4Colour(0.0, 0.0, 1.0));					//Instantiation of visualization attributes with blue colour
  sciCrystVisAtt->SetVisibility(true);							//Pass this object to Visualization Manager for visualization
  sciCrystVisAtt->SetForceSolid(true);
  //sciCrystVisAtt->SetForceWireframe(true);						//I still believe that it might make Crystal transparent
  sciCrystLog->SetVisAttributes(sciCrystVisAtt);					//Assignment of visualization attributes to the logical volume of the Crystal

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
  Ti =
	  new G4Element("Titanium",
			"Ti",
			z=22.,
			a=47.9*g/mole);
  O = 											//Define object for an element
      new G4Element("Oxygen", 							//Name of the element
            "O", 								//Symbol of the element
            z=8., 								//Atomic number of the element
            a=15.9994*g/mole);						//Molar mass of the element

  density = 4.23*g/cm3;
  TiO2 =
	  new G4Material("TiO2",
			 density,
			 ncomponents=2);
  TiO2->AddElement (Ti, natoms=1);
  TiO2->AddElement (O, natoms=2);

  sciReflMat = TiO2;

  // Position of the reflector relative to the crystal position
  sciReflPosX = sciCrystPosX;
  sciReflPosY = sciCrystPosY;
  sciReflPosZ = sciCrystPosZ - sciReflWindThick/2;					//Position of the Reflector relative to the Al Housing along the Z axis

  sciReflPos = G4ThreeVector(sciReflPosX,
		  	     sciReflPosY,
			     sciReflPosZ);

  // Define box for Reflector
  reflBoxSolid =
	  new G4Box("reflBoxSolid",
		    sciReflSizeX,
		    sciReflSizeY,
		    sciReflSizeZ);

  // Subtracts Crystal box from Reflector box
  sciReflSolid =
	  new G4SubtractionSolid("sciReflSolid",
		  		 reflBoxSolid,
				 sciCrystSolid,
				 0,
				 G4ThreeVector(sciCrystPosX, sciCrystPosY, sciReflWindThick/2));


  // Define Logical Volume for Reflector//
  sciReflLog =
	  new G4LogicalVolume(sciReflSolid,
			      sciReflMat,
			      "sciReflLog");

  // Visualization attributes for the Reflector logical volume
  sciReflVisAtt =
	  new G4VisAttributes(G4Colour(1.0, 1.0, 0.0));					//Instantiation of visualization attributes with yellow colour
  sciReflVisAtt->SetVisibility(true);							//Pass this object to Visualization Manager for visualization
  sciReflLog->SetVisAttributes(sciReflVisAtt);						//Assignment of visualization attributes to the logical volume of the Reflector

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
  Al =
	  new G4Element("Aluminum",
			"Al",
			z=13.,
			a=26.98*g/mole);

  density = 2.7*g/cm3;
  Al_Alloy =
          new G4Material("Aluminum_",
			 density,
			 ncomponents=1);
  Al_Alloy->AddElement (Al, natoms=1);

  sciHousMat = Al_Alloy;

  // Position of the housing relative to the crystal position
  sciHousPosX = sciCrystPosX;
  sciHousPosY = sciCrystPosY;
  sciHousPosZ = sciCrystPosZ - (sciReflWindThick/2 + sciHousWindThick/2);

  G4ThreeVector sciHousPos = G4ThreeVector(sciHousPosX,
		  			   sciHousPosY,
					   sciHousPosZ);

  // Define box for Housing
  housBoxASolid =
	  new G4Box("housBoxASolid",
	  	    sciHousSizeX,
		    sciHousSizeY,
		    sciHousSizeZ);

  // Subtracts Reflector box from Housing box
  sciHousSolid =
	  new G4SubtractionSolid("housBoxBSolid",
	  			 housBoxASolid,
				 reflBoxSolid,
				 0,
				 G4ThreeVector(sciReflPosX, sciReflPosY, sciHousWindThick/2));

  // Define Logical Volume for Housing
  sciHousLog =
  	  new G4LogicalVolume(sciHousSolid, 	     						//Housing solid shape
			      sciHousMat,              						//Housing material
			      "sciCaseLog");         						//Housing logic volume name

  // Visualization attributes for the Housing logical volume
  sciHousVisAtt =
	  new G4VisAttributes(G4Colour(0.5, 0.5, 0.5));				//Instantiation of visualization attributes with grey colour
  sciHousVisAtt->SetVisibility(true);						//Pass this object to Visualization Manager for visualization
  sciHousLog->SetVisAttributes(sciHousVisAtt);					//Assignment of visualization attributes to the logical volume of the Housing

  //--------------------------------------------------------//
  //******************** Quartz window *********************//
  //--------------------------------------------------------//
  // Dimensions of the Window (half-side)
  sciWindSizeX = sciCrystSizeX + sciReflWallThickX + sciHousWallThickX;						//X half-size of the Window
  sciWindSizeY = sciCrystSizeY + sciReflWallThickY + sciHousWallThickY;						//Y half-size of the Window
  sciWindSizeZ = 1.*mm;									        //Z half-size of the Window

  // Define compound elements for Quartz material

  Si =
	  new G4Element("Silicon",
		  	"Si",
			z=14.,
			a=28.09*g/mole);

  // Define Quartz material
  density = 2.66*g/cm3;									//Assign density of Quartz ot the density variable
  Quartz = 										//Define object for the Qartz material
	  new G4Material("Quartz", 							//Name of the material
		  	 density, 							//Density of the material
			 ncomponents=2);						//Number of the compound elements in the material
  Quartz->AddElement (Si, natoms=1);							//Adds chemical element and number of atoms of this element to the material
  Quartz->AddElement (O, natoms=2);

  sciWindMat = Quartz;

  // Position of the window relative to the crystal
  sciWindPosX = sciCrystPosX ;								//Position of the Window along the X axis
  sciWindPosY = sciCrystPosY ;								//Position of the Window along the Y axis
  sciWindPosZ = sciCrystPosZ + sciCrystSizeZ + sciWindSizeZ;	 			//Position of the Window relative to the Al Housing along the Z axis

  sciWindPos = G4ThreeVector(sciWindPosX, 				//Position of the Window in space relative to the Al Housing
		  	     sciWindPosY,
			     sciWindPosZ);

  // Define solid for the Window
  G4VSolid* sciWindSolid = 								//Define object for the Window's box
	  new G4Box("sciWindSolid",							//Name of the Window's box
		    sciWindSizeX, 							//X half_size of the box
		    sciWindSizeY, 							//Y half_size of the box
		    sciWindSizeZ);							//Z half_size of the box


  // Define Logical Volume for Window
  sciWindLog =
	  new G4LogicalVolume(sciWindSolid,
		  	      sciWindMat,
			      "sciWindLog");


  // Visualization attributes for the Window
  sciWindVisAtt =
	  new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));					//Instantiation of visualization attributes with cyan colour
  sciWindVisAtt->SetVisibility(true);							//Pass this object to Visualization Manager for visualization
  sciWindVisAtt->SetForceWireframe(true);						//I believe that it might make Window transparent
  sciWindLog->SetVisAttributes(sciWindVisAtt);						//Assignment of visualization attributes to the logical volume of the Window

  //--------------------------------------------------------//
  //******************* Flange material ********************//
  //--------------------------------------------------------//
  vacuumFlangeMat = nist->FindOrBuildMaterial("G4_Al", false);//Al_Alloy;
  C =
	  new G4Element("Carbon",
		  	"C",
			z=6.,
			a=12.011*g/mole);
  /*
  G4Element* Mg =
	  new G4Element("Manganese",
		  	"Mg",
			z=25.,
			a=54.938044*g/mole);

  G4Element* Cr =
	  new G4Element("Chromium",
		  	"Cr",
			z=24.,
			a=51.9961*g/mole);
  G4Element* Ni =
	  new G4Element("Nickel",
		  	"Ni",
			z=28.,
			a=58.6934*g/mole);
  G4Element* Mo =
	  new G4Element("Molybdenum",
		  	"Mo",
			z=42.,
			a=95.95*g/mole);
  G4Element* P =
	  new G4Element("Phosphorus",
		  	"P",
			z=15.,
			a=30.973761998*g/mole);
  G4Element* S =
	  new G4Element("Sulfur",
		  	"S",
			z=16.,
			a=32.06*g/mole);
  G4Element* N =
	  new G4Element("Nitrogen",
		  	"N",
			z=7.,
			a=14.007*g/mole);
  G4Element* Fe =
	  new G4Element("Iron",
		  	"Fe",
			z=26.,
			a=55.845*g/mole);

  density = 8.027*g/cm3;

  Steel_316L =
          new G4Material("Steel_316L",
             density,
             ncomponents=10);
  G4double fractionmass;
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
  insulationTubeMat = nist->FindOrBuildMaterial("G4_Al", false);
  /*
  H =
	  new G4Element("Hidrogen",
		  	"H",
			z=1.,
			a=1.008*g/mole);

  density = 0.946*g/cm3;

  Polypropylen_C3H6 =
          new G4Material("Polypropylen_C3H6",
             density,
             ncomponents=2);
  Polypropylen_C3H6->AddElement (C, natoms=3);
  Polypropylen_C3H6->AddElement (H, natoms=6);

  insulationTubeMat = Polypropylen_C3H6;
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
        if (vacuumChamber == "yes") {
            if (vacuumFlangeSizeY>sciHousSizeY*nbCrystInSegmentColumn) {
                circleR1 = vacuumFlangeSizeY/(tandPhi);
            }
            else {
                circleR1 = sciHousSizeY*nbCrystInSegmentColumn/(tandPhi);
            }
        }
        else {
            circleR1 = sciHousSizeY*nbCrystInSegmentColumn/(tandPhi);
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
  G4VSolid* segmentBox = new G4Box("segmentBox",
				sciHousSizeX*nbCrystInSegmentRow+gap*(nbCrystInSegmentRow-1)/2,
				sciHousSizeY*nbCrystInSegmentColumn,
				sciHousSizeZ+sciWindSizeZ);

  // Checking if the flange dimensions are not smaller than the segment dimensions
  if (vacuumFlangeSizeY<sciHousSizeY*nbCrystInSegmentColumn) {
                vacuumFlangeSizeY=sciHousSizeY*nbCrystInSegmentColumn;
  }

  //--------------------------------------------------------//
  //****************** Compton Suppressor ******************//
  //--------------------------------------------------------//
  if (ComptSupp == "yes") {
      Bi =
    	  new G4Element("Bismuth",
    		  	"Bi",
    			z=83.,
    			a=208.98*g/mole);
      Ge =
    	  new G4Element("Germanium",
    		  	"Ge",
    			z=32.,
    			a=72.63*g/mole);
    O =
    	  new G4Element("Oxygen",
    		  	"O",
    			z=8.,
    			a=15.99*g/mole);

      density = 7.13*g/cm3;
      BGO =
    	  new G4Material("BGO",
    		  	 density,
    		         ncomponents=3);
      BGO->AddElement (Bi, natoms=4);
      BGO->AddElement (Ge, natoms=3);
      BGO->AddElement (O, natoms=12);

      ComptSuppMat = BGO;

      ComptSuppSizeX = 24.*mm;								//Size and position of all components depends on Crystal size and position.
      ComptSuppSizeY = 108.*mm;
      ComptSuppSizeZ = sciHousSizeX*3*mm + (gap/2)*2*mm;

      // Position of the Compton Suppressor
      ComptSuppPosX = 0;									//Position of the Compton Suppressor along the X axis
      ComptSuppPosY = 0;									//Position of the Compton Suppressor along the Y axis
      ComptSuppPosZ = -300; 			 					//Position of the Compton Suppressor along the Z axis

      G4ThreeVector ComptSuppPos = G4ThreeVector(ComptSuppPosX,
    		  			    ComptSuppPosY,
    					    ComptSuppPosZ);
      ComptSuppSolidBox =
        	  new G4Box("ComptSuppSolid",
        		    117/2,
        		    30,
                    ComptSuppSizeZ);


    ComptSuppSolidBoxUp =
            new G4Box("ComptSuppSolidUp",
                  200,
                  30*std::cos(dPhi/2),
                  2*ComptSuppSizeZ);

    G4RotationMatrix ComptSuppRotmBoxUp  = G4RotationMatrix();               //** rotation matrix for positioning ComptSupp
    ComptSuppRotmBoxUp.rotateZ(dPhi/2);                                      //** rotation matrix for positioning ComptSupp

    G4ThreeVector positionComptSuppBoxUp = G4ThreeVector(-117/2, 30, 0);

    G4Transform3D transformComptSuppBoxUp = G4Transform3D(ComptSuppRotmBoxUp,positionComptSuppBoxUp);


    ComptSuppSolidBoxDown =
          new G4Box("ComptSuppSolidDown",
                200,
                30*std::cos(dPhi/2),
                2*ComptSuppSizeZ);

    G4RotationMatrix ComptSuppRotmBoxDown  = G4RotationMatrix();               //** rotation matrix for positioning ComptSupp
    ComptSuppRotmBoxDown.rotateZ(-dPhi/2);                               //** rotation matrix for positioning ComptSupp

    G4ThreeVector positionComptSuppBoxDown = G4ThreeVector(-117/2, -30, 0);

    G4Transform3D transformComptSuppBoxDown = G4Transform3D(ComptSuppRotmBoxDown,positionComptSuppBoxDown);


    G4VSolid* ComptSuppSolidBoxWithoutUp =
        new G4SubtractionSolid("ComptSuppSolidBoxWithoutUp",
                   ComptSuppSolidBox,
                   ComptSuppSolidBoxUp,
                   transformComptSuppBoxUp);

    G4VSolid* ComptSuppSolidBoxWithoutDown =
       new G4SubtractionSolid("ComptSuppSolidBoxWithoutDown",
                  ComptSuppSolidBoxWithoutUp,
                  ComptSuppSolidBoxDown,
                  transformComptSuppBoxDown);

    G4double rotationAngle=dPhi/2;

    ComptSuppTrapLog =
         new G4LogicalVolume(ComptSuppSolidBoxWithoutDown,
                     ComptSuppMat,
                     "ComptSuppTrap");

         for (G4int i = 0; i < nbSegments; i++) {

             G4RotationMatrix ComptSuppTrapRotm  = G4RotationMatrix();               //** rotation matrix for positioning ComptSupp
             ComptSuppTrapRotm.rotateZ(rotationAngle);                                      //** rotation matrix for positioning ComptSupp

             G4ThreeVector positionComptSuppTrap = G4ThreeVector((117/2+circleR1/std::cos(dPhi/2))*std::cos(rotationAngle), (117/2+circleR1/std::cos(dPhi/2))*std::sin(rotationAngle), 0);

             G4Transform3D transformComptSuppTrap = G4Transform3D(ComptSuppTrapRotm,positionComptSuppTrap);

             new G4PVPlacement(transformComptSuppTrap,
                               ComptSuppTrapLog,                	//Crystal logical volume
                               "ComptSuppTrapPl",              	    //Crystal positioning name
                               logicWorld,              				//its mother  volume
                               false,                   				//no boolean operation
                               100+i,                       				//copy number
                               fCheckOverlaps);          				//overlaps checking  */
             rotationAngle += dPhi;
         }



      // Visualization attributes for the Compton Suppressor logical volume
      ComptSuppVisAtt =
    	  new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));					//Instantiation of visualization attributes with blue colour
      ComptSuppVisAtt->SetVisibility(true);							    //Pass this object to Visualization Manager for visualization
      ComptSuppVisAtt->SetForceSolid(true);

      ComptSuppTrapLog->SetVisAttributes(ComptSuppVisAtt);					//Assignment of visualization attributes to the logical volume of the Crystal

  }


  //Define the vacuum chamber flange
  if (vacuumChamber == "yes") {
      G4VSolid* vacuumFlangeBox = new G4Box("vacuumFlangeBox",
    				vacuumFlangeSizeX,
    				vacuumFlangeSizeY,
    				vacuumFlangeSizeZ);
      // Subtracts Reflector box from Housing box
      G4VSolid* vacuumFlangeSolid = new G4SubtractionSolid("vacuumFlangeSolid",
  	  			    vacuumFlangeBox,
  				    segmentBox,
  				    0,
  				    G4ThreeVector(0, 0, (sciHousSizeZ+sciWindSizeZ)+vacuumFlangeSizeZ-(2*vacuumFlangeSizeZ-vacuumFlangeThickFrontOfScint)));
      vacuumFlangeBoxLog = new G4LogicalVolume(vacuumFlangeSolid,
                    vacuumFlangeMat,
                    "vacuumFlangeBoxLog");

      vacuumSideFlangeMat = Al_Alloy;
      G4RotationMatrix rotSideFlnge  = G4RotationMatrix();
      rotSideFlnge.rotateZ(dPhi/2);
      G4ThreeVector positionSideFlange1 = G4ThreeVector(0, 0, vacuumFlangeSizeX);
      G4Transform3D transformSideFlange1 = G4Transform3D(rotSideFlnge, positionSideFlange1);
      G4ThreeVector positionSideFlange2 = G4ThreeVector(0, 0, -vacuumFlangeSizeX-2*vacuumFlangeSizeZ);
      G4Transform3D transformSideFlange2 = G4Transform3D(rotSideFlnge, positionSideFlange2);
      G4double vacuumChamberSideFlangeThickness[] = {0, 2*vacuumFlangeSizeZ};
      G4double vacuumChamberSideFlangeInnerR[] = {0, 0};
      G4double vacuumChamberSideFlangeOuterR[] = {circleR1+2*vacuumFlangeSizeZ, circleR1+2*vacuumFlangeSizeZ};

      G4VSolid* vacuumChamberSideFlange = new G4Polyhedra("vacuumChamberSideFlange",
                     0,
                     2*3.1415926535897932384626433,
                     nbSegments,
                     2,
                     vacuumChamberSideFlangeThickness,
                     vacuumChamberSideFlangeInnerR,
                     vacuumChamberSideFlangeOuterR);
      vacuumChamberSideFlangeLog = new G4LogicalVolume(vacuumChamberSideFlange,
                    vacuumSideFlangeMat,
                    "vacuumChamberSideFlangeLog");
      new G4PVPlacement(transformSideFlange1,
                    vacuumChamberSideFlangeLog,                //its logical volume
                    "VacuumChamberSideFlangeLog",              //its name
                    logicWorld,                                //its mother  volume
                    false,                                     //no boolean operation
                    1,                                         //copy number
                    fCheckOverlaps);                           // checking overlaps
      new G4PVPlacement(transformSideFlange2,
                    vacuumChamberSideFlangeLog,                //its logical volume
                    "VacuumChamberSideFlangeLog",              //its name
                    logicWorld,                                //its mother  volume
                    false,                                     //no boolean operation
                    2,                                         //copy number
                    fCheckOverlaps);                           // checking overlaps
  }

  //Defines insulation tube between the field cage and the vacuum chamber which might be used for preventing sparks in the real setup
  //And its stopping power should be simulated
  //
  if (/*vacuumChamber == "yes" &&*/ insulationTube == "yes") {
      //Geometry of the insulation Tube
      insulationTubeInnerRadius = circleR1-insulationTubeThickness;
      insulationTubeOuterRadius = circleR1;
      G4VSolid* insulationTubeSolid = new G4Tubs("insulationTubeSolid",
    				insulationTubeInnerRadius,
                    insulationTubeOuterRadius,
    				vacuumFlangeSizeX,
    				0*deg,
                    360*deg);
      insulationTubeLog = new G4LogicalVolume(insulationTubeSolid,
                    insulationTubeMat,
                    "insulationTubeLog");
      new G4PVPlacement(0,
                    G4ThreeVector(0,0,0),
                    insulationTubeLog,                //its logical volume
                    "insulationTubePhys",              //its name
                    logicWorld,                                //its mother  volume
                    false,                                     //no boolean operation
                    1,                                         //copy number
                    fCheckOverlaps);                           // checking overlaps
      // Visualization attributes for the insulation tube
      insulationTubeVisAtt =
  	    new G4VisAttributes(G4Colour(1.0, 0.0, 0.0));					//Instantiation of visualization attributes with cyan colour
      insulationTubeVisAtt->SetVisibility(true);							//Pass this object to Visualization Manager for visualization
      insulationTubeVisAtt->SetForceWireframe(true);						//I believe that it might make Window transparent
      insulationTubeLog->SetVisAttributes(insulationTubeVisAtt);						//Assignment of visualization attributes to the logical volume of the Window
  }

  //Positioning of segments and crystals in the segment

  //In TotalCrystNb array will be stored coordinates of the all crystals, which could be used for further Doppler correction
  G4int TotalCrystNb = nbCrystInSegmentRow*nbCrystInSegmentColumn*nbSegments;   //Dimension of the dynamic the array
  G4ThreeVector *crystalPositionsArray = new G4ThreeVector[TotalCrystNb];                  //Dinamic mamory allocation for the array
  for (int i=0; i<TotalCrystNb; i++) {
    crystalPositionsArray[i] = G4ThreeVector(0.,0.,0.);                                    // Initialize all elements of the array to zero.
  }

  G4int i = 0;          //counter for reconstruction of crystal positions
  G4int crysNb = 1;     //crystal counter
	for (G4int iseg = 0; iseg < nbSegments ; iseg++) {
			G4double phi = iseg*dPhi;
			G4RotationMatrix rotm  = G4RotationMatrix();     //** rotation matrix for positioning segments
			rotm.rotateY(90*deg);                            //** rotation matrix for positioning segments
			rotm.rotateZ(phi);                               //** rotation matrix for positioning segments

            G4RotationMatrix rotm2  = G4RotationMatrix();    //### rotation matrix for reconstruction of crystal positions
			rotm2.rotateX(360*deg - phi);                    //### rotation matrix for reconstruction of crystal positions
            G4RotationMatrix rotm3  = G4RotationMatrix();    //### rotation matrix for reconstruction of crystal positions
			rotm3.rotateY(90*deg);                           //### rotation matrix for reconstruction of crystal positions

			G4ThreeVector uz = G4ThreeVector(std::cos(phi), std::sin(phi), 0.); //cooficient which will be used for preliminary rotation of the segments and crystals
            segmentBoxLog = new G4LogicalVolume(segmentBox,
                          segment_mat,
                          "segmentBoxLog");
			G4ThreeVector positionInSegment = G4ThreeVector(-(nbCrystInSegmentRow*sciHousSizeX+gap*(nbCrystInSegmentRow-1)/2-sciHousSizeX), -(nbCrystInSegmentColumn*sciHousSizeY-sciHousSizeY), (sciHousSizeZ-sciCrystSizeZ-sciWindSizeZ));
			for (G4int icrystRow = 0; icrystRow < nbCrystInSegmentColumn; icrystRow++) {
				for (G4int icrystCol = 0; icrystCol < nbCrystInSegmentRow; icrystCol++) {
						G4RotationMatrix rotm1  = G4RotationMatrix();

						G4ThreeVector positionCryst = (G4ThreeVector(0., 0., sciCrystPosZ) + positionInSegment);
						G4ThreeVector positionWind = (G4ThreeVector(0., 0., sciWindPosZ) + positionInSegment);
						G4ThreeVector positionRefl = (G4ThreeVector(0., 0., sciReflPosZ) + positionInSegment);
						G4ThreeVector positionHous = (G4ThreeVector(0., 0., sciHousPosZ) + positionInSegment);

                        crystalPositionsArray[crysNb - 1] = positionCryst; //assigning initial crystal positions in a segment into array

						G4Transform3D transformCryst = G4Transform3D(rotm1,positionCryst);
						G4Transform3D transformWind = G4Transform3D(rotm1,positionWind);
						G4Transform3D transformRefl = G4Transform3D(rotm1,positionRefl);
						G4Transform3D transformHous = G4Transform3D(rotm1,positionHous);

						// Crystal position
						new G4PVPlacement(transformCryst,			  //no rotation here rotm1 is empty, position
										  sciCrystLog,                //its logical volume
										  "sciCrystPl",               //its name
										  segmentBoxLog,              //its mother  volume
										  false,                      //no boolean operation
										  crysNb,                     //crystal unique number will
										  fCheckOverlaps);            // checking overlaps

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
            if (vacuumChamber == "yes") {
                //Flange positioning
                G4ThreeVector positionVacuumFlange = (circleR1+vacuumFlangeSizeZ)*uz;
    			G4Transform3D transformVacuumFlange = G4Transform3D(rotm, positionVacuumFlange);
                new G4PVPlacement(transformVacuumFlange, //position
    				  vacuumFlangeBoxLog,                //its logical volume
    				  "VacuumFlange",                    //its name
    				  logicWorld,                         //its mother  volume
    				  false,                              //no boolean operation
    				  iseg,                               //copy number
    				  fCheckOverlaps);                    // checking overlaps
                //Segment positioning
    			G4ThreeVector positionSegment = (circleR1+2*vacuumFlangeSizeZ+(sciHousSizeZ+sciWindSizeZ)-(2*vacuumFlangeSizeZ-vacuumFlangeThickFrontOfScint))*uz;
    			G4Transform3D transformSegment = G4Transform3D(rotm, positionSegment);
    			new G4PVPlacement(transformSegment, //position
    				  segmentBoxLog,                //its logical volume
    				  "Segment",                    //its name
    				  logicWorld,                   //its mother  volume
    				  false,                        //no boolean operation
    				  iseg,                         //copy number
    				  fCheckOverlaps);              // checking overlaps
                //Saving crystal positions in the crystalPositionsArray array
                for (; i < crysNb-1; i++) {
                    G4AffineTransform TransformCrystPos1;
                    TransformCrystPos1.SetNetRotation(rotm2); //rotates the crystal centers (in one segment) by angle phi around X
                    crystalPositionsArray[i] = TransformCrystPos1.TransformPoint(crystalPositionsArray[i]);

                    G4AffineTransform TransformCrystPos;
                    TransformCrystPos.SetNetRotation(rotm3); //rotates the crystal centers (in one segment) by 90deg around Y
                    TransformCrystPos.SetNetTranslation(positionSegment);
                    crystalPositionsArray[i] = TransformCrystPos.TransformPoint(crystalPositionsArray[i]);
                }
            }
            //segment position in case vacuumChamber is "no"
            else {
                //Segment positioning
                G4ThreeVector positionSegment = (circleR1+(sciHousSizeZ+sciWindSizeZ))*uz;
    			G4Transform3D transformSegment = G4Transform3D(rotm, positionSegment);
    			new G4PVPlacement(transformSegment, //position
    				  segmentBoxLog,                //its logical volume
    				  "Segment",                    //its name
    				  logicWorld,                   //its mother  volume
    				  false,                        //no boolean operation
    				  iseg,                         //copy number
    				  fCheckOverlaps);              // checking overlaps
                //Saving crystal positions in the crystalPositionsArray array
                for (; i < crysNb-1; i++) {
                    G4AffineTransform TransformCrystPos1;
                    TransformCrystPos1.SetNetRotation(rotm2); //rotates the crystal centers (in one segment) by angle phi around X
                    crystalPositionsArray[i] = TransformCrystPos1.TransformPoint(crystalPositionsArray[i]);

                    G4AffineTransform TransformCrystPos;
                    TransformCrystPos.SetNetRotation(rotm3); //rotates the crystal centers (in one segment) by 90deg around Y
                    TransformCrystPos.SetNetTranslation(positionSegment);
                    crystalPositionsArray[i] = TransformCrystPos.TransformPoint(crystalPositionsArray[i]);
                }
            }
	}

  // Prints dimensions of the scintillation array
  G4cout <<""<< G4endl;
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
  if (vacuumChamber == "yes") {
  G4cout <<"$$$$"<<" Flange material: "<<vacuumFlangeMat->GetName()<< G4endl;
  G4cout <<"$$$$"<<" SideFlange material: "<<vacuumSideFlangeMat->GetName()<< G4endl;
  G4cout <<"$$$$"<<" Flange width: "<<vacuumFlangeSizeY*2<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Flange thickness: "<<vacuumFlangeSizeZ*2<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Flange thickness in front of the window: "<<vacuumFlangeThickFrontOfScint<<"mm "<< G4endl;
  }
  G4cout <<"$$$$"<< G4endl;
  if (/*vacuumChamber == "yes" &&*/ insulationTube == "yes") {
  G4cout <<"$$$$"<<" Insulator material: "<<insulationTubeMat->GetName()<< G4endl;
  G4cout <<"$$$$"<<" Insulator thickness: "<<insulationTubeThickness<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Insulator tube outer radius: "<<insulationTubeOuterRadius<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Insulator tube inner radius: "<<insulationTubeInnerRadius<<"mm "<< G4endl;
  }
  G4cout <<"$$$$"<< G4endl;
  G4cout <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<< G4endl;
  G4cout <<""<< G4endl;
  G4cout <<"Positions of the crystal centers in the world:"<< G4endl;
  for (G4int i = 0; i < TotalCrystNb; i++) {
      G4cout << "CrystNb" << i+1 << ": " << crystalPositionsArray[i] << G4endl;
  }
  G4cout <<""<< G4endl;
  delete [] crystalPositionsArray; //Free memory allocated for the crystalPositionsArray array
  crystalPositionsArray = NULL;    //Clear a to prevent using invalid memory reference

  CreateScorers();

  return physWorld;
}

// ###################################################################################

void SpecMATSimDetectorConstruction::CreateScorers()
{

  G4SDManager* SDman = G4SDManager::GetSDMpointer();
  SDman->SetVerboseLevel(1);

  // declare crystal as a MultiFunctionalDetector scorer
  //
  G4MultiFunctionalDetector* cryst = new G4MultiFunctionalDetector("crystal");
  G4PSEnergyDeposit* primitiv = new G4PSEnergyDeposit("edep");
  cryst->RegisterPrimitive(primitiv);
  SDman->AddNewDetector(cryst);
  sciCrystLog->SetSensitiveDetector(cryst);

  if (ComptSupp == "yes") {
      G4MultiFunctionalDetector* ComptSupp = new G4MultiFunctionalDetector("ComptSupp");
      G4PSEnergyDeposit* ComptSuppPrimitiv = new G4PSEnergyDeposit("edep");
      ComptSupp->RegisterPrimitive(ComptSuppPrimitiv);
      SDman->AddNewDetector(ComptSupp);
      ComptSuppTrapLog->SetSensitiveDetector(ComptSupp);
  }
}

// ###################################################################################
