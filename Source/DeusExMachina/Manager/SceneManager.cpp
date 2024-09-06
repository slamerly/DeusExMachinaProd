// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneManager.h"
#include <Kismet/GameplayStatics.h>
#include "SceneTransition.h"

// Sets default values
ASceneManager::ASceneManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//PlayerCtrl = Cast<APlayerControllerDeusEx>(GetWorld()->GetFirstPlayerController());

}

// Called when the game starts or when spawned
void ASceneManager::BeginPlay()
{
	Super::BeginPlay();

	CurtainsVerifications();

	
}

void ASceneManager::CurtainsVerifications()
{
	if (!Curtains.IsEmpty())
	{
		float LocationCurtain = Curtains[0]->GetActorLocation().Y;
		if (LocationCurtain > SceneCenter->GetActorLocation().Y)
		{
			Algo::Reverse(Curtains);
		}

		InitializeCurtains();
	}
	else
	{
		//UE_LOG(LogTemp, Log, TEXT("No curtains selected in SceneManager"));
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("No curtains selected in SceneManager"));
	}
}

void ASceneManager::InitializeCurtains()
{
	//GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("%d"), Curtains.Num());
	for (int i = 0; i < Curtains.Num(); i++)
	{
		CurtainsInitialPositions.Add(Curtains[i]->GetActorLocation());
	}
	UE_LOG(LogTemp, Log, TEXT("Number of curtains: %f"), CurtainsInitialPositions[0].Y);
}

void ASceneManager::BeforeLevelChange(int pCurrentLevelIndex)
{
	PlayerCtrl->BlockPlayerInputs(EBlockPlayerCause::SceneTransition);
	//Ligth part
	CurtainsAnimation(true);

}

void ASceneManager::AfterLevelChange(int SaveCurrentLevelIndex, bool WithLoad)
{
	if (WithLoad)
	{
		LoadingScene();
	}
	TArray<SceneTransition> SceneTransitions;
	//UGameplayStatics::GetAllActorsOfClass(this, SceneTransitions);
}

void ASceneManager::LoadingScene()
{
}

void ASceneManager::SavingScene()
{
}

int ASceneManager::GetCurrentIndexScene()
{
	return 0;
}

void ASceneManager::CheckpointPlayerTransform(FTransform PlayerTransform)
{
}

FTransform ASceneManager::GetCheckpointPlayerTransform()
{
	return FTransform();
}

void ASceneManager::ChangeScene(const TSoftObjectPtr<UWorld>& NextLevel, int pTargetID, bool FromNarrationScene, bool WithLoad)
{
	TargetID = pTargetID;
	if (Scenes.Contains(NextLevel))
	{
		BeforeLevelChange(CurrentLevelIndex);
		//Delay

		ComeFromSceneIndex = CurrentLevelIndex;

		//Unload Scene
		FLatentActionInfo LatentInfo;
		UGameplayStatics::UnloadStreamLevel(this, Scenes[CurrentLevelIndex]->GetFName(), LatentInfo, true);

		CurrentLevelIndex = Scenes.Find(NextLevel);

		UGameplayStatics::LoadStreamLevel(this, NextLevel->GetFName(), true, true, LatentInfo);

		AfterLevelChange(ComeFromSceneIndex, WithLoad);

		if (FromNarrationScene)
		{
			PlayerCtrl->UnblockPlayerInputs(EBlockPlayerCause::Narration);
			PlayerCtrl->SetPlayerInputMode(EPlayerInputMode::PlayerMovement);
			UGameplayStatics::GetPlayerPawn(this, 0)->SetActorHiddenInGame(false);	
		}

		//Delay
		PlayerCtrl->UnblockPlayerInputs(EBlockPlayerCause::SceneTransition);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("Net scene not contains in SceneManager's Scenes array."));
	}
}

void ASceneManager::CurtainsAnimation(bool IsOpen)
{
}

// Called every frame
void ASceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

