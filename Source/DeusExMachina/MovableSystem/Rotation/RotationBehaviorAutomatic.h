#pragma once

#include "CoreMinimal.h"
#include "RotationBehaviorBase.h"
#include "AutomaticRotationDatas.h"
#include "RotationBehaviorAutomatic.generated.h"


UENUM()
enum class EAutoRotationState : uint8
{
	Inactive = 0,

	AutomaticRotation = 1,
	StartPhase = 2,
	EndPhase = 3,

	AutomaticRotationWithStop = 4,
	StopOnAngle = 5
};



UCLASS(ClassGroup = (MovableSystem), meta = (BlueprintSpawnableComponent, DisplayName = "Rotation Behavior Automatic", Tooltip = "Component to add to a Rotation Support if you want it to move automatically."))
class DEUSEXMACHINA_API URotationBehaviorAutomatic : public URotationBehaviorBase
{
	GENERATED_BODY()

public:
	URotationBehaviorAutomatic();

protected:
	virtual void BeginPlay() override;
	virtual void LateBeginPlay();

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



// ======================================================
//              Control Automatic Rotation
// ======================================================
protected:
	void LaunchAutomaticRotationBeginPlay();

public:
	/**
	* Function to call to start the automatic rotation with stop on this component. Also used to restart the rotation after the auto stop.
	* Only works if the automatic rotation with stop mode is used.
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Rotation")
	void StartAutomaticRotationWithStop();

	/**
	* Function to call to start the automatic rotation on this component.
	* Only works if the classic automatic rotation mode is used.
	* @param	bForceNoStartPhase		Skip the smooth start (start instantly at full speed)
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Rotation")
	void StartAutomaticRotation(bool bForceNoStartPhase = false);

	/**
	* Function to call to stop the automatic rotation on this component.
	* @param	bForceNoEndPhase		Skip the smooth end (stop instantly)
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Rotation")
	void StopAutomaticRotation(bool bForceNoEndPhase = false);

	/**
	* Function to call to stop every movement of this component.
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Rotation")
	void CancelAutomaticRotation();


// ======================================================
//                     Interaction
// ======================================================
public:
	/**
	* Function to call to trigger an interaction by a button on this behavior.
	* @param	Datas	The interaction datas.
	*/
	UFUNCTION(BlueprintCallable, Category = "Automatic Rotation")
	void TriggerAutoRotInteraction(struct FAutoRotInteractionDatas Datas);


// ======================================================
//                   Helper Functions
// ======================================================
public:
	/** Get the automatic rotation mode used. */
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation")
	EAutomaticRotationType GetAutoRotationType();


protected:
	bool IsAutomaticStopValid();
	bool IsStartPhaseValid();
	bool IsEndPhaseValid();

	/** Unsafe function. Please make sure 'IsAutomaticStopValid()' is called before this function to prevent potential crashes. */
	bool ComputeNextStopAngle();

	bool GetReverse();


// ======================================================
//          Editable Automatic Rotation Datas
// ======================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation", meta = (Tooltip = "Values of Automatic Rotation for this Rotation Support.\nEach data can be overriden."))
	FAutomaticRotationDatas AutomaticRotationValues;


// ======================================================
//        Automatic Rotation Internal Variables
// ======================================================
protected:
	EAutoRotationState CurrentState{ EAutoRotationState::Inactive };

	// =====================
	//  Automatic Speed
	// =====================
	float AutomaticRotationSpeed{ 0.0f };

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
	//  Auto rot with stop
	// =====================
	float AutomaticRotationDuration{ 0.0f };
	float AutomaticRotationTimer{ 0.0f };
	UCurveFloat* AutomaticRotationCurve{ nullptr };
	float AutomaticRotationAngle{ 0.0f };
	float AutomaticRotationAngleDone{ 0.0f };

	// =====================
	//  Auto stop
	// =====================
	int AutomaticStopDestAngle{ 0 };
	float AutomaticStopDuration{ 0.0f };
	float AutomaticStopTimer{ 0.0f };
	bool bAutomaticStopReverse{ false };
};
