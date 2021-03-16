// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class VehicleTestProjectTarget : TargetRules
{
	public VehicleTestProjectTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		CppStandard = CppStandardVersion.Cpp17;

		ExtraModuleNames.AddRange( new string[] { "VehicleTestProject" } );
	}
}
