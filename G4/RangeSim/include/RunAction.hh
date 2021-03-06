#ifndef RunAction_h
#define RunAction_h 1

#include "G4UserRunAction.hh"
#include "globals.hh"
#include <map>

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class G4Run;
class DetectorConstruction;
class PrimaryGeneratorAction;
class HistoManager;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class RunAction : public G4UserRunAction
{
  public:
    RunAction(DetectorConstruction*, PrimaryGeneratorAction*, HistoManager*);
   ~RunAction();

  public:
    void BeginOfRunAction(const G4Run*);
    void   EndOfRunAction(const G4Run*);
    
    void CountTraks0(G4int nt) { NbOfTraks0 += nt;}
    void CountTraks1(G4int nt) { NbOfTraks1 += nt;}
    void CountSteps0(G4int ns) { NbOfSteps0 += ns;}
    void CountSteps1(G4int ns) { NbOfSteps1 += ns;}
    void CountProcesses(G4String procName) { procCounter[procName]++;};
    
    void AddEdep(G4double val)     { edep += val;}
    void AddTrueRange (G4double l) { trueRange += l; trueRange2 += l*l;};
    void AddProjRange (G4double x) { projRange += x; projRange2 += x*x;};
    void AddTransvDev (G4double y) { transvDev += y; transvDev2 += y*y;};    
                                  
  private:
    DetectorConstruction*   detector;
    PrimaryGeneratorAction* primary;
    HistoManager*           histoManager;
      
    G4int           NbOfTraks0, NbOfTraks1;
    G4int           NbOfSteps0, NbOfSteps1;
    G4double        edep;
    G4double        trueRange, trueRange2;             
    G4double        projRange, projRange2;
    G4double        transvDev, transvDev2;
    std::map<G4String,G4int>   procCounter;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

