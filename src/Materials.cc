//
// ********************************************************************
// * Materials - Water with optical properties for Cherenkov          *
// ********************************************************************
//

#include "Materials.hh"
#include "G4SystemOfUnits.hh"
#include "G4MaterialPropertiesTable.hh"

Materials::Materials()
{
    fNistManager = G4NistManager::Instance();
    DefineMaterials();
    DefineCustomMaterials();
}

Materials::~Materials()
{
    // Materials are managed by Geant4, no need to delete
}

void Materials::DefineMaterials()
{
    // Define commonly used NIST materials
    // These can be accessed using GetMaterial("G4_XXX")
    
    // Gases
    fMaterialsMap["G4_AIR"] = fNistManager->FindOrBuildMaterial("G4_AIR");
    fMaterialsMap["G4_Galactic"] = fNistManager->FindOrBuildMaterial("G4_Galactic");
    
    // Metals
    fMaterialsMap["G4_Al"] = fNistManager->FindOrBuildMaterial("G4_Al");
    fMaterialsMap["G4_Fe"] = fNistManager->FindOrBuildMaterial("G4_Fe");
    fMaterialsMap["G4_Cu"] = fNistManager->FindOrBuildMaterial("G4_Cu");
    fMaterialsMap["G4_Pb"] = fNistManager->FindOrBuildMaterial("G4_Pb");
    fMaterialsMap["G4_W"] = fNistManager->FindOrBuildMaterial("G4_W");
    
    // Semiconductors
    fMaterialsMap["G4_Si"] = fNistManager->FindOrBuildMaterial("G4_Si");
    fMaterialsMap["G4_Ge"] = fNistManager->FindOrBuildMaterial("G4_Ge");
    
    // Scintillators
    fMaterialsMap["G4_PLASTIC_SC_VINYLTOLUENE"] = fNistManager->FindOrBuildMaterial("G4_PLASTIC_SC_VINYLTOLUENE");
    fMaterialsMap["G4_SODIUM_IODIDE"] = fNistManager->FindOrBuildMaterial("G4_SODIUM_IODIDE");
    fMaterialsMap["G4_BGO"] = fNistManager->FindOrBuildMaterial("G4_BGO");
    
    // Common materials
    fMaterialsMap["G4_WATER"] = fNistManager->FindOrBuildMaterial("G4_WATER");
    fMaterialsMap["G4_CONCRETE"] = fNistManager->FindOrBuildMaterial("G4_CONCRETE");
}

void Materials::DefineCustomMaterials()
{
    //
    // Water with optical properties for Cherenkov radiation
    //
    G4Material* water = fMaterialsMap["G4_WATER"];
    
    // Define optical photon energies (from 200nm to 800nm wavelength)
    const G4int nEntries = 32;
    G4double photonEnergy[nEntries];
    G4double refractiveIndex[nEntries];
    G4double absorption[nEntries];
    
    for (G4int i = 0; i < nEntries; i++) {
        // Energy range: 1.55 eV (800nm) to 6.20 eV (200nm)
        photonEnergy[i] = (1.55 + i * 0.15) * eV;
        
        // Refractive index of water (~1.33, with slight dispersion)
        refractiveIndex[i] = 1.33 + 0.01 * (photonEnergy[i] / eV - 1.55) / 4.65;
        
        // Absorption length in water (varies with wavelength, ~10m for visible)
        absorption[i] = 10.0 * m;
    }
    
    // Create material properties table
    G4MaterialPropertiesTable* waterMPT = new G4MaterialPropertiesTable();
    waterMPT->AddProperty("RINDEX", photonEnergy, refractiveIndex, nEntries);
    waterMPT->AddProperty("ABSLENGTH", photonEnergy, absorption, nEntries);
    
    water->SetMaterialPropertiesTable(waterMPT);
    
    // Store as "Water" for easy access
    fMaterialsMap["Water"] = water;
}

G4Material* Materials::GetMaterial(const G4String& name)
{
    // First check if material is in our map
    auto it = fMaterialsMap.find(name);
    if (it != fMaterialsMap.end()) {
        return it->second;
    }
    
    // If not found, try to get it from NIST
    G4Material* material = fNistManager->FindOrBuildMaterial(name);
    if (material) {
        fMaterialsMap[name] = material;
        return material;
    }
    
    G4cerr << "Material " << name << " not found!" << G4endl;
    return nullptr;
}
