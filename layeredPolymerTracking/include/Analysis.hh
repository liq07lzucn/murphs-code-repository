#ifndef Analysis_h
#define Analysis_h 1

#include "g4analysis_defs.hh"
#include "G4Event.hh"
#include "G4Run.hh"
#include "G4VHitsCollection.hh"

#include "TNtuple.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TFile.h"

#include "CaloHit.hh"

#include "G4String.hh"
#include "globals.hh"

// How do we want to save the results?
using namespace G4Root;

#define NUMLAYERS 1     // Number of layers in the detector
#define WORLDNUM 0      // Map between material and Number
#define GAPNUM 1
#define ABSNUM 2
#define NUMPID 5        // PID of 0, 1, 2,3 (placeholder,neutron,Triton,alpha)
class Analysis {

public:
    // Singleton
    static Analysis* GetInstance(){
       if(Analysis::singleton == NULL)
           Analysis::singleton = new Analysis();
       return Analysis::singleton;
    }

    virtual ~Analysis() {};

    void PrepareNewEvent(const G4Event* anEvent);
    void PrepareNewRun(const G4Run* aRun);
    void EndOfEvent(const G4Event* anEvent);
    void EndOfRun(const G4Run* aRun);

private:
    // Singleton Analysis
    Analysis();
    static Analysis *singleton;

    void ProcessHitCollection(G4VHitsCollection* hc, G4int eventID);
    
    // ROOT output objects
    TH1F* hEventTotEDepGap[NUMLAYERS+1];    /* Total Energy Deposited per Event */
    TH1F* hEventTotEDepAbs[NUMLAYERS+1];
    TH1F* hEventTotCalo;
    TH1F* hHitTotEDepGap[NUMLAYERS];        /* Total Energy Deposited per Hit */
    TH1F* hHitTotEDepAbs[NUMLAYERS]; 
    TH1F* hSecElecKinAbs[NUMLAYERS][NUMPID+1];/* Kinetic Energy of first secondary e- */
    
    TFile* outfile;

    // Accumulation Variables
   G4double eventEDepTot_Abs[NUMLAYERS+1];  /* Total Energy deposited in Abs */
   G4double eventEDepTot_Gap[NUMLAYERS+1];  /* Total Energy deposited in Gap */
};
#endif
