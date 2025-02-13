// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class P_7 : ModuleRules
{
	public P_7(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
