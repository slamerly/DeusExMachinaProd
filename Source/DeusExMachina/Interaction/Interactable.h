#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractable : public UInterface
{
	GENERATED_BODY()
};


class DEUSEXMACHINA_API IInteractable
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION()
	virtual FVector GetInteractableTooltipPosition() = 0;

	UFUNCTION()
	virtual void Interaction() = 0;

	UFUNCTION()
	virtual bool CanInteract() = 0;

	UFUNCTION()
	virtual bool IsInteractionHeavy() = 0;

	UFUNCTION()
	virtual void InteractionHeavyUpdate(FVector2D ControlValue) = 0;

	UFUNCTION()
	virtual void InteractionHeavyFinished() = 0;
};
