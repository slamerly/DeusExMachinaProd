// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneManager.h"
#include <Kismet/GameplayStatics.h>
#include "SceneTransition.h"
#include "Kismet/KismetMathLibrary.h" // Pour les interpolations
#include "UObject/ConstructorHelpers.h" // Pour charger les courbes
#include "Engine/LevelStreaming.h"
#include <DeusExMachina/BlueprintLibrary/LevelUtilitiesFunctions.h>
#include <Blueprint/WidgetLayoutLibrary.h>

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
		CurrentSceneIndex = 0;
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

	TimelineCurtains->SetPlayRate(CurtainsPlayRateAnimation);
	TimelineLights->SetPlayRate(LightsPlayRateAnimation);

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
					PlayerCtrl->UnblockPlayerInputs(EBlockPlayerCause::SceneTransition);
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

	// Update the ScenesNames when change Scenes
	if (PropertyChangedEvent.GetPropertyName() == GET_MEMBER_NAME_CHECKED(ASceneManager, Scenes))
	{
		ScenesNames.Empty();
		for (int i = 0; i < Scenes.Num(); i++)
		{
			if(Scenes[i] != nullptr)
				ScenesNames.Add(Scenes[i]->GetFName());
		}
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

void ASceneManager::Animations(bool CurtainsOpen, bool LightsAreOn)
{
	float CurtainsAnimationTime = 0, LightsAnimationTime = 0;
	if (bCurtainsAnimation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Curtain animation"));
		CurtainsAnimation(CurtainsOpen);
		CurtainsAnimationTime = TimelineCurtains->GetTimelineLength() + (TimelineCurtains->GetTimelineLength() * (1 - TimelineCurtains->GetPlayRate()));
		DelayAnimations = CurtainsAnimationTime;
	}
	if (bLightsAnimation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Light animation"));
		LightsAnimation(LightsAreOn);
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

void ASceneManager::BeginPlayAnimation_Implementation()
{
	// When we load the scene for the first time, we block the player inputand do a camera Fade
	PlayerCtrl->BlockPlayerInputs(EBlockPlayerCause::SceneTransition);
	if (bFadeBeginPlay)
	{
		UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(1.0f, 1.0f, 1.0f, FLinearColor::Black, false, true);
	}
}

// ======================================================
//             Curtains Functions
// ======================================================

// Verify if there are 2 curtains and put the right order for te correct animation
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

// Save the curtains positions for the animations
void ASceneManager::InitializeCurtains()
{
	//GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("%d"), Curtains.Num());
	for (int i = 0; i < Curtains.Num(); i++)
	{
		CurtainsInitialPosition.Add(Curtains[i]->GetActorLocation());
	}
	UE_LOG(LogTemp, Log, TEXT("Number of curtains: %f"), CurtainsInitialPosition[0].Y);
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

// ======================================================
//             Lights Functions
// ======================================================

// Take all the lights in the scene
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

// ======================================================
//             Change Scene Fucntions
// ======================================================

void ASceneManager::BeforeSceneChange(int pCurrentSceneIndex)
{
	//TODO - Saving Scene
	PlayerCtrl->BlockPlayerInputs(EBlockPlayerCause::SceneTransition);

	//Animations
	Animations(true, true);
}

void ASceneManager::AfterSceneChange(int IndexSaveSceneBefore, bool pWithLoad)
{
	
	if (pWithLoad)
	{
		LoadingScene();
	}

	//If the scene contains many transitions, select the good one
	TArray<AActor*> SceneTransitions;
	ULevelUtilitiesFunctions::GetAllActorsOfClassInSublevel(this, Scenes[CurrentSceneIndex], ASceneTransition::StaticClass(), SceneTransitions);

	ASceneTransition* GoodTransition = nullptr;

	UE_LOG(LogTemp, Warning, TEXT("Num: %d"), SceneTransitions.Num());
	for (int i = 0; i < SceneTransitions.Num(); i++)
	{
		ASceneTransition* CurrentSceneTransition = Cast<ASceneTransition>(SceneTransitions[i]);

		if (CurrentSceneTransition->GetTargetScene() == Scenes[IndexSaveSceneBefore] &&
			CurrentSceneTransition->GetIdSceneTransition() == TargetID)
		{
			if (CurrentSceneTransition->GetSpawnRomeo() != nullptr)
			{
				UGameplayStatics::GetPlayerPawn(this, 0)->SetActorLocation(CurrentSceneTransition->GetSpawnRomeo()->GetActorLocation());

				//Animations
				Animations(false, false);
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

// Event function streamlevel loaded
void ASceneManager::OnStreamSceneLoaded()
{
	//AfterLevelChange(RefSceneBefore, WithLoad);
	//Delay
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			AfterSceneChange(SaveIndexSceneBefore, WithLoad);

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

// Event function streamlevel unloaded
void ASceneManager::OnStreamSceneUnloaded()
{
	CurrentSceneIndex = Scenes.Find(NextScene);
	//Delay
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]
		{
			//Load NextScene
			FLatentActionInfo LatentInfo;
			UGameplayStatics::LoadStreamLevelBySoftObjectPtr(this, NextScene, true, true, LatentInfo);

			//Bind event when the level is loaded
			ULevelStreaming* LevelStreaming = UGameplayStatics::GetStreamingLevel(this, ScenesNames[CurrentSceneIndex]);
			LevelStreaming->OnLevelLoaded.AddDynamic(this, &ASceneManager::OnStreamSceneLoaded);
		});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 0.1f, false);
}

void ASceneManager::ChangeScene(const TSoftObjectPtr<UWorld>& pNextScene, int pTargetID, bool pFromNarrationScene, bool pWithLoad)
{
	TargetID = pTargetID;
	if (Scenes.Contains(pNextScene))
	{
		BeforeSceneChange(CurrentSceneIndex);
		DelayAnimations += 0.25f; // add more time to finish the animation before unload scene
		SaveIndexSceneBefore = CurrentSceneIndex;
		FromNarrationScene = pFromNarrationScene;
		WithLoad = pWithLoad;
		NextScene = pNextScene;

		//Delay
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]
			{
				//Unload Scene
				FLatentActionInfo LatentInfo;
				UGameplayStatics::UnloadStreamLevel(this, Scenes[CurrentSceneIndex]->GetFName(), LatentInfo, true);

				//UE_LOG(LogTemp, Warning, TEXT("First finish"));
				ULevelStreaming* StreamingLevel = UGameplayStatics::GetStreamingLevel(this, Scenes[CurrentSceneIndex]->GetFName());
				if (StreamingLevel)
				{
					// Bind to the OnLevelLoaded event
					StreamingLevel->OnLevelUnloaded.AddDynamic(this, &ASceneManager::OnStreamSceneUnloaded);
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

// ======================================================
//             Loading Fucntions
// ======================================================

// Loading save for the scene
void ASceneManager::LoadingScene()
{
}

// Saveing the current scene
void ASceneManager::SavingScene()
{
}

// ======================================================
//             Timeline Events
// ======================================================

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

void ASceneManager::LevelTransition(TSoftObjectPtr<UWorld> TargetLevel, float DelayAfterAnimation, bool CurtainsAnimation, bool LightsAnimation, bool FadeCamera)
{
	DelayAnimations = 0;

	PlayerCtrl->BlockPlayerInputs(EBlockPlayerCause::SceneTransition);
	Animations(CurtainsAnimation, LightsAnimation);

	//Remove all widgets on the screen
	UWidgetLayoutLibrary::RemoveAllWidgets(this);

	//Camera Fade
	if (FadeCamera)
	{
		UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraFade(0.0f, 1.0f, 2.0f, FLinearColor::Black, false, true);
	}

	//Delay
	FTimerDelegate TimerDelegate;
	DelayAnimations += DelayAfterAnimation;
	TimerDelegate.BindLambda([&]
		{
			UGameplayStatics::OpenLevelBySoftObjectPtr(this, TargetLevel);
		});
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayAnimations, false);
}

int ASceneManager::GetCurrentIndexScene()
{
	return CurrentSceneIndex;
}

TSoftObjectPtr<UWorld> ASceneManager::GetCurrentScene()
{
	return Scenes[CurrentSceneIndex];
}

void ASceneManager::CheckpointPlayerTransform(FTransform PlayerTransform)
{
}

FTransform ASceneManager::GetCheckpointPlayerTransform()
{
	return FTransform();
}

