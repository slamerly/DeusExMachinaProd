// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveProgressComponent.h"
#include <Kismet/GameplayStatics.h>
#include "SaveProgress.h"


// Sets default values for this component's properties
USaveProgressComponent::USaveProgressComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USaveProgressComponent::BeginPlay()
{
	Super::BeginPlay();

	// Search the game instance and verrify the type
	GameInstance = Cast<UGameInstanceDeusEx>(UGameplayStatics::GetGameInstance(this));

	if (!GameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("The game instance isn't of type GameInstanceDeusEx."));
	}
	
	// First save to say the player is safely arrived in the scene but not complete the puzzle.
	GameInstance->SetSaveProgress(GetOwner()->GetLevel()->GetOuter()->GetName(), false);
	
}

void USaveProgressComponent::PuzzleDone()
{
	// Save call when the puzzle is done.
	GameInstance->SetSaveProgress(GetOwner()->GetLevel()->GetOuter()->GetName(), true);
}

