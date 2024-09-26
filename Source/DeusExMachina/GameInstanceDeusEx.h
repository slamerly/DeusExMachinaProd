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
	
	// Save reference
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USaveGame* SaveRef = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UWorld*> MainLevelsList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FName, UWorld*> ScenesList;

public:
	USaveGame* GetSaveProgressRef();
	UFUNCTION(BlueprintCallable)
	void SetRefProgress(FString pSceneName, bool bIsDone);
	UFUNCTION(BlueprintCallable)
	void GetSaveMap();

	UFUNCTION(BlueprintCallable)
	void LoadProgress(FString pLevelName, FString pSceneName);
};
