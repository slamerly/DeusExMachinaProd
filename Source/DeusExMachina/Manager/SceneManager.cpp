// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneManager.h"
#include <Kismet/GameplayStatics.h>
#include "SceneTransition.h"
#include "Kismet/KismetMathLibrary.h" // Pour les interpolations
#include "UObject/ConstructorHelpers.h" // Pour charger les courbes
#include "Engine/LevelStreaming.h"
#include <DeusExMachina/BlueprintLibrary/LevelUtilitiesFunctions.h>
//#include <Engine/LevelStreamingDynamic.h>

// Sets default values
ASceneManager::ASceneManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimelineCurtains = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineCurtains"));
	TimelineLights = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineLights"));

}

// Called when the game starts or when spawned
void ASceneManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerCtrl = Cast<APlayerControllerDeusEx>(GetWorld()->GetFirstPlayerController());

	BeginPlayAnimation();

	if (!Scenes.IsEmpty())
	{
		FLatentActionInfo LatentInfo;
		UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), Scenes[0], true, true, LatentInfo);
		CurrentLevelIndex = 0;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Scenes list is empty in the SceneManager."));
	}

	if (FloatCurveCurtains == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no Float Curve for the curtains animation."));
	}

	CurtainsVerifications();

	//Delay
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			if (bFadeBeginPlay)
			{
				UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(1.0f, 0.0f, 2.0f, FLinearColor::Black, false, false);
			}
			//Delay
			FTimerDelegate TimerDelegate2;
			TimerDelegate2.BindLambda([&]
				{
					if (bBlockPlayerBeginPlay)
					{
						PlayerCtrl->UnblockPlayerInputs(EBlockPlayerCause::SceneTransition);
					}
				});

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate2, 0.5f, false);
			
		});

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.0f, false);
}

void ASceneManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ASceneManager, Scenes))
	{
		ScenesNames.Empty();
		for (int i = 0; i < Scenes.Num(); i++)
		{
			ScenesNames.Add(Scenes[i]->GetFName());
		}
	}
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
		CurtainsInitialPosition.Add(Curtains[i]->GetActorLocation());
	}
	UE_LOG(LogTemp, Log, TEXT("Number of curtains: %f"), CurtainsInitialPosition[0].Y);
}

void ASceneManager::GetSceneLights()
{
	TArray<AActor*> LightsFund;
	ULevelUtilitiesFunctions::GetAllActorsOfClassInSublevel(this, GetCurrentScene(), ALight::StaticClass(), LightsFund);

	Lights.Empty();
	LightsIntensity.Empty();

	UE_LOG(LogTemp, Log, TEXT("Number of light in the scene %d"), LightsFund.Num());

	for (int i = 0; i < LightsFund.Num(); i++)
	{
		ALight* TempLight = Cast<ALight>(LightsFund[i]);
		if (TempLight)
		{
			Lights.AddUnique(TempLight);
			LightsIntensity.AddUnique(Lights[i]->GetLightComponent()->Intensity);
		}
	}
}

void ASceneManager::LightsAnimation(bool IsOn)
{
	GetSceneLights();

	if (!Lights.IsEmpty())
	{
		// ==================
		//		Setup Timeline
		// ==================
		// Create the function to update link to the timeline
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("OnTimelineUpdateLights"));

		// Add the event to update
		TimelineLights->AddInterpFloat(FloatCurveCurtains, TimelineProgress);

		// Create the function of ending
		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("OnTimelineFinishedLights"));

		// Add the event of ending
		TimelineLights->SetTimelineFinishedFunc(TimelineFinished);

		TimelineLights->SetLooping(false);

		if (IsOn)
		{
			TimelineLights->PlayFromStart();
		}
		else
		{
			//Delay
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]
				{
					TimelineLights->ReverseFromEnd();
				});

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayToPutOnLights, false);
		}
	}
}

void ASceneManager::BeforeLevelChange(int pCurrentLevelIndex)
{
	//TODO - Saving Scene
	PlayerCtrl->BlockPlayerInputs(EBlockPlayerCause::SceneTransition);

	//Animations
	float CurtainsAnimationTime = 0, LightsAnimationTime = 0;
	if (bCurtainsAnimation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Curtain animation"));
		CurtainsAnimation(true);
		CurtainsAnimationTime = TimelineCurtains->GetTimelineLength() + (TimelineCurtains->GetTimelineLength() * (1 - TimelineCurtains->GetPlayRate()));
		DelayAnimations = CurtainsAnimationTime;
	}
	if (bLightsAnimation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Light animation"));
		LightsAnimation(true);
		LightsAnimationTime = TimelineLights->GetTimelineLength() + (TimelineLights->GetTimelineLength() * (1 - TimelineLights->GetPlayRate()));
		DelayAnimations = LightsAnimationTime;
	}
	if (bCurtainsAnimation && bLightsAnimation)
	{
		if (CurtainsAnimationTime > LightsAnimationTime)
			DelayAnimations = CurtainsAnimationTime;
		else
			DelayAnimations = LightsAnimationTime;
	}
	else if (!bCurtainsAnimation && !bLightsAnimation)
	{
		DelayAnimations = 0;
	}
}

