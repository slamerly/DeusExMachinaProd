#include "StandardTranslationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FStandardTranslationDatas::IsDataValid()
{
	return IsValid(StandardTransInteractionDatas) || (bOverrideValues && bOverrideCurveValue);
}

EStandardTranslationMode FStandardTranslationDatas::GetStandardTranslationMode()
{
	return bOverrideValues ? StandardTranslationMode : StandardTransInteractionDatas->StandardTranslationMode;
}

int FStandardTranslationDatas::GetTranslationSplinePoints()
{
	return bOverrideValues ? TranslationSplinePoints : StandardTransInteractionDatas->TranslationSplinePoints;
}

float FStandardTranslationDatas::GetTranslationSplineDistance()
{
	return bOverrideValues ? TranslationSplineDistance : StandardTransInteractionDatas->TranslationSplineDistance;
}

EStandardTranslationSpeedMode FStandardTranslationDatas::GetStandardTranslationSpeedMode()
{
	return bOverrideValues ? StandardTranslationSpeedMode : StandardTransInteractionDatas->StandardTranslationSpeedMode;
}

float FStandardTranslationDatas::GetTranslationDuration()
{
	return bOverrideValues ? TranslationDuration : StandardTransInteractionDatas->TranslationDuration;
}

float FStandardTranslationDatas::GetTranslationSpeed()
{
	return bOverrideValues ? TranslationSpeed : StandardTransInteractionDatas->TranslationSpeed;
}

UCurveFloat* FStandardTranslationDatas::GetTranslationCurve()
{
	return bOverrideCurveValue ? TranslationCurve : StandardTransInteractionDatas->TranslationCurve;
}
