#include "RotationBehaviorAutomatic.h"
#include "RotationSupport.h"
#include "DeusExMachina/MovableSystem/Interactables/AutoRotInteractionDatas.h"
#include "AnglesUtils.h"
#include "Defines.h"


URotationBehaviorAutomatic::URotationBehaviorAutomatic()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// ======================================================
//                     Begin Play
// ======================================================
void URotationBehaviorAutomatic::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->OnWorldBeginPlay.AddUObject(this, &URotationBehaviorAutomatic::LateBeginPlay);

	SetComponentTickEnabled(false);

	InitializeOwner(); //  RotationBehaviorBase

	if (AutomaticRotationValues.IsDataValid())
	{
		//  compute if the automatic speed is negative (will reverse every rotation)
		bAutomaticSpeedReverse = AutomaticRotationValues.GetRotationSpeed() < 0.0f;
	}
}

void URotationBehaviorAutomatic::LateBeginPlay()
{
	//  start automatic rotation if required
	if (!bOwnerRotSupportValid) return;
	if (!AutomaticRotationValues.IsDataValid()) return;

	if (AutomaticRotationValues.GetStartAutomatic())
	{
		LaunchAutomaticRotationBeginPlay();
	}
}



// ======================================================
//                        Tick
// ======================================================
void URotationBehaviorAutomatic::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bOwnerRotSupportValid) return;

	float AngleDone = 0.0f; //  used only for Automatic Rotation with Stop, but we cannot initialize variables inside switches.
	float RotationAdd = 0.0f;

	switch (CurrentState)
	{
	//  compute the normal phase of the automatic rotation
	case EAutoRotationState::AutomaticRotation:
		RotationAdd = AutomaticRotationSpeed * DeltaTime;
		break;

	//  compute the start phase of the automatic rotation
	case EAutoRotationState::StartPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			//  finished start phase, switch to classic automatic rotation
			PhaseTimer = PhaseTime;
			CurrentState = EAutoRotationState::AutomaticRotation;
		}

		RotationAdd = AutomaticRotationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime);
		break;

	//  compute the end phase of the automatic rotation
	case EAutoRotationState::EndPhase:
		PhaseTimer += DeltaTime;
		if (PhaseTimer >= PhaseTime)
		{
			//  finished end phase, stop automatic rotation
			CancelAutomaticRotation();
			break;
		}

		RotationAdd = AutomaticRotationSpeed * DeltaTime * PhaseCurve->GetFloatValue(PhaseTimer / PhaseTime);
		break;

	//  compute the movement phase of the automatic rotation with stop
	case EAutoRotationState::AutomaticRotationWithStop:
		AutomaticRotationTimer += DeltaTime;
		if (AutomaticRotationTimer >= AutomaticRotationDuration)
		{
			//  finished timer = arrived to stop angle destination
			//  switch state and start stop timer
			CurrentState = EAutoRotationState::StopOnAngle;
			AutomaticStopTimer = 0.0f;

			OwnerRotSupport->ForceInnerRotation(AutomaticStopDestAngle, true); //  reposition support to stop angle (security)
			break;
		}

		AngleDone = AutomaticRotationCurve->GetFloatValue(AutomaticRotationTimer / AutomaticRotationDuration) * AutomaticRotationAngle;
		RotationAdd = AngleDone - AutomaticRotationAngleDone; //  'AutomaticRotationAngleDone' serve here as a "angle done last frame"
		AutomaticRotationAngleDone = AngleDone;
		break;

	//  compute the stop phase of the automatic rotation with stop
	case EAutoRotationState::StopOnAngle:
		AutomaticStopTimer += DeltaTime;
		if (AutomaticStopTimer >= AutomaticStopDuration)
		{
			//  auto stop finished, restart
			StartAutomaticRotationWithStop();
		}
		break;
	}

	if (GetReverse()) RotationAdd *= -1.0f; //  reverse the desired rotation add if needed
	OwnerRotSupport->AddInnerRotation(RotationAdd, false); //  apply rotation add
}



// ======================================================
//              Control Automatic Rotation
// ======================================================
void URotationBehaviorAutomatic::LaunchAutomaticRotationBeginPlay()
{
	//  select auto rotation mode for begin play
	if (IsAutomaticStopValid())
	{
		StartAutomaticRotationWithStop();
	}
	else
	{
		StartAutomaticRotation();
	}
}

