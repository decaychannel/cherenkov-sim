//
// ********************************************************************
// * SteppingAction - Captures Cherenkov photon emission angles       *
// ********************************************************************
//

#include "SteppingAction.hh"
#include "RunAction.hh"

#include "G4Step.hh"
#include "G4Track.hh"
#include "G4OpticalPhoton.hh"
#include "G4VProcess.hh"
#include "G4VPhysicalVolume.hh"
#include "G4ThreeVector.hh"

#include <vector>

SteppingAction::SteppingAction(RunAction* runAction)
    : G4UserSteppingAction(),
      fRunAction(runAction)
{
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    G4Track* track = step->GetTrack();

    // We only act on the PRIMARY particle (parentID 0), the radiator itself.
    // This inherently keeps only the primary's Cherenkov photons (delta-ray
    // photons are secondaries of delta electrons, not of the primary).
    if (track->GetParentID() != 0)
        return;

    // ...only while it is inside the water radiator.
    const G4VPhysicalVolume* vol = step->GetPreStepPoint()->GetPhysicalVolume();
    if (!vol || vol->GetName() != "WaterTank")
        return;

    // Record the primary's speed (path-weighted) for the theoretical angle,
    // valid for any beam energy / particle.
    fRunAction->AddBeta(step->GetPreStepPoint()->GetBeta(), step->GetStepLength());

    // Reference direction = the actual step displacement, which is exactly the
    // particle direction G4Cerenkov uses when sampling photon angles. Measuring
    // against this (not the fixed +z beam axis) stays correct even when the
    // primary multiple-scatters (e.g. electrons).
    G4ThreeVector primaryDir = step->GetDeltaPosition();
    if (primaryDir.mag() <= 0.0)
        return;
    primaryDir = primaryDir.unit();

    // Loop over the Cherenkov photons emitted during this step and record the
    // angle between each photon and the primary's instantaneous direction.
    const std::vector<const G4Track*>* secondaries = step->GetSecondaryInCurrentStep();
    for (const G4Track* sec : *secondaries) {
        if (sec->GetDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
            continue;

        const G4VProcess* creator = sec->GetCreatorProcess();
        if (!creator || creator->GetProcessName() != "Cerenkov")
            continue;

        G4double angle = sec->GetMomentumDirection().angle(primaryDir);
        fRunAction->AddPhotonAngle(angle);
    }
}
