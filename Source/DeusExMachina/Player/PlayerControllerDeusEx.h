#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerControllerDeusEx.generated.h"

class ACharacterBaseDeusEx;
class IInteractable;
class UInputMappingContext;
class UInputAction;


UCLASS()
class DEUSEXMACHINA_API APlayerControllerDeusEx : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void LateBeginPlay();
	virtual void Tick(float DeltaTime) override;


protected:
	// ====================
	//       Inputs
	// ====================
	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* InteractInputAction;


	// ====================
	//     Interaction
	// ====================
	UPROPERTY(EditDefaultsOnly, Category = "Interaction", meta = (Tooltip = "The distance of the interaction raycast.\nIn cm.", ClampMin = 0))
	int InteractionRaycastDistance{ 160 };


protected:
	void Move(const struct FInputActionValue& value);
	void Look(const struct FInputActionValue& value);
	void Interact(const struct FInputActionValue& value);
	void InteractRelease(const struct FInputActionValue& value);

	void InteractionRaycast();


public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* pawn) override;


protected:
	ACharacterBaseDeusEx* Character{ nullptr };
	IInteractable* CurrentInteractable{ nullptr };
	bool bCurrentInteractableValid{ false };
};