void ASceneManager::AfterLevelChange(int IndexSaveSceneBefore, bool pWithLoad)
{
	
	if (pWithLoad)
	{
		LoadingScene();
	}

	//If the scene contains many transitions, select the good one
	TArray<AActor*> SceneTransitions;
	//UGameplayStatics::GetAllActorsOfClass(this, ASceneTransition::StaticClass(), SceneTransitions);
	ULevelUtilitiesFunctions::GetAllActorsOfClassInSublevel(this, Scenes[CurrentLevelIndex], ASceneTransition::StaticClass(), SceneTransitions);

	ASceneTransition* GoodTransition = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("Num: %d"), SceneTransitions.Num());
	for (int i = 0; i < SceneTransitions.Num(); i++)
	{
		ASceneTransition* CurrentSceneTransition = Cast<ASceneTransition>(SceneTransitions[i]);

		if (CurrentSceneTransition->GetTargetScene() == Scenes[IndexSaveSceneBefore] &&
			CurrentSceneTransition->GetTargetId() == TargetID)
		{
			if (CurrentSceneTransition->GetSpawnRomeo() != nullptr)
			{
				UGameplayStatics::GetPlayerPawn(this, 0)->SetActorLocation(CurrentSceneTransition->GetSpawnRomeo()->GetActorLocation());

				//Animations
				float CurtainsAnimationTime = 0, LightsAnimationTime = 0;				
				if (bCurtainsAnimation)
				{
					//UE_LOG(LogTemp, Warning, TEXT("After Loaded Curtain animation"));
					CurtainsAnimation(false);
					CurtainsAnimationTime = TimelineCurtains->GetTimelineLength() + (TimelineCurtains->GetTimelineLength() * (1 - TimelineCurtains->GetPlayRate()));
					DelayAnimations = CurtainsAnimationTime;
				}
				if (bLightsAnimation)
				{
					LightsAnimation(false);
					LightsAnimationTime = TimelineLights->GetTimelineLength() + (TimelineLights->GetTimelineLength() * (1 - TimelineLights->GetPlayRate()));
					DelayAnimations = LightsAnimationTime;
				}
				if (bCurtainsAnimation && bLightsAnimation)
				{
					if (CurtainsAnimationTime > LightsAnimationTime)
						DelayAnimations = CurtainsAnimationTime;
					else
						DelayAnimations = LightsAnimationTime;
				}
				else if(!bCurtainsAnimation && !bLightsAnimation)
				{
					DelayAnimations = 0;
				}
				return;
			}
			else
			{
				//Error
				GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red,
					FString::Printf(TEXT("SpawnRomeo is empty in %s"), *GoodTransition->GetName()));
				UE_LOG(LogTemp, Warning, TEXT("SpawnRomeo is empty in %s"), *GoodTransition->GetName());
			}
		}
	}
}

void ASceneManager::OnStreamLevelLoaded()
{
	UE_LOG(LogTemp, Warning, TEXT("Second finish"));
	//AfterLevelChange(RefSceneBefore, WithLoad);
	//Delay
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			AfterLevelChange(SaveIndexSceneBefore, WithLoad);

			//Narration
			if (FromNarrationScene)
			{
				PlayerCtrl->UnblockPlayerInputs(EBlockPlayerCause::Narration);
				PlayerCtrl->SetPlayerInputMode(EPlayerInputMode::PlayerMovement);
				UGameplayStatics::GetPlayerPawn(this, 0)->SetActorHiddenInGame(false);
			}

			//Delay
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindLambda([&]
				{
					PlayerCtrl->UnblockPlayerInputs(EBlockPlayerCause::SceneTransition);
				});
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayAnimations, false);
		});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, false);
}

void ASceneManager::OnStreamLevelUnloaded()
{
	CurrentLevelIndex = Scenes.Find(NextLevel);
	//Delay
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			//Load NextScene
			FLatentActionInfo LatentInfo;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NextLevel, true, true, LatentInfo);

			//Bind event when the level is loaded
			ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(this, ScenesNames[CurrentLevelIndex]);
			LevelStreaming->OnLevelLoaded.AddDynamic(this, &ASceneManager::OnStreamLevelLoaded);
		});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, false);
}

void ASceneManager::LoadingScene()
{
}

void ASceneManager::SavingScene()
{
}

