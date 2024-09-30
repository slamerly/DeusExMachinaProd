#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AutoTransInteractionDatas.generated.h"


UCLASS()
class DEUSEXMACHINA_API UAutoTransInteractionBaseDatas : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation Interaction",
		meta = (Tooltip = "If true, the automatic translation will start or stop on the interaction."))
	bool bStartStop{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation Interaction",
		meta = (Tooltip = "If true, the automatic translation will go reverse on the interaction.\nNote: If combined with StartStop, will reverse only when the translation will restart."))
	bool bReverse{ false };
};


USTRUCT(BlueprintType)
struct FAutoTransInteractionDatas
{
	GENERATED_BODY()

	//  Data Asset

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (Tooltip = "Data Asset with values of Auto Trans Interaction for this link."))
	UAutoTransInteractionBaseDatas* AutoTransInteractionDatas{ nullptr };


	//  Override datas

	UPROPERTY(EDitAnywhere, BlueprintReadOnly, meta = (Tooltip = "Overrides the values of the data asset for this link."))
	bool bOverrideValues{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation Interaction", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "If true, the automatic translation will start or stop on the interaction."))
	bool bStartStop{ false };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Automatic Translation Interaction", meta = (EditCondition = "bOverrideValues", EditConditionHides),
		meta = (Tooltip = "If true, the automatic translation will go reverse on the interaction.\nNote: If combined with StartStop, will reverse only when the translation will restart."))
	bool bReverse{ false };


	//  Getter functions
	bool IsDataValid();

	bool GetStartStop();
	bool GetReverse();
};
