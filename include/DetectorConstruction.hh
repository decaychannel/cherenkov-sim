//
// ********************************************************************
// * DetectorConstruction Header                                       *
// ********************************************************************
//

#ifndef DETECTORCONSTRUCTION_HH
#define DETECTORCONSTRUCTION_HH

#include "G4VUserDetectorConstruction.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "globals.hh"

class Materials;

class DetectorConstruction : public G4VUserDetectorConstruction
{
public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume* Construct() override;

private:
    void DefineMaterials();
    G4VPhysicalVolume* DefineVolumes();

    Materials* fMaterials;
    
    G4LogicalVolume* fWorldLogical;
    G4VPhysicalVolume* fWorldPhysical;
    
    // World dimensions
    G4double fWorldSizeX;
    G4double fWorldSizeY;
    G4double fWorldSizeZ;
};

#endif
