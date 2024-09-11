#include "RotationBehaviorBase.h"
#include "RotationSupport.h"
#include "Defines.h"

URotationBehaviorBase::URotationBehaviorBase()
{
	PrimaryComponentTick.bCanEverTick = false;
}



// ======================================================
//               Owner-related functions
// ======================================================
bool URotationBehaviorBase::InitializeOwner()
{
	OwnerRotSupport = Cast<ARotationSupport>(GetOwner());
	if (!IsValid(OwnerRotSupport)) return false;

	bOwnerRotSupportValid = true;
	return true;
}




// ======================================================
//            Component creation security
// ======================================================
void URotationBehaviorBase::OnComponentCreated()
{
	OwnerRotSupport = Cast<ARotationSupport>(GetOwner());
	if (IsValid(OwnerRotSupport)) return;

	kPRINT_COLOR("A Rotation Behavior Component can only be added on a RotationSupport object!", FColor::Orange);

	GetOwner()->RemoveOwnedComponent(this);
}