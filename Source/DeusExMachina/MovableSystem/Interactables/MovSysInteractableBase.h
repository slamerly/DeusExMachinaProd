#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovSysInteractableBase.generated.h"

UCLASS(Abstract)
class DEUSEXMACHINA_API AMovSysInteractableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	AMovSysInteractableBase();

protected:
	virtual void BeginPlay() override;



// ======================================================
//               Non-overridable Functions
// ======================================================
public:
	// ======================
	//  Disable Interactable
	// ======================

	/** Disable the interaction on this interactable. */
	UFUNCTION(BlueprintCallable, Category = "Moving System Interactable")
	void DisableInteractable();

	/** Enable the interaction on this interactable. */
	UFUNCTION(BlueprintCallable, Category = "Moving System Interactable")
	void EnableInteractable();

	/** Is this interactable disabled? */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Moving System Interactable")
	bool IsInteractableDisabled();


// ======================================================
//                Overridable Functions
// ======================================================
public:
	/** Instantly release the interaction if the player was using this interactable. */
	UFUNCTION(BlueprintCallable, Category = "Moving System Interactable")
	virtual void ForceReleaseInteractable();


// ======================================================
//                     Focus Actor
// ======================================================
public:
	UPROPERTY(EditInstanceOnly, Category = "Focus Actor", meta = (Tooltip = "The actor the camera will focus when using this interactable."))
	AActor* ActorToFocus{ nullptr };


// ======================================================
//                      Variables
// ======================================================
protected:
	bool bDisabled{ false };
};
