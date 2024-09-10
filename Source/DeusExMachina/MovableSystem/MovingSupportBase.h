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
//                  Testing Editor
// ======================================================
protected:
	UFUNCTION(CallInEditor, Category = "Testing Editor", meta = (Tooltip = "Apply the testing value you entered for this Moving Support."))
	virtual void ApplyTestingValues();

	TArray<UMovableObjectComponent*> RetrieveChildrensEditor();
	void ComputeChildrensOffsetEditor();
	void ApplyChildTestingEditor();



// ======================================================
//                    Variables
// ======================================================
protected:
	TArray<UMovableObjectComponent*> MovableChildrens;

	FTransform InnerTransform{ FTransform::Identity }; // Transform containing the movement created by the support

	FTransform StartTransform{ FTransform::Identity }; // Transform of the support at begin play in world space
	FTransform StartTransformRelative{ FTransform::Identity }; // Transform of the support at begin play in local space


	bool bIsSelfMovable{ false };
	UMovableObjectComponent* SelfMovable{ nullptr };
};
