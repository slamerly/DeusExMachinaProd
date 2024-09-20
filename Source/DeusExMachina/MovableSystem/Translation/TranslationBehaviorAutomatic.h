#pragma once

#include "CoreMinimal.h"
#include "TranslationBehaviorBase.h"
#include "AutomaticTranslationDatas.h"
#include "TranslationBehaviorAutomatic.generated.h"


UENUM()
enum class EAutoTranslationState : uint8
{
	Inactive = 0,
	AutomaticTranslation = 1,
	StartPhase = 2,
	EndPhase = 3,
	StopOnSplinePoint = 4
};


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
public:
	void StartAutomaticTranslation(bool bForceNoStartPhase = false);
	void StopAutomaticTranslation(bool bForceNoEndPhase = false);
	void CancelAutomaticTranslation();


// ======================================================
//                   Helper Functions
// ======================================================
protected:
	bool IsStartPhaseValid();
	bool IsEndPhaseValid();


// ======================================================
//         Editable Automatic Translation Datas
// ======================================================
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation", meta = (Tooltip = "Values of Automatic Translation for this Translation Support.\nEach data can be overriden."))
	FAutomaticTranslationDatas AutomaticTranslationValues;


// ======================================================
//        Automatic Translation Internal Variables
// ======================================================
protected:
	EAutoTranslationState CurrentState{ EAutoTranslationState::Inactive };

	float AutomaticTranslationSpeed{ 0.0f };
	bool bRunningReverse{ false };

	float PhaseTime{ 0.0f };
	float PhaseTimer{ 0.0f };
	UCurveFloat* PhaseCurve{ nullptr };
};
