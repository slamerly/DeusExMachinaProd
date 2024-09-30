#pragma once

#include "CoreMinimal.h"
#include "TranslationBehaviorBase.h"
#include "TranslationBehaviorStandard.generated.h"


UCLASS(ClassGroup = (MovableSystem), meta = (BlueprintSpawnableComponent, DisplayName = "Translation Behavior Standard", Tooltip = "Component to add to a Translation Support if you want it to be able to perform a standard translation."))
class DEUSEXMACHINA_API UTranslationBehaviorStandard : public UTranslationBehaviorBase
{
	GENERATED_BODY()

public:	
	UTranslationBehaviorStandard();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



// ======================================================
//              Control Standard Translation
// ======================================================
public:
	/**
	* Function to call to start the standard translation of this component.
	* @param	Datas			The standard translation datas
	* @param	bForceStart		(optionnal) Force the standard translation to start even if the component is already performing a standard translation
	*/
	UFUNCTION(BlueprintCallable, Category = "Standard Translation")
	void StartStandardTranslation(struct FStandardTranslationDatas Datas, bool bForceStart = false);

	/**
	* Function to call to stop every movement of this component.
	*/
	UFUNCTION(BlueprintCallable, Category = "Standard Translation")
	void CancelStandardTranslation();


// ======================================================
//                   Helper Functions
// ======================================================
protected:
	bool IsStandardTransValid(struct FStandardTranslationDatas Datas);
	float ConvertSplinePointsToSplineDistance(struct FStandardTranslationDatas Datas, int& DestSplinePoint);


// ======================================================
//         Standard Translation Internal Variables
// ======================================================
protected:
	bool bCurrentlyMoving{ false };

	float TranslationDuration{ 0.0f };
	float TranslationTimer{ 0.0f };
	float TranslationDistance{ 0.0f };
	UCurveFloat* TranslationCurve{ nullptr };

	float LastFrameDistanceDone{ 0.0f };
	int ForceSplinePoint{ -1 };
};
