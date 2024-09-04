#include "CharacterBaseDeusEx.h"

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

void ACharacterBaseDeusEx::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}