#include "CoreUObject.h"
#include "Modules/ModuleManager.h"

class FDeusExMachinaEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;

	virtual void ShutdownModule() override;
};