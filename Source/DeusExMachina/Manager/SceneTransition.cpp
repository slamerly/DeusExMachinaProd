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
}

ASceneTransition::~ASceneTransition()
{
}

void ASceneTransition::PostActorCreated()
{
	Super::PostActorCreated();
	// initialize the id number
	TArray<AActor*> OtherSceneTransition;
	ULevelUtilitiesFunctions::GetAllActorsOfClassInSublevel(this, GetLevel()->GetOuter(), GetClass(), OtherSceneTransition);
	IdSceneTransition = OtherSceneTransition.Num() - 1;
}

void ASceneTransition::BeginPlay()
{
	Super::BeginPlay();
	CollisionBox->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASceneTransition::OnComponentBeginOverlap);

	// find scene manager
	SceneManager = UGameplayStatics::GetActorOfClass(GetWorld(), ASceneManager::StaticClass());

	if (SceneManager == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("SceneManager not find."));
	}
}

/*
void ASceneTransition::GetAllLightsInSubLevel()
{
	GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("SceneManager not find."));
	if (SceneManager)
	{
		TArray<AActor*> LightsFund;
		ULevelUtilitiesFunctions::GetAllActorsOfClassInSublevel(this, GetLevel()->GetOuter(), ALight::StaticClass(), LightsFund);

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
*/

void ASceneTransition::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("Guy."));
	if (UGameplayStatics::GetPlayerPawn(this, 0) == OtherActor)
	{
		if (SceneManager)
		{
			Cast<ASceneManager>(SceneManager)->ChangeScene(TargetScene, TargetID, false, true);
		}
	}
}
