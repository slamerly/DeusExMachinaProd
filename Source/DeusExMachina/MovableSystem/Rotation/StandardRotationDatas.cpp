#include "StandardRotationDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FStandardRotationDatas::IsDataValid() const
{
	return IsValid(StandardRotInteractionDatas) || (bOverrideValues && bOverrideCurveValue);
}

float FStandardRotationDatas::GetRotationAngle() const
{
	return bOverrideValues ? RotationAngle : StandardRotInteractionDatas->RotationAngle;
}

float FStandardRotationDatas::GetRotationDuration() const
{
	return bOverrideValues ? RotationDuration : StandardRotInteractionDatas->RotationDuration;
}

UCurveFloat* FStandardRotationDatas::GetRotationCurve() const
{
	return bOverrideCurveValue ? RotationCurve : StandardRotInteractionDatas->RotationCurve;
}





// ======================================================
//             Getter Functions (Blueprint)
// ======================================================
float UStandardRotationDatasGet::GetRotationAngle(const FStandardRotationDatas& Datas)
{
	return Datas.GetRotationAngle();
}

float UStandardRotationDatasGet::GetRotationDuration(const FStandardRotationDatas& Datas)
{
	return Datas.GetRotationDuration();
}

UCurveFloat* UStandardRotationDatasGet::GetRotationCurve(const FStandardRotationDatas& Datas)
{
	return Datas.GetRotationCurve();
}
