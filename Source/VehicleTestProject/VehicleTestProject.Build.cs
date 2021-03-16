// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class VehicleTestProject : ModuleRules
{
	public VehicleTestProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });


		//PublicAdditionalLibraries.Add(Path.Combine(Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")), "DQNAgent.a"));

		//PublicIncludePaths.AddRange(new string[] {"ThirdParty", "/usr/local/include"});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
