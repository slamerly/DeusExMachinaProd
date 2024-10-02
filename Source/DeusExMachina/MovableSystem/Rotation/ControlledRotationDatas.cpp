#include "ControlledRotationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FControlledRotationDatas::IsDataValid() const
{
	return IsValid(ControlledRotInteractionDatas) || (bOverrideSpeed && bOverrideStartup && bOverrideSnap);
}

float FControlledRotationDatas::GetRotationSpeed() const
{
	return bOverrideSpeed ? RotationSpeed : ControlledRotInteractionDatas->RotationSpeed;
}

float FControlledRotationDatas::GetStartupDuration() const
{
	return bOverrideStartup ? StartupDuration : ControlledRotInteractionDatas->StartupDuration;
}

UCurveFloat* FControlledRotationDatas::GetStartupCurve() const
{
	return bOverrideStartup ? StartupCurve : ControlledRotInteractionDatas->StartupCurve;
}

bool FControlledRotationDatas::GetSnapIgnoreClamp() const
{
	return bOverrideSnap ? bSnapIgnoreClamp : ControlledRotInteractionDatas->bSnapIgnoreClamp;
}

bool FControlledRotationDatas::GetSnapIgnoreRanges() const
{
	return bOverrideSnap ? bSnapIgnoreRanges : ControlledRotInteractionDatas->bSnapIgnoreRanges;
}





// ======================================================
//             Getter Functions (Blueprint)
// ======================================================
float UControlledRotationDatasGet::GetRotationSpeed(const FControlledRotationDatas& Datas)
{
	return Datas.GetRotationSpeed();
}

float UControlledRotationDatasGet::GetStartupDuration(const FControlledRotationDatas& Datas)
{
	return Datas.GetStartupDuration();
}

UCurveFloat* UControlledRotationDatasGet::GetStartupCurve(const FControlledRotationDatas& Datas)
{
	return Datas.GetStartupCurve();
}

bool UControlledRotationDatasGet::GetSnapIgnoreClamp(const FControlledRotationDatas& Datas)
{
	return Datas.GetSnapIgnoreClamp();
}

bool UControlledRotationDatasGet::GetSnapIgnoreRanges(const FControlledRotationDatas& Datas)
{
	return Datas.GetSnapIgnoreRanges();
}
