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
};

/**
 * 
 */
UCLASS()
class DEUSEXMACHINA_API USaveProgress : public USaveGame
{
	GENERATED_BODY()
	
protected:
	//Array to save all scenes and there status.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSceneProgress> ScenesProgress;

public:
	// Get the informations in the save file
	TArray<FSceneProgress> GetScenesProgress();

	/**
	* Created or modify a line in ScenesProgress.
	* @param	pSceneName	The scene name.
	* @param	bIsDone		The status of the puzzle, true if it's done.
	*/
	void SetProgress(FString pSceneName, bool bIsDone);

};
