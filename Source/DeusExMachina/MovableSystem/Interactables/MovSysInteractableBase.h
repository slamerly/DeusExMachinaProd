#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovSysInteractableBase.generated.h"

UCLASS(Abstract)
class DEUSEXMACHINA_API AMovSysInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovSysInteractableBase();

protected:
	virtual void BeginPlay() override;



// ======================================================
//               Non-overridable Functions
// ======================================================
public:
	// ======================
	//  Disable Interactable
	// ======================
	void DisableInteractable();
	void EnableInteractable();
	bool IsInteractableDisabled();


// ======================================================
//                Overridable Functions
// ======================================================
public:
	virtual void ForceReleaseInteractable();


// ======================================================
//                      Variables
// ======================================================
protected:
	bool bDisabled{ false };
};
