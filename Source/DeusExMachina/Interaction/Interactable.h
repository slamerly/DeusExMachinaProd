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

	UFUNCTION(BlueprintNativeEvent)
	FVector GetInteractableTooltipPosition();

	UFUNCTION(BlueprintNativeEvent)
	void Interaction();

	UFUNCTION(BlueprintNativeEvent)
	bool CanInteract();

	UFUNCTION(BlueprintNativeEvent)
	bool IsInteractionHeavy();

	UFUNCTION(BlueprintNativeEvent)
	void InteractionHeavyUpdate(FVector2D ControlValue);

	UFUNCTION(BlueprintNativeEvent)
	void InteractionHeavyFinished();

	UFUNCTION(BlueprintNativeEvent)
	bool IsInteractableBothSides();
};
