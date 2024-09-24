#include "RotationBehaviorControlled.h"
#include "RotationSupport.h"
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

	InitializeOwner();
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


	//  snap part
	if (CurrentState != EControlledRotationState::Snap) return;

	SnapTimer += DeltaTime;
	if (SnapTimer >= SnapDuration)
	{
		SnapTimer = SnapDuration;

		SetComponentTickEnabled(false);
		CurrentState = EControlledRotationState::Inactive;

		OwnerRotSupport->CurrentRotationState = ERotationState::NotRotating;
		OwnerRotSupport->StopMovementOnChildrens();
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
	}
	else
	{
		//  set state and disable tick for this component
		SetComponentTickEnabled(false);
		CurrentState = EControlledRotationState::Inactive;

		//  set state on support and stop movement on childrens
		OwnerRotSupport->CurrentRotationState = ERotationState::NotRotating;
		OwnerRotSupport->StopMovementOnChildrens();
	}
}

bool URotationBehaviorControlled::UpdateControlledRotation(float ControlValue)
{
	//  check validity
	if (!bOwnerRotSupportValid) return false;

	const float DeltaTime = UGameplayStatics::GetWorldDeltaSeconds(this);
	bool TriggeredClamp = false;
	float DesiredAngleAdd = 0.0f;

	switch (CurrentState)
	{
	case EControlledRotationState::ControlStartup:
		StartupTimer += DeltaTime;
		if (StartupTimer >= StartupDuration)
		{
			StartupTimer = StartupDuration;
			CurrentState = EControlledRotationState::Control;
		}

		DesiredAngleAdd = ControlValue * RotationSpeed * DeltaTime * StartupCurve->GetFloatValue(StartupTimer / StartupDuration);
		break;

	case EControlledRotationState::Control:
		DesiredAngleAdd = ControlValue * RotationSpeed * DeltaTime;
		break;
	}

	TriggeredClamp = OwnerRotSupport->AddInnerRotation(DesiredAngleAdd, true);


	if (ControlValue == 0.0f) //  The player stopped the rotation (joystick) but kept the control (interaction button)
	{
		if (bUseStartup) //  reset the startup (if using startup)
		{
			StartupTimer = 0.0f;
			CurrentState = EControlledRotationState::ControlStartup;
		}
	}
	else
	{
		LastInputedDirection = FMath::Sign<float>(ControlValue) * FMath::Sign<float>(RotationSpeed);
		LastInputedDirectionTimer = OwnerRotSupport->GetSupportValues().GetSnapDirectionDelay();
	}

	return TriggeredClamp;
}


// ======================================================
//                   Helper Functions
// ======================================================
bool URotationBehaviorControlled::IsControlledRotValid(FControlledRotationDatas Datas)
{
	if (!Datas.IsDataValid()) return false;

	return Datas.GetRotationSpeed() != 0.0f;
}

bool URotationBehaviorControlled::IsStartupValid(FControlledRotationDatas Datas)
{
	if (!Datas.IsDataValid()) return false;

	return Datas.GetStartupDuration() > 0.0f && IsValid(Datas.GetStartupCurve());
}
