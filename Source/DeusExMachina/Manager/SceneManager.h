// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Engine/TargetPoint.h>
#include "SceneManagerInterface.h"
#include <DeusExMachina/Player/PlayerControllerDeusEx.h>
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChangeLevel|Curtains")
	TArray<FVector> CurtainsInitialPositions;


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
	//		Change Level
	// ==================
	int TargetID = 0;
	int ComeFromSceneIndex = 0;
	void BeforeLevelChange(int pCurrentLevelIndex);
	void AfterLevelChange(int SaveCurrentLevelIndex, bool WithLoad);

	
	// ==================
	//		Save System
	// ==================
	void LoadingScene();
	void SavingScene();	

// ======================================================
//             Events animations and sounds
// ======================================================
public:
	/**
	 *	Animation to do in the Begin Play.
	 * For now, we block the player and have a camera fade.
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Animation|BeginPlay")
	void BeginPlayAnimation();
	UPROPERTY(EditAnywhere, Category = "Animation|BeginPlay")
	bool bBlockPlayerBeginPlay = true;
	UPROPERTY(EditAnywhere, Category = "Animation|BeginPlay")
	bool bFadeBeginPlay = true;

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
