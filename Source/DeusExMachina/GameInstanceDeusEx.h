// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameFramework/SaveGame.h"
#include "GameInstanceDeusEx.generated.h"

/**
 * 
 */
UCLASS()
class DEUSEXMACHINA_API UGameInstanceDeusEx : public UGameInstance
{
	GENERATED_BODY()

protected:
	virtual void Init() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	

// ======================================================
//					Save System
// ======================================================

	// Save reference
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USaveGame* SaveRef = nullptr;

	// Put the main levels for the loading progress
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UWorld*> MainLevelsList;

	// Normaly automaticly add the scenes (sublevels) in this array for the loading progress.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UWorld*> ScenesList;

	bool DebugMode = false;

public:
	/**
	* Call to create or modify in the SaveProgress file.
	* @param pSceneName	Scene name reference.
	* @param bIsDone	Value to know if the scene is done.
	*/
	UFUNCTION(BlueprintCallable)
	void SetSaveProgress(FString pSceneName, bool bIsDone);

	// Display the array of the savefile
	UFUNCTION(BlueprintCallable)
	void DisplayScenesSaved();

	/**
	* Load the scene you want.
	* @param pLevelName	Level where there is the scene.
	* @param pSceneName	Name of the target scene.
	*/
	UFUNCTION(BlueprintCallable)
	void LoadProgress(FString pLevelName, FString pSceneName);

	UFUNCTION(BlueprintCallable)
	void SetDebugMode(bool NewStatus);

	UFUNCTION(BlueprintCallable)
	bool GetDebugMode();
};
