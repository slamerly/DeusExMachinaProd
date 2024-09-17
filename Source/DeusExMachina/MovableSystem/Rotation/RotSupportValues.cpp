#include "RotSupportValues.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FRotSupportValues::IsDataValid()
{
	return IsValid(RotSupportValues) || (ClampValuesOverride.bOverrideClampValues && SnapValuesOverride.bOverrideSnapValues && SnapCurvesOverride.bOverrideSnapCurves);
}

bool FRotSupportValues::GetUseClamp()
{
	return ClampValuesOverride.bOverrideClampValues ? ClampValuesOverride.bUseClamp : RotSupportValues->bUseClamp;
}

float FRotSupportValues::GetClampLowValue()
{
	return ClampValuesOverride.bOverrideClampValues ? ClampValuesOverride.ClampLowValue : RotSupportValues->ClampLowValue;
}

float FRotSupportValues::GetClampHighValue()
{
	return ClampValuesOverride.bOverrideClampValues ? ClampValuesOverride.ClampHighValue : RotSupportValues->ClampHighValue;
}

bool FRotSupportValues::GetUseSnap()
{
	return SnapValuesOverride.bOverrideSnapValues ? SnapValuesOverride.bUseSnap : RotSupportValues->bUseSnap;
}

TArray<FSnapValue> FRotSupportValues::GetSnapValues()
{
	return SnapValuesOverride.bOverrideSnapValues ? SnapValuesOverride.SnapValues : RotSupportValues->SnapValues;
}

float FRotSupportValues::GetSnapDirectionAdvantage()
{
	return SnapValuesOverride.bOverrideSnapValues ? SnapValuesOverride.SnapDirectionAdvantage : RotSupportValues->SnapDirectionAdvantage;
}

float FRotSupportValues::GetSnapDirectionDelay()
{
	return SnapValuesOverride.bOverrideSnapValues ? SnapValuesOverride.SnapDirectionDelay : RotSupportValues->SnapDirectionDelay;
}

float FRotSupportValues::GetSnapSpeed()
{
	return SnapValuesOverride.bOverrideSnapValues ? SnapValuesOverride.SnapSpeed : RotSupportValues->SnapSpeed;
}

UCurveFloat* FRotSupportValues::GetSnapCurveNeutralReverse()
{
	return SnapCurvesOverride.bOverrideSnapCurves ? SnapCurvesOverride.SnapCurveNeutralReverse : RotSupportValues->SnapCurveNeutralReverse;
}

UCurveFloat* FRotSupportValues::GetSnapCurveContinue()
{
	return SnapCurvesOverride.bOverrideSnapCurves ? SnapCurvesOverride.SnapCurveContinue : RotSupportValues->SnapCurveContinue;
}
