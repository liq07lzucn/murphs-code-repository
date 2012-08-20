#include "EventAction.hh"
#include "Analysis.hh"

#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4SDManager.hh"
#include "G4HCofThisEvent.hh"
#include "G4UnitsTable.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4ios.hh"

EventAction::EventAction() : G4UserEventAction(){
    // Nothing to be Done Here
}



void EventAction::BeginOfEventAction(const G4Event* event){

    G4cout<<"Starting Event: "<<event->GetEventID()<<G4endl;
    if(event->GetEventID() % 1000 == 0)
        G4cout<<"Starting Event: "<<event->GetEventID()<<G4endl;

    Analysis::GetInstance()->PrepareNewEvent(event);
}

/**
 * EndOfEventAction
 *
 * @param const G4Event* event - event to be processed
 *
 * At the end of an event we want to call analysis to proccess
 * this event, and record the useful information.
 */
void EventAction::EndOfEventAction(const G4Event* event){  

    Analysis::GetInstance()->EndOfEvent(event);

    // Some Work with the trajectories and sensitve detector
   /*
    if(event->GetEventID() % 100 == 0){
        G4TrajectoryContainer* trajCont = event->GetTrajectoryContainer();
        G4int nTraj = 0;
        if (trajCont){
            nTraj = trajCont->entries();
            G4cout<<"   "<<nTraj<<" trajectories stored in this event"<<G4endl;
        }
        G4VHitsCollection *hc = event->GetHCofThisEvent()->GetHC(0);
        G4cout<<"    "<<hc->GetSize()<<" hits stored in this event"<<G4endl;
    }
    */
}