void URotationBehaviorAutomatic::StartAutomaticRotationWithStop()
{
	//  check validities
	if (!bOwnerRotSupportValid) return;
	if (!IsAutomaticStopValid()) return;

	//  check auto reverse
	if (bAutomaticStopReverse)
	{
		bAutomaticStopReverse = false;
		bExteriorReverse = !bExteriorReverse;
	}

	//  set automatic rotation with stop values
	const bool StopSuccess = ComputeNextStopAngle();
	if (!StopSuccess)
	{
		kPRINT_ERROR("Automatic Rotation couldn't retrieve a valid stop point!");
		CancelAutomaticRotation(); //  stop everything if a stop angle couldn't be computed
		return;
	}

	AutomaticRotationDuration = AutomaticRotationAngle / FMath::Abs(AutomaticRotationValues.GetRotationSpeed());
	AutomaticRotationTimer = 0.0f;
	AutomaticRotationCurve = AutomaticRotationValues.GetRotationCurve();
	AutomaticRotationAngleDone = 0.0f;

	//  set state and tick for this component
	CurrentState = EAutoRotationState::AutomaticRotationWithStop;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::AutomaticRotation;
	OwnerRotSupport->StartMovementOnChildrens();
}

void URotationBehaviorAutomatic::StartAutomaticRotation(bool bForceNoStartPhase)
{
	//  check validities
	if (!bOwnerRotSupportValid) return;
	if (!AutomaticRotationValues.IsDataValid()) return;
	if (AutomaticRotationValues.GetAutomaticRotationType() != EAutomaticRotationType::AutomaticRotation) return;

	//  set rotation speed
	AutomaticRotationSpeed = FMath::Abs(AutomaticRotationValues.GetRotationSpeed());

	//  set start phase values
	PhaseTimer = 0.0f;
	PhaseTime = bForceNoStartPhase ? 0.0f : AutomaticRotationValues.GetStartDuration();
	PhaseCurve = AutomaticRotationValues.GetStartCurve();

	//  set state and tick for this component
	CurrentState = (bForceNoStartPhase || !IsStartPhaseValid()) ? EAutoRotationState::AutomaticRotation : EAutoRotationState::StartPhase;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::AutomaticRotation;
	OwnerRotSupport->StartMovementOnChildrens();
}

void URotationBehaviorAutomatic::StopAutomaticRotation(bool bForceNoEndPhase)
{
	//  check validities
	if (!bOwnerRotSupportValid) return;
	if (!AutomaticRotationValues.IsDataValid()) return;

	if (bForceNoEndPhase || !IsEndPhaseValid())
	{
		//  stop instantly if there is no end phase
		CancelAutomaticRotation();
		return;
	}

	//  set rotation speed
	AutomaticRotationSpeed = FMath::Abs(AutomaticRotationValues.GetRotationSpeed());

	//  set end phase values
	PhaseTimer = 0.0f;
	PhaseTime = AutomaticRotationValues.GetEndDuration();
	PhaseCurve = AutomaticRotationValues.GetEndCurve();

	//  set state and tick for this component
	SetComponentTickEnabled(true);
	CurrentState = EAutoRotationState::EndPhase;

	//  set state on support and start movement on childrens (should not be useful but we never know)
	OwnerRotSupport->CurrentRotationState = ERotationState::AutomaticRotation;
	OwnerRotSupport->StartMovementOnChildrens();
}

void URotationBehaviorAutomatic::CancelAutomaticRotation()
{
	//  stop everything
	SetComponentTickEnabled(false);
	CurrentState = EAutoRotationState::Inactive;

	OwnerRotSupport->CurrentRotationState = ERotationState::NotRotating;
	OwnerRotSupport->StopMovementOnChildrens();
}



// ======================================================
//                     Interaction
// ======================================================
void URotationBehaviorAutomatic::TriggerAutoRotInteraction(FAutoRotInteractionDatas Datas)
{
	//  check if button interaction is possible
	if (!bOwnerRotSupportValid) return;
	if (!Datas.IsDataValid()) return;

	if (CurrentState == EAutoRotationState::AutomaticRotationWithStop || CurrentState == EAutoRotationState::StopOnAngle)
	{
		kPRINT_WARNING("You can't trigger a automatic rotation interaction with a MovSys Button on an automatic Rotation Support in automatic stop mode. If you need it to be a thing, please ask Cyril.");
		return;
	}

	//  select wether to start, stop and apply reverse depending of the current state of automatic rotation and interaction datas
	if (Datas.GetStartStop())
	{
		if (CurrentState == EAutoRotationState::AutomaticRotation || CurrentState == EAutoRotationState::StartPhase)
		{
			StopAutomaticRotation();
		}
		else
		{
			if (Datas.GetReverse())
			{
				bExteriorReverse = !bExteriorReverse;
			}
			StartAutomaticRotation();
		}
	}
	else
	{
		if (Datas.GetReverse())
		{
			bExteriorReverse = !bExteriorReverse;
			if (CurrentState == EAutoRotationState::AutomaticRotation || CurrentState == EAutoRotationState::StartPhase)
			{
				StartAutomaticRotation();
			}
		}
	}
}



