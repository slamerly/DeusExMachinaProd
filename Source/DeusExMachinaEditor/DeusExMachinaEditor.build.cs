using UnrealBuildTool;

public class DeusExMachinaEditor : ModuleRules
{
    public DeusExMachinaEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.Add("DeusExMachinaEditor/Public");
        PrivateIncludePaths.Add("DeusExMachinaEditor/Private");

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "UnrealEd",
            "DetailCustomizations",
            "PropertyEditor",
            "EditorStyle",
            "DeusExMachina"
        });

        PrivateDependencyModuleNames.AddRange(new string[]
        {
            "Slate",
            "SlateCore"
        });
    }
}