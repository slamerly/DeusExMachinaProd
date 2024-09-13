#include "RotationBehaviorStandard.h"
#include "RotationSupport.h"
#include "StandardRotationDatas.h"


URotationBehaviorStandard::URotationBehaviorStandard()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void URotationBehaviorStandard::BeginPlay()
{
	Super::BeginPlay();

	SetComponentTickEnabled(false);

	InitializeOwner();
}


// ======================================================
//                        Tick
// ======================================================
void URotationBehaviorStandard::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!bOwnerRotSupportValid) return;
	if (!bCurrentlyRotating) return;

	RotationTimer += DeltaTime;
	if (RotationTimer >= RotationDuration)
	{
		RotationTimer = RotationDuration;
		CancelStandardRotation();
	}

	const float DesiredAngle = RotationAngle * RotationCurve->GetFloatValue(RotationTimer / RotationDuration);
	OwnerRotSupport->AddInnerRotation(DesiredAngle - LastFrameRotAngle);
	
	LastFrameRotAngle = DesiredAngle;
}


// ======================================================
//              Control Standard Rotation
// ======================================================
void URotationBehaviorStandard::StartStandardRotation(FStandardRotationDatas Datas, bool bForceStart)
{
	//  check validities
	if (!bOwnerRotSupportValid) return;
	if (!IsStandardRotValid(Datas)) return;

	//  check already rotating
	if (bCurrentlyRotating && !bForceStart) return;

	//  set standard rot values
	RotationAngle = Datas.GetRotationAngle();
	RotationCurve = Datas.GetRotationCurve();
	RotationDuration = Datas.GetRotationDuration();
	RotationTimer = 0.0f;
	LastFrameRotAngle = 0.0f;

	//  set rotating and tick for this component
	bCurrentlyRotating = true;
	SetComponentTickEnabled(true);

	//  set state on support and start movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::StandardRotation;
	OwnerRotSupport->StartMovementOnChildrens();
}

void URotationBehaviorStandard::CancelStandardRotation()
{
	if (!bCurrentlyRotating) return;

	//  set rotating and tick for this component
	bCurrentlyRotating = false;
	SetComponentTickEnabled(false);

	//  set state on support and stop movement on childrens
	OwnerRotSupport->CurrentRotationState = ERotationState::NotRotating;
	OwnerRotSupport->StopMovementOnChildrens();
}


// ======================================================
//                   Helper Functions
// ======================================================
bool URotationBehaviorStandard::IsStandardRotValid(FStandardRotationDatas Datas)
{
	if (!Datas.IsDataValid()) return false;

	return Datas.GetRotationDuration() > 0.0f && IsValid(Datas.GetRotationCurve());
}

