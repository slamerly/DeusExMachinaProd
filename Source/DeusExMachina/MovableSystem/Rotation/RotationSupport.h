#pragma once

#include "CoreMinimal.h"
#include "DeusExMachina/MovableSystem/MovingSupportBase.h"
#include "RotSupportValues.h"
#include "RotationSupport.generated.h"

class USceneComponent;
class UStaticMeshComponentPlus;
class UArrowComponent;


UENUM()
enum class ERotationState : uint8
{
	NotRotating = 0,
	AutomaticRotation = 1,
	StandardRotation = 2
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

	/**
	* Add an inner rotation value to this Rotation Support. Allows clamp testing.
	* Note that if you test clamp and if the inner rotation of this support is already outside of the clamp range, it will go back to the nearest clamp.
	* @param	InnerRotAdd		The rotation value you want to add.
	* @param	TestClamp		Do you want this rotation add to test the clamp?
	* @return					True if the rotation was added successfully, False if clamp was reached.
	*/
	bool AddInnerRotation(float InnerRotAdd, bool TestClamp);

	/**
	* Set the inner rotation value of this Rotation Support to a specific value.
	* @param	InnerRot			The value you want to set.
	* @param	AbsoluteRotation	True = will set inner rotation to this exact value. | False = will search for the nearest angle corresponding to the value (default).
	*/
	void ForceInnerRotation(float InnerRot, bool AbsoluteRotation = false);

protected:
	void ComputeInnerTransform();



// ======================================================
//                   Clamp Functions
// ======================================================
public:
	bool UseValidClamp();
	void GetClampValues(float& ClampLow, float& ClampHigh);
	
	/**
	* Simulate a rotation on this support, allowing to know if it would trigger clamp.
	* Note that if the inner rotation of this support is already outside of the clamp range, it will return the angle to go back to the nearest clamp.
	* @param	InputRotationAngle		The rotation angle you want to simulate on this support.
	* @param	ClampedRotationAngle	[OUT] The rotation angle needed to reach clamp on this support.
	* @return							True if the simulation triggers clamp.
	*/
	bool SimulateRotationWithClamp(const float InputRotationAngle, float& ClampedRotationAngle);




// ======================================================
//                 Overrided Functions
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
//            Editable Rot Support Variables
// ======================================================
protected:
	UPROPERTY(EditAnywhere, Category = "Rotation Support", meta = (Tooltip = "The values of snap and clamp of this rotation support."))
	FRotSupportValues RotSupportValues;

	UPROPERTY(EditAnywhere, Category = "Rotation Support", meta = (Tooltip = "Hide the rotation base mesh component in game."))
	bool bDisableSupportVisibility{ false };

	UPROPERTY(EditAnywhere, Category = "Rotation Support", meta = (Tooltip = "Allow the player to not be rotated by this support."))
	bool bDisableSupportCollision{ false };	



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