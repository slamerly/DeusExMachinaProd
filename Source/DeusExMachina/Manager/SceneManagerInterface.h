// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SceneManagerInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI, NotBlueprintable) //  'NotBlueprintable' here means that the interface cannot be implemented by a blueprint
class USceneManagerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DEUSEXMACHINA_API ISceneManagerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	virtual int GetCurrentIndexScene() = 0;

	UFUNCTION(BlueprintCallable)
	virtual TSoftObjectPtr<UWorld> GetCurrentScene() = 0;

	UFUNCTION(BlueprintCallable)
	virtual void CheckpointPlayerTransform(FTransform PlayerTransform) = 0;

	UFUNCTION(BlueprintCallable)
	virtual FTransform GetCheckpointPlayerTransform() = 0;

	UFUNCTION(BlueprintCallable)
	virtual void ChangeScene(const TSoftObjectPtr<UWorld>& NextLevel, int pTargetID, bool FromNarrationScene, bool WithLoad) = 0;

	UFUNCTION(BlueprintCallable)
	virtual void CurtainsAnimation(bool IsOpen) = 0;

	/**
	* Function call to translate to an other level
	* @param	TargetLevel	Level you want to go
	* @param	DelayAfterAnimation	Delay between the end of the animations and the switch of level
	*/
	UFUNCTION(BlueprintCallable)
	virtual void LevelTransition(TSoftObjectPtr<UWorld> TargetLevel, float DelayAfterAnimation, bool CurtainsAnimation, bool LightsAnimation, bool FadeCamera) = 0;
};
