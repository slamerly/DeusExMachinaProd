#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StandardTranslationDatas.generated.h"


UENUM(BlueprintType)
enum class EStandardTranslationMode : uint8
{
	MoveSplinePoints = 0 UMETA(Tooltip = "In this mode, the Translation Support will move on the spline for a number of points."),
	MoveDistance = 1 UMETA(Tooltip = "In this mode, the Translation Support will move on the spline for a fixed distance.")
};


UENUM(BlueprintType)
enum class EStandardTranslationSpeedMode : uint8
{
	TranslationDuration = 0 UMETA(Tooltip = "In this mode, the Translation Support speed will vary so the translation always take the same duration."),
	TranslationSpeed = 1 UMETA(Tooltip = "In this mode, the Translation Support speed will be always the same (except the spline interpolation).")
};


UCLASS()
class DEUSEXMACHINA_API UStandardTranslationBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (Tooltip = "The standard translation mode to use for this Translation Support."))
	EStandardTranslationMode StandardTranslationMode{ EStandardTranslationMode::MoveSplinePoints };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas", 
		meta = (EditCondition = "StandardTranslationMode == EStandardTranslationMode::MoveSplinePoints", EditConditionHides),
		meta = (Tooltip = "The number of spline points to move this Translation Support (0 mean the support won't move).\nA negative value will make the support going reverse."))
	int TranslationSplinePoints{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "StandardTranslationMode == EStandardTranslationMode::MoveDistance", EditConditionHides),
		meta = (Tooltip = "The distance on the spline this Translation Support will do (0 mean the support won't move).\nA negative value will make the support going reverse.\n(In cm)"))
	float TranslationSplineDistance{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (Tooltip = "The standard translation speed mode to use for this Translation Support."))
	EStandardTranslationSpeedMode StandardTranslationSpeedMode{ EStandardTranslationSpeedMode::TranslationDuration };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "StandardTranslationSpeedMode == EStandardTranslationSpeedMode::TranslationDuration", EditConditionHides),
		meta = (Tooltip = "The time that the complete Standard Translation will take.\n(In seconds)", ClampMin = 0.0f))
	float TranslationDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "StandardTranslationSpeedMode == EStandardTranslationSpeedMode::TranslationSpeed", EditConditionHides),
		meta = (Tooltip = "The speed of the Standard Translation.\n(In cm/s)", ClampMin = 0.0f))
	float TranslationSpeed{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (Tooltip = "The interpolation curve that will smooth the Standard Translation.\n(Must be a normalized curve)"))
	UCurveFloat* TranslationCurve{ nullptr };
};


USTRUCT(BlueprintType)
struct FStandardTranslationDatas
{
	GENERATED_BODY()


	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Standard Translation for this link."))
	UStandardTranslationBaseDatas* StandardTransInteractionDatas{ nullptr };


	//  Override datas

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the values of the data asset for this link."))
	bool bOverrideValues{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "The standard translation mode to use for this Translation Support."))
	EStandardTranslationMode StandardTranslationMode{ EStandardTranslationMode::MoveSplinePoints };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "bOverrideValues && StandardTranslationMode == EStandardTranslationMode::MoveSplinePoints", EditConditionHides),
		meta = (Tooltip = "The number of spline points to move this Translation Support (0 mean the support won't move).\nA negative value will make the support going reverse."))
	int TranslationSplinePoints{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "bOverrideValues && StandardTranslationMode == EStandardTranslationMode::MoveDistance", EditConditionHides),
		meta = (Tooltip = "The distance on the spline this Translation Support will do (0 mean the support won't move).\nA negative value will make the support going reverse.\n(In cm)"))
	float TranslationSplineDistance{ 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "The standard translation speed mode to use for this Translation Support."))
	EStandardTranslationSpeedMode StandardTranslationSpeedMode{ EStandardTranslationSpeedMode::TranslationDuration };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "bOverrideValues && StandardTranslationSpeedMode == EStandardTranslationSpeedMode::TranslationDuration", EditConditionHides),
		meta = (Tooltip = "The time that the complete Standard Translation will take.\n(In seconds)", ClampMin = 0.0f))
	float TranslationDuration{ 1.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas",
		meta = (EditCondition = "bOverrideValues && StandardTranslationSpeedMode == EStandardTranslationSpeedMode::TranslationSpeed", EditConditionHides),
		meta = (Tooltip = "The speed of the Standard Translation.\n(In cm/s)", ClampMin = 0.0f))
	float TranslationSpeed{ 1.0f };


	UPROPERTY(EDitAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the curve value of the data asset for this link."))
	bool bOverrideCurveValue{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Standard Translation Datas", meta = (EditCondition = "bOverrideCurveValue", EditConditionHides),
		meta = (Tooltip = "The interpolation curve that will smooth the Standard Translation.\n(Must be a normalized curve)"))
	UCurveFloat* TranslationCurve{ nullptr };


	//  Getter functions
	bool IsDataValid();

	EStandardTranslationMode GetStandardTranslationMode();
	int GetTranslationSplinePoints();
	float GetTranslationSplineDistance();
	EStandardTranslationSpeedMode GetStandardTranslationSpeedMode();
	float GetTranslationDuration();
	float GetTranslationSpeed();
	UCurveFloat* GetTranslationCurve();
};
