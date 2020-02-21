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



  G4int nbSegments;
  G4int nbCrystInSegmentRow;
  G4int nbCrystInSegmentColumn;
  G4int natoms, ncomponents;
  G4int TotalCrystNb;
  G4int i;
  G4int crysNb;
  G4int iseg;
  G4int icrystRow;
  G4int icrystCol;
  G4int AlphaSourceFlag;
  G4int BfieldFlag;

  G4String ComptSuppFlag;
  G4String vacuumChamber;
  G4String sourceHolder;
  G4String sourceHousingHilde;
  G4String fieldCageEpoxy;

  G4double a, z, density, densitySTM, densityEpoxy, densityAluminum5083;
  G4double fractionmass, fractionmassMn, fractionmassFe, fractionmassCu, fractionmassMg, fractionmassSi, fractionmassZn, fractionmassCr, fractionmassTi, fractionmassAl;
  G4double worldSizeXY;
  G4double worldSizeZ;
  G4double gap;
  G4double vacuumFlangeSizeX;
  G4double vacuumFlangeSizeY;
  G4double vacuumFlangeSizeZ;
  G4double vacuumFlangeThickFrontOfScint;
  G4double vacuumChamberShift;
  G4double vacuumChamberThickness;
  G4double vacuumChamberInnerRadius;
  G4double vacuumChamberOuterRadius;

  G4double vacuumChamberThickness2;
  G4double vacuumChamberInnerRadius2;
  G4double vacuumChamberOuterRadius2;

  G4double vacuumChamberThickness3;
  G4double vacuumChamberInnerRadius3;
  G4double vacuumChamberOuterRadius3;

  G4double vacuumChamberThickness4;
  G4double vacuumChamberInnerRadius4;
  G4double vacuumChamberOuterRadius4;

  G4double vacuumChamberThickness5;
  G4double vacuumChamberInnerRadius5;
  G4double vacuumChamberOuterRadius5;

  G4double vacuumChamberThickness6;
  G4double vacuumChamberInnerRadius6;
  G4double vacuumChamberOuterRadius6;

  G4double vacuumChamberThickness7;
  G4double vacuumChamberInnerRadius7;
  G4double vacuumChamberOuterRadius7;

  G4double gasVolumeOuterRadius;

  G4double sourceHolderThickness;
  G4double sourceHolderInnerRadius;
  G4double sourceHolderOuterRadius;
  G4double sourceHolderLeftInnerRadius;
  G4double sourceHolderLeftOuterRadius;
  G4double sourceHolderRightInnerRadius;
  G4double sourceHolderRightOuterRadius;
  G4double sourceHousingHildeThickness;
  G4double sourceHousingHildeInnerRadius;
  G4double sourceHousingHildeOuterRadius;

  G4double fieldCageEpoxyThickness;
  G4double fieldCageEpoxyInnerRadius;
  G4double fieldCageEpoxyOuterRadius;
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
  G4double ComptSuppPosX;
  G4double ComptSuppPosY;
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
  G4double rotationAngle;
  G4double phi;
  G4double pointSourcePositionZ;
  G4double sourceHousingHildePositionZ;

  G4Element* Ce;
  G4Element* La;
  G4Element* Br;
  G4Element* O;
  G4Element* Si;
  G4Element* Ti;
  G4Element* Al;
  G4Element* Carb;
  G4Element* H;
  G4Element* Bi;
  G4Element* Ge;
  G4Element* N;
  G4Element* Mg;
  G4Element* Cr;
  G4Element* Ni;
  G4Element* Mo;
  G4Element* P;
  G4Element* S;
  G4Element* Fe;
  G4Element* Chlor;
  G4Element* Mn;
  //G4Element* Fe;
  G4Element* Cu;
  //G4Element* Mg;
  //G4Element* Si;
  G4Element* Zn;
  //G4Element* Cr;
  //G4Element* Ti;
  //G4Element* Al;
  G4Element* Fluorine;
  G4Element* Argon;

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
  G4Material* fieldCageEpoxyMat;
  G4Material* vacuumChamberMat;
  G4Material* PVC;
  G4Material* sourceHolderMat;
  G4Material* sourceHousingHildeMat;
  G4Material* Air;
  G4Material* segment_mat;
  G4Material* Steel_316L;
  G4Material* Aluminum5083;
  G4Material* epoxy;
  G4Material* ArCF4;
  G4Material* gasVolumeMat;

  G4NistManager* nist;

  G4ThreeVector sciCrystPos;
  G4ThreeVector ComptSuppPos;
  G4ThreeVector sciWindPos;
  G4ThreeVector sciReflPos;
  G4ThreeVector sciHousPos;
  G4ThreeVector positionSideFlange1;
  G4ThreeVector positionSideFlange2;
  G4ThreeVector positionComptSuppBoxUp;
  G4ThreeVector positionComptSuppBoxDown;
  G4ThreeVector positionComptSuppTrap;
  G4ThreeVector uz;
  G4ThreeVector positionInSegment;
  G4ThreeVector* crystalPositionsArray;
  G4ThreeVector positionCryst;
  G4ThreeVector positionWind;
  G4ThreeVector positionRefl;
  G4ThreeVector positionHous;
  G4ThreeVector positionVacuumFlange;
  G4ThreeVector positionSegment;

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
  G4VSolid* segmentBox;
  G4VSolid* vacuumFlangeBox;
  G4VSolid* vacuumFlangeSolid;
  G4VSolid* vacuumChamberSideFlange;
  G4VSolid* fieldCageEpoxySolid;
  G4VSolid* vacuumChamberSolid;
  G4VSolid* vacuumChamberSolid2;
  G4VSolid* vacuumChamberSolid3;
  G4VSolid* vacuumChamberSolid4;
  G4VSolid* vacuumChamberSolid5;
  G4VSolid* vacuumChamberSolid6;
  G4VSolid* vacuumChamberSolid7;
  G4VSolid* sourceHolderSolid;
  G4VSolid* sourceHolderLeftSolid;
  G4VSolid* sourceHolderRightSolid;
  G4VSolid* sourceHousingHildeSolid;
  G4VSolid* gasVolumeSolid;

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
  G4LogicalVolume* fieldCageEpoxyLog;
  G4LogicalVolume* vacuumChamberLog;
  G4LogicalVolume* vacuumChamberLog2;
  G4LogicalVolume* vacuumChamberLog3;
  G4LogicalVolume* vacuumChamberLog4;
  G4LogicalVolume* vacuumChamberLog5;
  G4LogicalVolume* vacuumChamberLog6;
  G4LogicalVolume* vacuumChamberLog7;
  G4LogicalVolume* sourceHolderLog;
  G4LogicalVolume* sourceHolderLeftLog;
  G4LogicalVolume* sourceHolderRightLog;
  G4LogicalVolume* sourceHousingHildeLog;
  G4LogicalVolume* segmentBoxLog;
  G4LogicalVolume* gasVolumeLog;

  G4VPhysicalVolume* physWorld;

  G4VisAttributes* worldVisAtt;
  G4VisAttributes* sciCrystVisAtt;
  G4VisAttributes* ComptSuppVisAtt;
  G4VisAttributes* ComptSuppVisAttUp;
  G4VisAttributes* ComptSuppVisAttDown;
  G4VisAttributes* sciWindVisAtt;
  G4VisAttributes* sciReflVisAtt;
  G4VisAttributes* sciHousVisAtt;
  G4VisAttributes* fieldCageEpoxyVisAtt;
  G4VisAttributes* vacuumChamberVisAtt;
  G4VisAttributes* vacuumChamberVisAtt2;
  G4VisAttributes* vacuumChamberVisAtt3;
  G4VisAttributes* vacuumChamberVisAtt4;
  G4VisAttributes* vacuumChamberVisAtt5;
  G4VisAttributes* vacuumChamberVisAtt6;
  G4VisAttributes* vacuumChamberVisAtt7;
  G4VisAttributes* sourceHolderVisAtt;
  G4VisAttributes* sourceHolderLeftVisAtt;
  G4VisAttributes* sourceHolderRightVisAtt;
  G4VisAttributes* sourceHousingHildeVisAtt;
  G4VisAttributes* gasVolumeVisAtt;

  G4RotationMatrix rotSideFlnge;
  G4RotationMatrix ComptSuppRotmBoxUp;
  G4RotationMatrix ComptSuppRotmBoxDown;
  G4RotationMatrix ComptSuppTrapRotm;
  G4RotationMatrix rotm;
  G4RotationMatrix rotm1;
  G4RotationMatrix rotm2;
  G4RotationMatrix rotm3;

  G4Transform3D transformSideFlange1;
  G4Transform3D transformSideFlange2;
  G4Transform3D transformComptSuppBoxUp;
  G4Transform3D transformComptSuppBoxDown;
  G4Transform3D transformComptSuppTrap;
  G4Transform3D transformCryst;
  G4Transform3D transformWind;
  G4Transform3D transformRefl;
  G4Transform3D transformHous;
  G4Transform3D transformVacuumFlange;
  G4Transform3D transformSegment;

  G4AffineTransform TransformCrystPos1;
  G4AffineTransform TransformCrystPos;

  G4SDManager* SDman;
  G4MultiFunctionalDetector* cryst;
  G4PSEnergyDeposit* primitiv;
  G4MultiFunctionalDetector* ComptSupp;
  G4PSEnergyDeposit* ComptSuppPrimitiv;
  G4MultiFunctionalDetector* AlphaTracker;
  G4PSEnergyDeposit* AlphaTrackerPrimitiv;

  G4bool fCheckOverlaps;

