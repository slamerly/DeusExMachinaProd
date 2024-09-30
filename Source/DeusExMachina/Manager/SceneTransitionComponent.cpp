// Fill out your copyright notice in the Description page of Project Settings.

#include "SceneTransitionComponent.h"
#include "Components/BoxComponent.h"
#include "SceneManager.h"
#include <Kismet/GameplayStatics.h>
#include <DeusExMachina/BlueprintLibrary/LevelUtilitiesFunctions.h>

// Sets default values for this component's properties
USceneTransitionComponent::USceneTransitionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void USceneTransitionComponent::BeginPlay()
{
	Super::BeginPlay();

	// find scene collision box in owner
	CollisionBox = GetOwner()->FindComponentByClass<UBoxComponent>();
	if (CollisionBox != nullptr)
	{
		CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &USceneTransitionComponent::OnComponentBeginOverlap);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, FString::Printf(TEXT("No collision box for %s"), *GetOwner()->GetFName().ToString()));
	}

	// find scene manager
	SceneManager = UGameplayStatics::GetActorOfClass(GetWorld(), ASceneManager::StaticClass());

	if (SceneManager == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("SceneManager not found."));
	}
}

void USceneTransitionComponent::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UGameplayStatics::GetPlayerPawn(this, 0) == OtherActor)
	{
		if (SceneManager)
		{
			Cast<ASceneManager>(SceneManager)->ChangeScene(TargetScene, false);
		}
	}
}

TSoftObjectPtr<UWorld> USceneTransitionComponent::GetTargetScene()
{
	return TargetScene;
}

