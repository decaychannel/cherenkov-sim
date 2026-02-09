//
// ********************************************************************
// * ActionInitialization Implementation                               *
// ********************************************************************
//

#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
// #include "RunAction.hh"
// #include "EventAction.hh"
// #include "SteppingAction.hh"

ActionInitialization::ActionInitialization()
    : G4VUserActionInitialization()
{
}

ActionInitialization::~ActionInitialization()
{
}

void ActionInitialization::BuildForMaster() const
{
    // For multi-threaded mode
    // SetUserAction(new RunAction());
}

void ActionInitialization::Build() const
{
    // Primary generator is mandatory
    SetUserAction(new PrimaryGeneratorAction());

    // Optional user actions
    // SetUserAction(new RunAction());
    // SetUserAction(new EventAction());
    // SetUserAction(new SteppingAction());
}
