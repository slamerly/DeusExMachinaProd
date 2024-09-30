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

	/**
	* Modulate an angle by a base 360. (Variant that works with integers)
	* @param	Angle	The angle to modulate.
	* @return			The angle between 0 and 360.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Angles Utils")
	static int ModuloAngleInt(const int Angle);


	/**
	* Get the delta between two angles in degrees, with a negative value if needed.
	* @param	AngleFrom	The from angle.
	* @param	AngleTo		The to angle.
	* @return				The signed delta angle. Will be between -180 and 180.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Angles Utils")
	static float SignedDeltaAngle(const float AngleFrom, const float AngleTo);


	/**
	* Get the delta between two angles in degrees, with a negative value if needed. (Variant that works with integers)
	* @param	AngleFrom	The from angle.
	* @param	AngleTo		The to angle.
	* @return				The signed delta angle. Will be between -180 and 180.
	*/
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Angles Utils")
	static int SignedDeltaAngleInt(const int AngleFrom, const int AngleTo);
};
