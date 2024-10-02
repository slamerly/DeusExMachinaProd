#include "AutoTransInteractionDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FAutoTransInteractionDatas::IsDataValid() const
{
	return IsValid(AutoTransInteractionDatas) || bOverrideValues;
}

bool FAutoTransInteractionDatas::GetStartStop() const
{
	return bOverrideValues ? bStartStop : AutoTransInteractionDatas->bStartStop;
}

bool FAutoTransInteractionDatas::GetReverse() const
{
	return bOverrideValues ? bReverse : AutoTransInteractionDatas->bReverse;
}





// ======================================================
//             Getter Functions (Blueprint)
// ======================================================
bool UAutoTransInteractionDatasGet::GetStartStop(const FAutoTransInteractionDatas& Datas)
{
	return Datas.GetStartStop();
}

bool UAutoTransInteractionDatasGet::GetReverse(const FAutoTransInteractionDatas& Datas)
{
	return Datas.GetReverse();
}
