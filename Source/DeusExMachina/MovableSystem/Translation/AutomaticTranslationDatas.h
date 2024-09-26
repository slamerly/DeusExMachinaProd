#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AutomaticTranslationDatas.generated.h"


UENUM(BlueprintType)
enum class EAutomaticTranslationType : uint8
{
	AutomaticTranslation = 0 UMETA(Tooltip = "The Translation Support move along the spline at a defined speed and doesn't stop automatically on spline points.\nSupport stop and restart from MovSys Buttons."),
	StopOnSplinePoints = 1 UMETA(Tooltip = "The Translation Support move along the spline and automatically stop on spline points.\nDoes not support stop and restart from MovSys Buttons.")
};

UENUM(BlueprintType)
enum class EStopBehavior : uint8
{
	StopEveryPoint = 0 UMETA(Tooltip = "The Translation Support stops every time it reaches a spline point during its automatic movement."),
	StopSpecifedPoint = 1 UMETA(Tooltip = "The Translation Support stops when it reaches a spline point during its automatic movement only if the spline point index correspond to one specified.")
};

USTRUCT(BlueprintType)
struct FStopSplinePoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Spline point index this Translation Support will stop.", ClampMin = 0))
	int StopSplineIndex{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Stop duration for this index.\n(In seconds)", ClampMin = 0.0f))
	float StopIndexDuration{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Does it restart in reverse after stopping on this index?"))
	bool bStopIndexReverse{ false };
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
		meta = (Tooltip = "The Automatic Translation type for this Translation Support."))
	EAutomaticTranslationType AutomaticTranslationType{ EAutomaticTranslationType::AutomaticTranslation };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation", meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::StopOnSplinePoints", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the translation when in Automatic Stop mode.\n(Must be a normalized curve.)"))
	UCurveFloat* TranslationCurve{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation",
		meta = (Tooltip = "Does the Translation Support rotate automatically at begin play?"))
	bool bStartAutomatic{ true };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The duration of the start phase of the Automatic Translation (triggered at begin play or when using a MovSys Button).\n(In seconds)"))
	float StartDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the start phase of the Automatic Translation (triggered at begin play or when using a MovSys Button).\n(Must be a normalized curve.)"))
	UCurveFloat* StartCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The duration of the end phase of the Automatic Translation (triggered when using a MovSys Button).\n(In seconds)"))
	float EndDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the end phase of the Automatic Translation (triggered when using a MovSys Button).\n(Must be a normalized curve.)"))
	UCurveFloat* EndCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", 
		meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::StopOnSplinePoints", EditConditionHides),
		meta = (Tooltip = "Choose the automatic stop behavior of the Translation Support."))
	EStopBehavior StopBehavior{ EStopBehavior::StopEveryPoint };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", 
		meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::StopOnSplinePoints && StopBehavior == EStopBehavior::StopEveryPoint", EditConditionHides),
		meta = (Tooltip = "The duration of the stop when stopping on a spline point during the automatic movement.\n(In seconds)"))
	float GlobalStopDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", 
		meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::StopOnSplinePoints && StopBehavior == EStopBehavior::StopSpecifedPoint", EditConditionHides),
		meta = (Tooltip = "List of spline points index this Translation Support will automatically stop on.\nAlso have a stop duration parameter for each index.", ClampMin = 0))
	TArray<FStopSplinePoint> StopSplinePoints;
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
		meta = (Tooltip = "The Automatic Translation type for this Translation Support."))
	EAutomaticTranslationType AutomaticTranslationType{ EAutomaticTranslationType::AutomaticTranslation };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation", meta = (EditCondition = "bOverrideAutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The speed of the Automatic Translation.\nA positive speed makes the support going forward, a negative speed makes the support going backward.\n(In cm/s)"))
	float TranslationSpeed{ 50.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation", meta = (EditCondition = "bOverrideAutomaticTranslation && AutomaticTranslationType == EAutomaticTranslationType::StopOnSplinePoints", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the translation when in Automatic Stop mode.\n(Must be a normalized curve.)"))
	UCurveFloat* TranslationCurve{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation", meta = (EditCondition = "bOverrideAutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "Does the Translation Support rotate automatically at begin play?"))
	bool bStartAutomatic{ true };


	//  Override start phase

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "Overrides the 'Start Phase' part of the datas for this component."))
	bool bOverrideStartPhase{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "bOverrideStartPhase && AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The duration of the start phase of the Automatic Translation.\n(In seconds)"))
	float StartDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "bOverrideStartPhase && AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the start phase of the Automatic Translation.\n(Must be a normalized curve.)"))
	UCurveFloat* StartCurve{ nullptr };


	//  Override end phase

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "Overrides the 'End Phase' part of the datas for this component."))
	bool bOverrideEndPhase{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "bOverrideEndPhase && AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The duration of the end phase of the Automatic Translation.\n(In seconds)"))
	float EndDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "bOverrideEndPhase && AutomaticTranslationType == EAutomaticTranslationType::AutomaticTranslation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the end phase of the Automatic Translation.\n(Must be a normalized curve.)"))
	UCurveFloat* EndCurve{ nullptr };


	//  Override automatic stop

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "AutomaticTranslationType == EAutomaticTranslationType::StopOnSplinePoints", EditConditionHides),
		meta = (Tooltip = "Overrides the 'Automatic Stop' part of the datas for this component."))
	bool bOverrideAutomaticStop{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", 
		meta = (EditCondition = "bOverrideAutomaticStop && AutomaticTranslationType == EAutomaticTranslationType::StopOnSplinePoints", EditConditionHides),
		meta = (Tooltip = "Choose the behavior of the Translation Support when it reaches a spline point during its automatic movement."))
	EStopBehavior StopBehavior{ EStopBehavior::StopEveryPoint };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop", 
		meta = (EditCondition = "bOverrideAutomaticStop && AutomaticTranslationType == EAutomaticTranslationType::StopOnSplinePoints && StopBehavior == EStopBehavior::StopEveryPoint", EditConditionHides),
		meta = (Tooltip = "The duration of the stop when stopping on a spline point during the automatic movement.\n(In seconds)"))
	float GlobalStopDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (EditCondition = "bOverrideAutomaticStop && AutomaticTranslationType == EAutomaticTranslationType::StopOnSplinePoints && StopBehavior == EStopBehavior::StopSpecifedPoint", EditConditionHides),
		meta = (Tooltip = "List of spline points index this Translation Support will automatically stop on.\nAlso have a stop duration parameter for each index.", ClampMin = 0))
	TArray<FStopSplinePoint> StopSplinePoints;


	//  Getter functions
	bool IsDataValid();

	float GetTranslationSpeed();
	UCurveFloat* GetTranslationCurve();
	bool GetStartAutomatic();
	EAutomaticTranslationType GetAutomaticTranslationType();

	float GetStartDuration();
	UCurveFloat* GetStartCurve();

	float GetEndDuration();
	UCurveFloat* GetEndCurve();

	EStopBehavior GetStopBehavior();
	float GetGlobalStopDuration();
	TArray<FStopSplinePoint> GetStopSplinePoints();
};