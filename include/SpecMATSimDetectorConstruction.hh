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

    G4double worldSizeXY;
    G4double worldSizeZ;

    G4int nbSegments;
    G4int nbCrystInSegmentRow;
    G4int nbCrystInSegmentColumn;

    G4String vacuumChamber;
    G4double vacuumFlangeSizeX;
    G4double vacuumFlangeSizeY;
    G4double vacuumFlangeSizeZ;
    G4double vacuumFlangeThickFrontOfScint;

    G4double dPhi;
    G4double half_dPhi;
    G4double tandPhi;

    G4double circleR1;

    G4double sciCrystSizeX;
    G4double sciCrystSizeY;
    G4double sciCrystSizeZ;

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

    G4Material* default_mat;
    G4Material* CeBr3;
    G4Material* LaBr3;
    G4Material* sciCrystMat;
    G4Material* Quartz;
    G4Material* TiO2;
    G4Material* Al_Alloy;
    G4Material* Air;
    G4Material* segment_mat;

    G4ThreeVector sciCrystPos;
    G4ThreeVector sciWindPos;
    G4ThreeVector sciReflPos;


    G4VSolid* solidWorld;
    G4VSolid* sciCrystSolid;
    G4VSolid* sciWindSolid;
    G4VSolid* reflBoxSolid;
    G4VSolid* sciReflSolid;
    G4VSolid* housBoxASolid;
    G4VSolid* housBoxBSolid;
    G4VSolid* sciHousSolid;


    G4LogicalVolume* logicWorld;
    G4LogicalVolume* sciCrystLog;
    G4LogicalVolume* sciWindLog;
    G4LogicalVolume* sciReflLog;
    G4LogicalVolume* sciHousLog;

    G4LogicalVolume* vacuumFlangeBoxLog;
    G4LogicalVolume* vacuumChamberSideFlangeLog;
    G4LogicalVolume* segmentBoxLog;

    G4VPhysicalVolume* physWorld;

    G4VisAttributes* sciCrystVisAtt;
    G4VisAttributes* sciWindVisAtt;
    G4VisAttributes* sciReflVisAtt;
    G4VisAttributes* sciHousVisAtt;

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
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif
