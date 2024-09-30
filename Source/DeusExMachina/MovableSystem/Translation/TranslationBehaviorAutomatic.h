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
	
	AutomaticTranslationWithStop = 4,
	StopOnSplinePoint = 5
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
