// Fill out your copyright notice in the Description page of Project Settings.


#include "SceneManager.h"
#include <Kismet/GameplayStatics.h>
#include "SceneTransition.h"
#include "Kismet/KismetMathLibrary.h" // Pour les interpolations
#include "UObject/ConstructorHelpers.h" // Pour charger les courbes
#include <DeusExMachina/BlueprintLibrary/LevelUtilitiesFunctions.h>

// Sets default values
ASceneManager::ASceneManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TimelineCurtains = CreateDefaultSubobject<UTimelineComponent>(TEXT("TimelineCurtains"));

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

void ASceneManager::LightsAnimation(bool IsOn)
{
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
	//Ligth part
	GetSceneLights();
	//TODO - Animation Lights

	CurtainsAnimation(true);

	DelayAnimations = TimelineCurtains->GetTimelineLength() + (TimelineCurtains->GetTimelineLength() * (1 - TimelineCurtains->GetPlayRate()));
	//TODO - version Lights
}

void ASceneManager::AfterLevelChange(int SaveCurrentLevelIndex, bool WithLoad)
{
	if (WithLoad)
	{
		LoadingScene();
	}
	//TArray<SceneTransition> SceneTransitions;
	//UGameplayStatics::GetAllActorsOfClass(this, SceneTransitions);
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

// TODO
void ASceneManager::OnTimelineUpdateLights(float Value)
{
	/*
	float Intencity = 0;
	for (int i = 0; i < Lights.Num(); i++)
	{
		Intencity = FMath::Lerp(Lights[i]->GetLightComponent()->, FinalPos, Value);
	}
	*/
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

void ASceneManager::ChangeScene(const TSoftObjectPtr<UWorld>& NextLevel, int pTargetID, bool FromNarrationScene, bool WithLoad)
{
	
	TargetID = pTargetID;
	if (Scenes.Contains(NextLevel))
	{
		BeforeLevelChange(CurrentLevelIndex);
		DelayAnimations += 0.25f;
		//Delay
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]
			{
				ComeFromSceneIndex = CurrentLevelIndex;

				//Unload Scene
				FLatentActionInfo LatentInfo;
				UGameplayStatics::UnloadStreamLevel(this, Scenes[CurrentLevelIndex]->GetFName(), LatentInfo, true);

				CurrentLevelIndex = Scenes.Find(NextLevel);

				//UGameplayStatics::LoadStreamLevel(this, NextLevel, true, true, LatentInfo);
				UGameplayStatics::LoadStreamLevelBySoftObjectPtr(GetWorld(), NextLevel, true, true, LatentInfo);

				//AfterLevelChange(ComeFromSceneIndex, WithLoad);

				//Delay
				PlayerCtrl->UnblockPlayerInputs(EBlockPlayerCause::SceneTransition);
			});

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DelayAnimations, false);

		UE_LOG(LogTemp, Warning, TEXT("DelayAnimation: %f"), DelayAnimations);


		//Narration
		if (FromNarrationScene)
		{
			PlayerCtrl->UnblockPlayerInputs(EBlockPlayerCause::Narration);
			PlayerCtrl->SetPlayerInputMode(EPlayerInputMode::PlayerMovement);
			UGameplayStatics::GetPlayerPawn(this, 0)->SetActorHiddenInGame(false);	
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 120, FColor::Red, TEXT("Net scene not contains in SceneManager's Scenes array."));
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

