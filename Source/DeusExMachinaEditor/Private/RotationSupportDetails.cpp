#include "RotationSupportDetails.h"
#include "DeusExMachina/MovableSystem/Rotation/RotationSupport.h"

#include "DetailCategoryBuilder.h"
#include "DetailsDisplayManager.h"
#include "DetailLayoutBuilder.h"

#define LOCTEXT_NAMESPACE "RotationSupportDetails"


TSharedRef<IDetailCustomization> FRotationSupportDetails::MakeInstance()
{
	return MakeShareable(new FRotationSupportDetails);
}

void FRotationSupportDetails::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	DetailBuilder.GetObjectsBeingCustomized(ObjectsToEdit);

	IDetailCategoryBuilder& BehaviorCategory = DetailBuilder.EditCategory("Rotation Behaviors");

	IDetailPropertyRow& BehaviorAutomaticProperty = BehaviorCategory.AddProperty("bBehaviorAutomatic", ARotationSupport::StaticClass());
	BehaviorAutomaticProperty.DisplayName(FText::FromString("Automatic"));

	IDetailPropertyRow& BehaviorControlledProperty = BehaviorCategory.AddProperty("bBehaviorControlled", ARotationSupport::StaticClass());
	BehaviorControlledProperty.DisplayName(FText::FromString("Controlled"));

	IDetailPropertyRow& BehaviorStandardProperty = BehaviorCategory.AddProperty("bBehaviorStandard", ARotationSupport::StaticClass());
	BehaviorStandardProperty.DisplayName(FText::FromString("Standard"));
}

FReply FRotationSupportDetails::EditObjects()
{
	return FReply::Handled();
}
