#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AutomaticTranslationDatas.generated.h"


UENUM(BlueprintType)
enum class EStopBehavior : uint8
{
	NotStop = 0 UMETA(Tooltip = "The Translation Support does not stop when reaching a spline point during its automatic movement."),
	StopEveryPoint = 1 UMETA(Tooltip = "The Translation Support stops every time it reaches a spline point during its automatic movement."),
	StopSpecifedPoint = 2 UMETA(Tooltip = "The Translation Support stops when it reaches a spline point during its automatic movement only if the spline point index correspond to one specified.")
};


UCLASS()
class DEUSEXMACHINA_API UAutomaticTranslationBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation",
		meta = (Tooltip = "The speed of the Automatic Translation.\nA positive speed makes the support going forward, a negative speed makes the support going backward.\n(In cm/s)"))
	float TranslationSpeed{ 50.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation",
		meta = (Tooltip = "Does the Translation Support rotate automatically at begin play?"))
	bool bStartAutomatic{ true };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (Tooltip = "Choose the behavior of the Translation Support when it reaches a spline point during its automatic movement."))
	EStopBehavior StopBehavior{ EStopBehavior::NotStop };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", meta = (EditCondition = "StopBehavior != EStopBehavior::NotStop", EditConditionHides),
		meta = (Tooltip = "The duration of the stop when reaching a stop condition during the automatic movement.\n(In seconds)"))
	float StopDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", meta = (EditCondition = "StopBehavior == EStopBehavior::StopSpecifedPoint", EditConditionHides),
		meta = (Tooltip = "Index of the spline points this Translation Support will stop.", ClampMin = 0))
	TArray<int> StopSplineIndex;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase",
		meta = (Tooltip = "The duration of the start phase of the Automatic Translation.\n(In seconds)"))
	float StartDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase",
		meta = (Tooltip = "The interpolation curve of the start phase of the Automatic Translation.\n(Must be a normalized curve.)"))
	UCurveFloat* StartCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase",
		meta = (Tooltip = "The duration of the end phase of the Automatic Translation.\n(In seconds)"))
	float EndDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase",
		meta = (Tooltip = "The interpolation curve of the end phase of the Automatic Translation.\n(Must be a normalized curve.)"))
	UCurveFloat* EndCurve{ nullptr };
};


USTRUCT(BlueprintType)
struct FAutomaticTranslationDatas
{
	GENERATED_BODY()


	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Automatic Translation for this Translation Support."))
	UAutomaticTranslationBaseDatas* AutomaticTranslationDatas{ nullptr };


	//  Override automatic translation

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'Automatic Translation' part of the datas for this component."))
	bool bOverrideAutomaticTranslation{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation", meta = (EditCondition = "bOverrideAutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The speed of the Automatic Translation.\nA positive speed makes the support going forward, a negative speed makes the support going backward.\n(In cm/s)"))
	float TranslationSpeed{ 50.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation", meta = (EditCondition = "bOverrideAutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "Does the Translation Support rotate automatically at begin play?"))
	bool bStartAutomatic{ true };


	//  Override automatic stop

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'Automatic Stop' part of the datas for this component."))
	bool bOverrideAutomaticStop{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", meta = (EditCondition = "bOverrideAutomaticStop", EditConditionHides),
		meta = (Tooltip = "Choose the behavior of the Translation Support when it reaches a spline point during its automatic movement."))
	EStopBehavior StopBehavior{ EStopBehavior::NotStop };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", meta = (EditCondition = "bOverrideAutomaticStop && StopBehavior != EStopBehavior::NotStop", EditConditionHides),
		meta = (Tooltip = "The duration of the stop when reaching a stop condition during the automatic movement.\n(In seconds)"))
	float StopDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", meta = (EditCondition = "bOverrideAutomaticStop && StopBehavior == EStopBehavior::StopSpecifedPoint", EditConditionHides),
		meta = (Tooltip = "Index of the spline points this Translation Support will stop.", ClampMin = 0))
	TArray<int> StopSplineIndex;


	//  Override start phase

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'Start Phase' part of the datas for this component."))
	bool bOverrideStartPhase{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "bOverrideStartPhase", EditConditionHides),
		meta = (Tooltip = "The duration of the start phase of the Automatic Translation.\n(In seconds)"))
	float StartDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "bOverrideStartPhase", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the start phase of the Automatic Translation.\n(Must be a normalized curve.)"))
	UCurveFloat* StartCurve{ nullptr };


	//  Override end phase

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'End Phase' part of the datas for this component."))
	bool bOverrideEndPhase{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "bOverrideEndPhase", EditConditionHides),
		meta = (Tooltip = "The duration of the end phase of the Automatic Translation.\n(In seconds)"))
	float EndDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "bOverrideEndPhase", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the end phase of the Automatic Translation.\n(Must be a normalized curve.)"))
	UCurveFloat* EndCurve{ nullptr };


	//  Getter functions
	bool IsDataValid();

	float GetTranslationSpeed();
	bool GetStartAutomatic();

	EStopBehavior GetStopBehavior();
	float GetStopDuration();
	TArray<int> GetStopSplineIndex();

	float GetStartDuration();
	UCurveFloat* GetStartCurve();

	float GetEndDuration();
	UCurveFloat* GetEndCurve();
};