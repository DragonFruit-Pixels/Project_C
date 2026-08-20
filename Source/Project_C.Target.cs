// Copyright DragonFruit Pixels. All Rights Reserved.

using UnrealBuildTool;

public class Project_CTarget : TargetRules
{
	public Project_CTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		// Pinned to concrete versions on purpose. `Latest` drifts when the engine is upgraded,
		// which turns an engine bump into a surprise compile break. Bump these deliberately.
		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;

		ExtraModuleNames.Add("ProjectC");
	}
}
