// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/TargetPoint.h>
#include "SceneManagerInterface.h"
#include <DeusExMachina/Player/PlayerControllerDeusEx.h>
#include "Components/TimelineComponent.h"
#include "Components/LightComponent.h"
#include <Engine/Light.h>
#include "SceneManager.generated.h"


UCLASS()
class DEUSEXMACHINA_API ASceneManager : public AActor, public ISceneManagerInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASceneManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

private:
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
	UFUNCTION(BlueprintCallable)
	void LightsAnimation(bool IsOn);
	void GetSceneLights();
	TArray<ALight*> Lights; //Lights of the current scene
	TArray<float> LightsIntensity; //Ligths' intensity to manage the animations

	// ==================
	//		Change Scene
	// ==================
	int CurrentSceneIndex = 0;
	int TargetID = 0; //The id of the SceneTransition to identify the SceneTransition linked in the new scene
	int SaveIndexSceneBefore = 0; //Save the index of the scene come from
	float DelayAnimations = .0f; // Time to do the animations
	// Need the 2 next bool, for the event functions OnStreamSceneLoaded, OnStreamSceneUnloaded
	bool FromNarrationScene = false;
	bool WithLoad = false;
	TSoftObjectPtr<UWorld> NextScene; //ref of the scene to transit
	void BeforeSceneChange(int pCurrentSceneIndex);
	void AfterSceneChange(int IndexSaveSceneBefore, bool WithLoad);

	//Functions event scene loading
	UFUNCTION()
	void OnStreamSceneLoaded();
	UFUNCTION()
	void OnStreamSceneUnloaded();

	
	// ==================
	//		Save System
	// ==================
	void LoadingScene();
	void SavingScene();	

// ======================================================
//             Events animations and sounds
// ======================================================
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	const TObjectPtr<ATargetPoint> SceneCenter;

	// =======================
	//		Change Level
	// =======================
	/**
	 *	Animation to do in the Begin Play.
	 * For now, we block the player and have a camera fade.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "LevelTransition|BeginPlay")
	void BeginPlayAnimation();
	// beginplay
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelTransition|BeginPlay")
	bool bFadeBeginPlay = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelTransition|BeginPlay")
	bool bCurtainsBeginPlay = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelTransition|BeginPlay")
	bool bLightsBeginPlay = true;
	// Transition
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelTransition|Transition")
	UWorld* NextLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelTransition|Transition")
	float DelayAfterAnimation = 2.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelTransition|Transition")
	bool bCurtainsAnimationTL = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelTransition|Transition")
	bool bLightsAnimationTL = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LevelTransition|Transition")
	bool bFadeTransition = true;

	// =======================
	//		Change Scene
	// =======================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<const TSoftObjectPtr<UWorld>> Scenes;
	// Array of names of scenes add in Scenes, use for loading functions
	UPROPERTY(VisibleAnywhere, Category = "Default")
	TArray<FName> ScenesNames;
	// Update the ScenesNames
	UFUNCTION(CallInEditor, Category = "Default")
	void UpdateScenesNames();
	/**
	* Function to call animations for curtains and lights.
	* @param	CurtainsOpen	Know if the curtains are curently open
	* @param	LighsAreOn	Know if the lights are on
	*/
	void Animations(bool CurtainsOpen, bool LightsAreOn);

	// =======================
	//		Curtains
	// =======================
	TArray<FVector> CurtainsInitialPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeScene")
	TArray<const TObjectPtr<AActor>> Curtains;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeScene")
	bool bCurtainsAnimation = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeScene|Curtains")
	float CurtainsFinalDistancePos = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeScene|Curtains")
	float CurtainsPlayRateAnimation = 1.0f;
	
	// Timeline
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimelineCurtains;
	// La courbe flottante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeScene|Curtains")
	UCurveFloat* FloatCurveCurtains;
	UFUNCTION()
	void OnTimelineUpdateCurtains(float Value);
	// Fonction appelée à la fin de la timeline
	UFUNCTION()
	void OnTimelineFinishedCurtains();

	// =======================
	//		Lights
	// =======================
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeScene")
	bool bLightsAnimation = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeScene|Lights")
	float DelayToPutOnLights = 0.2f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeScene|Lights")
	float LightsPlayRateAnimation = 1.0f;

	// Timeline
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTimelineComponent* TimelineLights;
	// La courbe flottante
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeScene|Lights")
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

	void ChangeScene(const TSoftObjectPtr<UWorld>& NextScene, int pTargetID, bool FromNarrationScene, bool WithLoad) override;

	void CurtainsAnimation(bool IsOpen) override;

	void LevelTransition() override;
};
