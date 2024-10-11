#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingSupportBase.generated.h"

class UMovableObjectComponent;
class USceneComponent;


UCLASS(Abstract)
class DEUSEXMACHINA_API AMovingSupportBase : public AActor
{
	GENERATED_BODY()

public:
	AMovingSupportBase();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;



// ======================================================
//             Non-overridable Functions
// ======================================================
public:
	// ====================
	//  Movable Childrens
	// ====================
	
	/** 
	* Add a new movable child to this support.
	* @param	MovableChild	The movable child to add.
	*/
	UFUNCTION(BlueprintCallable)
	void AddMovableChild(UMovableObjectComponent* MovableChild);

	/**
	* Activate all movable childs of this support to make them follow this support's movements.
	*/
	UFUNCTION(BlueprintCallable, Category = "Moving Support")
	void StartMovementOnChildrens();

	/**
	* Desactivate all movable childs of this support to make them inactive (for performance reasons).
	* @param	Delay	Set it to true if you need the childs to be active two more frames after this function. (Useful for movement behaviors.)
	*/
	UFUNCTION(BlueprintCallable, Category = "Moving Support")
	void StopMovementOnChildrens(bool Delay = true);


	UFUNCTION()
	void UpdateEveryChildrens(); //  bindable




// ======================================================
//               Overridable Functions
// ======================================================
public:
	// ====================
	//      Transform
	// ====================
	virtual FTransform GetObjectTransform();

	// ====================
	//   Support Movement
	// ====================
	/** Return true if the support is currently performing a movement. */
	UFUNCTION(BlueprintCallable, BlueprintPure)
	virtual bool IsCurrentlyMoving();


	// ====================
	//    Debug Movement
	// ====================

	/** Currently unimplemented */
	virtual bool GetPlayerInRange(FVector PlayerPosition);



// ======================================================
//                  Level Editor
// ======================================================
protected:
	UFUNCTION(CallInEditor, Category = "Level Editor", meta = (Tooltip = "Apply the editor value you entered for this Moving Support."))
	virtual void ApplyEditorValues();

	TArray<UMovableObjectComponent*> RetrieveChildrensEditor();
	void ComputeChildrensOffsetEditor();
	void ApplyChildLevelEditor();



// ======================================================
//                    Variables
// ======================================================
protected:
	TArray<UMovableObjectComponent*> MovableChildrens;

	bool bIsSelfMovable{ false };
	UMovableObjectComponent* SelfMovable{ nullptr };

	int StopMovementPending{ 2 };
};
