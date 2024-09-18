#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RotationBehaviorBase.generated.h"

class ARotationSupport;


UCLASS( Abstract )
class DEUSEXMACHINA_API URotationBehaviorBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	URotationBehaviorBase();

	/** Called when a component is created (not loaded). This can happen in the editor or during gameplay */
	virtual void OnComponentCreated() override;


// ======================================================
//              Owner-related functions
// ======================================================
protected:
	bool InitializeOwner();


// ======================================================
//              Owner-related variables
// ======================================================
protected:
	ARotationSupport* OwnerRotSupport{ nullptr };
	bool bOwnerRotSupportValid{ false };
};
