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
	int GetInnerSplineIndex();
	float GetProgressToNextIndex();

	/**
	* Add a translation distance to this Translation Support along its spline.
	* @param	TranslationAdd				The translation value you want to add.
	* @param	StopIfSplinePointReached	Cancel the remaining movement if a spline point is reached.
	* @return								True if the movement reached a spline point.
	*/
	bool AddTranslationAlongSpline(const float TranslationAdd, const bool StopIfSplinePointReached = false);

	/**
	* Set the position of this Translation Support to a specific spline position.
	* @param	SplineIndex				The spline index you want to put this support on (will be modulated if superior to max spline index).
	* @param	ProgressToNextPoint		(optionnal) The progress of the support between the specified spline point and the next one.
	*/
	void ForcePositionOnSpline(const int SplineIndex, const float ProgressToNextPoint = 0.0f);

protected:
	void ComputeInnerTransform();



// ======================================================
//                   Utility Functions
// ======================================================
public:
	/** Get the distance between the support actual position and the next point on its spline. */
	float GetDistanceToNextSplinePoint();

	/** Get the distance between the support actual position and the current point on its spline. (Useful when the support is between two spline points.) */
	float GetDistanceToCurrentSplinePoint();

	/** Get the direction from spline point A to B. */
	FVector GetDirectionFromToSplinePoint(const int SplineIndexA, const int SplineIndexB);

	/** Get the distance from the spline point A to B. */
	float GetDistanceFromToSplinePoint(const int SplineIndex, const int SplineIndexB);

protected:
	int GetNextSplineIndex(const int SplineIndex);
	int GetPrevSplineIndex(const int SplineIndex);



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
//               Inner Translation Variables
// ======================================================
protected:
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

	UPROPERTY(EditAnywhere, Category = "Translation Support", meta = (Tooltip = "If you want to have a visual representation of where the spline points are in the world during game."))
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