public:
  SpecMATSimDetectorConstruction();
  virtual ~SpecMATSimDetectorConstruction();

  virtual G4VPhysicalVolume* Construct();

  G4double ComputeCircleR1();

  void SetPointSourcePositionZ(G4double val){pointSourcePositionZ = val;}
  G4double GetPointSourcePositionZ(void){return pointSourcePositionZ;}

  void SetNbSegments(G4int val){nbSegments = val;}
  G4int GetNbSegments(void){return nbSegments;}
  void SetNbCrystInSegmentRow(G4int val){nbCrystInSegmentRow = val;}
  G4int GetNbCrystInSegmentRow(void){return nbCrystInSegmentRow;}
  void SetNbCrystInSegmentColumn(G4int val){nbCrystInSegmentColumn = val;}
  G4int GetNbCrystInSegmentColumn(void){return nbCrystInSegmentColumn;}

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
  G4Material* GetSciWindMat(){return sciWindMat;}

  void SetSciReflWallThickX(G4double val){sciReflWallThickX = val;}
  G4double GetSciReflWallThickX(void){return sciReflWallThickX;}
  void SetSciReflWallThickY(G4double val){sciReflWallThickY = val;}
  G4double GetSciReflWallThickY(void){return sciReflWallThickY;}
  void SetSciReflWindThick(G4double val){sciReflWindThick = val;}
  G4double GetSciReflWindThick(void){return sciReflWindThick;}
  G4Material* GetSciReflMat(){return sciReflMat;}


  void SetSciHousWallThickX(G4double val){sciHousWallThickX = val;}
  G4double GetSciHousWallThickX(void){return sciHousWallThickX;}
  void SetSciHousWallThickY(G4double val){sciHousWallThickY = val;}
  G4double GetSciHousWallThickY(void){return sciHousWallThickY;}
  void SetSciHousWindThick(G4double val){sciHousWindThick = val;}
  G4double GetSciHousWindThick(void){return sciHousWindThick;}
  G4Material* GetSciHousMat(){return sciHousMat;}

  void SetSciHousSizeX(G4double val){sciHousSizeX = val;}
  G4double GetSciHousSizeX(void){return sciHousSizeX;}
  void SetSciHousSizeY(G4double val){sciHousSizeY = val;}
  G4double GetSciHousSizeY(void){return sciHousSizeY;}
  void SetSciHousSizeZ(G4double val){sciHousSizeZ = val;}
  G4double GetSciHousSizeZ(void){return sciHousSizeZ;}

  G4Material* GetSciCrystMat(){return sciCrystMat;}

  void SetSourceHolder (G4String val){sourceHolder = val;}
  G4String GetSourceHolder(){return sourceHolder;}
  G4Material* GetSourceHolderMat(){return sourceHolderMat;}
  G4double GetSourceHolderInnerRadius(void){return sourceHolderInnerRadius;}
  G4double GetSourceHolderOuterRadius(void){return sourceHolderOuterRadius;}

  void SetSourceHousingHilde (G4String val){sourceHousingHilde = val;}
  G4String GetSourceHousingHilde(){return sourceHousingHilde;}
  G4Material* GetSourceHousingHildeMat(){return sourceHousingHildeMat;}
  G4double GetSourceHousingHildeInnerRadius(void){return sourceHousingHildeInnerRadius;}
  G4double GetSourceHousingHildeOuterRadius(void){return sourceHousingHildeOuterRadius;}

  void SetVacuumChamber (G4String val){vacuumChamber = val;}
  G4String GetVacuumChamber(){return vacuumChamber;}
  G4Material* GetVacuumFlangeMat(){return vacuumFlangeMat;}
  G4Material* GetVacuumChamberMat(){return vacuumChamberMat;}
  void SetVacuumChamberThickness(G4double val){vacuumChamberThickness = val;}
  G4double GetVacuumChamberThickness(void){return vacuumChamberThickness;}
  void SetVacuumChamberShift(G4double val){vacuumChamberShift = val;}
  G4double GetVacuumChamberShift(void){return vacuumChamberShift;}
  G4double GetVacuumChamberInnerRadius(void){return vacuumChamberInnerRadius;}
  G4double GetVacuumChamberOuterRadius(void){return vacuumChamberOuterRadius;}


  G4String GetFieldCageEpoxy(){return fieldCageEpoxy;}
  G4Material* GetFieldCageEpoxyMat(){return fieldCageEpoxyMat;}
  G4double GetFieldCageEpoxyInnerRadius(void){return fieldCageEpoxyInnerRadius;}
  G4double GetFieldCageEpoxyOuterRadius(void){return fieldCageEpoxyOuterRadius;}
  G4String GetComptSuppFlag(){return ComptSuppFlag;}

  G4int GetAlphaTrackerFlag(){return AlphaSourceFlag;}
  G4Material* GetGasVolumeMat(){return gasVolumeMat;}
  G4double GetGasVolumeOuterRadius(void){return gasVolumeOuterRadius;}

  G4ThreeVector* GetCrystalPositionsArray(void){return crystalPositionsArray;}

  void SetGap(G4double val){gap = val;}
  G4double GetGap(void){return gap;}
};

// ###################################################################################

#endif
