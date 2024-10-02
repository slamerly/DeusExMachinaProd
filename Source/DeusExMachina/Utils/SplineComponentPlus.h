#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "SplineComponentPlus.generated.h"

/**
* A classic Spline Mesh Component with additionnal functionnalities
*/
UCLASS(ClassGroup = MovSysCustomComponent, meta = (BlueprintSpawnableComponent), Blueprintable)
class DEUSEXMACHINA_API USplineComponentPlus : public USplineComponent
{
	GENERATED_BODY()
	

public:
	/** Called when the spline is updated */
	virtual void UpdateSpline() override;

	UPROPERTY(EditAnywhere, Category = "Spline Component Plus")
	bool bRectTangents{ false };
};
