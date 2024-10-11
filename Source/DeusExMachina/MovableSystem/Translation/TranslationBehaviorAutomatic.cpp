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

	//  bind 'LateBeginPlay' to next tick to be sure that all childs movable are correctly setup
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UTranslationBehaviorAutomatic::LateBeginPlay);

	SetComponentTickEnabled(false);

	InitializeOwner(); //  TranslationBehaviorBase

	if (AutomaticTranslationValues.IsDataValid())
	{
		//  compute if the automatic speed is negative (will reverse every translation)
		bAutomaticSpeedReverse = AutomaticTranslationValues.GetTranslationSpeed() < 0.0f;
	}
}

void UTranslationBehaviorAutomatic::LateBeginPlay()
{
	//  start automatic translation if required
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
	float TranslationAdd = 0.0f;

	switch (CurrentState)
	{
	//  compute the normal phase of the automatic translation
	case EAutoTranslationState::AutomaticTranslation:
		TranslationAdd = AutomaticTranslationSpeed * DeltaTime;
		break;

	//  compute the start phase of the automatic translation
	case EAutoTranslationState::StartPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			//  finished start phase, switch to classic automatic translation
			PhaseTimer = PhaseTime;
			CurrentState = EAutoTranslationState::AutomaticTranslation;
		}

		TranslationAdd = AutomaticTranslationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime);
		break;

	//  compute the end phase of the automatic translation
	case EAutoTranslationState::EndPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			//  finished end phase, stop automatic translation
			CancelAutomaticTranslation();
			break;
		}

		TranslationAdd = AutomaticTranslationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime);
		break;

	//  compute the movement phase of the automatic translation with stop
	case EAutoTranslationState::AutomaticTranslationWithStop:
		AutomaticTranslationTimer += DeltaTime;
		if (AutomaticTranslationTimer >= AutomaticTranslationDuration)
		{
			//  finished timer = arrived to stop spline point destination
			//  switch state and start stop timer
			CurrentState = EAutoTranslationState::StopOnSplinePoint;
			AutomaticStopTimer = 0.0f;

			OwnerTransSupport->ForcePositionOnSpline(AutomaticStopDestIndex, 0.0f); //  reposition support to stop point (security)

			//  broadcast OnAutoTranslationAutoStop event
			OnAutoTranslationAutoStop.Broadcast(AutomaticStopDestIndex);
			break;
		}

		DistanceDone = AutomaticTranslationCurve->GetFloatValue(AutomaticTranslationTimer / AutomaticTranslationDuration) * AutomaticTranslationDistance;
		TranslationAdd = DistanceDone - AutomaticTranslationDistanceDone; //  'AutomaticTranslationDistanceDone' serve here as a "distance done last frame"
		AutomaticTranslationDistanceDone = DistanceDone;
		break;

	//  compute the stop phase of the automatic translation with stop
	case EAutoTranslationState::StopOnSplinePoint:
		AutomaticStopTimer += DeltaTime;
		if (AutomaticStopTimer >= AutomaticStopDuration)
		{
			//  auto stop finished, restart
			StartAutomaticTranslationWithStop();
		}
		break;
	}

	if (GetReverse()) TranslationAdd *= -1.0f; //  reverse the desired translation add if needed
	OwnerTransSupport->AddTranslationAlongSpline(TranslationAdd); //  apply translation add
}



// ======================================================
//             Control Automatic Translation
// ======================================================
void UTranslationBehaviorAutomatic::LaunchAutomaticTranslationBeginPlay()
{
	//  select auto translation mode for begin play
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

	//  check auto reverse
	if (bAutomaticStopReverse)
	{
		bAutomaticStopReverse = false;
		bExteriorReverse = !bExteriorReverse;
	}

	//  set automatic translation with stop values
	const bool StopSuccess = ComputeNextStopPoint();
	if (!StopSuccess)
	{
		kPRINT_ERROR("Automatic Translation couldn't retrieve a valid stop point!");
		CancelAutomaticTranslation(); //  stop everything if a stop point couldn't be computed
		return;
	}

	AutomaticTranslationDuration = AutomaticTranslationDistance / FMath::Abs(AutomaticTranslationValues.GetTranslationSpeed());
	AutomaticTranslationTimer = 0.0f;
	AutomaticTranslationCurve = AutomaticTranslationValues.GetTranslationCurve();
	AutomaticTranslationDistanceDone = 0.0f;

	//  set state and tick for this component
	CurrentState = EAutoTranslationState::AutomaticTranslationWithStop;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerTransSupport->CurrentTranslationState = ETranslationState::AutomaticTranslation;
	OwnerTransSupport->StartMovementOnChildrens();

	//  broadcast OnAutoTranslationStart event
	OnAutoTranslationAutoResume.Broadcast(AutomaticStopDestIndex);
}

