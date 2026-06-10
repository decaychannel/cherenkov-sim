//
// ********************************************************************
// * RunAction - Accumulates Cherenkov photon emission angles         *
// ********************************************************************
//

#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include "G4Accumulable.hh"
#include "globals.hh"

class G4Run;

class RunAction : public G4UserRunAction
{
public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

    // Called by SteppingAction for every primary-muon Cherenkov photon
    void AddPhotonAngle(G4double angle);   // emission angle in radians

    // Called by SteppingAction with the primary's beta in water, weighted by the
    // number of photons radiated in that step. Photon-weighting means only the
    // radiating (above-threshold) part of the path enters the theoretical angle.
    void AddBeta(G4double beta, G4double weight);  // weight = photons in the step

private:
    G4Accumulable<G4int>    fNPhotons;   // number of Cherenkov photons
    G4Accumulable<G4double> fSumAngle;   // sum of emission angles
    G4Accumulable<G4double> fSumAngle2;  // sum of squared angles (for std dev)

    G4Accumulable<G4double> fSumBetaW;   // sum of beta * photon-weight
    G4Accumulable<G4double> fSumW;       // sum of photon-weight
};

#endif
