// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class dyncam_unreal_testTarget : TargetRules
{
	public dyncam_unreal_testTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "dyncam_unreal_test" } );
	}
}
