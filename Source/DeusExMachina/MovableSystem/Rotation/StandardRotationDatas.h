#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Kismet/BlueprintFunctionLibrary.h"
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
	bool IsDataValid() const;

	float GetRotationAngle() const;
	float GetRotationDuration() const;
	UCurveFloat* GetRotationCurve() const;
};


UCLASS()
class DEUSEXMACHINA_API UStandardRotationDatasGet : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:

	/**
	* Get the 'RotationAngle' value, either the base one or the overriden one if needed out of this struct.
	* @return	The angle that the complete Standard Rotation will apply to the Rotation Support.
				A positive angle will apply a clockwise rotation and a negative angle will apply an anticlockwise rotation.
				(In degrees)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Rotation Datas", meta = (ReturnDisplayName = "RotationAngle"))
	static float GetRotationAngle(const FStandardRotationDatas& Datas);

	/**
	* Get the 'RotationDuration' value, either the base one or the overriden one if needed out of this struct.
	* @return	The time that the complete Standard Rotation will take.
				(In seconds)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Rotation Datas", meta = (ReturnDisplayName = "RotationDuration"))
	static float GetRotationDuration(const FStandardRotationDatas& Datas);

	/**
	* Get the 'RotationCurve' value, either the base one or the overriden one if needed out of this struct.
	* @return	The interpolation curve that will smooth the Standard Rotation.
				(Must be a normalized curve)
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Standard Rotation Datas", meta = (ReturnDisplayName = "RotationCurve"))
	static UCurveFloat* GetRotationCurve(const FStandardRotationDatas& Datas);
};