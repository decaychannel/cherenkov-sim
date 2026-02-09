//
// ********************************************************************
// * DetectorConstruction - Water tank for Cherenkov radiation        *
// ********************************************************************
//

#include "DetectorConstruction.hh"
#include "Materials.hh"

#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction()
    : G4VUserDetectorConstruction(),
      fMaterials(nullptr),
      fWorldLogical(nullptr),
      fWorldPhysical(nullptr),
      fWorldSizeX(2.0*m),
      fWorldSizeY(2.0*m),
      fWorldSizeZ(2.0*m)
{
}

DetectorConstruction::~DetectorConstruction()
{
    delete fMaterials;
}

void DetectorConstruction::DefineMaterials()
{
    fMaterials = new Materials();
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    DefineMaterials();
    return DefineVolumes();
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
    // Get materials
    G4Material* worldMaterial = fMaterials->GetMaterial("G4_AIR");

    //
    // World
    //
    auto worldSolid = new G4Box("World",
                                fWorldSizeX/2,
                                fWorldSizeY/2,
                                fWorldSizeZ/2);

    fWorldLogical = new G4LogicalVolume(worldSolid,
                                        worldMaterial,
                                        "World");

    fWorldPhysical = new G4PVPlacement(nullptr,              // no rotation
                                       G4ThreeVector(),      // at (0,0,0)
                                       fWorldLogical,        // logical volume
                                       "World",              // name
                                       nullptr,              // mother volume
                                       false,                // no boolean operation
                                       0,                    // copy number
                                       true);                // check overlaps

    // World visualization - wireframe style
    auto worldVisAtt = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.3));
    worldVisAtt->SetVisibility(true);
    worldVisAtt->SetForceWireframe(true);
    fWorldLogical->SetVisAttributes(worldVisAtt);

    //
    // Water tank for Cherenkov radiation (50cm x 50cm x 50cm)
    //
    G4Material* waterMaterial = fMaterials->GetMaterial("Water");
    
    auto waterSolid = new G4Box("WaterTank", 25*cm, 25*cm, 25*cm);
    auto waterLogical = new G4LogicalVolume(waterSolid, 
                                            waterMaterial,
                                            "WaterTank");
    
    new G4PVPlacement(nullptr,                    // no rotation
                      G4ThreeVector(0, 0, 0),     // position at center
                      waterLogical,               // logical volume
                      "WaterTank",                // name
                      fWorldLogical,              // mother volume
                      false,                      // no boolean operation
                      0,                          // copy number
                      true);                      // check overlaps
    
    // Water visualization - semi-transparent blue color
    auto waterVisAtt = new G4VisAttributes(G4Colour(0.0, 0.4, 1.0, 0.4));
    waterVisAtt->SetVisibility(true);
    waterVisAtt->SetForceSolid(true);
    waterLogical->SetVisAttributes(waterVisAtt);

    return fWorldPhysical;
}
