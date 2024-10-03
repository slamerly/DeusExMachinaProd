#pragma once

#include "IDetailCustomization.h"


class FReply;


class FRotationSupportDetails : public IDetailCustomization
{
public:
	/** Makes a new instance of this detail layout class for a specific detail view requesting it */
	static TSharedRef<IDetailCustomization> MakeInstance();

	/** IDetailCustomization interface */
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;

	FReply EditObjects();


private:
	TArray<TWeakObjectPtr<UObject>> ObjectsToEdit;
};