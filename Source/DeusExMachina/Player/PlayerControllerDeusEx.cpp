#include "PlayerControllerDeusEx.h"
#include "CharacterBaseDeusEx.h"

#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputAction.h"
#include "InputActionValue.h"

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