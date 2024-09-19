// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveProgressComponent.h"
#include <Kismet/GameplayStatics.h>
#include "SaveProgress.h"


// Sets default values for this component's properties
USaveProgressComponent::USaveProgressComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void USaveProgressComponent::BeginPlay()
{
	Super::BeginPlay();

	GameInstance = Cast<UGameInstanceDeusEx>(UGameplayStatics::GetGameInstance(GetWorld()));

	if (!GameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("The game instance isn't of type GameInstanceDeusEx."));
	}

	//SaveProgress = Cast<USaveProgress>(GameInstance->GetSaveRef());
	//SaveProgress->GetScenesProgress().Add(GetOwner()->GetWorld(), false);
	//UGameplayStatics::SaveGameToSlot()
	GameInstance->SetSaveProgress(GetOwner()->GetWorld(), false);
	
}

void USaveProgressComponent::PuzzleDone()
{
	//SaveProgress->GetScenesProgress().Add(GetOwner()->GetWorld(), true);
	GameInstance->SetSaveProgress(GetOwner()->GetWorld(), true);
}

