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

	GameInstance = Cast<UGameInstanceDeusEx>(UGameplayStatics::GetGameInstance(this));

	if (!GameInstance)
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("The game instance isn't of type GameInstanceDeusEx."));
	}
	
	GameInstance->SetRefProgress(GetOwner()->GetLevel()->GetOuter()->GetName(), false);
	
}

void USaveProgressComponent::PuzzleDone()
{
	GameInstance->SetRefProgress(GetOwner()->GetLevel()->GetOuter()->GetName(), true);
}

