// Copyright DragonFruit Pixels. All Rights Reserved.

using UnrealBuildTool;

public class ProjectC : ModuleRules
{
	public ProjectC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",   // enabled by default in 5.8, nothing to add to the .uproject
			"GameplayTags"     // an engine module, not a plugin: available with no setup
		});

		PrivateDependencyModuleNames.AddRange(new string[]
		{
			"Slate",
			"SlateCore",
			"UMG",
			"AIModule",
			"NavigationSystem"
		});
	}
}
