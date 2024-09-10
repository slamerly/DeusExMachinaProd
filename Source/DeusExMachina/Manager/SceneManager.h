// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/TargetPoint.h>
#include "SceneManagerInterface.h"
#include <DeusExMachina/Player/PlayerControllerDeusEx.h>
#include "Components/TimelineComponent.h"
#include <Engine/Light.h>
#include "SceneManager.generated.h"


UCLASS()
class DEUSEXMACHINA_API ASceneManager : public AActor, public ISceneManagerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASceneManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<const TSoftObjectPtr<UWorld>> Scenes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	const TObjectPtr<ATargetPoint> SceneCenter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<const TObjectPtr<AActor>> Curtains;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:

	int CurrentLevelIndex = 0;
	TObjectPtr<APlayerControllerDeusEx> PlayerCtrl;

	// ==================
	//		Curtains
	// ==================
	UFUNCTION(BlueprintCallable)
	void CurtainsVerifications();
	void InitializeCurtains();

	// ==================
	//		Lights
	// ==================
	void GetSceneLights();
	TArray<ALight*> Lights;
	UFUNCTION(BlueprintCallable)
	void LightsAnimation(bool IsOn);

	// ==================
	//		Change Level
	// ==================
	int TargetID = 0;
	int ComeFromSceneIndex = 0;
	void BeforeLevelChange(int pCurrentLevelIndex);
	void AfterLevelChange(int SaveCurrentLevelIndex, bool WithLoad);
	float DelayAnimations = .0f;

	
	// ==================
	//		Save System
	// ==================
	void LoadingScene();
	void SavingScene();	

// ======================================================
//             Events animations and sounds
// ======================================================
protected:
	/**
	 *	Animation to do in the Begin Play.
	 * For now, we block the player and have a camera fade.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Animation|BeginPlay")
	void BeginPlayAnimation();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|BeginPlay")
	bool bBlockPlayerBeginPlay = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation|BeginPlay")
	bool bFadeBeginPlay = true;

	// ==================
	//		Curtains
	// ==================
	TArray<FVector> CurtainsInitialPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeLevel|Curtains")
	float CurtainsFinalDistancePos = 0;
	
	// Timeline
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimelineCurtains;
	// La courbe flottante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeLevel|Curtains")
	UCurveFloat* FloatCurveCurtains;
	UFUNCTION()
	void OnTimelineUpdateCurtains(float Value);
	// Fonction appelée à la fin de la timeline
	UFUNCTION()
	void OnTimelineFinishedCurtains();

	// ==================
	//		Lights
	// ==================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeLevel|Lights")
	float DelayToPutOnLights = 0.2f;

	// Timeline
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimelineLights;
	// La courbe flottante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeLevel|Curtains")
	UCurveFloat* FloatCurveLights;
	UFUNCTION()
	void OnTimelineUpdateLights(float Value);
	// Fonction appelée à la fin de la timeline
	UFUNCTION()
	void OnTimelineFinishedLights();
	


// ======================================================
//             Scene Manager Interface
// ======================================================
public:
	int GetCurrentIndexScene() override;

	TSoftObjectPtr<UWorld> GetCurrentScene() override;

	void CheckpointPlayerTransform(FTransform PlayerTransform) override;

	FTransform GetCheckpointPlayerTransform() override;

	void ChangeScene(const TSoftObjectPtr<UWorld>& NextLevel, int pTargetID, bool FromNarrationScene, bool WithLoad) override;

	void CurtainsAnimation(bool IsOpen) override;
};
