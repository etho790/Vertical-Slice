// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MyTemplate : ModuleRules
{
	public MyTemplate(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "GameplayAbilities", "GameplayTags", "UMG", "Slate", "SlateCore", "CableComponent", "OnlineSubsystemUtils", "Steamworks", "Networking", "Niagara" });
        DynamicallyLoadedModuleNames.Add("OnlineSubsystemSteam");

        PrivateDependencyModuleNames.Add("OnlineSubsystem");
    }
}
