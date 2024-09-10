#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "AnglesUtils.generated.h"


UCLASS()
class DEUSEXMACHINA_API UAnglesUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	* Modulate an angle by a base 360.
	* @param	Angle	The angle to modulate.
	* @return			The angle between 0 and 360.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Angles Utils")
	static float ModuloAngle(const float Angle);
};
