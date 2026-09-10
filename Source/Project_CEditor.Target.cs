using UnrealBuildTool;

public class Project_CEditorTarget : TargetRules
{
	public Project_CEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;

		DefaultBuildSettings = BuildSettingsVersion.V7;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_8;

		ExtraModuleNames.Add("ProjectC");
	}
}
