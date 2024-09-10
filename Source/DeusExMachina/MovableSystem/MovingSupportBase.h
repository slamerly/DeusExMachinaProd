#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MovingSupportBase.generated.h"

class UMovableObjectComponent;
class USceneComponent;


UCLASS()
class DEUSEXMACHINA_API AMovingSupportBase : public AActor
{
	GENERATED_BODY()

public:
	AMovingSupportBase();

protected:
	virtual void BeginPlay() override;



// ======================================================
//             Non-overridable Functions
// ======================================================
public:
	// ====================
	//   Support Movement
	// ====================
	void ApplyInnerMovement(USceneComponent* ComponentToMove);

	// ====================
	//  Movable Childrens
	// ====================
	void AddMovableChild(UMovableObjectComponent* MovableChild);
	void StartMovementOnChildrens();
	void StopMovementOnChildrens();

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
	virtual FTransform GetObjectTransformRelative();

	// ====================
	//   Support Movement
	// ====================
	virtual bool IsCurrentlyMoving();


	// ====================
	//    Debug Movement
	// ====================
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

	FTransform InnerTransform{ FTransform::Identity }; // Transform containing the movement created by the support since the start (begin play)

	FTransform StartTransform{ FTransform::Identity }; // Transform of the support at begin play in world space
	FTransform StartTransformRelative{ FTransform::Identity }; // Transform of the support at begin play in local space

	// TODO: After finishing the core of Movable System, check if StartTransform are still relevant (it shouldn't be for rotations, maybe for translations?)


	bool bIsSelfMovable{ false };
	UMovableObjectComponent* SelfMovable{ nullptr };
};
