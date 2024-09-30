#include "TranslationBehaviorBase.h"
#include "TranslationSupport.h"
#include "Defines.h"

UTranslationBehaviorBase::UTranslationBehaviorBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}



// ======================================================
//               Owner-related functions
// ======================================================
bool UTranslationBehaviorBase::InitializeOwner()
{
	OwnerTransSupport = Cast<ATranslationSupport>(GetOwner());
	if (!IsValid(OwnerTransSupport)) return false;

	bOwnerTransSupportValid = true;
	return true;
}




// ======================================================
//            Component creation security
// ======================================================
void UTranslationBehaviorBase::OnComponentCreated()
{
	OwnerTransSupport = Cast<ATranslationSupport>(GetOwner());
	if (IsValid(OwnerTransSupport)) return;

	kPRINT_COLOR("A Translation Behavior Component can only be added on a TranslationSupport object!", FColor::Orange);

	GetOwner()->RemoveOwnedComponent(this);
}