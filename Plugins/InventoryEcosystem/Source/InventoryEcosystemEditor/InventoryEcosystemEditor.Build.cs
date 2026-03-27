// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InventoryEcosystemEditor : ModuleRules
{
	public InventoryEcosystemEditor(ReadOnlyTargetRules Target) : base(Target)
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
			
		PublicDependencyModuleNames.AddRange(
        			new string[]
        			{
        				"Core",
        				"UnrealEd",
        				// ... add other public dependencies that you statically link with here ...
        			}
        		);
        			
        		
        		PrivateDependencyModuleNames.AddRange(
        			new string[]
        			{
				        "Core",
        				"CoreUObject",
        				"GameplayTags",
        				"Engine",
        				"Slate",
        				"SlateCore",
				        "InputCore",
				        "EditorStyle",
				        "UMG",
				        "Projects", 
				        "InteractSystem",
        				// ... add private dependencies that you statically link with here ...	
        			}
        		);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
