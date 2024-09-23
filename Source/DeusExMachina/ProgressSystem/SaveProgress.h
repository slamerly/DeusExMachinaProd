// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SaveProgress.generated.h"

USTRUCT(BlueprintType)
struct FSceneProgress
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SceneName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bPuzzleDone;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString LevelName;
};

/**
 * 
 */
UCLASS()
class DEUSEXMACHINA_API USaveProgress : public USaveGame
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSceneProgress> ScenesProgress;

public:
	TArray<FSceneProgress> GetScenesProgress();
	void SetScenesProgress(FString pSceneName, bool bIsDone, FString pLevelName);

};
