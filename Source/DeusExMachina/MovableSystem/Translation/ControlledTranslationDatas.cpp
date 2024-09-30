#include "ControlledTranslationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FControlledTranslationDatas::IsDataValid()
{
	return IsValid(ControlledTransInteractionDatas) || (bOverrideSpeed && bOverrideStartup && bOverrideSnap && bOverrideSnapCurves);
}


float FControlledTranslationDatas::GetTranslationSpeed()
{
	return bOverrideSpeed ? TranslationSpeed : ControlledTransInteractionDatas->TranslationSpeed;
}

float FControlledTranslationDatas::GetStartupDuration()
{
	return bOverrideStartup ? StartupDuration : ControlledTransInteractionDatas->StartupDuration;
}

UCurveFloat* FControlledTranslationDatas::GetStartupCurve()
{
	return bOverrideStartup ? StartupCurve : ControlledTransInteractionDatas->StartupCurve;
}


bool FControlledTranslationDatas::GetUseSnap()
{
	return bOverrideSnap ? bUseSnap : ControlledTransInteractionDatas->bUseSnap;
}

float FControlledTranslationDatas::GetSnapPreference()
{
	return bOverrideSnap ? SnapPreference : ControlledTransInteractionDatas->SnapPreference;
}

float FControlledTranslationDatas::GetSnapSpeed()
{
	return bOverrideSnap ? SnapSpeed : ControlledTransInteractionDatas->SnapSpeed;
}

UCurveFloat* FControlledTranslationDatas::GetSnapCurveNeutralReverse()
{
	return bOverrideSnapCurves ? SnapCurveNeutralReverse : ControlledTransInteractionDatas->SnapCurveNeutralReverse;
}

UCurveFloat* FControlledTranslationDatas::GetSnapCurveContinue()
{
	return bOverrideSnapCurves ? SnapCurveContinue : ControlledTransInteractionDatas->SnapCurveContinue;
}
