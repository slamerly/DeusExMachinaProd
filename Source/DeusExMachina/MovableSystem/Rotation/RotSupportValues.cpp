#include "RotSupportValues.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FRotSupportValues::IsDataValid()
{
	return IsValid(RotSupportValues) || (bOverrideClampValues /*&& bOverrideSnapValues*/);
}

bool FRotSupportValues::GetUseClamp()
{
	return bOverrideClampValues ? bUseClamp : RotSupportValues->bUseClamp;
}

float FRotSupportValues::GetClampLowValue()
{
	return bOverrideClampValues ? ClampLowValue : RotSupportValues->ClampLowValue;
}

float FRotSupportValues::GetClampHighValue()
{
	return bOverrideClampValues ? ClampHighValue : RotSupportValues->ClampHighValue;
}
