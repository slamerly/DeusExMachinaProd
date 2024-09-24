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
	void AddMovableChild(UMovableObjectComponent* MovableChild);
	void StartMovementOnChildrens();
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
