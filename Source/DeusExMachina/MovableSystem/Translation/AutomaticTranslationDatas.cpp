#include "AutomaticTranslationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FAutomaticTranslationDatas::IsDataValid() const
{
	return IsValid(AutomaticTranslationDatas) ||
		(bOverrideAutomaticTranslation && bOverrideAutomaticStop && bOverrideStartPhase && bOverrideEndPhase);
}


float FAutomaticTranslationDatas::GetTranslationSpeed() const
{
	return bOverrideAutomaticTranslation ? TranslationSpeed : AutomaticTranslationDatas->TranslationSpeed;
}

UCurveFloat* FAutomaticTranslationDatas::GetTranslationCurve() const
{
	return bOverrideAutomaticTranslation ? TranslationCurve : AutomaticTranslationDatas->TranslationCurve;
}

bool FAutomaticTranslationDatas::GetStartAutomatic() const
{
	return bOverrideAutomaticTranslation ? bStartAutomatic : AutomaticTranslationDatas->bStartAutomatic;
}

EAutomaticTranslationType FAutomaticTranslationDatas::GetAutomaticTranslationType() const
{
	return bOverrideAutomaticTranslation ? AutomaticTranslationType : AutomaticTranslationDatas->AutomaticTranslationType;
}


float FAutomaticTranslationDatas::GetStartDuration() const
{
	return bOverrideStartPhase ? StartDuration : AutomaticTranslationDatas->StartDuration;
}

UCurveFloat* FAutomaticTranslationDatas::GetStartCurve() const
{
	return bOverrideStartPhase ? StartCurve : AutomaticTranslationDatas->StartCurve;
}


float FAutomaticTranslationDatas::GetEndDuration() const
{
	return bOverrideEndPhase ? EndDuration : AutomaticTranslationDatas->EndDuration;
}

UCurveFloat* FAutomaticTranslationDatas::GetEndCurve() const
{
	return bOverrideEndPhase ? EndCurve : AutomaticTranslationDatas->EndCurve;
}


EStopBehavior FAutomaticTranslationDatas::GetStopBehavior() const
{
	return bOverrideAutomaticStop ? StopBehavior : AutomaticTranslationDatas->StopBehavior;
}

float FAutomaticTranslationDatas::GetGlobalStopDuration() const
{
	return bOverrideAutomaticStop ? GlobalStopDuration : AutomaticTranslationDatas->GlobalStopDuration;
}

TArray<FStopSplinePoint> FAutomaticTranslationDatas::GetStopSplinePoints() const
{
	return bOverrideAutomaticStop ? StopSplinePoints : AutomaticTranslationDatas->StopSplinePoints;
}





// ======================================================
//             Getter Functions (Blueprint)
// ======================================================
float UAutomaticTranslationDatasGet::GetTranslationSpeed(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetTranslationSpeed();
}

EAutomaticTranslationType UAutomaticTranslationDatasGet::GetAutomaticTranslationType(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetAutomaticTranslationType();
}

UCurveFloat* UAutomaticTranslationDatasGet::GetTranslationCurve(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetTranslationCurve();
}

float UAutomaticTranslationDatasGet::GetStartAutomatic(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetStartAutomatic();
}


float UAutomaticTranslationDatasGet::GetStartDuration(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetStartDuration();
}

UCurveFloat* UAutomaticTranslationDatasGet::GetStartCurve(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetStartCurve();
}


float UAutomaticTranslationDatasGet::GetEndDuration(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetEndDuration();
}

UCurveFloat* UAutomaticTranslationDatasGet::GetEndCurve(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetEndCurve();
}


EStopBehavior UAutomaticTranslationDatasGet::GetStopBehavior(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetStopBehavior();
}

float UAutomaticTranslationDatasGet::GetGlobalStopDuration(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetGlobalStopDuration();
}

TArray<FStopSplinePoint> UAutomaticTranslationDatasGet::GetStopSplinePoints(const FAutomaticTranslationDatas& Datas)
{
	return Datas.GetStopSplinePoints();
}