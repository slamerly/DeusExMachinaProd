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
	Super::Tick(DeltaTime);

	if (PlayerInputMode != EPlayerInputMode::PlayerMovement) return;

	bool bLastInteractableValid = IsValid(CurrentInteractable);
	CurrentInteractable = InteractionRaycast();
	if (bLastInteractableValid && !IsValid(CurrentInteractable))
	{
		HudRef->HideInteractionTooltip();
	}
}


// ======================================================
//	        	Interaction (raycast)
// ======================================================
AActor* APlayerControllerDeusEx::InteractionRaycast()
{
	AActor* InteractableFound = nullptr;

	if (!IsValid(Character)) return InteractableFound;

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
		return InteractableFound; //  return if the raycast didn't found any object
	}

	if (!OutInteraction.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return InteractableFound; //  return if the found object doesn't implement the Interactable interface
	}

	if (!IInteractable::Execute_CanInteract(OutInteraction.GetActor()))
	{
		return InteractableFound; //  return if the found interactable object can't interact this frame
	}

	const float ForwardDot = FVector::DotProduct(Character->GetActorForwardVector(), -OutInteraction.GetActor()->GetActorForwardVector()); //  1 in the player is in front of the interactable, -1 if he's in the back

	if (FMath::Abs(ForwardDot) < 0.7f) return InteractableFound; //  return if the player is on the side (neither in front or in the back)

	if (!IInteractable::Execute_IsInteractableBothSides(OutInteraction.GetActor()) && ForwardDot < 0.7f)
	{
		return InteractableFound; //  return if the found interactable object can't be interacted from behind and if the player is not in front of it.
	}

	InteractableFound = OutInteraction.GetActor();

	const FVector TooltipWorldPos = IInteractable::Execute_GetInteractableTooltipPosition(InteractableFound);
	FVector2D TooltipScreenPos;
	const bool TooltipInScreen = UGameplayStatics::ProjectWorldToScreen(this, TooltipWorldPos, TooltipScreenPos, true);
	if (TooltipInScreen)
	{
		const FVector2D ViewportSize = GEngine->GameViewport->Viewport->GetSizeXY();
		HudRef->ShowInteractionTooltip(TooltipScreenPos / ViewportSize);
	}

	return InteractableFound;
}

AActor* APlayerControllerDeusEx::GetCurrentInteractable(EInteractableValid& InteractableValid)
{
	if (IsValid(CurrentInteractable))
	{
		InteractableValid = EInteractableValid::Valid;
		return CurrentInteractable;
	}
	else
	{
		InteractableValid = EInteractableValid::Invalid;
		return nullptr;
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

	//  broadcast OnPlayerStartInteraction event
	OnPlayerStartInteraction.Broadcast(CurrentInteractable);
}

void APlayerControllerDeusEx::InteractRelease(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (PlayerInputMode != EPlayerInputMode::InteractionHeavy) return;

	if (!IsValid(CurrentInteractable))
	{
		SetPlayerInputMode(EPlayerInputMode::PlayerMovement); //  security
		return;
	}

	if (!IInteractable::Execute_IsInteractionHeavy(CurrentInteractable)) return;

	//  actual function
	IInteractable::Execute_InteractionHeavyFinished(CurrentInteractable);
	SetPlayerInputMode(EPlayerInputMode::PlayerMovement);

	//  broadcast OnPlayerFinishInteraction event
	OnPlayerFinishInteraction.Broadcast(CurrentInteractable);
}


// ======================================================
//             Interaction control (input)
// ======================================================
void APlayerControllerDeusEx::InteractControlController(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (PlayerInputMode != EPlayerInputMode::InteractionHeavy) return;

	if (!IsValid(CurrentInteractable)) return;

	if (!IInteractable::Execute_IsInteractionHeavy(CurrentInteractable)) return;

	//  actual function
	const FVector2D ControlValue = value.Get<FVector2D>();
	IInteractable::Execute_InteractionHeavyUpdate(CurrentInteractable, ControlValue, false);
}

void APlayerControllerDeusEx::InteractControlKeyboard(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (PlayerInputMode != EPlayerInputMode::InteractionHeavy) return;

	if (!IsValid(CurrentInteractable)) return;

	if (!IInteractable::Execute_IsInteractionHeavy(CurrentInteractable)) return;

	//  actual function
	const FVector2D ControlValue = value.Get<FVector2D>();
	IInteractable::Execute_InteractionHeavyUpdate(CurrentInteractable, ControlValue, true);
}

void APlayerControllerDeusEx::InteractControlRelease(const FInputActionValue& value)
{
	//  checks
	if (bPlayerBlocked) return;

	if (PlayerInputMode != EPlayerInputMode::InteractionHeavy) return;

	if (!IsValid(CurrentInteractable)) return;

	if (!IInteractable::Execute_IsInteractionHeavy(CurrentInteractable)) return;

	//  actual function
	IInteractable::Execute_InteractionHeavyUpdate(CurrentInteractable, FVector2D::ZeroVector, false);
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

	enhanced_input->BindAction(InteractControlInputActionController, ETriggerEvent::Triggered, this, &APlayerControllerDeusEx::InteractControlController);
	enhanced_input->BindAction(InteractControlInputActionController, ETriggerEvent::Completed, this, &APlayerControllerDeusEx::InteractControlRelease);
	enhanced_input->BindAction(InteractControlInputActionController, ETriggerEvent::Canceled, this, &APlayerControllerDeusEx::InteractControlRelease);

	enhanced_input->BindAction(InteractControlInputActionKeyboard, ETriggerEvent::Triggered, this, &APlayerControllerDeusEx::InteractControlKeyboard);
	enhanced_input->BindAction(InteractControlInputActionKeyboard, ETriggerEvent::Completed, this, &APlayerControllerDeusEx::InteractControlRelease);
	enhanced_input->BindAction(InteractControlInputActionKeyboard, ETriggerEvent::Canceled, this, &APlayerControllerDeusEx::InteractControlRelease);
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


// ======================================================
//      Inputs Settings (Player Controller Interface)
// ======================================================
float APlayerControllerDeusEx::GetControlDirectionDelay()
{
	return ControlDirectionDelay;
}
