#pragma once

#include "CoreMinimal.h"
#include "DeusExMachina/MovableSystem/MovingSupportBase.h"
#include "GameFramework/Actor.h"
#include "TranslationSupport.generated.h"

class USceneComponent;
class UStaticMeshComponent;
class USplineComponentPlus;
class UArrowComponent;

UENUM()
enum class ETranslationState : uint8
{
	NotMoving = 0,
	AutomaticTranslation = 1,
	StandardTranslation = 2,
	ControlledTranslation = 3
};


UCLASS()
class DEUSEXMACHINA_API ATranslationSupport : public AMovingSupportBase
{
	GENERATED_BODY()
	
public:	
	ATranslationSupport();

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
	UStaticMeshComponent* TranslationBase;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly)
	USplineComponentPlus* TranslationSpline;



// ======================================================
//               Inner Rotation Functions
// ======================================================
public:
	float GetDistanceFromSplineOrigin() const;
	int GetInnerSplineIndex() const;
	float GetProgressToNextIndex() const;

	/**
	* Add a translation distance to this Translation Support along its spline.
	* @param	TranslationAdd				The translation value you want to add.
	*/
	void AddTranslationAlongSpline(const float TranslationAdd);

	/**
	* Set the position of this Translation Support to a specific spline position.
	* @param	DistanceFromOrigin		The distance along the spline you want to set the support.
	*/
	void ForcePositionOnSpline(const float DistanceFromOrigin);

	/**
	* Set the position of this Translation Support to a specific spline position.
	* @param	SplineIndex				The spline index you want to put this support on (will be modulated if superior to max spline index).
	* @param	ProgressToNextPoint		(optionnal) The progress of the support between the specified spline point and the next one.
	*/
	void ForcePositionOnSpline(const int SplineIndex, const float ProgressToNextPoint = 0.0f);

protected:
	void ComputeInnerTransform();



// ======================================================
//                    Clamp Functions
// ======================================================
public:
	/**
	* Clamp a movement from the support current position to ensure the result isn't out of the specified clamp range.
	* Note that A will be the clamp low and B the clamp high, so if A is 4 and B is 1 the clamp will be between 4 and 1 and not between 1 and 4.
	* This function can return an opposite movement if the support is already outside the clamp range.
	* @param	Movement			The movement you want to clamp.
	* @param	SplineIndexA		The low bound of the clamp range.
	* @param	SplineIndexB		The high bound of the clamp range.
	* @return						The clamped movement.
	*/
	float ClampMovementBetweenSplinePoints(const float Movement, const int SplineIndexA, const int SplineIndexB);



// ======================================================
//                   Utility Functions
// ======================================================
public:
	/** Get the distance along spline between the support actual position and the next point on its spline. */
	float GetSplineDistanceToNextSplinePoint();

	/** Get the distance along spline between the support actual position and the spline point A. */
	float GetSplineDistanceToPoint(const int SplineIndexA);

	/** Get the distance on the spline from the spline point A to B. */
	float GetSplineDistanceAToB(const int SplineIndexA, const int SplineIndexB);


	int GetNextSplineIndex(const int SplineIndex);
	int GetPrevSplineIndex(const int SplineIndex);

	int GetNumberOfSplinePoints();

protected:
	void ComputeDistanceFromSplineOrigin();
	void ComputeInnerIndexAndProgress();



// ======================================================
//                 Overrided Functions
// ======================================================
public:
	// ====================
	//      Transform
	// ====================
	FTransform GetObjectTransform() override;

	// ====================
	//   Support Movement
	// ====================
	bool IsCurrentlyMoving() override;

	// ====================
	//    Debug Movement
	// ====================
	bool GetPlayerInRange(FVector PlayerPosition) override;



// ======================================================
//               Inner Translation Variables
// ======================================================
protected:
	float DistanceFromSplineOrigin{ 0.0f };
	int InnerSplineIndex{ 0 };
	float ProgressToNextIndex{ 0.0f };

public:
	ETranslationState CurrentTranslationState{ ETranslationState::NotMoving };



// ======================================================
//            Editable Rot Support Variables
// ======================================================
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Translation Support", meta = (Tooltip = "Hide the translation base mesh component in game."))
	bool bDisableSupportVisibility{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Translation Support", meta = (Tooltip = "Allow the player to not be moved by this support."))
	bool bDisableSupportCollision{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Translation Support", meta = (Tooltip = "If you want to have a visual representation of where the spline points are in the world during game."))
	bool bShowSplinePointsVisual{ false };

	TArray<UArrowComponent*> SplinePointsVisualArrows;



// ======================================================
//                Level Editor Functions
// ======================================================
protected:
	void ApplyEditorValues() override;

	void UpdateSplinePointsVisual();



// ======================================================
//                Level Editor Variables
// ======================================================
public:
	UPROPERTY(EditAnywhere, Category = "Level Editor", meta = (Tooltip = "The spline index you want to apply to this Translation Support.\nCan be used for testing and for applying a specific spline index at begin play."))
	int EditorSplineIndex{ 0 }; //  'EditorSplineIndex' also serve as a begin play spline index
};
