#include "AutoRotInteractionDatas.h"


// ======================================================
//                  Getter Functions
// ======================================================
bool FAutoRotInteractionDatas::IsDataValid() const
{
	return IsValid(AutoRotInteractionDatas) || bOverrideValues;
}

bool FAutoRotInteractionDatas::GetStartStop() const
{
	return bOverrideValues ? bStartStop : AutoRotInteractionDatas->bStartStop;
}

bool FAutoRotInteractionDatas::GetReverse() const
{
	return bOverrideValues ? bReverse : AutoRotInteractionDatas->bReverse;
}





// ======================================================
//             Getter Functions (Blueprint)
// ======================================================
bool UAutoRotInteractionDatasGet::GetStartStop(const FAutoRotInteractionDatas& Datas)
{
	return Datas.GetStartStop();
}

bool UAutoRotInteractionDatasGet::GetReverse(const FAutoRotInteractionDatas& Datas)
{
	return Datas.GetReverse();
}