void UTranslationBehaviorAutomatic::StartAutomaticTranslation(bool bForceNoStartPhase)
{
	//  check validities
	if (!bOwnerTransSupportValid) return;
	if (!AutomaticTranslationValues.IsDataValid()) return;
	if (AutomaticTranslationValues.GetAutomaticTranslationType() != EAutomaticTranslationType::AutomaticTranslation) return;

	//  set translation speed
	AutomaticTranslationSpeed = FMath::Abs(AutomaticTranslationValues.GetTranslationSpeed());

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

	//  broadcast OnAutoTranslationStart event
	OnAutoTranslationStart.Broadcast();
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
	AutomaticTranslationSpeed = FMath::Abs(AutomaticTranslationValues.GetTranslationSpeed());

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
	//  stop everything
	SetComponentTickEnabled(false);
	CurrentState = EAutoTranslationState::Inactive;

	OwnerTransSupport->CurrentTranslationState = ETranslationState::NotMoving;
	OwnerTransSupport->StopMovementOnChildrens();

	//  broadcast OnAutoTranslationStop event
	OnAutoTranslationStop.Broadcast();
}



// ======================================================
//                     Interaction
// ======================================================
void UTranslationBehaviorAutomatic::TriggerAutoTransInteraction(FAutoTransInteractionDatas Datas)
{
	//  check if button interaction is possible
	if (!bOwnerTransSupportValid) return;
	if (!Datas.IsDataValid()) return;

	if (CurrentState == EAutoTranslationState::AutomaticTranslationWithStop || CurrentState == EAutoTranslationState::StopOnSplinePoint)
	{
		kPRINT_WARNING("You can't trigger a automatic translation interaction with a MovSys Button on an automatic Translation Support in automatic stop mode. If you need it to be a thing, please ask Cyril.");
		return;
	}
	
	//  select wether to start, stop and apply reverse depending of the current state of automatic rotation and interaction datas
	if (Datas.GetStartStop())
	{
		if (CurrentState == EAutoTranslationState::AutomaticTranslation || CurrentState == EAutoTranslationState::StartPhase)
		{
			StopAutomaticTranslation();
		}
		else
		{
			if (Datas.GetReverse())
			{
				bExteriorReverse = !bExteriorReverse;
			}
			StartAutomaticTranslation();
		}
	}
	else
	{
		if (Datas.GetReverse())
		{
			bExteriorReverse = !bExteriorReverse;
			if (CurrentState == EAutoTranslationState::AutomaticTranslation || CurrentState == EAutoTranslationState::StartPhase)
			{
				StartAutomaticTranslation();
			}
		}
	}

	//  broadcast 'OnAutoTranslationTriggered' event
	OnAutoTranslationTriggered.Broadcast(Datas);
}



// ======================================================
//                   Helper Functions
// ======================================================
EAutomaticTranslationType UTranslationBehaviorAutomatic::GetAutoTranslationType()
{
	if (IsAutomaticStopValid()) return EAutomaticTranslationType::StopOnSplinePoints;
	return EAutomaticTranslationType::AutomaticTranslation;
}

bool UTranslationBehaviorAutomatic::IsAutomaticStopValid()
{
	//  check automatic translation type is automatic stop
	if (!AutomaticTranslationValues.IsDataValid()) return false;
	if (AutomaticTranslationValues.GetAutomaticTranslationType() != EAutomaticTranslationType::StopOnSplinePoints) return false;

	//  check validities of automatic stop values
	if (!IsValid(AutomaticTranslationValues.GetTranslationCurve())) return false;
	switch (AutomaticTranslationValues.GetStopBehavior())
	{
	case EStopBehavior::StopEveryPoint:
		return AutomaticTranslationValues.GetGlobalStopDuration() > 0.0f;

	case EStopBehavior::StopSpecifedPoint:
		//  check if at least one index of the stop specified is a valid index on the owner support spline
		if (!bOwnerTransSupportValid) return false;

		for (FStopSplinePoint StopPoint : AutomaticTranslationValues.GetStopSplinePoints())
		{
			const int StopIndex = StopPoint.StopSplineIndex;
			if (StopIndex >= 0 && StopIndex < OwnerTransSupport->GetNumberOfSplinePoints()) return true;
		}

		return false; //  if this line is reached, there is no index in the array that is reachable, so the automatic stop is invalid

	default:
		return false; //  this can't be reached but the compiler need that
	}
}

bool UTranslationBehaviorAutomatic::IsStartPhaseValid()
{
	//  check validity of the starting phase
	if (!AutomaticTranslationValues.IsDataValid()) return false;

	return AutomaticTranslationValues.GetStartDuration() > 0.0f && IsValid(AutomaticTranslationValues.GetStartCurve());
}

bool UTranslationBehaviorAutomatic::IsEndPhaseValid()
{
	//  check validity of the ending phase
	if (!AutomaticTranslationValues.IsDataValid()) return false;

	return AutomaticTranslationValues.GetEndDuration() > 0.0f && IsValid(AutomaticTranslationValues.GetEndCurve());
}

bool UTranslationBehaviorAutomatic::ComputeNextStopPoint()
{
	//  find the starting index point (if going reverse & between two points, select the next point in the spline)
	int StopPointIndex = OwnerTransSupport->GetInnerSplineIndex();
	if(GetReverse() && OwnerTransSupport->GetProgressToNextIndex() > 0.0f) StopPointIndex = OwnerTransSupport->GetNextSplineIndex(StopPointIndex);

	//  loop through all points in the spline
	const int WhileIndexStart = StopPointIndex; //  while loop security
	bool bFirstLoopIteration = true; //  while loop security

	bool bStopIndexValid = false;
	while (!bStopIndexValid)
	{
		//  check if we aren't in an infite loop
		if (StopPointIndex == WhileIndexStart)
		{
			if (bFirstLoopIteration)
			{
				bFirstLoopIteration = false;
			}
			else
			{
				return false;
			}
		}

		//  select the next point in order
		if (GetReverse())
		{
			StopPointIndex = OwnerTransSupport->GetPrevSplineIndex(StopPointIndex);
		}
		else
		{
			StopPointIndex = OwnerTransSupport->GetNextSplineIndex(StopPointIndex);
		}
		
		switch (AutomaticTranslationValues.GetStopBehavior())
		{
		case EStopBehavior::StopEveryPoint:
			//  if stop behavior is every point, any point index is valid (will return the first one in order)
			bStopIndexValid = true;
			AutomaticStopDuration = AutomaticTranslationValues.GetGlobalStopDuration();
			bAutomaticStopReverse = false;
			AutomaticStopDestIndex = StopPointIndex;
			break;

		case EStopBehavior::StopSpecifedPoint:
			//  if stop behavior is specified point, we need to check if the currently evaluated point index is on the stop point list
			for (FStopSplinePoint StopPoint : AutomaticTranslationValues.GetStopSplinePoints())
			{
				if (StopPoint.StopSplineIndex == StopPointIndex)
				{
					bStopIndexValid = true;
					AutomaticStopDuration = StopPoint.StopIndexDuration;
					bAutomaticStopReverse = StopPoint.bStopIndexReverse;
					AutomaticStopDestIndex = StopPointIndex;
				}
			}
			break;
		}
	}

	//  search successful, set Automatic Translation Distance to the distance until the found stop point
	if (GetReverse())
	{
		AutomaticTranslationDistance = OwnerTransSupport->GetSplineDistanceToPointReversed(StopPointIndex);
	}
	else
	{
		AutomaticTranslationDistance = OwnerTransSupport->GetSplineDistanceToPoint(StopPointIndex);
	}

	return true;
}

bool UTranslationBehaviorAutomatic::GetReverse()
{
	return bExteriorReverse ^ bAutomaticSpeedReverse; // XOR
}



// ======================================================
//               Automatic Translation Datas
// ======================================================
void UTranslationBehaviorAutomatic::ChangeAutomaticTranslationValues(FAutomaticTranslationDatas NewAutoTransValues)
{
	if (!NewAutoTransValues.IsDataValid())
	{
		kPRINT_ERROR("Tried changing values of an automatic translation with an invalid struct!");
		return;
	}

	CancelAutomaticTranslation();

	AutomaticTranslationValues = NewAutoTransValues;

	//  compute if the automatic speed is negative (will reverse every translation)
	bAutomaticSpeedReverse = AutomaticTranslationValues.GetTranslationSpeed() < 0.0f;

	if (!bOwnerTransSupportValid) return;

	if (AutomaticTranslationValues.GetStartAutomatic())
	{
		LaunchAutomaticTranslationBeginPlay();
	}
}