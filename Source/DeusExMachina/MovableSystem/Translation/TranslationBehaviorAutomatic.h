#pragma once

#include "CoreMinimal.h"
#include "TranslationBehaviorBase.h"
#include "AutomaticTranslationDatas.h"
#include "DeusExMachina/MovableSystem/Interactables/AutoTransInteractionDatas.h"
#include "TranslationBehaviorAutomatic.generated.h"


UENUM()
enum class EAutoTranslationState : uint8
{
	Inactive = 0,
	
	AutomaticTranslation = 1,
	StartPhase = 2,
	EndPhase = 3,
	
	AutomaticTranslationWithStop = 4,
	StopOnSplinePoint = 5
};


DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FAutomaticTranslationStart, UTranslationBehaviorAutomatic, OnAutoTranslationStart);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE(FAutomaticTranslationStop, UTranslationBehaviorAutomatic, OnAutoTranslationStop);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FAutomaticTranslationTrigger, UTranslationBehaviorAutomatic, OnAutoTranslationTriggered, const FAutoTransInteractionDatas&, Datas);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FAutomaticTranslationAutoStop, UTranslationBehaviorAutomatic, OnAutoTranslationAutoStop, int, StopSplineIndex);
DECLARE_DYNAMIC_MULTICAST_SPARSE_DELEGATE_OneParam(FAutomaticTranslationAutoResume, UTranslationBehaviorAutomatic, OnAutoTranslationAutoResume, int, NextStopSplineIndex);


UCLASS(ClassGroup = (MovableSystem), meta = (BlueprintSpawnableComponent, DisplayName = "Translation Behavior Automatic", Tooltip = "Component to add to a Translation Support if you want it to move automatically."))
class DEUSEXMACHINA_API UTranslationBehaviorAutomatic : public UTranslationBehaviorBase
{
	GENERATED_BODY()

public:
	UTranslationBehaviorAutomatic();

protected:
	virtual void BeginPlay() override;
	virtual void LateBeginPlay();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



// ======================================================
//              Control Automatic Translation
// ======================================================
protected:
	void LaunchAutomaticTranslationBeginPlay();

public:
	/**
	* Function to call to start the automatic translation with stop on this component. Also used to restart the translation after the auto stop.
	* Only works if the automatic translation with stop mode is used.
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Translation")
	void StartAutomaticTranslationWithStop();

	/**
	* Function to call to start the automatic translation on this component.
	* Only works if the classic automatic translation mode is used.
	* @param	bForceNoStartPhase		Skip the smooth start (start instantly at full speed)
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Translation")
	void StartAutomaticTranslation(bool bForceNoStartPhase = false);

	/**
	* Function to call to stop the automatic translation on this component.²
	* @param	bForceNoEndPhase		Skip the smooth end (stop instantly)
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Translation")
	void StopAutomaticTranslation(bool bForceNoEndPhase = false);

	/**
	* Function to call to stop every movement of this component.
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Translation")
	void CancelAutomaticTranslation();


// ======================================================
//                     Interaction
// ======================================================
public:
	/**
	* Function to call to trigger an interaction by a button on this behavior.
	* @param	Datas	The interaction datas.
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Translation")
	void TriggerAutoTransInteraction(struct FAutoTransInteractionDatas Datas);


// ======================================================
//                   Helper Functions
// ======================================================
public:
	/** Get the automatic translation mode used. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Translation")
	EAutomaticTranslationType GetAutoTranslationType();


protected:
	bool IsAutomaticStopValid();
	bool IsStartPhaseValid();
	bool IsEndPhaseValid();

	/** Unsafe function. Please make sure 'IsAutomaticStopValid()' is called before this function to prevent potential crashes. */
	bool ComputeNextStopPoint();

	bool GetReverse();


// ======================================================
//                   Delegate Events
// ======================================================
public:
	/** Called when the automatic translation start on this component. */
	UPROPERTY(BlueprintAssignable, Category = "Automatic Translation|Events")
	FAutomaticTranslationStart OnAutoTranslationStart;

	/** Called when the automatic translation stop on this component. */
	UPROPERTY(BlueprintAssignable, Category = "Automatic Translation|Events")
	FAutomaticTranslationStop OnAutoTranslationStop;

	/** Called when the automatic translation is triggered by a button on this component. */
	UPROPERTY(BlueprintAssignable, Category = "Automatic Translation|Events")
	FAutomaticTranslationTrigger OnAutoTranslationTriggered;

	/** Called when the automatic translation automatically stop on a spline point on this component. */
	UPROPERTY(BlueprintAssignable, Category = "Automatic Translation|Events")
	FAutomaticTranslationAutoStop OnAutoTranslationAutoStop;

	/** Called when the automatic translation automatically resume after stopping on a spline point on this component. */
	UPROPERTY(BlueprintAssignable, Category = "Automatic Translation|Events")
	FAutomaticTranslationAutoResume OnAutoTranslationAutoResume;


// ======================================================
//         Editable Automatic Translation Datas
// ======================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation", meta = (Tooltip = "Values of Automatic Translation for this Translation Support.\nEach data can be overriden."))
	FAutomaticTranslationDatas AutomaticTranslationValues;

	/**
	* Set new Automatic Translation Values to this Automatic Translation Behavior.
	* Note that this will stop the current translation and restart with the new values. (If 'Start Automatic' is off, it will not restart automatically).
	* @param	NewAutoTransValues	The new values to set for this behavior.
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Translation")
	void ChangeAutomaticTranslationValues(FAutomaticTranslationDatas NewAutoTransValues);


// ======================================================
//        Automatic Translation Internal Variables
// ======================================================
protected:
	EAutoTranslationState CurrentState{ EAutoTranslationState::Inactive };

	// =====================
	//  Automatic Speed
	// =====================
	float AutomaticTranslationSpeed{ 0.0f };

	// =====================
	//  Reverses
	// =====================
	bool bExteriorReverse{ false };
	bool bAutomaticSpeedReverse{ false };


	// =====================
	//  Start and End phases
	// =====================
	float PhaseTime{ 0.0f };
	float PhaseTimer{ 0.0f };
	UCurveFloat* PhaseCurve{ nullptr };


	// =====================
	//  Auto trans with stop
	// =====================
	float AutomaticTranslationDuration{ 0.0f };
	float AutomaticTranslationTimer{ 0.0f };
	UCurveFloat* AutomaticTranslationCurve{ nullptr };
	float AutomaticTranslationDistance{ 0.0f };
	float AutomaticTranslationDistanceDone{ 0.0f };

	// =====================
	//  Auto stop
	// =====================
	int AutomaticStopDestIndex{ 0 };
	float AutomaticStopDuration{ 0.0f };
	float AutomaticStopTimer{ 0.0f };
	bool bAutomaticStopReverse{ false };
};
