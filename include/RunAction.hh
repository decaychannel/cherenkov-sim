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

    // Called by SteppingAction for every primary step inside the water, so the
    // theoretical angle can be derived from the real, path-averaged beam beta.
    void AddBeta(G4double beta, G4double length);  // beta and step length in water

private:
    G4Accumulable<G4int>    fNPhotons;   // number of Cherenkov photons
    G4Accumulable<G4double> fSumAngle;   // sum of emission angles
    G4Accumulable<G4double> fSumAngle2;  // sum of squared angles (for std dev)

    G4Accumulable<G4double> fSumBetaLen; // sum of beta*length (path-weighted)
    G4Accumulable<G4double> fSumLen;     // sum of step length in water
};

#endif
