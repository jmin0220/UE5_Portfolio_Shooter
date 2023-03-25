// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ShooterProject : ModuleRules
{
	public ShooterProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        // �� �迭�� �߰��� ����� ���� ����� �ڵ忡�� ����� �� ������, �ڵ����� ���� �ý��ۿ� ���� ����ǰ� ��ũ��.
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTasks", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
