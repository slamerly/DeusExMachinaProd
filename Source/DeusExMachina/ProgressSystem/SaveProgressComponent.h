// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <DeusExMachina/GameInstanceDeusEx.h>
#include "SaveProgress.h"
#include "SaveProgressComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DEUSEXMACHINA_API USaveProgressComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USaveProgressComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	void PuzzleDone();

private:
	UGameInstanceDeusEx* GameInstance = nullptr;
	//USaveProgress* SaveProgress = nullptr;
		
};
