#include "TranslationBehaviorAutomatic.h"
#include "TranslationSupport.h"
#include "Defines.h"

UTranslationBehaviorAutomatic::UTranslationBehaviorAutomatic()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// ======================================================
//                     Begin Play
// ======================================================
void UTranslationBehaviorAutomatic::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->OnWorldBeginPlay.AddUObject(this, &UTranslationBehaviorAutomatic::LateBeginPlay);

	SetComponentTickEnabled(false);

	InitializeOwner();
}

void UTranslationBehaviorAutomatic::LateBeginPlay()
{
	if (!bOwnerTransSupportValid) return;
	if (!AutomaticTranslationValues.IsDataValid()) return;

	if (AutomaticTranslationValues.GetStartAutomatic())
	{
		LaunchAutomaticTranslationBeginPlay();
	}
}



// ======================================================
//                        Tick
// ======================================================
void UTranslationBehaviorAutomatic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bOwnerTransSupportValid) return;

	float DistanceDone = 0.0f; //  used only for Automatic Translation With Stop, but we cannot initialize variables inside switches.

	switch (CurrentState)
	{
	//  compute the normal phase of the automatic translation
	case EAutoTranslationState::AutomaticTranslation:
		OwnerTransSupport->AddTranslationAlongSpline(AutomaticTranslationSpeed * DeltaTime);
		break;

	//  compute the start phase of the automatic translation
	case EAutoTranslationState::StartPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			PhaseTimer = PhaseTime;
			CurrentState = EAutoTranslationState::AutomaticTranslation;
		}
		OwnerTransSupport->AddTranslationAlongSpline(AutomaticTranslationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime));
		break;

	//  compute the end phase of the automatic translation
	case EAutoTranslationState::EndPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			CancelAutomaticTranslation();
			break;
		}
		OwnerTransSupport->AddTranslationAlongSpline(AutomaticTranslationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime));
		break;

	//  compute the movement phase of the automatic translation with stop
	case EAutoTranslationState::AutomaticTranslationWithStop:
		AutomaticTranslationTimer += DeltaTime;
		if (AutomaticTranslationTimer >= AutomaticTranslationDuration)
		{
			CurrentState = EAutoTranslationState::StopOnSplinePoint;
			AutomaticStopDuration = AutomaticTranslationValues.GetStopDuration();
			AutomaticStopTimer = 0.0f;
		}
		DistanceDone = AutomaticTranslationCurve->GetFloatValue(AutomaticTranslationTimer / AutomaticTranslationDuration) * AutomaticTranslationDistance;
		OwnerTransSupport->AddTranslationAlongSpline(DistanceDone - AutomaticTranslationDistanceDone); //  'AutomaticTranslationDistanceDone' serve here as a "distance done last frame"
		AutomaticTranslationDistanceDone = DistanceDone;
		break;

	//  compute the stop phase of the automatic translation with stop
	case EAutoTranslationState::StopOnSplinePoint:
		AutomaticStopTimer += DeltaTime;
		if (AutomaticStopTimer >= AutomaticStopDuration)
		{
			StartAutomaticTranslationWithStop();
		}
		break;
	}
}




// ======================================================
//             Control Automatic Translation
// ======================================================
void UTranslationBehaviorAutomatic::LaunchAutomaticTranslationBeginPlay()
{
	if (IsAutomaticStopValid())
	{
		StartAutomaticTranslationWithStop();
	}
	else
	{
		StartAutomaticTranslation();
	}
}

void UTranslationBehaviorAutomatic::StartAutomaticTranslationWithStop()
{
	//  check validities
	if (!bOwnerTransSupportValid) return;
	if (!IsAutomaticStopValid()) return;

	//  set automatic translation with stop values
	AutomaticTranslationDistance = DistanceUntilNextStopPoint();
	AutomaticTranslationDuration = AutomaticTranslationDistance / AutomaticTranslationValues.GetTranslationSpeed();
	AutomaticTranslationTimer = 0.0f;
	AutomaticTranslationCurve = AutomaticTranslationValues.GetTranslationCurve();
	AutomaticTranslationDistanceDone = 0.0f;

	//  set state and tick for this component
	CurrentState = EAutoTranslationState::AutomaticTranslationWithStop;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerTransSupport->CurrentTranslationState = ETranslationState::AutomaticTranslation;
	OwnerTransSupport->StartMovementOnChildrens();
}

void UTranslationBehaviorAutomatic::StartAutomaticTranslation(bool bForceNoStartPhase)
{
	//  check validities
	if (!bOwnerTransSupportValid) return;
	if (!AutomaticTranslationValues.IsDataValid()) return;
	if (AutomaticTranslationValues.GetAutomaticTranslationType() != EAutomaticTranslationType::AutomaticTranslation) return;

	//  set translation speed
	AutomaticTranslationSpeed = AutomaticTranslationValues.GetTranslationSpeed();
	if (bRunningReverse) AutomaticTranslationSpeed *= -1.0f;

	//  set start phase values
	PhaseTimer = 0.0f;
	PhaseTime = bForceNoStartPhase ? 0.0f : AutomaticTranslationValues.GetStartDuration();
	PhaseCurve = AutomaticTranslationValues.GetStartCurve();

	//  set state and tick for this component
	CurrentState = (bForceNoStartPhase || !IsStartPhaseValid()) ? EAutoTranslationState::AutomaticTranslation : EAutoTranslationState::StartPhase;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerTransSupport->CurrentTranslationState = ETranslationState::AutomaticTranslation;
	OwnerTransSupport->StartMovementOnChildrens();
}

