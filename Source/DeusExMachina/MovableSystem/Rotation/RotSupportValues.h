#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RotSupportValues.generated.h"


USTRUCT(BlueprintType)
struct FSnapValue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Angle to which the rotation support can snap.\n(Angle between 0 and 360 degrees)", ClampMin = 0, ClampMax = 360))
	int SnapAngle{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Limit the range of this snap value."))
	bool bLimitedRangeSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The lower range of the snap.\n(In degrees)\n0 means that it can't snap from a lower angle.", ClampMin = 0,
		EditCondition = "bLimitedRangeSnap", EditConditionHides))
	int SnapRangeLower{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "The upper range of the snap.\n(In degrees)\n0 means that it can't snap from a upper angle.", ClampMin = 0,
		EditCondition = "bLimitedRangeSnap", EditConditionHides))
	int SnapRangeUpper{ 0 };
};


UCLASS()
class DEUSEXMACHINA_API URotSupportBaseValues : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp",
		meta = (Tooltip = "Does the rotation support use clamping?"))
	bool bUseClamp{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp",
		meta = (Tooltip = "The lower clamp value, aka the one that should be used when rotating anticlockwise.\nCan be out of the - 180 / 180 range to allow complex clamping.\n(In degrees)"))
	int ClampLowValue{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp",
		meta = (Tooltip = "The higher clamp value, aka the one that should be used when rotating clockwise.\nCan be out of the - 180 / 180 range to allow complex clamping.\n(In degrees)"))
	int ClampHighValue{ 0 };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap",
		meta = (Tooltip = "Does the rotation support use snaping?"))
	bool bUseSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap",
		meta = (Tooltip = "Angles to which the rotation support can snap.\nSnap angles are between 0 and 360 degrees and will take the modulated inner angle of the support to search for snap."))
	TArray<FSnapValue> SnapValues;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (ClampMin = 0.0f, ClampMax = 1.0f),
		meta = (Tooltip = "When searching for the nearest snap, the advantage it will give to the direction the support was rotating to.\nFor exemple, an advantage of 0.7 means that it will snap to the \"destination\" if less than 70 % of the way is missing."))
	float SnapDirectionAdvantage{ 0.65f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (ClampMin = 0.0f),
		meta = (Tooltip = "How many seconds does it remember the last inputed direction when releasing the joystick.\nFor the 'SnapDirectionAdvantage' parameter."))
	float SnapDirectionDelay{ 0.3f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (ClampMin = 0.0f),
		meta = (Tooltip = "The speed of the rotation support when snapping.\n(In degrees per second)"))
	float SnapSpeed{ 90.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap",
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The rotation support is not moving (neutral)\n- The snap goes in the opposite direction than the rotation support movement (reverse)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveNeutralReverse{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap",
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The snap goes in the same direction than the rotation support movement (continue)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveContinue{ nullptr };
};


USTRUCT(BlueprintType)
struct FClampValuesOverride
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the clamp values of the data asset for this Rotation Support."))
	bool bOverrideClampValues{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp", meta = (EditCondition = "bOverrideClampValues", EditConditionHides),
		meta = (Tooltip = "Does the rotation support use clamping?"))
	bool bUseClamp{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp", meta = (EditCondition = "bOverrideClampValues", EditConditionHides),
		meta = (Tooltip = "The lower clamp value, aka the one that should be used when rotating anticlockwise.\nCan be out of the - 180 / 180 range to allow complex clamping.\n(In degrees)"))
	int ClampLowValue{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp", meta = (EditCondition = "bOverrideClampValues", EditConditionHides),
		meta = (Tooltip = "The higher clamp value, aka the one that should be used when rotating clockwise.\nCan be out of the - 180 / 180 range to allow complex clamping.\n(In degrees)"))
	int ClampHighValue{ 0 };
};


USTRUCT(BlueprintType)
struct FSnapValuesOverride
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the snap values of the data asset for this Rotation Support."))
	bool bOverrideSnapValues{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (EditCondition = "bOverrideSnapValues", EditConditionHides),
		meta = (Tooltip = "Does the rotation support use snaping?"))
	bool bUseSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (EditCondition = "bOverrideSnapValues", EditConditionHides),
		meta = (Tooltip = "Angles to which the rotation support can snap.\nSnap angles are between 0 and 360 degrees and will take the modulated inner angle of the support to search for snap."))
	TArray<FSnapValue> SnapValues;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (ClampMin = 0.0f, ClampMax = 1.0f), meta = (EditCondition = "bOverrideSnapValues", EditConditionHides),
		meta = (Tooltip = "When searching for the nearest snap, the advantage it will give to the direction the support was rotating to.\nFor exemple, an advantage of 0.7 means that it will snap to the \"destination\" if less than 70 % of the way is missing."))
	float SnapDirectionAdvantage{ 0.65f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (ClampMin = 0.0f), meta = (EditCondition = "bOverrideSnapValues", EditConditionHides),
		meta = (Tooltip = "How many seconds does it remember the last inputed direction when releasing the joystick.\nFor the 'SnapDirectionAdvantage' parameter."))
	float SnapDirectionDelay{ 0.3f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (ClampMin = 0.0f), meta = (EditCondition = "bOverrideSnapValues", EditConditionHides),
		meta = (Tooltip = "The speed of the rotation support when snapping.\n(In degrees per second)"))
	float SnapSpeed{ 90.0f };
};


USTRUCT(BlueprintType)
struct FSnapCurvesOverride
{
	GENERATED_BODY()


	UPROPERTY(EDitAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the snap curves of the data asset for this Rotation Support."))
	bool bOverrideSnapCurves{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (EditCondition = "bOverrideSnapCurves", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The rotation support is not moving (neutral)\n- The snap goes in the opposite direction than the rotation support movement (reverse)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveNeutralReverse{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Snap", meta = (EditCondition = "bOverrideSnapCurves", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The snap goes in the same direction than the rotation support movement (continue)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveContinue{ nullptr };
};


USTRUCT(BlueprintType)
struct FRotSupportValues
{
	GENERATED_BODY()

	//  Data Asset
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values for this Rotation Support"))
	URotSupportBaseValues* RotSupportValues{ nullptr };


	//  Override clamp datas
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FClampValuesOverride ClampValuesOverride;


	//  Override snap values
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSnapValuesOverride SnapValuesOverride;


	//  Override snap curves
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FSnapCurvesOverride SnapCurvesOverride;


	//  Getter functions
	bool IsDataValid() const;

	bool GetUseClamp() const;
	int GetClampLowValue() const;
	int GetClampHighValue() const;

	bool GetUseSnap() const;
	TArray<FSnapValue> GetSnapValues() const;
	float GetSnapDirectionAdvantage() const;
	float GetSnapDirectionDelay() const;
	float GetSnapSpeed() const;

	UCurveFloat* GetSnapCurveNeutralReverse() const;
	UCurveFloat* GetSnapCurveContinue() const;
};
