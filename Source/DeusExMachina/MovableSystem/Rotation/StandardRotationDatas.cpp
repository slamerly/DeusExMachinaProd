#include "StandardRotationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FStandardRotationDatas::IsDataValid()
{
	return IsValid(StandardRotInteractionDatas) || (bOverrideValues && bOverrideCurveValue);
}

float FStandardRotationDatas::GetRotationAngle()
{
	return bOverrideValues ? RotationAngle : StandardRotInteractionDatas->RotationAngle;
}

float FStandardRotationDatas::GetRotationDuration()
{
	return bOverrideValues ? RotationDuration : StandardRotInteractionDatas->RotationDuration;
}

UCurveFloat* FStandardRotationDatas::GetRotationCurve()
{
	return bOverrideCurveValue ? RotationCurve : StandardRotInteractionDatas->RotationCurve;
}
