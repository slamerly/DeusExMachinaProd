#include "AutomaticRotationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FAutomaticRotationDatas::IsDataValid() const
{
	return IsValid(AutomaticRotationDatas) ||
		(bOverrideAutomaticRotationPart && bOverrideStartPhasePart && bOverrideEndPhasePart && bOverrideAutomaticStop);
}


float FAutomaticRotationDatas::GetRotationSpeed() const
{
	return bOverrideAutomaticRotationPart ? RotationSpeed : AutomaticRotationDatas->RotationSpeed;
}

EAutomaticRotationType FAutomaticRotationDatas::GetAutomaticRotationType() const
{
	return bOverrideAutomaticRotationPart ? AutomaticRotationType : AutomaticRotationDatas->AutomaticRotationType;
}

UCurveFloat* FAutomaticRotationDatas::GetRotationCurve() const
{
	return bOverrideAutomaticRotationPart ? RotationCurve : AutomaticRotationDatas->RotationCurve;
}

bool FAutomaticRotationDatas::GetStartAutomatic() const
{
	return bOverrideAutomaticRotationPart ? bStartAutomatic : AutomaticRotationDatas->bStartAutomatic;
}


float FAutomaticRotationDatas::GetStartDuration() const
{
	return bOverrideStartPhasePart ? StartDuration : AutomaticRotationDatas->StartDuration;
}

UCurveFloat* FAutomaticRotationDatas::GetStartCurve() const
{
	return bOverrideStartPhasePart ? StartCurve : AutomaticRotationDatas->StartCurve;
}


float FAutomaticRotationDatas::GetEndDuration() const
{
	return bOverrideEndPhasePart ? EndDuration : AutomaticRotationDatas->EndDuration;
}

UCurveFloat* FAutomaticRotationDatas::GetEndCurve() const
{
	return bOverrideEndPhasePart ? EndCurve : AutomaticRotationDatas->EndCurve;
}


ERotStopBehavior FAutomaticRotationDatas::GetStopBehavior() const
{
	return bOverrideAutomaticStop ? StopBehavior : AutomaticRotationDatas->StopBehavior;
}

int FAutomaticRotationDatas::GetStopIntervalAngle() const
{
	return bOverrideAutomaticStop ? StopIntervalAngle : AutomaticRotationDatas->StopIntervalAngle;
}

float FAutomaticRotationDatas::GetGlobalStopDuration() const
{
	return bOverrideAutomaticStop ? GlobalStopDuration : AutomaticRotationDatas->GlobalStopDuration;
}

TArray<FStopDefinedAngle> FAutomaticRotationDatas::GetStopDefinedAngles() const
{
	return bOverrideAutomaticStop ? StopDefinedAngles : AutomaticRotationDatas->StopDefinedAngles;
}





// ======================================================
//             Getter Functions (Blueprint)
// ======================================================
float UAutomaticRotationDatasGet::GetRotationSpeed(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetRotationSpeed();
}

EAutomaticRotationType UAutomaticRotationDatasGet::GetAutomaticRotationType(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetAutomaticRotationType();
}

UCurveFloat* UAutomaticRotationDatasGet::GetRotationCurve(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetRotationCurve();
}

bool UAutomaticRotationDatasGet::GetStartAutomatic(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetStartAutomatic();
}


float UAutomaticRotationDatasGet::GetStartDuration(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetStartDuration();
}

UCurveFloat* UAutomaticRotationDatasGet::GetStartCurve(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetStartCurve();
}


float UAutomaticRotationDatasGet::GetEndDuration(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetEndDuration();
}

UCurveFloat* UAutomaticRotationDatasGet::GetEndCurve(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetEndCurve();
}


ERotStopBehavior UAutomaticRotationDatasGet::GetStopBehavior(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetStopBehavior();
}

int UAutomaticRotationDatasGet::GetStopIntervalAngle(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetStopIntervalAngle();
}

float UAutomaticRotationDatasGet::GetGlobalStopDuration(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetGlobalStopDuration();
}

TArray<FStopDefinedAngle> UAutomaticRotationDatasGet::GetStopDefinedAngles(const FAutomaticRotationDatas& Datas)
{
	return Datas.GetStopDefinedAngles();
}