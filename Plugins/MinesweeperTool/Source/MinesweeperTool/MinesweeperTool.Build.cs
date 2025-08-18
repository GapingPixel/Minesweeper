// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MinesweeperTool : ModuleRules
{
	public MinesweeperTool(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(new[]
        {
            "Core", "CoreUObject", "Slate", "SlateCore", "EditorStyle"
        });

        PrivateDependencyModuleNames.AddRange(new[]
        {
            "ApplicationCore", "InputCore",
            "UnrealEd", "LevelEditor", "ToolMenus",
            "Projects" 
        });
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
