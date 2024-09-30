#include "RotationBehaviorControlled.h"
#include "RotationSupport.h"
#include "DeusExMachina/Player/PlayerControllerInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Defines.h"

URotationBehaviorControlled::URotationBehaviorControlled()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URotationBehaviorControlled::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	InitializeOwner(); //  RotationBehaviorBase
}


// ======================================================
//         Tick (snap and Last Inputed Direction)
// ======================================================
void URotationBehaviorControlled::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//  last inputed direction part
	LastInputedDirectionTimer -= DeltaTime;
	if (LastInputedDirectionTimer <= 0.0f) LastInputedDirection = 0;


	if (!bOwnerRotSupportValid) return;

	//  snap part
	if (CurrentState != EControlledRotationState::Snap) return;

	SnapTimer += DeltaTime;
	if (SnapTimer >= SnapDuration)
	{
		//  snap timer finished, stop movement
		OwnerRotSupport->ForceInnerRotation(SnapAngleDest, true); //  reposition support to snap angle (security)
		OwnerRotSupport->OnRotationSupportSnapped.Broadcast(SnapAngleDest);

		SetComponentTickEnabled(false);
		CurrentState = EControlledRotationState::Inactive;

		OwnerRotSupport->CurrentRotationState = ERotationState::NotRotating;
		OwnerRotSupport->StopMovementOnChildrens();
		return;
	}
	
	const float DesiredSnapAngle = FMath::Lerp<float, float>(SnapAngleDest, SnapAngleStart, SnapCurve->GetFloatValue(SnapTimer / SnapDuration));
	OwnerRotSupport->ForceInnerRotation(DesiredSnapAngle, true);
}


// ======================================================
//              Control Controlled Rotation
// ======================================================
bool URotationBehaviorControlled::StartControlledRotation(FControlledRotationDatas Datas)
{
	//  check validities
	if (!bOwnerRotSupportValid) return false;
	if (!IsControlledRotValid(Datas)) return false;

	//  check already in control
	if (CurrentState != EControlledRotationState::Inactive && CurrentState != EControlledRotationState::Snap) return false;

	//  set controlled rot values
	CurrentDatas = Datas;
	RotationSpeed = Datas.GetRotationSpeed();

	//  set startup values & state
	if (IsStartupValid(Datas))
	{
		bUseStartup = true;
		StartupTimer = 0.0f;
		StartupDuration = Datas.GetStartupDuration();
		StartupCurve = Datas.GetStartupCurve();
		CurrentState = EControlledRotationState::ControlStartup;
	}
	else
	{
		bUseStartup = false;
		CurrentState = EControlledRotationState::Control;
	}

	//  enable tick on this component
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::ControlledRotation;
	OwnerRotSupport->StartMovementOnChildrens();

	//  broadcast OnControlledRotationStart event
	OnControlledRotationStart.Broadcast();

	return true;
}

void URotationBehaviorControlled::StopControlledRotation(bool DontTriggerSnap)
{
	//  check validity
	if (!bOwnerRotSupportValid) return;

	//  check already in control
	if (CurrentState != EControlledRotationState::Control && CurrentState != EControlledRotationState::ControlStartup) return;

	//  check snap
	const FRotSupportValues OwnerValues = OwnerRotSupport->GetSupportValues();
	const float SnapSearchAdvantage = LastInputedDirection * ((OwnerValues.GetSnapDirectionAdvantage() * 2.0f) - 1.0f); //  converts snap direction advantage to generic snap search advantage

	if (OwnerRotSupport->SearchSnapAngle(OwnerRotSupport->GetInnerRotation(), SnapAngleDest, SnapSearchAdvantage, CurrentDatas.GetSnapIgnoreClamp(), CurrentDatas.GetSnapIgnoreRanges()))
	{
		//  set snap values & state
		SnapAngleStart = OwnerRotSupport->GetInnerRotation();
		SnapTimer = 0.0f;
		SnapDuration = FMath::Abs(SnapAngleDest - SnapAngleStart) / OwnerValues.GetSnapSpeed();

		SnapCurve = FMath::RoundToInt(FMath::Sign<float>(SnapAngleDest - SnapAngleStart)) == LastInputedDirection ? OwnerValues.GetSnapCurveContinue() : OwnerValues.GetSnapCurveNeutralReverse();

		CurrentState = EControlledRotationState::Snap;

		//  broadcast OnControlledRotationStop event
		OnControlledRotationStop.Broadcast(true);
	}
	else
	{
		//  set state and disable tick for this component
		SetComponentTickEnabled(false);
		CurrentState = EControlledRotationState::Inactive;

		//  set state on support and stop movement on childrens
		OwnerRotSupport->CurrentRotationState = ERotationState::NotRotating;
		OwnerRotSupport->StopMovementOnChildrens();

		//  broadcast OnControlledRotationStop event
		OnControlledRotationStop.Broadcast(false);
	}
}

bool URotationBehaviorControlled::UpdateControlledRotation(float ControlValue)
{
	//  check validity
	if (!bOwnerRotSupportValid) return false;

	//  preparation
	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	bool TriggeredClamp = false;
	float DesiredAngleAdd = 0.0f;

	switch (CurrentState)
	{
	case EControlledRotationState::ControlStartup:
		//  startup phase, interpolate movement with the curve
		StartupTimer += DeltaTime;
		if (StartupTimer >= StartupDuration)
		{
			//  startup finished, switch to normal control
			StartupTimer = StartupDuration;
			CurrentState = EControlledRotationState::Control;
		}

		DesiredAngleAdd = ControlValue * RotationSpeed * DeltaTime * StartupCurve->GetFloatValue(StartupTimer / StartupDuration);
		break;

	case EControlledRotationState::Control:
		//  normal control
		DesiredAngleAdd = ControlValue * RotationSpeed * DeltaTime;
		break;
	}

	//  apply controlled rotation with checking clamp
	TriggeredClamp = OwnerRotSupport->AddInnerRotation(DesiredAngleAdd, true);


	//  manage startup and last inputed direction depending of wether or not the player used the joystick this frame
	if (ControlValue == 0.0f)
	{ 
		//  the player stopped the rotation (joystick) but kept the control (interaction button)
		if (bUseStartup)
		{
			//  reset the startup (if using startup)
			StartupTimer = 0.0f;
			CurrentState = EControlledRotationState::ControlStartup;
		}
	}
	else
	{
		//  player used joystick, set last inputed direction values
		const int InputedDirection = FMath::Sign<float>(ControlValue) * FMath::Sign<float>(RotationSpeed);

		if ((InputedDirection != LastInputedDirection) && bUseStartup)
		{
			//  the player reversed its direction, so reset the startup (if using startup)
			StartupTimer = 0.0f;
			CurrentState = EControlledRotationState::ControlStartup;
		}

		LastInputedDirection = InputedDirection;
		LastInputedDirectionTimer = Cast<IPlayerControllerInterface>(UGameplayStatics::GetPlayerController(OwnerRotSupport, 0))->GetControlDirectionDelay();
	}

	return TriggeredClamp;
}


// ======================================================
//                   Helper Functions
// ======================================================
bool URotationBehaviorControlled::IsControlledRotValid(FControlledRotationDatas Datas)
{
	//  check validity of the controlled rotation values
	if (!Datas.IsDataValid()) return false;

	return Datas.GetRotationSpeed() != 0.0f;
}

bool URotationBehaviorControlled::IsStartupValid(FControlledRotationDatas Datas)
{
	//  check validity of the startup values
	if (!Datas.IsDataValid()) return false;

	return Datas.GetStartupDuration() > 0.0f && IsValid(Datas.GetStartupCurve());
}
