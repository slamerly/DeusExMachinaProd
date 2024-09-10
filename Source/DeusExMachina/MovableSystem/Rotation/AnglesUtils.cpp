#include "AnglesUtils.h"
#include "Kismet/KismetMathLibrary.h"

float UAnglesUtils::ModuloAngle(const float Angle)
{
	const float AngleMod = UKismetMathLibrary::GenericPercent_FloatFloat(Angle, 360.0f);

	if (AngleMod >= 0.0f) return AngleMod;

	return AngleMod + 360.0f;
}
