#include "ControlledRotationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FControlledRotationDatas::IsDataValid()
{
	return IsValid(ControlledRotInteractionDatas) || (bOverrideSpeed && bOverrideStartup && bOverrideSnap);
}

float FControlledRotationDatas::GetRotationSpeed()
{
	return bOverrideSpeed ? RotationSpeed : ControlledRotInteractionDatas->RotationSpeed;
}

float FControlledRotationDatas::GetStartupDuration()
{
	return bOverrideStartup ? StartupDuration : ControlledRotInteractionDatas->StartupDuration;
}

UCurveFloat* FControlledRotationDatas::GetStartupCurve()
{
	return bOverrideStartup ? StartupCurve : ControlledRotInteractionDatas->StartupCurve;
}

bool FControlledRotationDatas::GetSnapIgnoreClamp()
{
	return bOverrideSnap ? bSnapIgnoreClamp : ControlledRotInteractionDatas->bSnapIgnoreClamp;
}

bool FControlledRotationDatas::GetSnapIgnoreRanges()
{
	return bOverrideSnap ? bSnapIgnoreRanges : ControlledRotInteractionDatas->bSnapIgnoreRanges;
}
