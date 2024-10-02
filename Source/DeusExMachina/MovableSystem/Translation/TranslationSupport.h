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


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_TwoParams(FTransSupportMove, ATranslationSupport, OnTranslationSupportMoved, int, SplineIndex, float, ProgressToNextIndex);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FTransSupportClamp, ATranslationSupport, OnTranslationSupportClamped, int, ClampSplineIndex);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FTransSupportSnap, ATranslationSupport, OnTranslationSupportSnapped, int, SnapSplineIndex);


UCLASS()
class DEUSEXMACHINA_API ATranslationSupport : public AMovingSupportBase
{
	GENERATED_BODY()
	
public:	
	ATranslationSupport();

protected:
	virtual void BeginPlay() override;



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
//              Inner Translation Functions
// ======================================================
public:
	/**
	* Add a translation distance to this Translation Support along its spline.
	* @param	TranslationAdd				The translation value you want to add.
	*/
	UFUNCTION(BlueprintCallable, Category = "Translation Support")
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
	UFUNCTION(BlueprintCallable, Category = "Translation Support")
	void ForcePositionOnSpline(const int SplineIndex, const float ProgressToNextPoint = 0.0f);



// ======================================================
//            Compute Inner Translation Helpers
// ======================================================
protected:
	void ComputeInnerTransform();
	void ComputeDistanceFromSplineOrigin();
	void ComputeInnerIndexAndProgress();

public:
	void ComputeInnerIndexAndProgress(const float DistanceFromOrigin, int& OutIndex, float& OutProgress);



// ======================================================
//              Inner Translation Getters
// ======================================================
public:
	/** Get the support distance from the spline origin. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	float GetDistanceFromSplineOrigin() const;

	/** Get the support inner spline index. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	int GetInnerSplineIndex() const;

	/** Get the support progress to next index. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	float GetProgressToNextIndex() const;



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
	* @param	ClampIndex			[OUT] The spline index chosen for clamp. Will be equal to the inner spline index if no clamp.
	* @return						The clamped movement.
	*/
	UFUNCTION(BlueprintCallable, Category = "Translation Support")
	float ClampMovementBetweenSplinePoints(const float Movement, const int SplineIndexA, const int SplineIndexB, int& ClampIndex);



// ======================================================
//                    Snap Functions
// ======================================================
public:
	/**
	* Search for the nearest spline point to snap on.
	* @param	InputDistanceOnSpline	The distance on spline from which you wnt to search a snap point.
	* @param	SnapSearchAdvantage		(optionnal) Adantage for searching points with a lower or a higher distance on spline than input. 0.5f is no advantage. (Between 0 and 1)
	* @return							The found snap point index.
	*/
	UFUNCTION(BlueprintCallable, Category = "Translation Support")
	int SearchNearestSplinePointToSnap(const float InputDistanceOnSpline, const float SnapSearchAdvantage = 0.5f);



// ======================================================
//              Spline Distance Functions
// ======================================================
public:
	/** Get the distance along spline from the support actual position to the next point on its spline. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	float GetSplineDistanceToNextSplinePoint();

	/** Get the distance along spline from the support actual position to the spline point A. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	float GetSplineDistanceToPoint(const int SplineIndexA);

	/** Get the distance along spline from the support actual position to the spline point A, going in the reverse direction of the spline. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	float GetSplineDistanceToPointReversed(const int SplineIndexA);

	/** Get the distance on the spline from the spline point A to B. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	float GetSplineDistanceAToB(const int SplineIndexA, const int SplineIndexB);



// ======================================================
//                   Spline Getters
// ======================================================
public:
	/**
	* Get the next index in the spline. (Going in the spline direction.)
	* @param	SplineIndex		The index from which you want to compute.
	* @return					The next index of the spline.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	int GetNextSplineIndex(const int SplineIndex);

	/**
	* Get the previous index in the spline. (Going in reverse spline direction.)
	* @param	SplineIndex		The index from which you want to compute.
	* @return					The previous index of the spline.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	int GetPrevSplineIndex(const int SplineIndex);


	/** Get the number of point in the translation spline. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	int GetNumberOfSplinePoints();

	/** Get the length of the translation spline. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Translation Support")
	float GetFullSplineLength();



// ======================================================
//            Moving Support Base Functions
// ======================================================
public:
	FTransform GetObjectTransform() override;
	bool IsCurrentlyMoving() override;



// ======================================================
//                Spline Visual Function
// ======================================================
protected:
	void UpdateSplinePointsVisual();



// ======================================================
//                   Delegate Events
// ======================================================
public:
	/** Called when this translation support move. */
	UPROPERTY(BlueprintAssignable, Category = "Translation Support|Events")
	FTransSupportMove OnTranslationSupportMoved;

	/** Called when this translation support reach a clamp. */
	UPROPERTY(BlueprintAssignable, Category = "Translation Support|Events")
	FTransSupportClamp OnTranslationSupportClamped;

	/** Called when this translation support is snapped (called when the snap is finished). */
	UPROPERTY(BlueprintAssignable, Category = "Translation Support|Events")
	FTransSupportSnap OnTranslationSupportSnapped;



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
//           Editable Trans Support Variables
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



// ======================================================
//                Level Editor Variables
// ======================================================
public:
	UPROPERTY(EditAnywhere, Category = "Level Editor", meta = (Tooltip = "The spline index you want to apply to this Translation Support.\nCan be used for testing and for applying a specific spline index at begin play."))
	int EditorSplineIndex{ 0 }; //  'EditorSplineIndex' also serve as a begin play spline index
};
