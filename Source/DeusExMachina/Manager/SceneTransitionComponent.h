// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Engine/TargetPoint.h>
#include "SceneTransitionComponent.generated.h"



UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class DEUSEXMACHINA_API USceneTransitionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USceneTransitionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	// Collision box
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Default")
	class UBoxComponent* CollisionBox = nullptr;

	//Target
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Target")
	TSoftObjectPtr<UWorld> TargetScene;

public:
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable)
	void PassToNextScene(TSoftObjectPtr<UWorld> NextScene);

	UFUNCTION(BlueprintCallable)
	TSoftObjectPtr<UWorld> GetTargetScene();

	UFUNCTION(BlueprintCallable)
	void SetTargetScene(TSoftObjectPtr<UWorld> NewTargetScene);

private:
	TObjectPtr<AActor> SceneManager;
};
