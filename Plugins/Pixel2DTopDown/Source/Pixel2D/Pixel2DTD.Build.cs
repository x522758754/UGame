//Polyart Studio 2021


using UnrealBuildTool;

public class Pixel2DTD : ModuleRules
{
	public Pixel2DTD(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add("Pixel2D/Private");
        PrivateIncludePaths.Add("Pixel2D/Classes");

        PublicDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "Engine",
                "RenderCore",
                "RHI",
                "SlateCore",
                "Slate",
                "Paper2D"
            }
        );

        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Renderer",
            }
        );
		
		PrecompileForTargets = PrecompileTargetsType.Any;

    }
}
