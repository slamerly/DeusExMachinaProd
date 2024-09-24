#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ControlledTranslationDatas.generated.h"


UCLASS()
class DEUSEXMACHINA_API UControlledTranslationBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Speed",
		meta = (Tooltip = "The speed of the Translation Support when performing a Controlled Translation.\n(In cm/s)"))
	float TranslationSpeed{ 60.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Startup",
		meta = (Tooltip = "The duration of the 'startup' phase of the Controlled Translation.\nThe startup phase occurs when the translation support starts moving.\n(In seconds)"))
	float StartupDuration{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Startup",
		meta = (Tooltip = "The interpolation curve of the 'startup' phase of the Controlled Translation.\n(Must be a normalized curve)"))
	UCurveFloat* StartupCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap",
		meta = (Tooltip = "Does the Translation Support snap on a spline point when releasing the control?"))
	bool bUseSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (ClampMin = 0.0f, ClampMax = 1.0f),
		meta = (Tooltip = "Does the Translation Support prefer snapping to the spline point 0 or spline point 1"))
	float SnapPreference{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap",
		meta = (Tooltip = "The speed of the Translation Support when snapping.\nNote that the curves can make it feel faster or slower in game.\n(In cm/s)"))
	float SnapSpeed{ 60.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap",
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The Translation Support is not moving (neutral)\n- The snap goes in the opposite direction than the Translation Support movement (reverse)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveNeutralReverse{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap",
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The snap goes in the same direction than the Translation Support movement (continue)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveContinue{ nullptr };
};


USTRUCT(BlueprintType)
struct FControlledTranslationDatas
{
	GENERATED_BODY()

	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Controlled Translation for this link."))
	UControlledTranslationBaseDatas* ControlledTransInteractionDatas{ nullptr };


	//  Override datas

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the speed value of the data asset for this link."))
	bool bOverrideSpeed{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Speed", meta = (EditCondition = "bOverrideSpeed", EditConditionHides),
		meta = (Tooltip = "The speed of the Translation Support when performing a Controlled Translation.\n(In cm/s)"))
	float TranslationSpeed{ 60.0f };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the startup values of the data asset for this link."))
	bool bOverrideStartup{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Startup", meta = (EditCondition = "bOverrideStartup", EditConditionHides),
		meta = (Tooltip = "The duration of the 'startup' phase of the Controlled Translation.\nThe startup phase occurs when the translation support starts moving.\n(In seconds)"))
	float StartupDuration{ 0.8f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Startup", meta = (EditCondition = "bOverrideStartup", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the 'startup' phase of the Controlled Translation.\n(Must be a normalized curve)"))
	UCurveFloat* StartupCurve{ nullptr };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the snap values of the data asset for this link."))
	bool bOverrideSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (EditCondition = "bOverrideSnap", EditConditionHides),
		meta = (Tooltip = "Does the Translation Support snap on a spline point when releasing the control?"))
	bool bUseSnap{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (ClampMin = 0.0f, ClampMax = 1.0f, EditCondition = "bOverrideSnap", EditConditionHides),
		meta = (Tooltip = "Does the Translation Support prefer snapping to the spline point 0 or spline point 1"))
	float SnapPreference{ 0.5f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (EditCondition = "bOverrideSnap", EditConditionHides),
		meta = (Tooltip = "The speed of the Translation Support when snapping.\nNote that the curves can make it feel faster or slower in game.\n(In cm/s)"))
	float SnapSpeed{ 60.0f };


	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the snap curves of the data asset for this link."))
	bool bOverrideSnapCurves{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (EditCondition = "bOverrideSnapCurves", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The Translation Support is not moving (neutral)\n- The snap goes in the opposite direction than the Translation Support movement (reverse)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveNeutralReverse{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Controlled Translation Snap", meta = (EditCondition = "bOverrideSnapCurves", EditConditionHides),
		meta = (Tooltip = "The interpolation curve of the snapping used when:\n- The snap goes in the same direction than the Translation Support movement (continue)\n(Must be a normalized curve)"))
	UCurveFloat* SnapCurveContinue{ nullptr };


	//  Getter functions
	bool IsDataValid();

	float GetTranslationSpeed();
	float GetStartupDuration();
	UCurveFloat* GetStartupCurve();

	bool GetUseSnap();
	float GetSnapPreference();
	float GetSnapSpeed();
	UCurveFloat* GetSnapCurveNeutralReverse();
	UCurveFloat* GetSnapCurveContinue();
};