#include "RotSupportValues.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FRotSupportValues::IsDataValid() const
{
	return IsValid(RotSupportValues) || (ClampValuesOverride.bOverrideClampValues && SnapValuesOverride.bOverrideSnapValues && SnapCurvesOverride.bOverrideSnapCurves);
}

bool FRotSupportValues::GetUseClamp() const
{
	return ClampValuesOverride.bOverrideClampValues ? ClampValuesOverride.bUseClamp : RotSupportValues->bUseClamp;
}

int FRotSupportValues::GetClampLowValue() const
{
	return ClampValuesOverride.bOverrideClampValues ? ClampValuesOverride.ClampLowValue : RotSupportValues->ClampLowValue;
}

int FRotSupportValues::GetClampHighValue() const
{
	return ClampValuesOverride.bOverrideClampValues ? ClampValuesOverride.ClampHighValue : RotSupportValues->ClampHighValue;
}

bool FRotSupportValues::GetUseSnap() const
{
	return SnapValuesOverride.bOverrideSnapValues ? SnapValuesOverride.bUseSnap : RotSupportValues->bUseSnap;
}

TArray<FSnapValue> FRotSupportValues::GetSnapValues() const
{
	return SnapValuesOverride.bOverrideSnapValues ? SnapValuesOverride.SnapValues : RotSupportValues->SnapValues;
}

float FRotSupportValues::GetSnapDirectionAdvantage() const
{
	return SnapValuesOverride.bOverrideSnapValues ? SnapValuesOverride.SnapDirectionAdvantage : RotSupportValues->SnapDirectionAdvantage;
}

float FRotSupportValues::GetSnapSpeed() const
{
	return SnapValuesOverride.bOverrideSnapValues ? SnapValuesOverride.SnapSpeed : RotSupportValues->SnapSpeed;
}

UCurveFloat* FRotSupportValues::GetSnapCurveNeutralReverse() const
{
	return SnapCurvesOverride.bOverrideSnapCurves ? SnapCurvesOverride.SnapCurveNeutralReverse : RotSupportValues->SnapCurveNeutralReverse;
}

UCurveFloat* FRotSupportValues::GetSnapCurveContinue() const
{
	return SnapCurvesOverride.bOverrideSnapCurves ? SnapCurvesOverride.SnapCurveContinue : RotSupportValues->SnapCurveContinue;
}
