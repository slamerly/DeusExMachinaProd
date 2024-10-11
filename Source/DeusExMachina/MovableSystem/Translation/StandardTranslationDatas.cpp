#include "StandardTranslationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FStandardTranslationDatas::IsDataValid() const
{
	return IsValid(StandardTransInteractionDatas) || (bOverrideValues && bOverrideCurveValue);
}

EStandardTranslationMode FStandardTranslationDatas::GetStandardTranslationMode() const
{
	return bOverrideValues ? StandardTranslationMode : StandardTransInteractionDatas->StandardTranslationMode;
}

int FStandardTranslationDatas::GetTranslationSplinePoints() const
{
	return bOverrideValues ? TranslationSplinePoints : StandardTransInteractionDatas->TranslationSplinePoints;
}

float FStandardTranslationDatas::GetTranslationSplineDistance() const
{
	return bOverrideValues ? TranslationSplineDistance : StandardTransInteractionDatas->TranslationSplineDistance;
}

EStandardTranslationSpeedMode FStandardTranslationDatas::GetStandardTranslationSpeedMode() const
{
	return bOverrideValues ? StandardTranslationSpeedMode : StandardTransInteractionDatas->StandardTranslationSpeedMode;
}

float FStandardTranslationDatas::GetTranslationDuration() const
{
	return bOverrideValues ? TranslationDuration : StandardTransInteractionDatas->TranslationDuration;
}

float FStandardTranslationDatas::GetTranslationSpeed() const
{
	return bOverrideValues ? TranslationSpeed : StandardTransInteractionDatas->TranslationSpeed;
}

UCurveFloat* FStandardTranslationDatas::GetTranslationCurve() const
{
	return bOverrideCurveValue ? TranslationCurve : StandardTransInteractionDatas->TranslationCurve;
}





// ======================================================
//             Getter Functions (Blueprint)
// ======================================================
EStandardTranslationMode UStandardTranslationDatasGet::GetStandardTranslationMode(const FStandardTranslationDatas& Datas)
{
	return Datas.GetStandardTranslationMode();
}

int UStandardTranslationDatasGet::GetTranslationSplinePoints(const FStandardTranslationDatas& Datas)
{
	return Datas.GetTranslationSplinePoints();
}

float UStandardTranslationDatasGet::GetTranslationSplineDistance(const FStandardTranslationDatas& Datas)
{
	return Datas.GetTranslationSplineDistance();
}

EStandardTranslationSpeedMode UStandardTranslationDatasGet::GetStandardTranslationSpeedMode(const FStandardTranslationDatas& Datas)
{
	return Datas.GetStandardTranslationSpeedMode();
}

float UStandardTranslationDatasGet::GetTranslationDuration(const FStandardTranslationDatas& Datas)
{
	return Datas.GetTranslationDuration();
}

float UStandardTranslationDatasGet::GetTranslationSpeed(const FStandardTranslationDatas& Datas)
{
	return Datas.GetTranslationSpeed();
}

UCurveFloat* UStandardTranslationDatasGet::GetTranslationCurve(const FStandardTranslationDatas& Datas)
{
	return Datas.GetTranslationCurve();
}



// ======================================================
//            Custom Make Function (Blueprint)
// ======================================================
FStandardTranslationDatas UStandardTranslationDatasGet::MakeStandardTranslationDatas(EStandardTranslationMode StandardTranslationMode, int TranslationSplinePoints, float TranslationSplineDistance, EStandardTranslationSpeedMode StandardTranslationSpeedMode, float TranslationDuration, float TranslationSpeed, UCurveFloat* TranslationCurve)
{
	FStandardTranslationDatas Datas;

	Datas.bOverrideValues = true;
	Datas.bOverrideCurveValue = true;

	Datas.StandardTranslationMode = StandardTranslationMode;
	Datas.TranslationSplinePoints = TranslationSplinePoints;
	Datas.TranslationSplineDistance = TranslationSplineDistance;
	Datas.StandardTranslationSpeedMode = StandardTranslationSpeedMode;
	Datas.TranslationDuration = TranslationDuration;
	Datas.TranslationSpeed = TranslationSpeed;
	Datas.TranslationCurve = TranslationCurve;

	return Datas;
}
