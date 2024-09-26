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
	void StartAutomaticRotationWithStop();

public:
	void StartAutomaticRotation(bool bForceNoStartPhase = false);
	void StopAutomaticRotation(bool bForceNoEndPhase = false);
	void CancelAutomaticRotation();


// ======================================================
//                     Interaction
// ======================================================
public:
	void TriggerAutoRotInteraction(struct FAutoRotInteractionDatas Datas);


// ======================================================
//                   Helper Functions
// ======================================================
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

	float AutomaticRotationSpeed{ 0.0f };

	bool bExteriorReverse{ false };
	bool bAutomaticSpeedReverse{ false };

	float PhaseTime{ 0.0f };
	float PhaseTimer{ 0.0f };
	UCurveFloat* PhaseCurve{ nullptr };

	float AutomaticRotationDuration{ 0.0f };
	float AutomaticRotationTimer{ 0.0f };
	UCurveFloat* AutomaticRotationCurve{ nullptr };
	float AutomaticRotationAngle{ 0.0f };
	float AutomaticRotationAngleDone{ 0.0f };

	int AutomaticStopDestAngle{ 0 };
	float AutomaticStopDuration{ 0.0f };
	float AutomaticStopTimer{ 0.0f };
	bool bAutomaticStopReverse{ false };
};
