#pragma once

#include "CoreMinimal.h"
#include "DeusExMachina/MovableSystem/MovingSupportBase.h"
#include "RotationSupport.generated.h"

class USceneComponent;
class UStaticMeshComponentPlus;
class UArrowComponent;

UCLASS()
class DEUSEXMACHINA_API ARotationSupport : public AMovingSupportBase
{
	GENERATED_BODY()
	
public:	
	ARotationSupport();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;


// ======================================================
//                  Default Components
// ======================================================
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USceneComponent* SceneRootComponent;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UStaticMeshComponentPlus* RotationBase;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UArrowComponent* Arrow;



// ======================================================
//               Overrided Functions (category name temp)
// ======================================================
public:
	// ====================
	//      Transform
	// ====================
	FTransform GetObjectTransform() override;
	FTransform GetObjectTransformRelative() override;

	// ====================
	//   Support Movement
	// ====================
	bool IsCurrentlyMoving() override;


	// ====================
	//    Debug Movement
	// ====================
	bool GetPlayerInRange(FVector PlayerPosition) override;





// ======================================================
//              Testing Editor Functions
// ======================================================
protected:
	void ApplyTestingValues() override;

// ======================================================
//              Testing Editor Variables
// ======================================================
public:
	UPROPERTY(EditAnywhere, Category = "Testing Editor", meta = (Tooltip = "The angle you want to test for this Rotation Support and for its childs.\nWarning: this angle will be applied in the world space and will not take in account potential\nrotation this object could have due to previous rotation testing on its parent."))
	float TestingAngle{ 0.0f };





// ======================================================
//            Prevent Rotation Base Movement
// ======================================================
public:
	void OnRotationBaseMovedEditor();
};
