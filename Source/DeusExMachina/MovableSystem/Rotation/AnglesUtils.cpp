#include "AnglesUtils.h"
#include "Kismet/KismetMathLibrary.h"

float UAnglesUtils::ModuloAngle(const float Angle)
{
	const float AngleMod = UKismetMathLibrary::GenericPercent_FloatFloat(Angle, 360.0f);

	if (AngleMod >= 0.0f) return AngleMod;

	return AngleMod + 360.0f;
}

int UAnglesUtils::ModuloAngleInt(const int Angle)
{
	const int AngleMod = Angle % 360;

	if (AngleMod >= 0) return AngleMod;

	return AngleMod + 360;
}


float UAnglesUtils::SignedDeltaAngle(const float AngleFrom, const float AngleTo)
{
	const float Diff = ModuloAngle(ModuloAngle(AngleTo) - ModuloAngle(AngleFrom));

	if (Diff < 360.0f - Diff) return Diff;

	return -1.0f * (360.0f - Diff);
}

int UAnglesUtils::SignedDeltaAngleInt(const int AngleFrom, const int AngleTo)
{
	const int Diff = ModuloAngleInt(ModuloAngleInt(AngleTo) - ModuloAngleInt(AngleFrom));

	if (Diff < 360 - Diff) return Diff;

	return -1 * (360 - Diff);
}
