#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
		meta = (Tooltip = "The interpolation curve of the rotation when in Automatic Stop mode.\n(Must be a normalized curve)"))
	UCurveFloat* RotationCurve{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation",
		meta = (Tooltip = "Does the Rotation Support rotate automatically at begin play?"))
	bool bStartAutomatic{ true };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The duration of the start phase of the Automatic Rotation.\n(In seconds)", ClampMin = 0.0f))
	float StartDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase", meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the start phase of the Automatic Rotation.\n(Must be a normalized curve)"))
	UCurveFloat* StartCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The duration of the end phase of the Automatic Rotation.\n(In seconds)", ClampMin = 0.0f))
	float EndDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase", meta = (EditCondition = "AutomaticRotationType == EAutomaticRotationType::AutomaticRotation", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the end phase of the Automatic Rotation.\n(Must be a normalized curve)"))
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
	bool IsDataValid() const;

	float GetRotationSpeed() const;
	EAutomaticRotationType GetAutomaticRotationType() const;
	UCurveFloat* GetRotationCurve() const;
	bool GetStartAutomatic() const;

	float GetStartDuration() const;
	UCurveFloat* GetStartCurve() const;

	float GetEndDuration() const;
	UCurveFloat* GetEndCurve() const;

	ERotStopBehavior GetStopBehavior() const;
	int GetStopIntervalAngle() const;
	float GetGlobalStopDuration() const;
	TArray<FStopDefinedAngle> GetStopDefinedAngles() const;
};


UCLASS()
class DEUSEXMACHINA_API UAutomaticRotationDatasGet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	* Get the 'RotationSpeed' value, either the base one or the overriden one if needed out of this struct.
	* @return	The speed of the Automatic Rotation.
				A positive speed make a clockwise rotation, a negative speed make an anticlockwise rotation.
				(In degrees per second)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas", meta = (ReturnDisplayName = "RotationSpeed"))
	static float GetRotationSpeed(const FAutomaticRotationDatas& Datas);

	/**
	* Get the 'AutomaticRotationType' value, either the base one or the overriden one if needed out of this struct.
	* @return	The Automatic Rotation type for this Rotation Support.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas", meta = (ReturnDisplayName = "AutomaticRotationType"))
	static EAutomaticRotationType GetAutomaticRotationType(const FAutomaticRotationDatas& Datas);

	/**
	* Get the 'RotationCurve' value, either the base one or the overriden one if needed out of this struct.
	* @return	The interpolation curve of the rotation when in Automatic Stop mode.
				(Must be a normalized curve)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas", meta = (ReturnDisplayName = "RotationCurve"))
	static UCurveFloat* GetRotationCurve(const FAutomaticRotationDatas& Datas);

	/**
	* Get the 'StartAutomatic' value, either the base one or the overriden one if needed out of this struct.
	* @return	Does the Rotation Support rotate automatically at begin play?
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas", meta = (ReturnDisplayName = "StartAutomatic"))
	static bool GetStartAutomatic(const FAutomaticRotationDatas& Datas);


	/**
	* Get the 'StartDuration' value, either the base one or the overriden one if needed out of this struct.
	* @return	The duration of the start phase of the Automatic Rotation.
				(In seconds)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas|Start Phase", meta = (ReturnDisplayName = "StartDuration"))
	static float GetStartDuration(const FAutomaticRotationDatas& Datas);

	/**
	* Get the 'StartCurve' value, either the base one or the overriden one if needed out of this struct.
	* @return	The interpolation curve of the start phase of the Automatic Rotation.
				(Must be a normalized curve)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas|Start Phase", meta = (ReturnDisplayName = "StartCurve"))
	static UCurveFloat* GetStartCurve(const FAutomaticRotationDatas& Datas);


	/**
	* Get the 'EndDuration' value, either the base one or the overriden one if needed out of this struct.
	* @return	The duration of the end phase of the Automatic Rotation.
				(In seconds)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas|End Phase", meta = (ReturnDisplayName = "EndDuration"))
	static float GetEndDuration(const FAutomaticRotationDatas& Datas);

	/**
	* Get the 'EndCurve' value, either the base one or the overriden one if needed out of this struct.
	* @return	The interpolation curve of the end phase of the Automatic Rotation.
				(Must be a normalized curve)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas|End Phase", meta = (ReturnDisplayName = "EndCurve"))
	static UCurveFloat* GetEndCurve(const FAutomaticRotationDatas& Datas);


	/**
	* Get the 'StopBehavior' value, either the base one or the overriden one if needed out of this struct.
	* @return	The automatic stop behavior of the Rotation Support.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas|Automatic Stop", meta = (ReturnDisplayName = "StopBehavior"))
	static ERotStopBehavior GetStopBehavior(const FAutomaticRotationDatas& Datas);

	/**
	* Get the 'StopIntervalAngle' value, either the base one or the overriden one if needed out of this struct.
	* @return	The angle interval the Rotation Support will do before automatically stop.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas|Automatic Stop", meta = (ReturnDisplayName = "StopIntervalAngle"))
	static int GetStopIntervalAngle(const FAutomaticRotationDatas& Datas);

	/**
	* Get the 'GlobalStopDuration' value, either the base one or the overriden one if needed out of this struct.
	* @return	The duration of the stop when stopping during the Automatic Rotation.
				(In seconds)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas|Automatic Stop", meta = (ReturnDisplayName = "GlobalStopDuration"))
	static float GetGlobalStopDuration(const FAutomaticRotationDatas& Datas);

	/**
	* Get the 'StopDefinedAngles' value, either the base one or the overriden one if needed out of this struct.
	* @return	List of angles values this Rotation Support will automatically stop on.
				Also have a stop duration parameter for each angle.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Automatic Rotation Datas|Automatic Stop", meta = (ReturnDisplayName = "StopDefinedAngles"))
	static TArray<FStopDefinedAngle> GetStopDefinedAngles(const FAutomaticRotationDatas& Datas);
};