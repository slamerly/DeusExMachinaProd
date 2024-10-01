#include "ControlledTranslationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FControlledTranslationDatas::IsDataValid() const
{
	return IsValid(ControlledTransInteractionDatas) || (bOverrideSpeed && bOverrideStartup && bOverrideSnap && bOverrideSnapCurves);
}


float FControlledTranslationDatas::GetTranslationSpeed() const
{
	return bOverrideSpeed ? TranslationSpeed : ControlledTransInteractionDatas->TranslationSpeed;
}

float FControlledTranslationDatas::GetStartupDuration() const
{
	return bOverrideStartup ? StartupDuration : ControlledTransInteractionDatas->StartupDuration;
}

UCurveFloat* FControlledTranslationDatas::GetStartupCurve() const
{
	return bOverrideStartup ? StartupCurve : ControlledTransInteractionDatas->StartupCurve;
}


bool FControlledTranslationDatas::GetUseSnap() const
{
	return bOverrideSnap ? bUseSnap : ControlledTransInteractionDatas->bUseSnap;
}

float FControlledTranslationDatas::GetSnapPreference() const
{
	return bOverrideSnap ? SnapPreference : ControlledTransInteractionDatas->SnapPreference;
}

float FControlledTranslationDatas::GetSnapSpeed() const
{
	return bOverrideSnap ? SnapSpeed : ControlledTransInteractionDatas->SnapSpeed;
}

UCurveFloat* FControlledTranslationDatas::GetSnapCurveNeutralReverse() const
{
	return bOverrideSnapCurves ? SnapCurveNeutralReverse : ControlledTransInteractionDatas->SnapCurveNeutralReverse;
}

UCurveFloat* FControlledTranslationDatas::GetSnapCurveContinue() const
{
	return bOverrideSnapCurves ? SnapCurveContinue : ControlledTransInteractionDatas->SnapCurveContinue;
}





// ======================================================
//             Getter Functions (Blueprint)
// ======================================================
float UControlledTranslationDatasGet::GetTranslationSpeed(const FControlledTranslationDatas& Datas)
{
	return Datas.GetTranslationSpeed();
}

float UControlledTranslationDatasGet::GetStartupDuration(const FControlledTranslationDatas& Datas)
{
	return Datas.GetStartupDuration();
}

UCurveFloat* UControlledTranslationDatasGet::GetStartupCurve(const FControlledTranslationDatas& Datas)
{
	return Datas.GetStartupCurve();
}


bool UControlledTranslationDatasGet::GetUseSnap(const FControlledTranslationDatas& Datas)
{
	return Datas.GetUseSnap();
}

float UControlledTranslationDatasGet::GetSnapPreference(const FControlledTranslationDatas& Datas)
{
	return Datas.GetSnapPreference();
}

float UControlledTranslationDatasGet::GetSnapSpeed(const FControlledTranslationDatas& Datas)
{
	return Datas.GetSnapSpeed();
}

UCurveFloat* UControlledTranslationDatasGet::GetSnapCurveNeutralReverse(const FControlledTranslationDatas& Datas)
{
	return Datas.GetSnapCurveNeutralReverse();
}

UCurveFloat* UControlledTranslationDatasGet::GetSnapCurveContinue(const FControlledTranslationDatas& Datas)
{
	return Datas.GetSnapCurveContinue();
}
