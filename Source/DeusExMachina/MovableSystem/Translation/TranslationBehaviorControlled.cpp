#include "TranslationBehaviorControlled.h"
#include "TranslationSupport.h"
#include "DeusExMachina/Player/PlayerControllerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"

UTranslationBehaviorControlled::UTranslationBehaviorControlled()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UTranslationBehaviorControlled::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	InitializeOwner(); //  TranslationBehaviorBase
}


// ======================================================
//         Tick (snap and Last Inputed Direction)
// ======================================================
void UTranslationBehaviorControlled::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//  last inputed direction part
	LastInputedDirectionTimer -= DeltaTime;
	if (LastInputedDirectionTimer <= 0.0f) LastInputedDirection = 0;


	if (!bOwnerTransSupportValid) return;

	//  snap part
	if (CurrentState != EControlledTranslationState::Snap) return;

	SnapTimer += DeltaTime;
	if (SnapTimer >= SnapDuration)
	{
		//  snap timer finished, stop movement
		OwnerTransSupport->ForcePositionOnSpline(SnapPointDest, 0.0f); //  reposition support to snap point (security)
		OwnerTransSupport->OnTranslationSupportSnapped.Broadcast(SnapPointDest);

		SetComponentTickEnabled(false);
		CurrentState = EControlledTranslationState::Inactive;

		OwnerTransSupport->CurrentTranslationState = ETranslationState::NotMoving;
		OwnerTransSupport->StopMovementOnChildrens();
		return;
	}

	const float DistanceDone = FMath::Lerp<float, float>(SnapDistance, 0.0f, SnapCurve->GetFloatValue(SnapTimer / SnapDuration));

	int ClampedIndex = 0;
	const float SnapAdd = OwnerTransSupport->ClampMovementBetweenSplinePoints(DistanceDone - SnapDistanceDone, 0, 1, ClampedIndex);
	OwnerTransSupport->AddTranslationAlongSpline(SnapAdd); //  here, 'SnapDistanceDone' serve as a "distance done last frame"
	SnapDistanceDone = DistanceDone;
}


// ======================================================
//             Control Controlled Translation
// ======================================================
bool UTranslationBehaviorControlled::StartControlledTranslation(FControlledTranslationDatas Datas)
{
	//  check validities
	if (!bOwnerTransSupportValid) return false;
	if (!IsControlledTransValid(Datas)) return false;

	//  check already in control
	if (CurrentState != EControlledTranslationState::Inactive && CurrentState != EControlledTranslationState::Snap) return false;

	//  set controlled rot values
	CurrentDatas = Datas;
	TranslationSpeed = Datas.GetTranslationSpeed();

	//  set startup values & state
	if (IsStartupValid(Datas))
	{
		bUseStartup = true;
		StartupTimer = 0.0f;
		StartupDuration = Datas.GetStartupDuration();
		StartupCurve = Datas.GetStartupCurve();
		CurrentState = EControlledTranslationState::ControlStartup;
	}
	else
	{
		bUseStartup = false;
		CurrentState = EControlledTranslationState::Control;
	}

	//  enable tick on this component
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerTransSupport->CurrentTranslationState = ETranslationState::ControlledTranslation;
	OwnerTransSupport->StartMovementOnChildrens();

	//  broadcast OnControlledTranslationStart event
	OnControlledTranslationStart.Broadcast();

	return true;
}

void UTranslationBehaviorControlled::StopControlledTranslation(bool DontTriggerSnap)
{
	//  check validity
	if (!bOwnerTransSupportValid) return;

	//  check already in control
	if (CurrentState != EControlledTranslationState::Control && CurrentState != EControlledTranslationState::ControlStartup) return;

	if (!DontTriggerSnap && IsSnapValid(CurrentDatas))
	{
		//  compute and set snap values
		SnapPointDest = OwnerTransSupport->SearchNearestSplinePointToSnap(OwnerTransSupport->GetDistanceFromSplineOrigin(), CurrentDatas.GetSnapPreference());
		if (SnapPointDest == OwnerTransSupport->GetInnerSplineIndex()) //  snap going in the opposite of spline direction
		{
			SnapDistance = OwnerTransSupport->GetSplineDistanceToPointReversed(SnapPointDest) * -1.0f; //  need to store a negatve distance so the snap will go in the correct direction
		}
		else //  snap going in the spline direction
		{
			SnapDistance = OwnerTransSupport->GetSplineDistanceToPoint(SnapPointDest);
		}
		SnapDistanceDone = 0.0f;
		SnapDuration = FMath::Abs(SnapDistance) / CurrentDatas.GetSnapSpeed();
		SnapTimer = 0.0f;

		SnapCurve = FMath::RoundToInt(FMath::Sign<float>(SnapDistance)) == LastInputedDirection ? CurrentDatas.GetSnapCurveContinue() : CurrentDatas.GetSnapCurveNeutralReverse();

		//  set snap state
		CurrentState = EControlledTranslationState::Snap;

		//  broadcast OnControlledTranslationStop event
		OnControlledTranslationStop.Broadcast(true);
	}
	else
	{
		//  set state and disable tick for this component
		SetComponentTickEnabled(false);
		CurrentState = EControlledTranslationState::Inactive;

		//  set state on support and stop movement on childrens
		OwnerTransSupport->CurrentTranslationState = ETranslationState::NotMoving;
		OwnerTransSupport->StopMovementOnChildrens();

		//  broadcast OnControlledTranslationStop event
		OnControlledTranslationStop.Broadcast(false);
	}
}

