#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4VSolid.hh"
#include "CaloSensitiveDetector.hh"

class G4Box;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;
class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction{

	public:
		DetectorConstruction();
		virtual ~DetectorConstruction();

		virtual G4VPhysicalVolume* Construct();

		void PrintCaloParameters();

		G4double GetWorldSizeXY()			{return worldSizeXY;};
		G4double GetWorldSizeZ()			{return worldSizeZ;};
		G4double GetCaloThickness()		    {return caloThickness;};
		G4double GetCaloRadius()			{return oRadius;};

		G4Material* GetAbsorberMaterial()		{return absMaterial;};
		G4Material* GetGapMaterial()			{return gapMaterial;};
		
        G4double    GetAbsorberThickness()		{return absThickness;};
		G4double	GetGapThickness()			{return gapThickness;};

		const G4VPhysicalVolume* GetPhysicalWorld()		{return worldPV;};

		void SetAbsorberMaterial(G4String);
		void SetAbsorberThickness(G4double);

		void SetGapMaterial(G4String);
		void SetGapThickness(G4double);

		void SetCaloRadius(G4double);

		void UpdateGeometry();
	private:
		//! Define needed materials
		void DefineMaterials();
		//! Initialzie geometry parametners
		void ComputeParameters();
		//! Creates the Calorimeter (detector)
		G4VPhysicalVolume* ConstructCalorimeter();
		//! Sets the visualtiation attributes
		void SetVisAttributes();
		//! Sets the Sensitve Detectors
		void SetSensitiveDetectors();

		// Geometry Names
		G4LogicalVolume* worldLV;			// Pointer to world logical volume
		G4VPhysicalVolume* worldPV;		// Pointer to world physical volume
		G4VSolid* worldS;						// Pointer to world's solid

        G4LogicalVolume* absLV;				// Absorber Volume
		G4VPhysicalVolume* absPV;			
		G4VSolid*	absS;

        G4LogicalVolume* gapRLV;			// Gap Volume (right)
		G4VPhysicalVolume* gapRPV;			
		G4VSolid*	gapRS;

        G4LogicalVolume* gapLLV;			// Gap Volume (left)
		G4VPhysicalVolume* gapLPV;			
		G4VSolid*	gapLS;

		// Materials
		G4Material* defaultMaterial;    	// Vacumun
		G4Material* absMaterial;   		// Detector material
		G4Material* gapMaterial;        	// Gap material

		// Geometry parameters
        G4double absThickness;	      	// Thickness of Absorber
		G4double gapThickness;        	// Thickness of Gap 
		G4double oRadius;		    			// Outer Radius of Detector
		G4double iRadius;						// Inner radious of  Detector
		G4double startAngle;
		G4double spanAngle;

		G4double caloThickness;         // Thickness of entire calorimeter
		G4double worldSizeXY;
		G4double worldSizeZ;

		// Sensitive Detectors
		CaloSensitiveDetector* absSD;
		CaloSensitiveDetector* gapSD;

		G4bool  fCheckOverlaps; // option to activate checking of volumes overlaps
		DetectorMessenger* detectorMessenger;		// Pointer to the messenger
};
#endif

