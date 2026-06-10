//
// ********************************************************************
// * ActionInitialization Implementation                               *
// ********************************************************************
//

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"
// #include "EventAction.hh"

ActionInitialization::ActionInitialization()
    : G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::BuildForMaster() const
{
    // Master thread needs a RunAction to merge per-thread results
    SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
    // Primary generator is mandatory
    SetUserAction(new PrimaryGeneratorAction());

    // RunAction accumulates the Cherenkov angles; SteppingAction feeds it
    auto runAction = new RunAction();
    SetUserAction(runAction);
    SetUserAction(new SteppingAction(runAction));
}