bool UTranslationBehaviorControlled::UpdateControlledTranslation(float ControlValue)
{
	//  check validity
	if (!bOwnerTransSupportValid) return false;

	//  preparation
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	bool TriggeredClamp = false;
	float DesiredDistAdd = 0.0f;

	switch (CurrentState)
	{
	case EControlledTranslationState::ControlStartup:
		//  startup phase, interpolate movement with curve
		StartupTimer += DeltaTime;
		if (StartupTimer >= StartupDuration)
		{
			//  startup finished, switch to normal control
			StartupTimer = StartupDuration;
			CurrentState = EControlledTranslationState::Control;
		}

		DesiredDistAdd = ControlValue * TranslationSpeed * DeltaTime * StartupCurve->GetFloatValue(StartupTimer / StartupDuration);
		break;

	case EControlledTranslationState::Control:
		//  normal control
		DesiredDistAdd = ControlValue * TranslationSpeed * DeltaTime;
		break;
	}

	//  clamp 'DesiredDistAdd' between spline points 0 and 1
	int ClampedIndex = 0;
	DesiredDistAdd = OwnerTransSupport->ClampMovementBetweenSplinePoints(DesiredDistAdd, 0, 1, ClampedIndex);
	if (FMath::IsNearlyZero(DesiredDistAdd))
	{
		//  if the clamped 'DesiredDistAdd' is nearly zero, it means that clamped have taken effect
		TriggeredClamp = true;
		OwnerTransSupport->OnTranslationSupportClamped.Broadcast(ClampedIndex);
	}

	//  apply controled translation
	OwnerTransSupport->AddTranslationAlongSpline(DesiredDistAdd);

	
	//  manage startup and last inputed direction depending of wether or not the player used the joystick this frame
	if (ControlValue == 0.0f) 
	{
		//  the player stopped the rotation (joystick) but kept the control (interaction button)
		if (bUseStartup) 
		{
			//  reset the startup (if using startup)
			StartupTimer = 0.0f;
			CurrentState = EControlledTranslationState::ControlStartup;
		}
	}
	else
	{
		//  player used joystick, set last inputed direction values
		const int InputedDirection = FMath::Sign<float>(ControlValue) * FMath::Sign<float>(TranslationSpeed);

		if ((InputedDirection != LastInputedDirection) && bUseStartup)
		{
			//  the player reversed its direction, so reset the startup (if using startup)
			StartupTimer = 0.0f;
			CurrentState = EControlledTranslationState::ControlStartup;
		}

		LastInputedDirection = InputedDirection;
		LastInputedDirectionTimer = Cast<IPlayerControllerInterface>(UGameplayStatics::GetPlayerController(OwnerTransSupport, 0))->GetControlDirectionDelay();
	}

	return TriggeredClamp;
}


// ======================================================
//                   Helper Functions
// ======================================================
bool UTranslationBehaviorControlled::IsControlledTransValid(FControlledTranslationDatas Datas)
{
	//  check validity of the controlled translation values
	if (!Datas.IsDataValid()) return false;

	return Datas.GetTranslationSpeed() != 0.0f;
}

bool UTranslationBehaviorControlled::IsStartupValid(FControlledTranslationDatas Datas)
{
	//  check validity of the startup values
	if (!Datas.IsDataValid()) return false;

	return Datas.GetStartupDuration() > 0.0f && IsValid(Datas.GetStartupCurve());
}

bool UTranslationBehaviorControlled::IsSnapValid(FControlledTranslationDatas Datas)
{
	//  check validity of the snap values
	if (!Datas.IsDataValid()) return false;
	if (!Datas.GetUseSnap()) return false;
	
	return Datas.GetSnapSpeed() > 0.0f && IsValid(Datas.GetSnapCurveNeutralReverse()) && IsValid(Datas.GetSnapCurveContinue());
}

