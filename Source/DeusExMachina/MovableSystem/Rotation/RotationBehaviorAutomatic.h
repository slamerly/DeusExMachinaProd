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
	EndPhase = 3
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
public:
	void StartAutomaticRotation(bool bForceNoStartPhase = false);
	void StopAutomaticRotation(bool bForceNoEndPhase = false);
	void CancelAutomaticRotation();


// ======================================================
//                   Helper Functions
// ======================================================
protected:
	bool IsStartPhaseValid();
	bool IsEndPhaseValid();


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
	bool bRunningReverse{ false };

	float PhaseTime{ 0.0f };
	float PhaseTimer{ 0.0f };
	UCurveFloat* PhaseCurve{ nullptr };
};
