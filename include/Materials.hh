//
// ********************************************************************
// * Materials Header                                                  *
// ********************************************************************
//

#ifndef MATERIALS_HH
#define MATERIALS_HH

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "globals.hh"

#include <map>

class Materials
{
public:
    Materials();
    ~Materials();

    G4Material* GetMaterial(const G4String& name);

private:
    void DefineMaterials();
    void DefineCustomMaterials();

    G4NistManager* fNistManager;
    std::map<G4String, G4Material*> fMaterialsMap;
};

#endif
