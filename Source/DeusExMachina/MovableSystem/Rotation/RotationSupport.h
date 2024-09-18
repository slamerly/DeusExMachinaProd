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
	StandardRotation = 2,
	ControlledRotation = 3
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

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
	virtual void PostEditMove(bool bFinished) override;
#endif // WITH_EDITOR


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

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UArrowComponent* ClampVisualLow;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	UArrowComponent* ClampVisualHigh;



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
//                Utility Get Functions
// ======================================================
public:
	const FRotSupportValues GetSupportValues() { return RotSupportValues; }



// ======================================================
//                   Clamp Functions
// ======================================================
public:
	bool UseValidClamp();
	void GetClampValues(int& ClampLow, int& ClampHigh);
	
	/**
	* Simulate a rotation on this support, allowing to know if it would trigger clamp.
	* @param	InputRotationAngle		The rotation angle you want to simulate on this support.
	* @param	ClampedRotationAngle	[OUT] The rotation angle needed to reach clamp on this support.
	* @return							True if the simulation triggers clamp.
	*/
	bool SimulateRotationWithClamp(const float InputRotationAngle, float& ClampedRotationAngle);



// ======================================================
//                   Snap Functions
// ======================================================
public:
	bool UseValidSnap();

	/**
	* Search for the nearest snap angle from an inputed angle. Will prevent going through clamps if the support uses clamping.
	* Note that even if the snap values are stored between 0 and 360, will return the angle in the range of the input.
	* @param	InputRotationAngle		The rotation angle from which you want to search a snap angle.
	* @param	SnapAngle				[OUT] The found snap angle. Will be set to 'InputRotationAngle' if the snap search fails.
	* @param	SnapSearchAdvantage		(optionnal) Advantage for searching angles with a lower or a higher value than input. 0 is no advantage. (Between -1 and 1)
	* @param	IgnoreClamp				(optionnal) Force the snap search to not compute clamp, even if the support uses clamping.
	* @param	IgnoreSnapRanges		(optionnal) Force the snap search to ignore the range of every snap values.
	* @return							True if the search was successful. False if the search couldn't find a suitable snap angle (the support doesn't uses snap, it was blocked by clamp, the input was outside any snap range). 
	*/
	bool SearchSnapAngle(const float InputRotationAngle, float& SnapAngle, const float SnapSearchAdvantage = 0.0f, const bool IgnoreClamp = false, const bool IgnoreSnapRanges = false);




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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Support", meta = (Tooltip = "The values of snap and clamp of this rotation support."))
	FRotSupportValues RotSupportValues;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Support|Advanced", meta = (Tooltip = "Hide the rotation base mesh component in game."))
	bool bDisableSupportVisibility{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Support|Advanced", meta = (Tooltip = "If you want to hide the visual arrow for the inner angle on this support."))
	bool bHideAngleArrowVisual{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Support|Advanced", meta = (Tooltip = "Allow the player to not be rotated by this support."))
	bool bDisableSupportCollision{ false };



// ======================================================
//               Level Editor Functions
// ======================================================
protected:
	void ApplyEditorValues() override;

	void UpdateClampVisual();

	UFUNCTION(BlueprintCallable)
	void UpdateSnapVisual(); //  Callable from blueprint to be able to call this function on the "Construction Script"




// ======================================================
//               Level Editor Variables
// ======================================================
public:
	UPROPERTY(EditAnywhere, Category = "Level Editor", meta = (Tooltip = "The angle you want to apply for this Rotation Support.\nCan be used for testing and for applying a specific angle at begin play."))
	float EditorAngle{ 0.0f }; //  'EditorAngle' also serve as a begin play angle

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Editor", meta = (Tooltip = "If you want to hide the visual representation of clamps on this support."))
	bool bHideClampVisual{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level Editor", meta = (Tooltip = "If you want to hide the visual representation of snaps on this support."))
	bool bHideSnapVisual{ false };

protected:
	TArray<UArrowComponent*> SnapArrows;



// ======================================================
//            Prevent Rotation Base Movement
// ======================================================
public:
	void OnRotationBaseMovedEditor();
};