#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AutomaticRotationDatas.generated.h"


UENUM(BlueprintType)
enum class EAutomaticRotationType : uint8
{
	AutomaticRotation = 0 UMETA(Tooltip = "The Rotation Support rotate and doesn't stop automatically on defined angles.\nSupport stop and restart from MovSys Buttons."),
	StopOnAngles = 1 UMETA(Tooltip = "The Rotation Support rotate and automatically stop on on defined angles.\nDoes not support stop and restart from MovSys Buttons.")
};

UENUM(BlueprintType)
enum class ERotStopBehavior : uint8
{
	StopAngleInterval = 0 UMETA(Tooltip = "The Rotation Support stops every time it's rotation since last stop reach a defined interval."),
	StopSpecifedAngles = 1 UMETA(Tooltip = "The Rotation Support stops when it reaches any of the defined angle\nSupport more customization for each defined angle.")
};

USTRUCT(BlueprintType)
struct FStopDefinedAngle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Angle on which this Rotation Support will stop.", ClampMin = 0, ClampMax = 360))
	int StopAngle{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Stop duration for this angle.\n(In seconds)", ClampMin = 0.0f))
	float StopAngleDuration{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Does it restart in reverse after stopping on this angle?"))
	bool bStopAngleReverse{ false };
};


UCLASS()
class DEUSEXMACHINA_API UAutomaticRotationBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation", 
		meta = (Tooltip = "The speed of the Automatic Rotation.\nA positive speed make a clockwise rotation, a negative speed make an anticlockwise rotation.\n(In degrees per second)"))
	float RotationSpeed{ 75.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation",
		meta = (Tooltip = "The Automatic Rotation type for this Rotation Support."))
	EAutomaticRotationType AutomaticRotationType{ EAutomaticRotationType::AutomaticRotation };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation", meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::StopOnAngles", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the rotation when in Automatic Stop mode.\n(Must be a normalized curve.)"))
	UCurveFloat* RotationCurve{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation",
		meta = (Tooltip = "Does the Rotation Support rotate automatically at begin play?"))
	bool bStartAutomatic{ true };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The duration of the start phase of the Automatic Rotation.\n(In seconds)", ClampMin = 0.0f))
	float StartDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the start phase of the Automatic Rotation.\n(Must be a normalized curve.)"))
	UCurveFloat* StartCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The duration of the end phase of the Automatic Rotation.\n(In seconds)", ClampMin = 0.0f))
	float EndDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the end phase of the Automatic Rotation.\n(Must be a normalized curve.)"))
	UCurveFloat* EndCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::StopOnAngles", EditConditionHides),
		meta = (Tooltip = "Choose the automatic stop behavior of the Rotation Support."))
	ERotStopBehavior StopBehavior{ ERotStopBehavior::StopAngleInterval };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::StopOnAngles && StopBehavior == ERotStopBehavior::StopAngleInterval", EditConditionHides),
		meta = (Tooltip = "The angle interval the Rotation Support will do before automatically stop.", ClampMin = 0))
	int StopIntervalAngle{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::StopOnAngles && StopBehavior == ERotStopBehavior::StopAngleInterval", EditConditionHides),
		meta = (Tooltip = "The duration of the stop when stopping during the Automatic Rotation.\n(In seconds)", ClampMin = 0.0f))
	float GlobalStopDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::StopOnAngles && StopBehavior == ERotStopBehavior::StopSpecifedAngles", EditConditionHides),
		meta = (Tooltip = "List of angles values this Rotation Support will automatically stop on.\nAlso have a stop duration parameter for each angle.", ClampMin = 0))
	TArray<FStopDefinedAngle> StopDefinedAngles;
};


USTRUCT(BlueprintType)
struct FAutomaticRotationDatas
{
	GENERATED_BODY()


	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Automatic Rotation for this Rotation Support."))
	UAutomaticRotationBaseDatas* AutomaticRotationDatas{ nullptr };


