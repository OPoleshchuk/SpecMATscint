/// \file SpecMATSimDetectorConstruction.cc
/// \brief Implementation of the SpecMATSimDetectorConstruction class

#include "SpecMATSimDetectorConstruction.hh"

#include "G4NistManager.hh"
#include "G4Box.hh"
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

// ###################################################################################

SpecMATSimDetectorConstruction::SpecMATSimDetectorConstruction()
: G4VUserDetectorConstruction(),
  fCheckOverlaps(true)
{
  //****************************************************************************//
  //********************************* World ************************************//
  //****************************************************************************//
  // Dimensions of world
  //half-size
  worldSizeXY = 60*cm;
  worldSizeZ  = 60*cm;

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
  G4NistManager* nist = G4NistManager::Instance();
  default_mat = nist->FindOrBuildMaterial("G4_AIR", false);

  solidWorld =
    new G4Box("World",                       //its name
       worldSizeXY, worldSizeXY, worldSizeZ); //its size

  logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        Air,         //its material
                        "World");            //its name

  physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      fCheckOverlaps);       // checking overlaps
  //****************************************************************************//
  //******************************* Detector Array *****************************//
  //****************************************************************************//
  // How many segments and crystal rings in the detector

  nbSegments = 6;
  nbCrystInSegmentRow = 3;
  nbCrystInSegmentColumn = 4;

  vacuumChamber = "yes"; //"yes"/"no"
  vacuumFlangeSizeX = 300*mm;
  vacuumFlangeSizeY = 150*mm;
  vacuumFlangeSizeZ = 10*mm;
  vacuumFlangeThickFrontOfScint = 1*mm;

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
  sciCrystSizeX = 19.*mm;								//Size and position of all components depends on Crystal size and position.
  sciCrystSizeY = 19.*mm;
  sciCrystSizeZ = 19.*mm;

  // Define CeBr3 material and its compounds
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
  sciCrystVisAtt->SetForceWireframe(true);						//I still believe that it might make Crystal transparent
  sciCrystLog->SetVisAttributes(sciCrystVisAtt);					//Assignment of visualization attributes to the logical volume of the Crystal

  //--------------------------------------------------------//
  //*********************** Reflector **********************//
  //--------------------------------------------------------//
  // Thickness of reflector walls
  sciReflWallThickX = 0.5*mm;
  sciReflWallThickY = 0.5*mm;
  sciReflWindThick = 1.2*mm;

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
			      TiO2,
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
  sciHousWallThickX = 3.5*mm;
  sciHousWallThickY = 3.5*mm;
  sciHousWindThick = 0.8*mm;


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
          new G4Material("Aluminum_Alloy",
			 density,
			 ncomponents=1);
  Al_Alloy->AddElement (Al, natoms=1);

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
			      Al_Alloy,              						//Housing material
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
		  	      Quartz,
			      "sciWindLog");


  // Visualization attributes for the Window
  sciWindVisAtt =
	  new G4VisAttributes(G4Colour(0.0, 1.0, 1.0));					//Instantiation of visualization attributes with cyan colour
  sciWindVisAtt->SetVisibility(true);							//Pass this object to Visualization Manager for visualization
  sciWindVisAtt->SetForceWireframe(true);						//I believe that it might make Window transparent
  sciWindLog->SetVisAttributes(sciWindVisAtt);						//Assignment of visualization attributes to the logical volume of the Window

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
        circleR1 = 0;
    }
    else if (nbSegments == 2) {
        circleR1 = 0;
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
  G4NistManager* nist = G4NistManager::Instance();
  segment_mat = nist->FindOrBuildMaterial("G4_Galactic", false);
  G4VSolid* segmentBox = new G4Box("segmentBox",
				sciHousSizeX*nbCrystInSegmentRow,
				sciHousSizeY*nbCrystInSegmentColumn,
				sciHousSizeZ+sciWindSizeZ);
  segmentBoxLog = new G4LogicalVolume(segmentBox,
                segment_mat,
                "segmentBoxLog");

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
                    Al_Alloy,
                    "vacuumFlangeBoxLog");

      G4RotationMatrix rotSideFlnge  = G4RotationMatrix();
      rotSideFlnge.rotateZ(dPhi/2);
      G4ThreeVector positionSideFlange1 = G4ThreeVector(0, 0, vacuumFlangeSizeX);
      G4Transform3D transformSideFlange1 = G4Transform3D(rotSideFlnge, positionSideFlange1);
      G4ThreeVector positionSideFlange2 = G4ThreeVector(0, 0, -vacuumFlangeSizeX-2*vacuumFlangeSizeZ);
      G4Transform3D transformSideFlange2 = G4Transform3D(rotSideFlnge, positionSideFlange2);
      G4double vacuumChamberSideFlangeThickness[] = {0, 2*vacuumFlangeSizeZ, 0};
      G4double vacuumChamberSideFlangeInnerR[] = {0, 0, 0};
      G4double vacuumChamberSideFlangeOuterR[] = {0, circleR1+2*vacuumFlangeSizeZ, circleR1+2*vacuumFlangeSizeZ};

      G4VSolid* vacuumChamberSideFlange = new G4Polyhedra("vacuumChamberSideFlange",
                     0,
                     2*3.1415926535897932384626433,
                     nbSegments,
                     2,
                     vacuumChamberSideFlangeThickness,
                     vacuumChamberSideFlangeInnerR,
                     vacuumChamberSideFlangeOuterR);
      vacuumChamberSideFlangeLog = new G4LogicalVolume(vacuumChamberSideFlange,
                    Al_Alloy,
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

  //Positioning of segments and crystals in the segment
  G4int crysNb = 1;
	for (G4int iseg = 0; iseg < nbSegments ; iseg++) {
			G4double phi = iseg*dPhi;
			G4RotationMatrix rotm  = G4RotationMatrix();
			rotm.rotateY(90*deg);
			rotm.rotateZ(phi);
			G4ThreeVector uz = G4ThreeVector(std::cos(phi),  std::sin(phi),0.);
			G4ThreeVector positionInSegment = G4ThreeVector(-(nbCrystInSegmentRow*sciHousSizeX-sciHousSizeX), -(nbCrystInSegmentColumn*sciHousSizeY-sciHousSizeY), (sciHousSizeZ-sciCrystSizeZ-sciWindSizeZ));
            //-(sciCrystPosZ - (sciReflWindThick/2 + sciHousWindThick/2)-sciWindPosZ)
			for (G4int icrystRow = 0; icrystRow < nbCrystInSegmentColumn; icrystRow++) {
				for (G4int icrystCol = 0; icrystCol < nbCrystInSegmentRow; icrystCol++) {
						G4RotationMatrix rotm1  = G4RotationMatrix();

						G4ThreeVector positionCryst = (G4ThreeVector(0., 0., sciCrystPosZ) + positionInSegment);
						G4ThreeVector positionWind = (G4ThreeVector(0., 0., sciWindPosZ) + positionInSegment);
						G4ThreeVector positionRefl = (G4ThreeVector(0., 0., sciReflPosZ) + positionInSegment);
						G4ThreeVector positionHous = (G4ThreeVector(0., 0., sciHousPosZ) + positionInSegment);

						G4Transform3D transformCryst = G4Transform3D(rotm1,positionCryst);
						G4Transform3D transformWind = G4Transform3D(rotm1,positionWind);
						G4Transform3D transformRefl = G4Transform3D(rotm1,positionRefl);
						G4Transform3D transformHous = G4Transform3D(rotm1,positionHous);

						// Crystal position
						new G4PVPlacement(transformCryst,			//rotation,position
										  sciCrystLog,           //its logical volume
										  "sciCrystPl",             //its name
										  segmentBoxLog,             //its mother  volume
										  false,                 //no boolean operation
										  crysNb,                 //copy number
										  fCheckOverlaps);       // checking overlaps

						new G4PVPlacement(transformWind,				 //rotation,position
										  sciWindLog,           //its logical volume
										  "sciWindPl",             //its name
										  segmentBoxLog,             //its mother  volume
										  false,                 //no boolean operation
										  crysNb,                 //copy number
										  fCheckOverlaps);       // checking overlaps

						new G4PVPlacement(transformRefl,				 //rotation,position
										  sciReflLog,           //its logical volume
										  "sciReflPl",             //its name
										  segmentBoxLog,             //its mother  volume
										  false,                 //no boolean operation
										  crysNb,                 //copy number
										  fCheckOverlaps);       // checking overlaps

						new G4PVPlacement(transformHous,				 //rotation,position
										  sciHousLog,           //its logical volume
										  "sciHousPl",             //its name
										  segmentBoxLog,             //its mother  volume
										  false,                 //no boolean operation
										  crysNb,                 //copy number
										  fCheckOverlaps);       // checking overlaps

						crysNb += 1;
						positionInSegment += G4ThreeVector(sciHousSizeX*2, 0., 0.);
				}
				positionInSegment -= G4ThreeVector(nbCrystInSegmentRow*sciHousSizeX*2, 0., 0.);
				positionInSegment += G4ThreeVector(0., sciHousSizeY*2, 0.);
			}
            if (vacuumChamber == "yes") {
                G4ThreeVector positionVacuumFlange = (circleR1+vacuumFlangeSizeZ)*uz;
    			G4Transform3D transformVacuumFlange = G4Transform3D(rotm, positionVacuumFlange);
                new G4PVPlacement(transformVacuumFlange, //position
    				  vacuumFlangeBoxLog,                //its logical volume
    				  "VacuumFlange",                    //its name
    				  logicWorld,                         //its mother  volume
    				  false,                              //no boolean operation
    				  iseg,                               //copy number
    				  fCheckOverlaps);                    // checking overlaps
    			G4ThreeVector positionSegment = (circleR1+2*vacuumFlangeSizeZ+(sciHousSizeZ+sciWindSizeZ)-(2*vacuumFlangeSizeZ-vacuumFlangeThickFrontOfScint))*uz;
    			G4Transform3D transformSegment = G4Transform3D(rotm, positionSegment);
    			new G4PVPlacement(transformSegment, //position
    				  segmentBoxLog,                //its logical volume
    				  "Segment",                    //its name
    				  logicWorld,                   //its mother  volume
    				  false,                        //no boolean operation
    				  iseg,                         //copy number
    				  fCheckOverlaps);              // checking overlaps
            }
            else {
                G4ThreeVector positionSegment = (circleR1+(sciHousSizeZ+sciWindSizeZ))*uz;
    			G4Transform3D transformSegment = G4Transform3D(rotm, positionSegment);
    			new G4PVPlacement(transformSegment, //position
    				  segmentBoxLog,                //its logical volume
    				  "Segment",                    //its name
    				  logicWorld,                   //its mother  volume
    				  false,                        //no boolean operation
    				  iseg,                         //copy number
    				  fCheckOverlaps);              // checking overlaps
            }
	}

  // Print materials
  //G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  //
  // Print dimensions of the scintillator array
  G4cout <<""<< G4endl;
  G4cout <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<< G4endl;
  G4cout <<"$$$$"<< G4endl;
  G4cout <<"$$$$"<<" Crystal material: "<<sciCrystMat->GetName()<< G4endl;
  G4cout <<"$$$$"<<" Single crystal dimensions: "<<sciCrystSizeX*2<<"mmx"<<sciCrystSizeY*2<<"mmx"<<sciCrystSizeZ*2<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Dimensions of a single crystal housing: "<<sciHousSizeX*2<<"mmx"<<sciHousSizeY*2<<"mmx"<<sciHousSizeZ*2<<"mm "<< G4endl;
  G4cout <<"$$$$"<<" Number of segments in the array: "<<nbSegments<<" "<< G4endl;
  G4cout <<"$$$$"<<" Number of crystals in a segment row: "<<nbCrystInSegmentRow<<" "<< G4endl;
  G4cout <<"$$$$"<<" Number of crystals in a segment column: "<<nbCrystInSegmentColumn<<" "<< G4endl;
  G4cout <<"$$$$"<<" Number of crystals in the array: "<<nbSegments*nbCrystInSegmentRow*nbCrystInSegmentColumn<<" "<< G4endl;
  G4cout <<"$$$$"<<" Radius of a circle inscribed in the array: "<<circleR1<<"mm "<< G4endl;
  G4cout <<"$$$$"<< G4endl;
  G4cout <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$"<< G4endl;
  G4cout <<""<< G4endl;
  CreateScorers();

  //
  //always return the physical World
  //
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
}

// ###################################################################################
