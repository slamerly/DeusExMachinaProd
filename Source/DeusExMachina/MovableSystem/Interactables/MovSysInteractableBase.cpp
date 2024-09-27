#include "MovSysInteractableBase.h"

AMovSysInteractableBase::AMovSysInteractableBase()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AMovSysInteractableBase::BeginPlay()
{
	Super::BeginPlay();
}



// ======================================================
//              Interactable Disable
// ======================================================
void AMovSysInteractableBase::DisableInteractable()
{
	bDisabled = true;
}

void AMovSysInteractableBase::EnableInteractable()
{
	bDisabled = false;
}

bool AMovSysInteractableBase::IsInteractableDisabled()
{
	return bDisabled;
}


// ======================================================
//                 Made to be overriden
// ======================================================
void AMovSysInteractableBase::ForceReleaseInteractable()
{
}
