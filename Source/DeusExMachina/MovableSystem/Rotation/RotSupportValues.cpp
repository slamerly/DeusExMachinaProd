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





// ======================================================
//             Getter Functions (Blueprint)
// ======================================================
bool URotSupportValuesGet::GetUseClamp(const FRotSupportValues& Datas)
{
	return Datas.GetUseClamp();
}

int URotSupportValuesGet::GetClampLowValue(const FRotSupportValues& Datas)
{
	return Datas.GetClampLowValue();
}

int URotSupportValuesGet::GetClampHighValue(const FRotSupportValues& Datas)
{
	return Datas.GetClampHighValue();
}


bool URotSupportValuesGet::GetUseSnap(const FRotSupportValues& Datas)
{
	return Datas.GetUseSnap();
}

TArray<FSnapValue> URotSupportValuesGet::GetSnapValues(const FRotSupportValues& Datas)
{
	return Datas.GetSnapValues();
}

float URotSupportValuesGet::GetSnapDirectionAdvantage(const FRotSupportValues& Datas)
{
	return Datas.GetSnapDirectionAdvantage();
}

float URotSupportValuesGet::GetSnapSpeed(const FRotSupportValues& Datas)
{
	return Datas.GetSnapSpeed();
}


UCurveFloat* URotSupportValuesGet::GetSnapCurveNeutralReverse(const FRotSupportValues& Datas)
{
	return Datas.GetSnapCurveNeutralReverse();
}

UCurveFloat* URotSupportValuesGet::GetSnapCurveContinue(const FRotSupportValues& Datas)
{
	return Datas.GetSnapCurveContinue();
}



// ======================================================
//            Custom Make Function (Blueprint)
// ======================================================
FRotSupportValues URotSupportValuesGet::MakeRotationSupportValues(bool bUseClamp, int ClampLowValue, int ClampHighValue, bool bUseSnap, TArray<FSnapValue> SnapValues, float SnapDirectionAdvantage, float SnapSpeed, UCurveFloat* SnapCurveNeutralReverse, UCurveFloat* SnapCurveContinue)
{
	FRotSupportValues Values;

	Values.ClampValuesOverride.bOverrideClampValues = true;
	Values.SnapValuesOverride.bOverrideSnapValues = true;
	Values.SnapCurvesOverride.bOverrideSnapCurves = true;


	Values.ClampValuesOverride.bUseClamp = bUseClamp;
	Values.ClampValuesOverride.ClampLowValue = ClampLowValue;
	Values.ClampValuesOverride.ClampHighValue = ClampHighValue;

	Values.SnapValuesOverride.bUseSnap = bUseSnap;
	Values.SnapValuesOverride.SnapValues = SnapValues;
	Values.SnapValuesOverride.SnapDirectionAdvantage = SnapDirectionAdvantage;
	Values.SnapValuesOverride.SnapSpeed = SnapSpeed;

	Values.SnapCurvesOverride.SnapCurveNeutralReverse = SnapCurveNeutralReverse;
	Values.SnapCurvesOverride.SnapCurveContinue = SnapCurveContinue;


	return Values;
}
