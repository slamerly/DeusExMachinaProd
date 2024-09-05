#include "CharacterBaseDeusEx.h"
#include "Components/CapsuleComponent.h"

ACharacterBaseDeusEx::ACharacterBaseDeusEx()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACharacterBaseDeusEx::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACharacterBaseDeusEx::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// ======================================================
//     Move & Look from Third Person template for basic player movement
// ======================================================
void ACharacterBaseDeusEx::Move(FVector2D MoveValue)
{
	// find out which way is forward
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	// get forward vector
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	// get right vector 
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	// add movement 
	AddMovementInput(ForwardDirection, MoveValue.Y);
	AddMovementInput(RightDirection, MoveValue.X);
}

void ACharacterBaseDeusEx::LookCamera(FVector2D LookValue)
{
	// add yaw and pitch input to controller
	AddControllerYawInput(LookValue.X);
	AddControllerPitchInput(LookValue.Y);
}

void ACharacterBaseDeusEx::GetCapsuleSize(float& radius, float& halfHeight) const
{
	const UCapsuleComponent& CapsuleComp = *GetCapsuleComponent();
	radius = CapsuleComp.GetScaledCapsuleRadius();
	halfHeight = CapsuleComp.GetScaledCapsuleHalfHeight();
}
