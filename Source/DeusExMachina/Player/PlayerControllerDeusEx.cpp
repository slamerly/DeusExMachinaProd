#include "PlayerControllerDeusEx.h"
#include "CharacterBaseDeusEx.h"

#include "DeusExMachina/Interaction/Interactable.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

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
}


// ======================================================
//	                  Tick
// ======================================================
void APlayerControllerDeusEx::Tick(float DeltaTime)
{
	InteractionRaycast();
}


// ======================================================
//	        	Interaction (raycast)
// ======================================================
void APlayerControllerDeusEx::InteractionRaycast()
{
	CurrentInteractable = nullptr;
	bCurrentInteractableValid = false;

	if (!IsValid(Character)) return;

	const FVector RaycastStart = Character->GetActorLocation();
	const FVector RaycastEnd = RaycastStart + Character->GetActorForwardVector() * InteractionRaycastDistance;
	float CapsuleRadius, CapsuleHalfHeight;
	Character->GetCapsuleSize(CapsuleRadius, CapsuleHalfHeight);
	FHitResult OutInteraction;

	bool InteractionHit = UKismetSystemLibrary::CapsuleTraceSingle(
		Character, RaycastStart, RaycastEnd, CapsuleRadius, CapsuleHalfHeight,
		ETraceTypeQuery::TraceTypeQuery1, false, { Character },
		EDrawDebugTrace::ForOneFrame, OutInteraction, true
	);

	if (!InteractionHit)
	{
		return;
	}

	if (!OutInteraction.GetActor()->GetClass()->ImplementsInterface(UInteractable::StaticClass()))
	{
		return;
	}

	CurrentInteractable = Cast<IInteractable>(OutInteraction.GetActor());
	bCurrentInteractableValid = true;


	//  check if the found object can be interacted this frame
	if (!CurrentInteractable->CanInteract())
	{
		CurrentInteractable = nullptr;
		bCurrentInteractableValid = false;
	}
}


// ======================================================
//          Move & Look (look temporary)
// ======================================================
void APlayerControllerDeusEx::Move(const FInputActionValue& value)
{
	if (!IsValid(Character)) return;

	const FVector2D MoveValue = value.Get<FVector2D>();

	Character->Move(MoveValue);
}

void APlayerControllerDeusEx::Look(const FInputActionValue& value)
{
	if (!IsValid(Character)) return;

	const FVector2D LookValue = value.Get<FVector2D>();

	Character->LookCamera(LookValue);
}


// ======================================================
//             Interaction (inputs)
// ======================================================
void APlayerControllerDeusEx::Interact(const FInputActionValue& value)
{
	if (!bCurrentInteractableValid) return;

	CurrentInteractable->Interaction();
}

void APlayerControllerDeusEx::InteractRelease(const FInputActionValue& value)
{
	if (!bCurrentInteractableValid) return;

	if (!CurrentInteractable->IsInteractionHeavy()) return;

	CurrentInteractable->InteractionHeavyFinished();
}


// ======================================================
//                 Setup Inputs
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