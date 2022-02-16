//Polyart Studio 2021


using UnrealBuildTool;

public class Pixel2DEditorTileMapTD : ModuleRules
{
	public Pixel2DEditorTileMapTD(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add("Pixel2DEditorTileMap/Private");
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
				"EditorStyle",
				"EditorWidgets",
				"Kismet",
				"KismetWidgets",
				"InputCore",
				"UnrealEd", // for FAssetEditorManager
				"PropertyEditor",
				"NavigationSystem",
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
				"LevelEditor",
				"Paper2D"
            });

	}
}