	//  Override automatic rotation part

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'Automatic Rotation' part of the datas for this component."))
	bool bOverrideAutomaticRotationPart{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideAutomaticRotationPart", EditConditionHides),
		meta = (Tooltip = "The speed of the Automatic Rotation.\nA positive speed make a clockwise rotation, a negative speed make an anticlockwise rotation.\n(In degrees per second)"))
	float RotationSpeed{ 75.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation", meta = (EditCondition = "bOverrideAutomaticRotationPart", EditConditionHides),
		meta = (Tooltip = "The Automatic Rotation type for this Rotation Support."))
	EAutomaticRotationType AutomaticRotationType{ EAutomaticRotationType::AutomaticRotation };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation", meta = (EditCondition = "bOverrideAutomaticRotationPart && AutomaticRotationType == EAutomaticRotationType::StopOnAngles", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the rotation when in Automatic Stop mode.\n(Must be a normalized curve.)"))
	UCurveFloat* RotationCurve{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideAutomaticRotationPart", EditConditionHides),
		meta = (Tooltip = "Does the Rotation Support rotate automatically at begin play?"))
	bool bStartAutomatic{ true };

	
	//  Override start phase part

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'Start Phase' part of the datas for this component."))
	bool bOverrideStartPhasePart{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideStartPhasePart && AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The duration of the start phase of the Automatic Rotation.\n(In seconds)", ClampMin = 0.0f))
	float StartDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideStartPhasePart && AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the start phase of the Automatic Rotation.\n(Must be a normalized curve.)"))
	UCurveFloat* StartCurve{ nullptr };


	//  Override end phase part

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'End Phase' part of the datas for this component"))
	bool bOverrideEndPhasePart{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideEndPhasePart && AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The duration of the end phase of the Automatic Rotation.\n(In seconds)", ClampMin = 0.0f))
	float EndDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideEndPhasePart && AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the end phase of the Automatic Rotation.\n(Must be a normalized curve.)"))
	UCurveFloat* EndCurve{ nullptr };


	//  Override automatic stop part

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::StopOnAngles", EditConditionHides), 
		meta = (Tooltip = "Overrides the 'Automatic Stop' part of the datas for this component."))
	bool bOverrideAutomaticStop{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (EditCondition = "bOverrideAutomaticStop && AutomaticRotationType == EAutomaticRotationType::StopOnAngles", EditConditionHides),
		meta = (Tooltip = "Choose the automatic stop behavior of the Rotation Support."))
	ERotStopBehavior StopBehavior{ ERotStopBehavior::StopAngleInterval };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (EditCondition = "bOverrideAutomaticStop && AutomaticRotationType == EAutomaticRotationType::StopOnAngles && StopBehavior == ERotStopBehavior::StopAngleInterval", EditConditionHides),
		meta = (Tooltip = "The angle interval the Rotation Support will do before automatically stop.", ClampMin = 0))
	int StopIntervalAngle{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (EditCondition = "bOverrideAutomaticStop && AutomaticRotationType == EAutomaticRotationType::StopOnAngles && StopBehavior == ERotStopBehavior::StopAngleInterval", EditConditionHides),
		meta = (Tooltip = "The duration of the stop when stopping during the Automatic Rotation.\n(In seconds)", ClampMin = 0.0f))
	float GlobalStopDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Stop",
		meta = (EditCondition = "bOverrideAutomaticStop && AutomaticRotationType == EAutomaticRotationType::StopOnAngles && StopBehavior == ERotStopBehavior::StopSpecifedAngles", EditConditionHides),
		meta = (Tooltip = "List of angles values this Rotation Support will automatically stop on.\nAlso have a stop duration parameter for each angle.", ClampMin = 0))
	TArray<FStopDefinedAngle> StopDefinedAngles;




	//  Getter functions
	bool IsDataValid();

	float GetRotationSpeed();
	EAutomaticRotationType GetAutomaticRotationType();
	UCurveFloat* GetRotationCurve();
	bool GetStartAutomatic();

	float GetStartDuration();
	UCurveFloat* GetStartCurve();

	float GetEndDuration();
	UCurveFloat* GetEndCurve();

	ERotStopBehavior GetStopBehavior();
	int GetStopIntervalAngle();
	float GetGlobalStopDuration();
	TArray<FStopDefinedAngle> GetStopDefinedAngles();
};