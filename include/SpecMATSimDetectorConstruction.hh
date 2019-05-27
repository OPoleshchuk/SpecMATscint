/// \file SpecMATSimDetectorConstruction.hh
/// \brief Definition of the SpecMATSimDetectorConstruction class

#ifndef SpecMATSimDetectorConstruction_h
#define SpecMATSimDetectorConstruction_h 1

#include "G4Material.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
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

#include "globals.hh"

class G4VPhysicalVolume;
class G4LogicalVolume;

/// Detector construction class to define materials and geometry.
///

class SpecMATSimDetectorConstruction : public G4VUserDetectorConstruction
{
  private:
    void DefineMaterials();
    void CreateScorers();

    G4double a, z, density;
    G4int natoms, ncomponents;
    G4double fractionmass;

    G4double worldSizeXY;
    G4double worldSizeZ;
    G4double gap;

    G4int nbSegments;
    G4int nbCrystInSegmentRow;
    G4int nbCrystInSegmentColumn;

    G4String vacuumChamber;
    G4double vacuumFlangeSizeX;
    G4double vacuumFlangeSizeY;
    G4double vacuumFlangeSizeZ;
    G4double vacuumFlangeThickFrontOfScint;

    G4String insulationTube;
    G4double insulationTubeThickness;
    G4double insulationTubeInnerRadius;
    G4double insulationTubeOuterRadius;

    G4String ComptSupp;

    G4double dPhi;
    G4double half_dPhi;
    G4double tandPhi;

    G4double circleR1;

    G4double sciCrystSizeX;
    G4double sciCrystSizeY;
    G4double sciCrystSizeZ;

    G4double ComptSuppSizeX;
    G4double ComptSuppSizeY;
    G4double ComptSuppSizeZ;

    G4double ComptSuppPosX;									//Position of the Crystal along the X axis
    G4double ComptSuppPosY;									//Position of the Crystal along the Y axis
    G4double ComptSuppPosZ;

    G4double sciCrystPosX;
    G4double sciCrystPosY;
    G4double sciCrystPosZ;

    G4double sciWindSizeX;
    G4double sciWindSizeY;
    G4double sciWindSizeZ;

    G4double sciWindPosX;
    G4double sciWindPosY;
    G4double sciWindPosZ;

    G4double sciReflWallThickX;
    G4double sciReflWallThickY;
    G4double sciReflWindThick;

    G4double sciReflSizeX;
    G4double sciReflSizeY;
    G4double sciReflSizeZ;

    G4double sciReflPosX;
    G4double sciReflPosY;
    G4double sciReflPosZ;

    G4double sciHousWallThickX;
    G4double sciHousWallThickY;
    G4double sciHousWindThick;

    G4double sciHousSizeX;
    G4double sciHousSizeY;
    G4double sciHousSizeZ;

    G4double sciHousPosX;
    G4double sciHousPosY;
    G4double sciHousPosZ;

    G4Element* Ce;
    G4Element* La;
    G4Element* Br;
    G4Element* O;
    G4Element* Si;
    G4Element* Ti;
    G4Element* Al;
    G4Element* C;
    G4Element* H;
    G4Element* Bi;
    G4Element* Ge;
    G4Element* N;

    G4Material* default_mat;
    G4Material* CeBr3;
    G4Material* LaBr3;
    G4Material* BGO;
    G4Material* sciCrystMat;
    G4Material* ComptSuppMat;
    G4Material* Quartz;
    G4Material* sciWindMat;
    G4Material* TiO2;
    G4Material* sciReflMat;
    G4Material* Al_Alloy;
    G4Material* sciHousMat;
    G4Material* vacuumFlangeMat;
    G4Material* vacuumSideFlangeMat;
    G4Material* Polypropylen_C3H6;
    G4Material* insulationTubeMat;
    G4Material* Air;
    G4Material* segment_mat;
    G4Material* Steel_316L;

    G4NistManager* nist;

    G4ThreeVector sciCrystPos;
    G4ThreeVector ComptSuppPos;
    G4ThreeVector sciWindPos;
    G4ThreeVector sciReflPos;
    G4ThreeVector sciHousPos;


    G4VSolid* solidWorld;
    G4VSolid* sciCrystSolid;
    G4VSolid* ComptSuppSolid;
    G4VSolid* ComptSuppSolidBox;
    G4VSolid* ComptSuppSolidBoxUp;
    G4VSolid* ComptSuppSolidBoxDown;
    G4VSolid* ComptSuppSolidBoxWithoutUp;
    G4VSolid* ComptSuppSolidBoxWithoutDown;
    G4VSolid* sciWindSolid;
    G4VSolid* reflBoxSolid;
    G4VSolid* sciReflSolid;
    G4VSolid* housBoxASolid;
    G4VSolid* housBoxBSolid;
    G4VSolid* sciHousSolid;