// ======================================================
//                   Helper Functions
// ======================================================
bool URotationBehaviorAutomatic::IsAutomaticStopValid()
{
	//  check automatic rotation type is automatic stop
	if (!AutomaticRotationValues.IsDataValid()) return false;
	if (AutomaticRotationValues.GetAutomaticRotationType() != EAutomaticRotationType::StopOnAngles) return false;

	//  check validities of automatic stop values
	if (!IsValid(AutomaticRotationValues.GetRotationCurve())) return false;
	switch (AutomaticRotationValues.GetStopBehavior())
	{
	case ERotStopBehavior::StopAngleInterval:
		return AutomaticRotationValues.GetStopIntervalAngle() > 0;

	case ERotStopBehavior::StopSpecifedAngles:
		return AutomaticRotationValues.GetStopDefinedAngles().Num() > 0;

	default:
		return false; //  this can't be reached but the compiler need that
	}
}

bool URotationBehaviorAutomatic::IsStartPhaseValid()
{
	//  check validity of starting phase
	if (!AutomaticRotationValues.IsDataValid()) return false;

	return AutomaticRotationValues.GetStartDuration() > 0.0f && IsValid(AutomaticRotationValues.GetStartCurve());
}

bool URotationBehaviorAutomatic::IsEndPhaseValid()
{
	//  check validity of endinf phase
	if (!AutomaticRotationValues.IsDataValid()) return false;

	return AutomaticRotationValues.GetEndDuration() > 0.0f && IsValid(AutomaticRotationValues.GetEndCurve());
}

bool URotationBehaviorAutomatic::ComputeNextStopAngle()
{
	const int CurrentInnerRot = FMath::RoundToInt(OwnerRotSupport->GetInnerRotationBase360());

	//  for the stop specified angles search
	int SmallestDeltaAngle = 500; //  (500 is arbitrary value greater than any delta angle possible)
	FStopDefinedAngle NearestStopAngle;

	switch (AutomaticRotationValues.GetStopBehavior())
	{
	case ERotStopBehavior::StopAngleInterval:
		//  simply set automatic rotation angle to the angle interval
		AutomaticRotationAngle = AutomaticRotationValues.GetStopIntervalAngle();
		AutomaticStopDuration = AutomaticRotationValues.GetGlobalStopDuration();
		bAutomaticStopReverse = false;
		break;

	case ERotStopBehavior::StopSpecifedAngles:
		//  search the nearest stop angle in the list
		for (FStopDefinedAngle StopAngle : AutomaticRotationValues.GetStopDefinedAngles())
		{
			int DeltaAngle = UAnglesUtils::SignedDeltaAngleInt(CurrentInnerRot, StopAngle.StopAngle);
			if (FMath::Abs(DeltaAngle) < 1) continue; //  avoid to return a stop angle that is equal or nearly equal to the current inner angle

			if (GetReverse() && DeltaAngle > 0) DeltaAngle -= 360; //  delta angle must be negative if going reverse (go in the opposite circle direction)
			if (!GetReverse() && DeltaAngle < 0) DeltaAngle += 360; //  delta angle must be positive if not going reverse (go in the opposite circle direction)

			DeltaAngle = FMath::Abs(DeltaAngle);
			if (DeltaAngle < SmallestDeltaAngle)
			{
				SmallestDeltaAngle = DeltaAngle;
				NearestStopAngle = StopAngle;
			}
		}

		if (SmallestDeltaAngle >= 500) return false; //  the search found nothing
		else
		{
			//  set automatic rotation angle to the smallest found
			AutomaticRotationAngle = SmallestDeltaAngle;
			AutomaticStopDuration = NearestStopAngle.StopAngleDuration;
			bAutomaticStopReverse = NearestStopAngle.bStopAngleReverse;
		}

		break;
	}


	if (GetReverse())
	{
		AutomaticStopDestAngle = FMath::RoundToInt(OwnerRotSupport->GetInnerRotation() - AutomaticRotationAngle);
	}
	else
	{
		AutomaticStopDestAngle = FMath::RoundToInt(OwnerRotSupport->GetInnerRotation() + AutomaticRotationAngle);
	}

	return true;
}

bool URotationBehaviorAutomatic::GetReverse()
{
	return bExteriorReverse ^ bAutomaticSpeedReverse; // XOR
}
