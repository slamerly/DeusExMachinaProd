#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StandardRotationDatas.generated.h"

UCLASS()
class DEUSEXMACHINA_API UStandardRotationBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Rotation Datas",
		meta = (Tooltip = "The angle that the complete Standard Rotation will apply to the Rotation Support.\nA positive angle will apply a clockwise rotation and a negative angle will apply an anticlockwise rotation.\n(In degrees)"))
	float RotationAngle{ 60.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Rotation Datas",
		meta = (Tooltip = "The time that the complete Standard Rotation will take.\n(In seconds)"))
	float RotationDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Rotation Datas",
		meta = (Tooltip = "The interpolation curve that will smooth the Standard Rotation.\n(Must be a normalized curve)"))
	UCurveFloat* RotationCurve{ nullptr };
};


USTRUCT(BlueprintType)
struct FStandardRotationDatas
{
	GENERATED_BODY()

	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Standard Rotation for this link."))
	UStandardRotationBaseDatas* StandardRotInteractionDatas{ nullptr };


	//  Override datas

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the values of the data asset for this link."))
	bool bOverrideValues{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Rotation Datas", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "The angle that the complete Standard Rotation will apply to the Rotation Support.\nA positive angle will apply a clockwise rotation and a negative angle will apply an anticlockwise rotation.\n(In degrees)"))
	float RotationAngle{ 60.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Rotation Datas", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "The time that the complete Standard Rotation will take.\n(In seconds)"))
	float RotationDuration{ 1.0f };


	UPROPERTY(EDitAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the curve value of the data asset for this link."))
	bool bOverrideCurveValue{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Rotation Datas", meta = (EditCondition = "bOverrideCurveValue", EditConditionHides),
		meta = (Tooltip = "The interpolation curve that will smooth the Standard Rotation.\n(Must be a normalized curve)"))
	UCurveFloat* RotationCurve{ nullptr };


	//  Getter functions
	bool IsDataValid();

	float GetRotationAngle();
	float GetRotationDuration();
	UCurveFloat* GetRotationCurve();
};
