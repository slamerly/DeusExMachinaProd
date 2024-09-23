#include "AutoTransInteractionDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FAutoTransInteractionDatas::IsDataValid()
{
	return IsValid(AutoTransInteractionDatas) || bOverrideValues;
}

bool FAutoTransInteractionDatas::GetStartStop()
{
	return bOverrideValues ? bStartStop : AutoTransInteractionDatas->bStartStop;
}

bool FAutoTransInteractionDatas::GetReverse()
{
	return bOverrideValues ? bReverse : AutoTransInteractionDatas->bReverse;
}
