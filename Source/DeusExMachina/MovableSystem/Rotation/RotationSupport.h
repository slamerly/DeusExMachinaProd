#pragma once

#include "CoreMinimal.h"
#include "DeusExMachina/MovableSystem/MovingSupportBase.h"
#include "RotationSupport.generated.h"

class USceneComponent;
class UStaticMeshComponentPlus;
class UArrowComponent;


UENUM()
enum class ERotationState : uint8
{
	NotRotating = 0,
	AutomaticRotation = 1
};



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
//               Inner Rotation Functions
// ======================================================
public:
	float GetInnerRotation();
	float GetInnerRotationBase360();

	void AddInnerRotation(float InnerRotAdd);

	/**
	* Set the inner rotation value of this Rotation Support to a specific value.
	* @param	InnerRot			The value you want to set.
	* @param	AbsoluteRotation	True = will set inner rotation to this exact value. | False = will search for the nearest angle corresponding to the value (default).
	*/
	void ForceInnerRotation(float InnerRot, bool AbsoluteRotation = false);

protected:
	void ComputeInnerTransform();



// ======================================================
//                Overrided Functions
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
//               Inner Rotation Variables
// ======================================================
protected:
	float InnerRotation{ 0.0f };

public:
	ERotationState CurrentRotationState{ ERotationState::NotRotating };



// ======================================================
//               Level Editor Functions
// ======================================================
protected:
	void ApplyEditorValues() override;


// ======================================================
//               Level Editor Variables
// ======================================================
public:
	UPROPERTY(EditAnywhere, Category = "Level Editor", meta = (Tooltip = "The angle you want to apply for this Rotation Support.\nCan be used for testing and for applying a specific angle at begin play."))
	float EditorAngle{ 0.0f }; //  'EditorAngle' also serve as a begin play angle





// ======================================================
//            Prevent Rotation Base Movement
// ======================================================
public:
	void OnRotationBaseMovedEditor();
};