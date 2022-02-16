//Polyart Studio 2021


using UnrealBuildTool;

public class Pixel2DEditorTileReplacerTD : ModuleRules
{
	public Pixel2DEditorTileReplacerTD(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add("Pixel2DEditorTileReplacer/Private");
        PrivateIncludePaths.Add("Pixel2D/Classes");

        PrivateDependencyModuleNames.AddRange(
			new string[] {
				"Core",
				"CoreUObject",
				"ApplicationCore",
				"Json",
				"Slate",
				"SlateCore",
				"Engine",
				"InputCore",
				"UnrealEd", // for FAssetEditorManager
				"PropertyEditor",
				"RenderCore",
				"Paper2D",
                "Paper2DEditor",
				"Projects",
                "Pixel2DTD"
			});

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Settings",
				"IntroTutorials",
				"AssetTools",
				"LevelEditor"
            });

	}
}