void ASceneManager::BeginPlayAnimation_Implementation()
{
	// When we load the scene for the first time, we block the player inputand do a camera Fade
	if (bBlockPlayerBeginPlay)
	{
		PlayerCtrl->BlockPlayerInputs(EBlockPlayerCause::SceneTransition);
	}
	if (bFadeBeginPlay)
	{
		UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(1.0f, 1.0f, 1.0f, FLinearColor::Black, false, true);
	}
}

void ASceneManager::UpdateScenesNames()
{
	ScenesNames.Empty();
	for (int i = 0; i < Scenes.Num(); i++)
	{
		ScenesNames.Add(Scenes[i]->GetFName());
	}
}

void ASceneManager::OnTimelineUpdateCurtains(float Value)
{
	float FinalPos = CurtainsInitialPosition[0].Y + CurtainsFinalDistancePos;
	float NewLocationY = FMath::Lerp(CurtainsInitialPosition[0].Y, FinalPos, Value);
	Curtains[0]->SetActorLocation(FVector(Curtains[0]->GetActorLocation().X, NewLocationY, Curtains[0]->GetActorLocation().Z));

	FinalPos = CurtainsInitialPosition[1].Y - CurtainsFinalDistancePos;
	NewLocationY = FMath::Lerp(CurtainsInitialPosition[1].Y, FinalPos, Value);
	Curtains[1]->SetActorLocation(FVector(Curtains[1]->GetActorLocation().X, NewLocationY, Curtains[1]->GetActorLocation().Z));
}

void ASceneManager::OnTimelineFinishedCurtains()
{
}

void ASceneManager::OnTimelineUpdateLights(float Value)
{
	for (int i = 0; i < Lights.Num(); i++)
	{
		float NewIntensity = FMath::Lerp(LightsIntensity[i], .0f, Value);
		Lights[i]->GetLightComponent()->SetIntensity(NewIntensity);
	}
}

void ASceneManager::OnTimelineFinishedLights()
{
}

int ASceneManager::GetCurrentIndexScene()
{
	return CurrentLevelIndex;
}

TSoftObjectPtr<UWorld> ASceneManager::GetCurrentScene()
{
	return Scenes[CurrentLevelIndex];
}

void ASceneManager::CheckpointPlayerTransform(FTransform PlayerTransform)
{
}

FTransform ASceneManager::GetCheckpointPlayerTransform()
{
	return FTransform();
}

void ASceneManager::ChangeScene(const TSoftObjectPtr<UWorld>& pNextLevel, int pTargetID, bool pFromNarrationScene, bool pWithLoad)
{
	TargetID = pTargetID;
	if (Scenes.Contains(pNextLevel))
	{
		BeforeLevelChange(CurrentLevelIndex);
		DelayAnimations += 0.25f; // add more time to finish the animation before unload scene
		SaveIndexSceneBefore = CurrentLevelIndex;
		FromNarrationScene = pFromNarrationScene;
		WithLoad = pWithLoad;
		NextLevel = pNextLevel;

		//Delay
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]
			{
				//Unload Scene
				FLatentActionInfo LatentInfo;
				UGameplayStatics::UnloadStreamLevel(this, Scenes[CurrentLevelIndex]->GetFName(), LatentInfo, true);
				
				UE_LOG(LogTemp, Warning, TEXT("First finish"));
				ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, Scenes[CurrentLevelIndex]->GetFName());
				if (StreamingLevel)
				{
					// Bind to the OnLevelLoaded event
					StreamingLevel->OnLevelUnloaded.AddDynamic(this, &ASceneManager::OnStreamLevelUnloaded);
				}
			});
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayAnimations, false);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("Error: scene not contains in SceneManager's Scenes array."));
	}
}

void ASceneManager::CurtainsAnimation(bool IsOpen)
{
	if (FloatCurveCurtains)
	{
		// ==================
		//		Setup Timeline
		// ==================
		// Create the function to update link to the timeline
		FOnTimelineFloat TimelineProgress;
		TimelineProgress.BindUFunction(this, FName("OnTimelineUpdateCurtains"));

		// Add the event to update
		TimelineCurtains->AddInterpFloat(FloatCurveCurtains, TimelineProgress);

		// Create the function of ending
		FOnTimelineEvent TimelineFinished;
		TimelineFinished.BindUFunction(this, FName("OnTimelineFinishedCurtains"));

		// Add the event of ending
		TimelineCurtains->SetTimelineFinishedFunc(TimelineFinished);

		TimelineCurtains->SetLooping(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no Float Curve for the curtains animation."));
		return;
	}

	if (IsOpen)
	{
		TimelineCurtains->PlayFromStart();
	}
	else
	{
		TimelineCurtains->ReverseFromEnd();
	}
}

// Called every frame
void ASceneManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

