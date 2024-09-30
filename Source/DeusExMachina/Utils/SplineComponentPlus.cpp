#include "SplineComponentPlus.h"
#include "Defines.h"

void USplineComponentPlus::UpdateSpline()
{
	if (!bRectTangents)
	{
		Super::UpdateSpline();
		return;
	}

	for (int Index = 0; Index < GetNumberOfSplinePoints(); Index++)
	{
		const FVector CurrIndexLocation = GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::Local);

		int PrevIndex = Index - 1;
		if (PrevIndex < 0) PrevIndex += GetNumberOfSplinePoints();
		int NextIndex = (Index + 1) % GetNumberOfSplinePoints();


		SetTangentsAtSplinePoint(
			Index,
			CurrIndexLocation - GetLocationAtSplinePoint(PrevIndex, ESplineCoordinateSpace::Local), //  arrive tangent
			GetLocationAtSplinePoint(NextIndex, ESplineCoordinateSpace::Local) - CurrIndexLocation, //  leave tangent
			ESplineCoordinateSpace::Local,
			false //  do not update spline with this function, it would cause an infinite loop
		);
	}

	Super::UpdateSpline();
}