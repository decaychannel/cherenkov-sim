//
// ********************************************************************
// * SteppingAction - Captures Cherenkov photon emission angles       *
// ********************************************************************
//

#ifndef STEPPINGACTION_HH
#define STEPPINGACTION_HH

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class RunAction;
class G4Step;

class SteppingAction : public G4UserSteppingAction
{
public:
    explicit SteppingAction(RunAction* runAction);
    ~SteppingAction() override;

    void UserSteppingAction(const G4Step* step) override;

private:
    RunAction* fRunAction;
};

#endif
