// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDDeusEx.generated.h"

/**
 * 
 */
UCLASS()
class DEUSEXMACHINA_API AHUDDeusEx : public AHUD
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent)
	void ShowInteractionTooltip(FVector2D TooltipScreenFractionPos);

	UFUNCTION(BlueprintImplementableEvent)
	void HideInteractionTooltip();
};