    G4LogicalVolume* logicWorld;
    G4LogicalVolume* sciCrystLog;
    G4LogicalVolume* ComptSuppLogBox;
    G4LogicalVolume* ComptSuppLogUp;
    G4LogicalVolume* ComptSuppLogDown;
    G4LogicalVolume* ComptSuppTrapLog;
    G4LogicalVolume* sciWindLog;
    G4LogicalVolume* sciReflLog;
    G4LogicalVolume* sciHousLog;

    G4LogicalVolume* vacuumFlangeBoxLog;
    G4LogicalVolume* vacuumChamberSideFlangeLog;
    G4LogicalVolume* insulationTubeLog;
    G4LogicalVolume* segmentBoxLog;

    G4VPhysicalVolume* physWorld;

    G4VisAttributes* worldVisAtt;
    G4VisAttributes* sciCrystVisAtt;
    G4VisAttributes* ComptSuppVisAtt;
    G4VisAttributes* ComptSuppVisAttUp;
    G4VisAttributes* ComptSuppVisAttDown;
    G4VisAttributes* sciWindVisAtt;
    G4VisAttributes* sciReflVisAtt;
    G4VisAttributes* sciHousVisAtt;
    G4VisAttributes* insulationTubeVisAtt;

    G4bool  fCheckOverlaps;

  public:
    SpecMATSimDetectorConstruction();
    virtual ~SpecMATSimDetectorConstruction();

    virtual G4VPhysicalVolume* Construct();

    G4double ComputeCircleR1();

    void SetNbSegments(G4int val){nbSegments = val;}
    G4double GetNbSegments(void){return nbSegments;}
    void SetNbCrystInSegmentRow(G4int val){nbCrystInSegmentRow = val;}
    G4double GetNbCrystInSegmentRow(void){return nbCrystInSegmentRow;}
    void SetNbCrystInSegmentColumn(G4int val){nbCrystInSegmentColumn = val;}
    G4double GetNbCrystInSegmentColumn(void){return nbCrystInSegmentColumn;}


    void SetSciCrystSizeX(G4double val){sciCrystSizeX = val;}
    G4double GetSciCrystSizeX(void){return sciCrystSizeX;}
    void SetSciCrystSizeY(G4double val){sciCrystSizeY = val;}
    G4double GetSciCrystSizeY(void){return sciCrystSizeY;}
    void SetSciCrystSizeZ(G4double val){sciCrystSizeZ = val;}
    G4double GetSciCrystSizeZ(void){return sciCrystSizeZ;}


    void SetSciWindSizeX(G4double val){sciWindSizeX = val;}
    G4double GetSciWindSizeX(void){return sciWindSizeX;}
    void SetSciWindSizeY(G4double val){sciWindSizeY = val;}
    G4double GetSciWindSizeY(void){return sciWindSizeY;}
    void SetSciWindSizeZ(G4double val){sciWindSizeZ = val;}
    G4double GetSciWindSizeZ(void){return sciWindSizeZ;}


    void SetSciReflWallThickX(G4double val){sciReflWallThickX = val;}
    G4double GetSciReflWallThickX(void){return sciReflWallThickX;}
    void SetSciReflWallThickY(G4double val){sciReflWallThickY = val;}
    G4double GetSciReflWallThickY(void){return sciReflWallThickY;}
    void SetSciReflWindThick(G4double val){sciReflWindThick = val;}
    G4double GetSciReflWindThick(void){return sciReflWindThick;}


    void SetSciHousWallThickX(G4double val){sciHousWallThickX = val;}
    G4double GetSciHousWallThickX(void){return sciHousWallThickX;}
    void SetSciHousWallThickY(G4double val){sciHousWallThickY = val;}
    G4double GetSciHousWallThickY(void){return sciHousWallThickY;}
    void SetSciHousWindThick(G4double val){sciHousWindThick = val;}
    G4double GetSciHousWindThick(void){return sciHousWindThick;}

    void SetSciHousSizeX(G4double val){sciHousSizeX = val;}
    G4double GetSciHousSizeX(void){return sciHousSizeX;}
    void SetSciHousSizeY(G4double val){sciHousSizeY = val;}
    G4double GetSciHousSizeY(void){return sciHousSizeY;}
    void SetSciHousSizeZ(G4double val){sciHousSizeZ = val;}
    G4double GetSciHousSizeZ(void){return sciHousSizeZ;}

    void SetSciCrystMat (G4String);
    G4Material* GetSciCrystMat(){return sciCrystMat;}

    void SetVacuumChamber (G4String);
    G4String GetVacuumChamber(){return vacuumChamber;}
    void SetVacuumFlangeMat (G4String);
    G4Material* GetVacuumFlangeMat(){return vacuumFlangeMat;}

    void SetInsulationTube (G4String);
    G4String GetInsulationTube(){return insulationTube;}
    void SetInsulationTubeMat (G4String);
    G4Material* GetInsulationTubeMat(){return insulationTubeMat;}

    void SetGap(G4double val){gap = val;}
    G4double GetGap(void){return gap;}
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
