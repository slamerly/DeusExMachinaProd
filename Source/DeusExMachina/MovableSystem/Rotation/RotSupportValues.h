#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "RotSupportValues.generated.h"


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
	float ClampLowValue{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp",
		meta = (Tooltip = "The higher clamp value, aka the one that should be used when rotating clockwise.\nCan be out of the - 180 / 180 range to allow complex clamping.\n(In degrees)"))
	float ClampHighValue{ 0.0f };
};


USTRUCT(BlueprintType)
struct FRotSupportValues
{
	GENERATED_BODY()

	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values for this Rotation Support"))
	URotSupportBaseValues* RotSupportValues{ nullptr };


	//  Override clamp datas

	UPROPERTY(EDitAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the clamp values of the data asset for this Rotation Support."))
	bool bOverrideClampValues{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp", meta = (EditCondition = "bOverrideClampValues", EditConditionHides),
		meta = (Tooltip = "Does the rotation support use clamping?"))
	bool bUseClamp{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp", meta = (EditCondition = "bOverrideClampValues", EditConditionHides),
		meta = (Tooltip = "The lower clamp value, aka the one that should be used when rotating anticlockwise.\nCan be out of the - 180 / 180 range to allow complex clamping.\n(In degrees)"))
	float ClampLowValue{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Rotation Clamp", meta = (EditCondition = "bOverrideClampValues", EditConditionHides),
		meta = (Tooltip = "The higher clamp value, aka the one that should be used when rotating clockwise.\nCan be out of the - 180 / 180 range to allow complex clamping.\n(In degrees)"))
	float ClampHighValue{ 0.0f };


	//  Getter functions
	bool IsDataValid();

	bool GetUseClamp();
	float GetClampLowValue();
	float GetClampHighValue();
};
