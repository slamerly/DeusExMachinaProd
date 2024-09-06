#include "PlayerControllerDeusEx.h"
#include "CharacterBaseDeusEx.h"

#include "DeusExMachina/Interaction/Interactable.h"
#include "DeusExMachina/HUDDeusEx.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include <Defines.h>


// ======================================================
//          Begin Play & Late Begin Play
// ======================================================
void APlayerControllerDeusEx::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &APlayerControllerDeusEx::LateBeginPlay);
}

void APlayerControllerDeusEx::LateBeginPlay()
{
	HudRef = Cast<AHUDDeusEx>(GetHUD());
	if (!IsValid(HudRef))
	{
		kPRINT_ERROR("Player controller could not retrieve Deus Ex Hud class!");
	}
}


// ======================================================
//	                  Tick
// ======================================================
void APlayerControllerDeusEx::Tick(float DeltaTime)
{
	bool LastInteractableValid = IsValid(CurrentInteractable);
	InteractionRaycast();
	if (LastInteractableValid && !IsValid(CurrentInteractable))
	{
		HudRef->HideInteractionTooltip();
	}
}


// ======================================================
//	        	Interaction (raycast)
// ======================================================
void APlayerControllerDeusEx::InteractionRaycast()
{
	CurrentInteractable = nullptr;

	if (!IsValid(Character)) return;

	const FVector RaycastStart = Character->GetActorLocation();
	const FVector RaycastEnd = RaycastStart + Character->GetActorForwardVector() * InteractionRaycastDistance;
	float CapsuleRadius, CapsuleHalfHeight;
	Character->GetCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
	FHitResult OutInteraction;

	bool InteractionHit = UKismetSystemLibrary::CapsuleTraceSingle(
		Character, RaycastStart, RaycastEnd, CapsuleRadius, CapsuleHalfHeight,
		ETraceTypeQuery::TraceTypeQuery1, false, { Character },
		EDrawDebugTrace::None, OutInteraction, true
	);

	if (!InteractionHit)
	{
		return; //  return if the raycast didn't found any object
	}

	if (!OutInteraction.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return; //  return if the found object doesn't implement the Interactable interface
	}

	if (!IInteractable::Execute_CanInteract(OutInteraction.GetActor()))
	{
		return; //  return if the found interactable object can't interact this frame
	}

	CurrentInteractable = OutInteraction.GetActor();

	const FVector TooltipWorldPos = IInteractable::Execute_GetInteractableTooltipPosition(CurrentInteractable);
	FVector2D TooltipScreenPos;
	const bool TooltipInScreen = UGameplayStatics::ProjectWorldToScreen(this, TooltipWorldPos, TooltipScreenPos, true);
	if (TooltipInScreen)
	{
		const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		HudRef->ShowInteractionTooltip(TooltipScreenPos / ViewportSize);
	}
}


// ======================================================
//           Move & Look (look temporary)
// ======================================================
void APlayerControllerDeusEx::Move(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (PlayerInputMode != EPlayerInputMode::PlayerMovement) return;

	if (!IsValid(Character)) return;

	//  actual function
	const FVector2D MoveValue = value.Get<FVector2D>();
	Character->Move(MoveValue);
}

void APlayerControllerDeusEx::Look(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (!IsValid(Character)) return;

	//  actual function
	const FVector2D LookValue = value.Get<FVector2D>();
	Character->LookCamera(LookValue);
}


// ======================================================
//                 Interaction (input)
// ======================================================
void APlayerControllerDeusEx::Interact(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (PlayerInputMode != EPlayerInputMode::PlayerMovement) return;

	if (!IsValid(CurrentInteractable)) return;

	//  actual function
	IInteractable::Execute_Interaction(CurrentInteractable);

	if (IInteractable::Execute_IsInteractionHeavy(CurrentInteractable))
	{
		SetPlayerInputMode(EPlayerInputMode::InteractionHeavy);
	}
}

void APlayerControllerDeusEx::InteractRelease(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (PlayerInputMode != EPlayerInputMode::InteractionHeavy) return;

	if (!IsValid(CurrentInteractable)) return;

	if (!IInteractable::Execute_IsInteractionHeavy(CurrentInteractable)) return;

	//  actual function
	IInteractable::Execute_InteractionHeavyFinished(CurrentInteractable);
	SetPlayerInputMode(EPlayerInputMode::PlayerMovement);
}


