//
// ********************************************************************
// * RunAction - Accumulates Cherenkov photon emission angles         *
// ********************************************************************
//

#include "RunAction.hh"

#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"

#include <cmath>

RunAction::RunAction()
    : G4UserRunAction(),
      fNPhotons(0),
      fSumAngle(0.0),
      fSumAngle2(0.0),
      fSumBetaLen(0.0),
      fSumLen(0.0)
{
    // Register accumulables so they are merged across threads automatically
    auto accManager = G4AccumulableManager::Instance();
    accManager->Register(fNPhotons);
    accManager->Register(fSumAngle);
    accManager->Register(fSumAngle2);
    accManager->Register(fSumBetaLen);
    accManager->Register(fSumLen);
}

RunAction::~RunAction()
{
}

void RunAction::AddPhotonAngle(G4double angle)
{
    fNPhotons  += 1;
    fSumAngle  += angle;
    fSumAngle2 += angle * angle;
}

void RunAction::AddBeta(G4double beta, G4double length)
{
    fSumBetaLen += beta * length;
    fSumLen     += length;
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4AccumulableManager::Instance()->Reset();
}

void RunAction::EndOfRunAction(const G4Run* run)
{
    // Every thread must call Merge(): worker threads push their values into
    // the master's accumulables. Only the master then reports the result.
    G4AccumulableManager::Instance()->Merge();

    if (!IsMaster()) return;

    G4int    n     = fNPhotons.GetValue();
    G4int    nEvts = run->GetNumberOfEvent();

    if (n == 0) {
        G4cout << "\n--- Cherenkov angle: no optical photons recorded. ---\n" << G4endl;
        return;
    }

    G4double mean = fSumAngle.GetValue() / n;
    G4double var  = fSumAngle2.GetValue() / n - mean * mean;
    G4double rms  = (var > 0.0) ? std::sqrt(var) : 0.0;

    // Theoretical Cherenkov angle: cos(theta_c) = 1 / (n * beta).
    // beta is taken from the primary's actual path-averaged speed in water,
    // so this stays correct if the beam energy or particle is changed.
    // Water is dispersive (RINDEX runs 1.33 red -> 1.34 blue, see Materials.cc),
    // so we quote a band over that range; the measured mean falls inside it.
    const G4double nLow  = 1.33;   // red end  -> smallest angle
    const G4double nHigh = 1.34;   // blue end -> largest angle
    G4double totLen   = fSumLen.GetValue();
    G4double betaMean = (totLen > 0.0) ? fSumBetaLen.GetValue() / totLen : 0.0;

    G4cout << "\n"
           << "========================================================\n"
           << "        Cherenkov Cone Angle (primary muon only)        \n"
           << "========================================================\n"
           << "  Events processed     : " << nEvts << "\n"
           << "  Cherenkov photons    : " << n << "\n"
           << "  Mean emission angle  : " << mean / deg << " deg\n"
           << "  RMS spread           : " << rms  / deg << " deg\n"
           << "  --------------------------------------------------    \n";

    if (totLen > 0.0 && nLow * betaMean > 1.0) {
        G4double thetaLow  = std::acos(1.0 / (nLow  * betaMean)) / deg;
        G4double thetaHigh = std::acos(1.0 / (nHigh * betaMean)) / deg;
        G4cout << "  Primary mean beta    : " << betaMean << "\n"
               << "  Theoretical angle    : " << thetaLow << " - " << thetaHigh
               << " deg  (n=" << nLow << "-" << nHigh << ")\n"
               << "  (cos(theta_c)=1/(n*beta); dispersion gives the band)\n";
    } else {
        G4cout << "  Theoretical angle    : N/A (primary below threshold)\n";
    }

    G4cout << "========================================================\n"
           << G4endl;
}
