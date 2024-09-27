#include "TranslationBehaviorStandard.h"
#include "TranslationSupport.h"
#include "StandardTranslationDatas.h"
#include "Defines.h"

UTranslationBehaviorStandard::UTranslationBehaviorStandard()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTranslationBehaviorStandard::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	InitializeOwner(); //  TranslationBehaviorBase
}


// ======================================================
//                        Tick
// ======================================================
void UTranslationBehaviorStandard::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bOwnerTransSupportValid) return;
	if (!bCurrentlyMoving) return;

	//  standard translation
	TranslationTimer += DeltaTime;
	if (TranslationTimer >= TranslationDuration)
	{
		//  if a destination spline point is defined, reposition support to it (security)
		if(ForceSplinePoint != -1) OwnerTransSupport->ForcePositionOnSpline(ForceSplinePoint, 0.0f);

		//  timer finished, stop translation
		CancelStandardTranslation();
		return;
	}

	//  apply standard translation
	const float DistanceDone = TranslationDistance * TranslationCurve->GetFloatValue(TranslationTimer / TranslationDuration);
	OwnerTransSupport->AddTranslationAlongSpline(DistanceDone - LastFrameDistanceDone);

	LastFrameDistanceDone = DistanceDone;
}


// ======================================================
//              Control Standard Translation
// ======================================================
void UTranslationBehaviorStandard::StartStandardTranslation(FStandardTranslationDatas Datas, bool bForceStart)
{
	//  check validities
	if (!bOwnerTransSupportValid) return;
	if (!IsStandardTransValid(Datas)) return;

	//  check already moving
	if (bCurrentlyMoving && !bForceStart) return;

	//  set standard trans values
	TranslationCurve = Datas.GetTranslationCurve();
	TranslationTimer = 0.0f;
	LastFrameDistanceDone = 0.0f;

	//  compute standard translation distance to do depending of the mode
	ForceSplinePoint = -1;
	switch (Datas.GetStandardTranslationMode())
	{
	case EStandardTranslationMode::MoveSplinePoints:
		TranslationDistance = ConvertSplinePointsToSplineDistance(Datas, ForceSplinePoint);
		break;

	case EStandardTranslationMode::MoveDistance:
		TranslationDistance = Datas.GetTranslationSplineDistance();
		break;
	}

	//  compute standard translation duration depending of the mode
	switch (Datas.GetStandardTranslationSpeedMode())
	{
	case EStandardTranslationSpeedMode::TranslationDuration:
		TranslationDuration = Datas.GetTranslationDuration();
		break;

	case EStandardTranslationSpeedMode::TranslationSpeed:
		TranslationDuration = TranslationDistance / Datas.GetTranslationSpeed();
		break;
	}

	//  set moving and tick for this component
	bCurrentlyMoving = true;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerTransSupport->CurrentTranslationState = ETranslationState::StandardTranslation;
	OwnerTransSupport->StartMovementOnChildrens();
}

void UTranslationBehaviorStandard::CancelStandardTranslation()
{
	if (!bCurrentlyMoving) return;

	//  set rotating and tick for this component
	bCurrentlyMoving = false;
	SetComponentTickEnabled(false);

	//  set state on support and stop movement on childrens
	OwnerTransSupport->CurrentTranslationState = ETranslationState::NotMoving;
	OwnerTransSupport->StopMovementOnChildrens();
}


// ======================================================
//                   Helper Functions
// ======================================================
bool UTranslationBehaviorStandard::IsStandardTransValid(FStandardTranslationDatas Datas)
{
	//  check validity of standard translation values, depending of modes
	if (!Datas.IsDataValid()) return false;

	if (!IsValid(Datas.GetTranslationCurve())) return false;

	switch (Datas.GetStandardTranslationSpeedMode())
	{
	case EStandardTranslationSpeedMode::TranslationDuration:
		if (Datas.GetTranslationDuration() <= 0.0f) return false;
		break;

	case EStandardTranslationSpeedMode::TranslationSpeed:
		if (Datas.GetTranslationSpeed() <= 0.0f) return false;
		break;
	}

	switch (Datas.GetStandardTranslationMode())
	{
	case EStandardTranslationMode::MoveSplinePoints:
		if (Datas.GetTranslationSplinePoints() == 0) return false;
		break;

	case EStandardTranslationMode::MoveDistance:
		if (Datas.GetTranslationSplineDistance() == 0.0f) return false;
		break;
	}

	return true;
}

float UTranslationBehaviorStandard::ConvertSplinePointsToSplineDistance(FStandardTranslationDatas Datas, int& DestSplinePoint)
{
	//  do not check validities, this function will always be called in a context where 'IsStandardTransValid()' has been called before

	if (Datas.GetTranslationSplinePoints() == 0) return 0.0f;

	const int DistanceSign = FMath::Sign<int>(Datas.GetTranslationSplinePoints()); //  retrieve if the translation goes in the spline direction (positive) or the opposite (negative)

	const int AbsSplinePoints = FMath::Abs<int>(Datas.GetTranslationSplinePoints()); //  number of spline points to go through
	const int ModulatedSplinePoints = AbsSplinePoints % OwnerTransSupport->GetNumberOfSplinePoints(); //  same, but without having to loop on the spline
	const int NumberFullSpline = (AbsSplinePoints - ModulatedSplinePoints) / OwnerTransSupport->GetNumberOfSplinePoints(); //  how many loop of the spline do we need

	float SplineDistance = NumberFullSpline * OwnerTransSupport->GetFullSplineLength(); //  compute the distance of all needed loops


	int TargetSplinePoint = OwnerTransSupport->GetInnerSplineIndex();
	if (DistanceSign < 0) //  going in reverse spline direction
	{
		if (OwnerTransSupport->GetProgressToNextIndex() > 0.0f) TargetSplinePoint++; //  increment the starting index if the support is between two points

		//  retrieve the destination index
		TargetSplinePoint -= ModulatedSplinePoints;
		TargetSplinePoint %= OwnerTransSupport->GetNumberOfSplinePoints();
		if (TargetSplinePoint < 0) TargetSplinePoint += OwnerTransSupport->GetNumberOfSplinePoints();

		//  add the distance to found destination point
		SplineDistance += OwnerTransSupport->GetSplineDistanceToPointReversed(TargetSplinePoint);
	}
	else //  going in spline direction
	{
		//  retrieve the destination index
		TargetSplinePoint += ModulatedSplinePoints;
		TargetSplinePoint %= OwnerTransSupport->GetNumberOfSplinePoints();

		//  add the distance to found destination point
		SplineDistance += OwnerTransSupport->GetSplineDistanceToPoint(TargetSplinePoint);
	}

	DestSplinePoint = TargetSplinePoint; //  set the out destination index found

	SplineDistance *= DistanceSign; //  negate the distance if going reverse
	return SplineDistance;
}

