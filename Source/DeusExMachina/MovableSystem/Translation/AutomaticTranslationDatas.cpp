#include "AutomaticTranslationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FAutomaticTranslationDatas::IsDataValid()
{
	return IsValid(AutomaticTranslationDatas) ||
		(bOverrideAutomaticTranslation && bOverrideAutomaticStop && bOverrideStartPhase && bOverrideEndPhase);
}


float FAutomaticTranslationDatas::GetTranslationSpeed()
{
	return bOverrideAutomaticTranslation ? TranslationSpeed : AutomaticTranslationDatas->TranslationSpeed;
}

bool FAutomaticTranslationDatas::GetStartAutomatic()
{
	return bOverrideAutomaticTranslation ? bStartAutomatic : AutomaticTranslationDatas->bStartAutomatic;
}


EStopBehavior FAutomaticTranslationDatas::GetStopBehavior()
{
	return bOverrideAutomaticStop ? StopBehavior : AutomaticTranslationDatas->StopBehavior;
}

float FAutomaticTranslationDatas::GetStopDuration()
{
	return bOverrideAutomaticStop ? StopDuration : AutomaticTranslationDatas->StopDuration;
}

TArray<int> FAutomaticTranslationDatas::GetStopSplineIndex()
{
	return bOverrideAutomaticStop ? StopSplineIndex : AutomaticTranslationDatas->StopSplineIndex;
}


float FAutomaticTranslationDatas::GetStartDuration()
{
	return bOverrideStartPhase ? StartDuration : AutomaticTranslationDatas->StartDuration;
}

UCurveFloat* FAutomaticTranslationDatas::GetStartCurve()
{
	return bOverrideStartPhase ? StartCurve : AutomaticTranslationDatas->StartCurve;
}


float FAutomaticTranslationDatas::GetEndDuration()
{
	return bOverrideEndPhase ? EndDuration : AutomaticTranslationDatas->EndDuration;
}

UCurveFloat* FAutomaticTranslationDatas::GetEndCurve()
{
	return bOverrideEndPhase ? EndCurve : AutomaticTranslationDatas->EndCurve;
}
