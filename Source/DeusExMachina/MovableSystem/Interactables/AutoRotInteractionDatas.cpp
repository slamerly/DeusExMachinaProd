#include "AutoRotInteractionDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FAutoRotInteractionDatas::IsDataValid()
{
	return IsValid(AutoRotInteractionDatas) || bOverrideValues;
}

bool FAutoRotInteractionDatas::GetStartStop()
{
	return bOverrideValues ? bStartStop : AutoRotInteractionDatas->bStartStop;
}

bool FAutoRotInteractionDatas::GetReverse()
{
	return bOverrideValues ? bReverse : AutoRotInteractionDatas->bReverse;
}
