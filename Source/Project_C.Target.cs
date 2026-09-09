// Copyright DragonFruit Pixels. All Rights Reserved.

using UnrealBuildTool;

public class Project_CTarget : TargetRules
{
	public Project_CTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;

		ExtraModuleNames.Add("ProjectC");
	}
}
