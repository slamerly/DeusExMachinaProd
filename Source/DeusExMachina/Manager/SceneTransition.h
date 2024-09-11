// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <Engine/TargetPoint.h>
#include "SceneManager.h"
#include "GameFramework/Actor.h"
#include <Engine/Light.h>
#include "SceneTransition.generated.h"

UCLASS()
/**
 *
 */
class DEUSEXMACHINA_API ASceneTransition : public AActor
{
	GENERATED_BODY()

public:
	ASceneTransition(const FObjectInitializer& ObjectInitializer);

	~ASceneTransition();

protected:
	virtual void PostActorCreated() override;
	virtual void BeginPlay() override;

	// Collision box
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	class UBoxComponent* CollisionBox = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	int IdSceneTransition = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TObjectPtr<ATargetPoint> SpawnRomeo;

	//Light
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default")
	TArray<ALight*> Lights;
	UFUNCTION(CallInEditor)
	void GetAllLightsInSubLevel();
	*/

	//Target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	TSoftObjectPtr<UWorld> TargetScene;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	int TargetID = 0;

public:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	TSoftObjectPtr<UWorld> GetTargetScene();
	UFUNCTION()
	int GetTargetId();
	UFUNCTION()
	ATargetPoint* GetSpawnRomeo();

private:
	TObjectPtr<AActor> SceneManager;
};

