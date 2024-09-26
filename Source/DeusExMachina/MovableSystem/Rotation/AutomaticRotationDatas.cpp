#include "AutomaticRotationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FAutomaticRotationDatas::IsDataValid()
{
	return IsValid(AutomaticRotationDatas) ||
		(bOverrideAutomaticRotationPart && bOverrideStartPhasePart && bOverrideEndPhasePart && bOverrideAutomaticStop);
}


float FAutomaticRotationDatas::GetRotationSpeed()
{
	return bOverrideAutomaticRotationPart ? RotationSpeed : AutomaticRotationDatas->RotationSpeed;
}

EAutomaticRotationType FAutomaticRotationDatas::GetAutomaticRotationType()
{
	return bOverrideAutomaticRotationPart ? AutomaticRotationType : AutomaticRotationDatas->AutomaticRotationType;
}

UCurveFloat* FAutomaticRotationDatas::GetRotationCurve()
{
	return bOverrideAutomaticRotationPart ? RotationCurve : AutomaticRotationDatas->RotationCurve;
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


ERotStopBehavior FAutomaticRotationDatas::GetStopBehavior()
{
	return bOverrideAutomaticStop ? StopBehavior : AutomaticRotationDatas->StopBehavior;
}

int FAutomaticRotationDatas::GetStopIntervalAngle()
{
	return bOverrideAutomaticStop ? StopIntervalAngle : AutomaticRotationDatas->StopIntervalAngle;
}

float FAutomaticRotationDatas::GetGlobalStopDuration()
{
	return bOverrideAutomaticStop ? GlobalStopDuration : AutomaticRotationDatas->GlobalStopDuration;
}

TArray<FStopDefinedAngle> FAutomaticRotationDatas::GetStopDefinedAngles()
{
	return bOverrideAutomaticStop ? StopDefinedAngles : AutomaticRotationDatas->StopDefinedAngles;
}
