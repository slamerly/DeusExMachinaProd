// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneTransition.h"
#include <Kismet/GameplayStatics.h>
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include <DeusExMachina/BlueprintLibrary/LevelUtilitiesFunctions.h>

ASceneTransition::ASceneTransition(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;

	// create collision box
	CollisionBox = ObjectInitializer.CreateOptionalDefaultSubobject<UBoxComponent>(this, TEXT("CollisionBox"));
	if (CollisionBox)
	{
		SetRootComponent(CollisionBox);
	}

	// find scene manager
	SceneManager = UGameplayStatics::GetActorOfClass(this, ASceneManager::StaticClass());

	if (!SceneManager)
	{
		//UE_LOG(LogTemp, Log, TEXT("Pas SM: %s"), SceneManagerClass->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("SceneManager not find."));
	}

	// initialize the id number
	TArray<AActor*> OtherSceneTransition;
	UGameplayStatics::GetAllActorsOfClass(this, GetClass(), OtherSceneTransition);

	IdSceneTransition = OtherSceneTransition.Num() - 1;

	/*
	if (OtherSceneTransition.IsEmpty())
	{
		UE_LOG(LogTemp, Log, TEXT("IsEmpty"));
	}
	*/
}

ASceneTransition::~ASceneTransition()
{
}

void ASceneTransition::BeginPlay()
{
}

void ASceneTransition::GetAllLightsInSubLevel()
{
	GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("SceneManager not find."));
	UE_LOG(LogTemp, Log, TEXT("Doesn't work"));
	if (SceneManager)
	{
		UE_LOG(LogTemp, Log, TEXT("work"));
		TSubclassOf<ALight> LightClass;
		TArray<AActor*> LightsFund;
		ULevelUtilitiesFunctions::GetAllActorsOfClassInSublevel(this, Cast<ASceneManager>(SceneManager)->GetCurrentScene(), LightClass, LightsFund);

		UE_LOG(LogTemp, Log, TEXT("Number of light in the scene %d"), LightsFund.Num());

		for (int i = 0; i < LightsFund.Num(); i++)
		{
			ALight* TempLight = Cast<ALight>(LightsFund[i]);
			if (TempLight)
			{
				Lights.AddUnique(TempLight);
			}
		}
	}
}

void ASceneTransition::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (UGameplayStatics::GetPlayerPawn(this, 0) == OtherActor)
	{
		/*
		if (SceneManager->GetClass()->ImplementsInterface(USceneManagerInterface::StaticClass()))
		{
			//ISceneManagerInterface::ChangeScene(TargetScene, TargetID, false, true);
		}
		*/
		if (SceneManager)
		{
			Cast<ASceneManager>(SceneManager)->ChangeScene(TargetScene, TargetID, false, true);
		}
	}
}
