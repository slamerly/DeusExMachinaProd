#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AutoRotInteractionDatas.generated.h"


UCLASS()
class DEUSEXMACHINA_API UAutoRotInteractionBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation Interaction",
		meta = (Tooltip = "If true, the automatic rotation will start or stop on the interaction."))
	bool bStartStop{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation Interaction",
		meta = (Tooltip = "If true, the automatic rotation will go reverse on the interaction.\nNote: If combined with StartStop, will reverse only when the rotation will restart."))
	bool bReverse{ false };
};


USTRUCT(BlueprintType)
struct FAutoRotInteractionDatas
{
	GENERATED_BODY()

	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Auto Rot Interaction for this link."))
	UAutoRotInteractionBaseDatas* AutoRotInteractionDatas{ nullptr };


	//  Override datas

	UPROPERTY(EDitAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the values of the data asset for this link."))
	bool bOverrideValues{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation Interaction", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "If true, the automatic rotation will start or stop on the interaction."))
	bool bStartStop{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Rotation Interaction", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "If true, the automatic rotation will go reverse on the interaction.\nNote: If combined with StartStop, will reverse only when the rotation will restart."))
	bool bReverse{ false };


	//  Getter functions
	bool IsDataValid();

	bool GetStartStop();
	bool GetReverse();
};
