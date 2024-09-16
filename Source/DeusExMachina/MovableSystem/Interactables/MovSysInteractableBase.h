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


public:
	void DisableInteractable();
	void EnableInteractable();
	bool IsInteractableDisabled();

	virtual void ForceReleaseInteractable();

	
protected:
	bool bDisabled{ false };
};