// ======================================================
//             Interaction control (input)
// ======================================================
void APlayerControllerDeusEx::InteractControl(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (PlayerInputMode != EPlayerInputMode::InteractionHeavy) return;

	if (!IsValid(CurrentInteractable)) return;

	if (!IInteractable::Execute_IsInteractionHeavy(CurrentInteractable)) return;

	//  actual function
	const FVector2D ControlValue = value.Get<FVector2D>();
	IInteractable::Execute_InteractionHeavyUpdate(CurrentInteractable, ControlValue);
}

void APlayerControllerDeusEx::InteractControlRelease(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (PlayerInputMode != EPlayerInputMode::InteractionHeavy) return;

	if (!IsValid(CurrentInteractable)) return;

	if (!IInteractable::Execute_IsInteractionHeavy(CurrentInteractable)) return;

	//  actual function
	IInteractable::Execute_InteractionHeavyUpdate(CurrentInteractable, FVector2D::ZeroVector);
}


// ======================================================
//                  Setup Inputs
// ======================================================
void APlayerControllerDeusEx::SetupInputComponent()
{
	Super::SetupInputComponent();

	UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	subsystem->ClearAllMappings();
	subsystem->AddMappingContext(InputMapping, 0);

	UEnhancedInputComponent* enhanced_input = Cast<UEnhancedInputComponent>(InputComponent);

	enhanced_input->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &APlayerControllerDeusEx::Move);
	enhanced_input->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &APlayerControllerDeusEx::Look);

	enhanced_input->BindAction(InteractInputAction, ETriggerEvent::Started, this, &APlayerControllerDeusEx::Interact);
	enhanced_input->BindAction(InteractInputAction, ETriggerEvent::Completed, this, &APlayerControllerDeusEx::InteractRelease);
	enhanced_input->BindAction(InteractInputAction, ETriggerEvent::Canceled, this, &APlayerControllerDeusEx::InteractRelease);

	enhanced_input->BindAction(InteractControlInputAction, ETriggerEvent::Triggered, this, &APlayerControllerDeusEx::InteractControl);
	enhanced_input->BindAction(InteractControlInputAction, ETriggerEvent::Completed, this, &APlayerControllerDeusEx::InteractControlRelease);
	enhanced_input->BindAction(InteractControlInputAction, ETriggerEvent::Canceled, this, &APlayerControllerDeusEx::InteractControlRelease);
}


// ======================================================
//                Set Pawn
// ======================================================
void APlayerControllerDeusEx::SetPawn(APawn* pawn)
{
	Super::SetPawn(pawn);

	Character = Cast<ACharacterBaseDeusEx>(pawn);

	if (!IsValid(Character))
	{
		return;
	}
}




// ======================================================
//        Input Mode (Player Controller Interface)
// ======================================================
void APlayerControllerDeusEx::SetPlayerInputMode(EPlayerInputMode InputMode)
{
	PlayerInputMode = InputMode;
}

EPlayerInputMode APlayerControllerDeusEx::GetPlayerInputMode()
{
	return PlayerInputMode;
}


// ======================================================
//       Block Inputs (Player Controller Interface)
// ======================================================
void APlayerControllerDeusEx::BlockPlayerInputs(EBlockPlayerCause Cause)
{
	PlayerBlockedCauses.Add(Cause);

	if (!PlayerBlockedCauses.IsEmpty())
	{
		bPlayerBlocked = true;
	}
}

void APlayerControllerDeusEx::UnblockPlayerInputs(EBlockPlayerCause Cause)
{
	PlayerBlockedCauses.Remove(Cause);

	if (PlayerBlockedCauses.IsEmpty())
	{
		bPlayerBlocked = false;
	}
}

bool APlayerControllerDeusEx::IsPlayerBlocked()
{
	return bPlayerBlocked;
}


// ======================================================
//         Pause (Player Controller Interface)
// ======================================================
void APlayerControllerDeusEx::SetInPause(bool bInPauseValue)
{
	bInPause = bInPauseValue;
}

bool APlayerControllerDeusEx::GetInPause()
{
	return bInPause;
}
