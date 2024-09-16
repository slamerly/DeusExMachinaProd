#include "AutomaticRotationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FAutomaticRotationDatas::IsDataValid()
{
	return IsValid(AutomaticRotationDatas) ||
		(bOverrideAutomaticRotationPart && bOverrideStartPhasePart && bOverrideEndPhasePart);
}

float FAutomaticRotationDatas::GetRotationSpeed()
{
	return bOverrideAutomaticRotationPart ? RotationSpeed : AutomaticRotationDatas->RotationSpeed;
}

bool FAutomaticRotationDatas::GetStartAutomatic()
{
	return bOverrideAutomaticRotationPart ? bStartAutomatic : AutomaticRotationDatas->bStartAutomatic;
}

float FAutomaticRotationDatas::GetStartDuration()
{
	return bOverrideStartPhasePart ? StartDuration : AutomaticRotationDatas->StartDuration;
}

UCurveFloat* FAutomaticRotationDatas::GetStartCurve()
{
	return bOverrideStartPhasePart ? StartCurve : AutomaticRotationDatas->StartCurve;
}

float FAutomaticRotationDatas::GetEndDuration()
{
	return bOverrideEndPhasePart ? EndDuration : AutomaticRotationDatas->EndDuration;
}

UCurveFloat* FAutomaticRotationDatas::GetEndCurve()
{
	return bOverrideEndPhasePart ? EndCurve : AutomaticRotationDatas->EndCurve;
}
