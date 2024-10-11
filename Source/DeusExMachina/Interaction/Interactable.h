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

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FVector GetInteractableTooltipPosition();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interaction();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool CanInteract();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsInteractionHeavy();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractionHeavyUpdate(FVector2D ControlValue, bool KeyboardInput);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void InteractionHeavyFinished();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool IsInteractableBothSides();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AActor* GetInteractableFocusActor();
};
