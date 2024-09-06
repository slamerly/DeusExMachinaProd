#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"

#include "PlayerControllerInterface.h"
#include "PlayerControllerEnums.h"

#include "PlayerControllerDeusEx.generated.h"

class ACharacterBaseDeusEx;
class AHUDDeusEx;
class UInputMappingContext;
class UInputAction;


UCLASS()
class DEUSEXMACHINA_API APlayerControllerDeusEx : public APlayerController, public IPlayerControllerInterface
{
	GENERATED_BODY()

// ======================================================
//                Unreal overrides
// ======================================================
protected:
	virtual void BeginPlay() override;
	virtual void LateBeginPlay();
	virtual void Tick(float DeltaTime) override;


public:
	virtual void SetupInputComponent() override;
	virtual void SetPawn(APawn* pawn) override;



// ======================================================
//            Blueprint editable variables
// ======================================================
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

	UPROPERTY(EditDefaultsOnly, Category = "Inputs")
	UInputAction* InteractControlInputAction;


	// ====================
	//     Interaction
	// ====================
	UPROPERTY(EditDefaultsOnly, Category = "Interaction", meta = (Tooltip = "The distance of the interaction raycast.\nIn cm.", ClampMin = 0))
	int InteractionRaycastDistance{ 160 };



// ======================================================
//                     Functions
// ======================================================
protected:
	// ====================
	//       Inputs
	// ====================
	void Move(const struct FInputActionValue& value);
	void Look(const struct FInputActionValue& value);
	void Interact(const struct FInputActionValue& value);
	void InteractControl(const struct FInputActionValue& value);
	void InteractControlRelease(const struct FInputActionValue& value);
	void InteractRelease(const struct FInputActionValue& value);


	// ====================
	//     Interaction
	// ====================
	void InteractionRaycast();



// ======================================================
//               Specific variables
// ======================================================
protected:
	ACharacterBaseDeusEx* Character{ nullptr };

	AActor* CurrentInteractable{ nullptr };

	AHUDDeusEx* HudRef{ nullptr };


	// ====================
	//  Input mode, block & pause
	// ====================
	EPlayerInputMode PlayerInputMode{ EPlayerInputMode::PlayerMovement };
	
	bool bPlayerBlocked{ false };
	TSet<EBlockPlayerCause> PlayerBlockedCauses;

	bool bInPause{ false }; //  this value may not be useful after all, will check that when creating pause menu




// ======================================================
//             Player Controller Interface
// ======================================================
public:
	// ====================
	//     Input Mode
	// ====================
	void SetPlayerInputMode(EPlayerInputMode InputMode) override;
	EPlayerInputMode GetPlayerInputMode() override;

	// ====================
	//    Block Inputs
	// ====================
	void BlockPlayerInputs(EBlockPlayerCause Cause) override;
	void UnblockPlayerInputs(EBlockPlayerCause Cause) override;
	bool IsPlayerBlocked() override;

	// ====================
	//        Pause
	// ====================
	void SetInPause(bool bInPauseValue) override;
	bool GetInPause() override;
};
