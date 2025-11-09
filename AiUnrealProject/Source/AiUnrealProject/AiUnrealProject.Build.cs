// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AiUnrealProject : ModuleRules
{
	public AiUnrealProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"Niagara",
			"UMG",
			"Slate",
			"Json",          
			"JsonUtilities",     
			"HTTP",
			"SlateCore" ,             
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"AiUnrealProject",
			"AiUnrealProject/Variant_Strategy",
			"AiUnrealProject/Variant_Strategy/UI",
			"AiUnrealProject/Variant_TwinStick",
			"AiUnrealProject/Variant_TwinStick/AI",
			"AiUnrealProject/Variant_TwinStick/Gameplay",
			"AiUnrealProject/Variant_TwinStick/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
