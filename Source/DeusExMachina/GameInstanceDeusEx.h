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
	
	// Save reference
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USaveGame* SaveRef = nullptr;

public:
	USaveGame* GetSaveProgressRef();
	void SetSaveProgress(TSoftObjectPtr<UWorld> scene, bool bIsDone);
	void GetSaveMap();
};