void UTranslationBehaviorAutomatic::StopAutomaticTranslation(bool bForceNoEndPhase)
{
	//  check validities
	if (!bOwnerTransSupportValid) return;
	if (!AutomaticTranslationValues.IsDataValid()) return;

	if (bForceNoEndPhase || !IsEndPhaseValid())
	{
		//  stop instantly if there is no end phase
		CancelAutomaticTranslation();
		return;
	}

	//  set translation speed
	AutomaticTranslationSpeed = AutomaticTranslationValues.GetTranslationSpeed();
	if (bRunningReverse) AutomaticTranslationSpeed *= -1.0f;

	//  set end phase values
	PhaseTimer = 0.0f;
	PhaseTime = AutomaticTranslationValues.GetEndDuration();
	PhaseCurve = AutomaticTranslationValues.GetEndCurve();

	//  set state and tick for this component
	SetComponentTickEnabled(true);
	CurrentState = EAutoTranslationState::EndPhase;

	//  set state on support and start movement on childrens (should not be useful but we never know)
	OwnerTransSupport->CurrentTranslationState = ETranslationState::AutomaticTranslation;
	OwnerTransSupport->StartMovementOnChildrens();
}

void UTranslationBehaviorAutomatic::CancelAutomaticTranslation()
{
	SetComponentTickEnabled(false);
	CurrentState = EAutoTranslationState::Inactive;

	OwnerTransSupport->CurrentTranslationState = ETranslationState::NotMoving;
	OwnerTransSupport->StopMovementOnChildrens();
}



// ======================================================
//                   Helper Functions
// ======================================================
bool UTranslationBehaviorAutomatic::IsAutomaticStopValid()
{
	if (!AutomaticTranslationValues.IsDataValid()) return false;

	if (AutomaticTranslationValues.GetAutomaticTranslationType() != EAutomaticTranslationType::StopOnSplinePoints) return false;
	if (!IsValid(AutomaticTranslationValues.GetTranslationCurve())) return false;
	if (AutomaticTranslationValues.GetStopDuration() < 0.0f) return false;

	switch (AutomaticTranslationValues.GetStopBehavior())
	{
	case EStopBehavior::StopEveryPoint:
		return true;

	case EStopBehavior::StopSpecifedPoint:
		//  check if at least one index of the stop specified is a valid index on the owner support spline
		if (!bOwnerTransSupportValid) return false;

		for (int StopIndex : AutomaticTranslationValues.GetStopSplineIndex())
		{
			if (StopIndex >= 0 && StopIndex < OwnerTransSupport->GetNumberOfSplinePoints()) return true;
		}

		return false; //  if this line is reached, there is no index in the array that is reachable, so the automatic stop is invalid

	default:
		return false; //  this can't be reached but the compiler need that
	}
}

bool UTranslationBehaviorAutomatic::IsStartPhaseValid()
{
	if (!AutomaticTranslationValues.IsDataValid()) return false;

	return AutomaticTranslationValues.GetStartDuration() > 0.0f && IsValid(AutomaticTranslationValues.GetStartCurve());
}

bool UTranslationBehaviorAutomatic::IsEndPhaseValid()
{
	if (!AutomaticTranslationValues.IsDataValid()) return false;

	return AutomaticTranslationValues.GetEndDuration() > 0.0f && IsValid(AutomaticTranslationValues.GetEndCurve());
}

float UTranslationBehaviorAutomatic::DistanceUntilNextStopPoint()
{
	float DistanceUntilNextStop = OwnerTransSupport->GetDistanceToNextSplinePoint();

	if (AutomaticTranslationValues.GetStopBehavior() == EStopBehavior::StopSpecifedPoint)
	{
		int SplineCheckIndex = OwnerTransSupport->GetInnerSplineIndex();
		bool bSplineCheckValid = false;
		
		while (!bSplineCheckValid) //  if 'IsAutomaticStopValid()' has been called and returned true, this while loop shouldn't be infinite
		{
			SplineCheckIndex = OwnerTransSupport->GetNextSplineIndex(SplineCheckIndex);

			for (int StopIndex : AutomaticTranslationValues.GetStopSplineIndex())
			{
				if (StopIndex == SplineCheckIndex) bSplineCheckValid = true;
			}

			if (!bSplineCheckValid)
			{
				DistanceUntilNextStop += OwnerTransSupport->GetDistanceFromToSplinePoint(SplineCheckIndex, OwnerTransSupport->GetNextSplineIndex(SplineCheckIndex));
			}
		}
	}

	return DistanceUntilNextStop;
}
