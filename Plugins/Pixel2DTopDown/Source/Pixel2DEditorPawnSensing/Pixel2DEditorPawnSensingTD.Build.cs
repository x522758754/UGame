//Polyart Studio 2021

using UnrealBuildTool;

public class Pixel2DEditorPawnSensingTD : ModuleRules
{
	public Pixel2DEditorPawnSensingTD(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

        PrivateIncludePaths.Add("Pixel2DEditorPawnSensing/Private");
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
				"KismetWidgets",
				"Kismet",  // for FWorkflowCentricApplication
                "KismetCompiler",
				"PropertyEditor",
				"RenderCore",
				"Paper2D",
                "Paper2DEditor",
                "Pixel2DEditorTD",
				"ContentBrowser",
				"WorkspaceMenuStructure",
				"EditorStyle",
				"MeshPaint",
				"EditorWidgets",
				"Projects",
                "AnimationBlueprintEditor",
                "AssetTools",
                "AnimGraph",
                "BlueprintGraph",
                "GraphEditor",
                "Pixel2DTD"
			});

		PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"Settings",
				"IntroTutorials",
				"AssetTools",
				"LevelEditor",
                "AnimationBlueprintEditor",
                "AnimGraph",
                "BlueprintGraph",
                "GraphEditor",
                "KismetCompiler",
            });

	}
}
