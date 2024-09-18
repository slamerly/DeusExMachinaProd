#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AutomaticRotationDatas.generated.h"


UCLASS()
class DEUSEXMACHINA_API UAutomaticRotationBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation", 
		meta = (Tooltip = "The speed of the Automatic Rotation.\nA positive speed make a clockwise rotation, a negative speed make an anticlockwise rotation.\n(In degrees per second)"))
	float RotationSpeed{ 75.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation",
		meta = (Tooltip = "Does the Rotation Support rotate automatically at begin play?"))
	bool bStartAutomatic{ true };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase",
		meta = (Tooltip = "The duration of the start phase of the Automatic Rotation.\n(In seconds)"))
	float StartDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Start Phase",
		meta = (Tooltip = "The interpolation curve of the start phase of the Automatic Rotation.\n(Must be a normalized curve.)"))
	UCurveFloat* StartCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase",
		meta = (Tooltip = "The duration of the end phase of the Automatic Rotation.\n(In seconds)"))
	float EndDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "End Phase",
		meta = (Tooltip = "The interpolation curve of the end phase of the Automatic Rotation.\n(Must be a normalized curve.)"))
	UCurveFloat* EndCurve{ nullptr };
};


USTRUCT(BlueprintType)
struct FAutomaticRotationDatas
{
	GENERATED_BODY()


	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Automatic Rotation for this Rotation Support."))
	UAutomaticRotationBaseDatas* AutomaticRotationDatas{ nullptr };


	//  Override automatic rotation part

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'Automatic Rotation' part of the datas for this component"))
	bool bOverrideAutomaticRotationPart{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideAutomaticRotationPart", EditConditionHides),
		meta = (Tooltip = "The speed of the Automatic Rotation.\nA positive speed make a clockwise rotation, a negative speed make an anticlockwise rotation.\n(In degrees per second)"))
	float RotationSpeed{ 75.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideAutomaticRotationPart", EditConditionHides),
		meta = (Tooltip = "Does the Rotation Support rotate automatically at begin play?"))
	bool bStartAutomatic{ true };

	
	//  Override start phase part

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'Start Phase' part of the datas for this component"))
	bool bOverrideStartPhasePart{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideStartPhasePart", EditConditionHides),
		meta = (Tooltip = "The duration of the start phase of the Automatic Rotation.\n(In seconds)"))
	float StartDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideStartPhasePart", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the start phase of the Automatic Rotation.\n(Must be a normalized curve.)"))
	UCurveFloat* StartCurve{ nullptr };


	//  Override end phase part

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the 'End Phase' part of the datas for this component"))
	bool bOverrideEndPhasePart{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideEndPhasePart", EditConditionHides),
		meta = (Tooltip = "The duration of the end phase of the Automatic Rotation.\n(In seconds)"))
	float EndDuration{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (EditCondition = "bOverrideEndPhasePart", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the end phase of the Automatic Rotation.\n(Must be a normalized curve.)"))
	UCurveFloat* EndCurve{ nullptr };


	//  Getter functions
	bool IsDataValid();

	float GetRotationSpeed();
	bool GetStartAutomatic();

	float GetStartDuration();
	UCurveFloat* GetStartCurve();

	float GetEndDuration();
	UCurveFloat* GetEndCurve();
};